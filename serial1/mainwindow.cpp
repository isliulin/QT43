#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    worker = new mwWorker(this);
    worker->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ShowValue(float nw, float tw)
{
    QString val;

    val.sprintf("%.2f", nw);
    ui->le_nw->setText(val);

    val.sprintf("%.2f", tw);
    ui->le_tw->setText(val);
}

void MainWindow::ShowEStatus(char *s)
{
    ui->lb_es->setText(s);
}
