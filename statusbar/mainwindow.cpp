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

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton button;

    button = QMessageBox::question(this, tr("退出程序"),
        QString(tr("警告：程序有一个任务正在运行中，是否结束操作退出?")),
        QMessageBox::Yes | QMessageBox::No);

    if (button == QMessageBox::No)
    {
        event->ignore();  //忽略退出信号，程序继续运行
    }
    else if (button == QMessageBox::Yes)
    {
        event->accept();  //接受退出信号，程序退出
    }
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
