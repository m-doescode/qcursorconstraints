
# QCursorConstraints

A simple library for locking the cursor in Qt

## Why QCursorConstraints?

Qt currently lacks an official way to lock the cursor. Many tutorials will suggest using `QCursor::setPos`, however not only is this inconvenient, it also will not work on certain platforms, such as the Wayland display protocol on Linux

QCursorConstraints allows you to easily lock the cursor to a specific point on the window, or confine it to a specific rectangle with a very simple API

## Platform Compatability

These are the currently supported platforms
|Platform|Locking the cursor|Confining the cursor|
|--|:-:|:-:|
|Windows|✅|✅|
|X11 (Linux)|✅|✅|
|Wayland (Linux)|✅|✅|
|MacOS|❔ Untested|❔ Untested|
|Generic*|✅|✅|

\* The generic implementation uses Qt's `QCursor::setPos` directly. Any platform which supports this method will work automatically

## Usage

First include the header:

    #include <qcursorconstraints.h>

Then initialize the library at the entry point of your program:

    QCursorConstraints::init();

Then use any of the functions below:

\- `bool  QCursorConstraints::lockCursor(QWindow* window, QPoint  pos)`
`window` - The window to lock the cursor onto
`pos` - A point local to the window to lock the cursor onto
Locks the cursor at the specified point on a window

\- `bool  QCursorConstraints::lockCursor(QWindow* window)`
`window` - The window to lock the cursor onto
Locks the cursor at its current position

\- `bool  QCursorConstraints::confineCursor(QWindow* window, QRect rect)`
`window` - The window to confine the cursor within
`rect` - Rectangle local to the window representing the region to confine the cursor to
Confines the cursor to a specific region inside the specified window

\- `bool  QCursorConstraints::confineCursor(QWidget* widget, QMargins margin = {1, 1, 1, 1})`
`widget` - The widget to confine the cursor within
`margin` - Margin around the widget's geometry to confine the cursor within
Confines the cursor to a specified widget's geometry inside its parent window. The `margin` parameter can be used to determine how far in the "walls" of the confined region are. A larger number would be the cursor would be further confined into the widget, and smaller/negative numbers would mean a larger region

\- `bool unlockCursor(QWindow* window)`
`window` - The window to unlock the cursor from
Unlocks/unconfines the cursor from the specified window

\- `bool init()`
Initializes the library. Should only be called **once** per instance of your program (Any further calls are ignored.

All of the above functions return a boolean value indicating success. `false` would be returned if the function failed, and `true` if it succeeded. It is not necessary to unlock the cursor before locking/confining it again, the library will handle that for you automatically

Note that although many of the functions require the target window to be passed, it is **not** possible to lock the cursor onto two separate windows at the same time.

Currently, also, the library is unable to handle cases in which the window may unexpectedly move while the cursor is still confined. If this is an expected behavior, make sure to re-lock/confine the cursor whenever the window is moved or resized

## Add to CMake

    find_package(my_sample_lib CONFIG REQUIRED)
    target_link_libraries(main PRIVATE my_sample_lib::my_sample_lib)
