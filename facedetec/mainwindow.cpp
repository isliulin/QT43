#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "FaceDetec.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fdui = new FaceDetec;
    setCentralWidget(fdui);
}

MainWindow::~MainWindow()
{
    delete ui;
}
