#include <optional>
#include <QObject>
#include <QGuiApplication>

#include "../qcursorconstraints.h"

static bool cursorLocked = false;
static std::optional<QRect> confinedRect;
static std::optional<QPoint> lockedPoint;
static std::optional<QWindow*> targetWindow;

class GenericPointerConstraintsEventFilter : public QObject {
public:
    bool eventFilter(QObject* obj, QEvent* e) override
    {
        if (e->type() == QEvent::MouseMove) {
            if (lockedPoint.has_value()) {
                QCursor::setPos(lockedPoint.value());
                return true;
            }

            if (confinedRect.has_value()) {
                QPoint cPos = QCursor::pos();
                int mx = cPos.x(), my = cPos.y();

                QRect trueRect = confinedRect.value().translated(targetWindow.value()->position());

                if (cPos.x() > trueRect.right()) mx = trueRect.right();
                if (cPos.x() < trueRect.left()) mx = trueRect.left();
                if (cPos.y() > trueRect.bottom()) my = trueRect.bottom();
                if (cPos.y() < trueRect.top()) my = trueRect.top();

                QCursor::setPos(mx, my);
                return false;
            }
        }
        return QObject::eventFilter(obj, e);
    }
};

static GenericPointerConstraintsEventFilter* genericEventFilter;

static void init_generic() {
    genericEventFilter = new GenericPointerConstraintsEventFilter();
    qGuiApp->installEventFilter(genericEventFilter);
}

static bool unlockCursor_generic(QWindow* window) {
    if (!cursorLocked) return false;

    confinedRect = std::nullopt;
    lockedPoint = std::nullopt;
    cursorLocked = false;
    targetWindow = std::nullopt;
    return true;
}

static bool lockCursor_generic(QWindow* window, QPoint pos) {
    if (cursorLocked) unlockCursor_generic(window);
    lockedPoint = pos;
    cursorLocked = true;
    targetWindow = window;
    return true;
}

static bool confineCursor_generic(QWindow* window, QRect rect) {
    if (cursorLocked) unlockCursor_generic(window);
    confinedRect = rect;
    cursorLocked = true;
    targetWindow = window;
    return true;
}

QCursorConstraintsImpl QCursorConstraints::genericImpl {
    /* .init = */ &init_generic,
    /* .lockCursor = */ &lockCursor_generic,
    /* .confineCursor = */ &confineCursor_generic,
    /* .unlockCursor = */ &unlockCursor_generic,
};