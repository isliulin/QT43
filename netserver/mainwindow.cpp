#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    worker = new mwworker(this);
    connect(worker, SIGNAL(ShowStatus(string)), this, SLOT(ShowStatus(string)));

    worker->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ShowStatus(string s)
{
    QString qs;

    qs = qs.fromStdString(s);
    statusBar()->showMessage(qs, 1000);
}

void MainWindow::ShowTip(const char* s)
{
    QString qs;

    qs = s;
    ui->tip->setText(qs);
}

void MainWindow::GetText(string &s)
{
    QString qs;

    qs = ui->text->toPlainText();

    s = qs.toStdString();
}

void MainWindow::on_print_clicked()
{
    worker->msgq_push(1);
}
