#include <qpa/qplatformnativeinterface.h>
#include <QGuiApplication>
#include "../qcursorconstraints.h"
#include <windows.h>

static void init_win32() {
}

static bool unlockCursor_win32(QWindow* window) {
    return ClipCursor(NULL);
}

static bool lockCursor_win32(QWindow* window, QPoint pos) {
    QPoint globalPos = window->mapToGlobal(pos);
    RECT winRect { globalPos.x(), globalPos.y(), globalPos.x(), globalPos.y() };
    return ClipCursor(&winRect);
}

static bool confineCursor_win32(QWindow* window, QRect rect) {
    QRect globalRect(window->mapToGlobal(rect.topLeft()), rect.size());
    RECT winRect { globalRect.left(), globalRect.top(), globalRect.right(), globalRect.bottom() };
    return ClipCursor(&winRect);
}

QCursorConstraintsImpl QCursorConstraints::win32Impl {
    /* .init = */ &init_win32,
    /* .lockCursor = */ &lockCursor_win32,
    /* .confineCursor = */ &confineCursor_win32,
    /* .unlockCursor = */ &unlockCursor_win32,
};
