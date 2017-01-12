#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mwworker.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    worker = new mwworker(this);
    worker->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_login_clicked()
{
    worker->msgq_push(1);
}
