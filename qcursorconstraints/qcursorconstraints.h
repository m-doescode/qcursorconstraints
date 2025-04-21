#ifndef QCURSORCONSTRAINTS_H
#define QCURSORCONSTRAINTS_H

#include <QPoint>
#include <QWindow>

struct QCursorConstraintsImpl {
    void (*init)();
    bool (*lockCursor)(QWindow* window, QPoint pos);
    bool (*confineCursor)(QWindow* window, QRect rect);
    bool (*unlockCursor)(QWindow* window);
};

class QCursorConstraints
{
    static bool initialized;
    static QCursorConstraintsImpl* activeImpl;

    static QCursorConstraintsImpl genericImpl;
#ifdef _WIN32
    static QCursorConstraintsImpl win32Impl;
#else
    static QCursorConstraintsImpl waylandImpl;
    // static QCursorConstraintsImpl xcbImpl;
#endif
public:
    static bool init();
    static bool lockCursor(QWindow* window);
    static bool lockCursor(QWindow* window, QPoint pos);
    static bool confineCursor(QWidget* window, QMargins margin = {1, 1, 1, 1});
    static bool confineCursor(QWindow* window, QRect rect);
    static bool unlockCursor(QWindow* window);
};

#endif // QCURSORCONSTRAINTS_H
