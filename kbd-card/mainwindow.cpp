#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCodec>
#include <QSerialPortInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    msgcnt = 0;
    worker = new mwWorker(this);
    connect(worker, SIGNAL(ShowStatus(string)), this, SLOT(ShowStatus(string)));

    worker->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UpdateSerial()
{
    foreach (const QSerialPortInfo &qspinfo, QSerialPortInfo::availablePorts())
    {
        ui->cb_tty->addItem(qspinfo.portName());
    }
}

void MainWindow::AddCardMsg(string &s)
{
    QTextCodec *codec = QTextCodec::codecForName("GB2312");

    QString qs = codec->toUnicode(s.c_str(), s.size());
    if (msgcnt > 100)
    {

        msgcnt = 0;
    }
    ui->te_card->append(qs);
    msgcnt ++;
}

void MainWindow::ShowStatus(string s)
{
    QString qs;

    qs = qs.fromStdString(s);
    statusBar()->showMessage(qs, 1000);
}

void MainWindow::GetSerial(string &s)
{
    QString qs;

    qs = ui->cb_tty->currentText();

    s = qs.toStdString();
}

void MainWindow::ShowErrCnt(int err, int sus)
{
    err = sus;
    sus = err;
}

void MainWindow::GetMode(int &mode)
{
    mode = 0;
}

void MainWindow::on_pushButton_clicked()
{
    ui->te_card->clear();
    worker->clear();
}

void MainWindow::GetCntLimit(int &cnt)
{
    cnt = 0;
}

void MainWindow::GetInterval(int &sec)
{
    sec = 0;
}

bool MainWindow::GetFileOut(void)
{
    return false;
}

void MainWindow::on_pushButton_2_clicked()
{
    worker->msgq_push(1);
}

void MainWindow::on_pushButton_3_clicked()
{
    worker->msgq_push(2);
}

void MainWindow::on_pushButton_5_clicked()
{
    worker->msgq_push(4);
}

void MainWindow::on_pushButton_6_clicked()
{
    worker->msgq_push(5);
}

void MainWindow::on_pushButton_7_clicked()
{
    worker->msgq_push(6);
}

void MainWindow::on_pushButton_8_clicked()
{
    worker->msgq_push(7);
}

void MainWindow::on_pushButton_9_clicked()
{
    worker->msgq_push(8);
}

void MainWindow::on_pushButton_10_clicked()
{
    worker->msgq_push(9);
}
