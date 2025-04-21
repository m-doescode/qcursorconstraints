cmake_minimum_required(VERSION 3.16)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Widgets)

add_library(QCursorConstraints STATIC
  qcursorconstraints.cpp
  qcursorconstraints.h
  platform/generic.cpp
)

if (WIN32)
  target_sources(QCursorConstraints PRIVATE
    platform/win32.cpp
  )
else()
  target_sources(QCursorConstraints PRIVATE
    platform/wayland.cpp
    platform/wayland-pointer-constraints-unstable-v1-client-protocol.h
    platform/wayland-pointer-constraints-unstable-v1-client-protocol.cpp
  )
endif()

target_link_libraries(QCursorConstraints PRIVATE Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::GuiPrivate Qt${QT_VERSION_MAJOR}::Widgets)

if (NOT WIN32)
  find_package(PkgConfig)
  pkg_check_modules(WAYLAND REQUIRED wayland-client)

  target_include_directories(QCursorConstraints PRIVATE ${WAYLAND_INCLUDE_DIRS})
  target_link_libraries(QCursorConstraints PRIVATE ${WAYLAND_LIBRARIES})
endif()

target_compile_definitions(QCursorConstraints PRIVATE QCURSORCONSTRAINTS_LIBRARY)
