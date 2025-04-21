#include <qpa/qplatformnativeinterface.h>
#include <QGuiApplication>
#include "../qcursorconstraints.h"
// #include <winuser.h>

static void init_win32() {
}

static bool unlockCursor_win32(QWindow* window) {
}

static bool lockCursor_win32(QWindow* window, QPoint pos) {
    QPoint globalPos = window->mapToGlobal(pos);
    
}

static bool confineCursor_win32(QWindow* window, QRect rect) {
}

QCursorConstraintsImpl QCursorConstraints::waylandImpl {
    .init = &init_win32,
    .lockCursor = &lockCursor_win32,
    .confineCursor = &confineCursor_win32,
    .unlockCursor = &unlockCursor_win32,
};
