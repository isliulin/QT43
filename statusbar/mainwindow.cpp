#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    statusBarInit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::statusBarInit()
{
    icon.load(":/1.bmp");
    lb1.setPixmap(icon);
    icon.load(":/2.bmp");
    lb2.setPixmap(icon);
    icon.load(":/3.bmp");
    lb3.setPixmap(icon);

    statusBar()->addPermanentWidget(&lb1,1);
    statusBar()->addPermanentWidget(&lb2,1);
    statusBar()->addPermanentWidget(&lb3,1);
}
