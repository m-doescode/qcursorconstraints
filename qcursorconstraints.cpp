#include <QGuiApplication>
#include <QWidget>

#include "qcursorconstraints.h"

bool QCursorConstraints::initialized = false;
QCursorConstraintsImpl* QCursorConstraints::activeImpl = nullptr;

bool QCursorConstraints::init() {
    if (initialized) return false;

    QString platformName = QGuiApplication::platformName();
#ifdef _WIN32
    // Currently broken
    // if (platformName == "windows") {
        // activeImpl = &win32Impl;
    // } else
#else
    if (platformName == "wayland") {
        activeImpl = &waylandImpl;
    } else
    // } else if (platformName == "xcb") {
    //     activeImpl = &xcbImpl;
    // } else
#endif
    {
        activeImpl = &genericImpl;
    }

    activeImpl->init();

    initialized = true;
    return true;
}

bool QCursorConstraints::lockCursor(QWindow* window, QPoint pos) {
    return activeImpl->lockCursor(window, pos);
}

bool QCursorConstraints::lockCursor(QWindow* window) {
    return activeImpl->lockCursor(window, window->mapFromGlobal(QCursor::pos()));
}

bool QCursorConstraints::unlockCursor(QWindow* window) {
    return activeImpl->unlockCursor(window);
}

bool QCursorConstraints::confineCursor(QWidget* widget, QMargins margin) {
    QWindow* window = widget->window()->windowHandle();
    return activeImpl->confineCursor(window, widget->geometry().marginsRemoved(margin));
}

bool QCursorConstraints::confineCursor(QWindow* window, QRect rect) {
    return activeImpl->confineCursor(window, rect);
}
