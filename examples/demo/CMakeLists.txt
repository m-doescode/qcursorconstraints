cmake_minimum_required(VERSION 3.16)

project(demo VERSION 0.1 LANGUAGES CXX)

set(CMAKE_MODULE_PATH cmake)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include(CPM)

# If you're using this yourself, you'll want something similar to this instead
CPMAddPackage("gh:m-doescode/qcursorconstraints@1.0.0")

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

qt_add_executable(demo ${PROJECT_SOURCES})
target_include_directories(demo PRIVATE "../core")
target_link_libraries(demo PRIVATE Qt${QT_VERSION_MAJOR}::Widgets QCursorConstraints)

include(GNUInstallDirs)
install(TARGETS demo
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
