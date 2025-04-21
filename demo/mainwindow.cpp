#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "qcursorconstraints.h"
#include <qcursor.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QCursorConstraints::init();

    connect(ui->lockButton, &QPushButton::pressed, this, [=]() {
        QCursorConstraints::lockCursor(window()->windowHandle());
    });

    connect(ui->lockButton, &QPushButton::released, this, [=]() {
        QCursorConstraints::unlockCursor(window()->windowHandle());
    });

    connect(ui->confineButton, &QPushButton::pressed, this, [=]() {
        QCursorConstraints::confineCursor(ui->confineButton);
    });

    connect(ui->confineButton, &QPushButton::released, this, [=]() {
        QCursorConstraints::unlockCursor(window()->windowHandle());
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
