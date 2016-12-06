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
    QString qs;

    qs.sprintf("%d", err);
    ui->lb_errcnt->setText(qs);
    qs.sprintf("%d", sus);
    ui->lb_suscnt->setText(qs);
}

void MainWindow::GetMode(int &mode)
{
    QString qs;

    qs = ui->cb_mode->currentText();
    qs = qs.at(0);
    mode = qs.toInt();
}

void MainWindow::on_pushButton_clicked()
{
    ui->te_card->clear();
    ui->lb_errcnt->clear();
    ui->lb_suscnt->clear();
    worker->clear();
}

void MainWindow::GetCntLimit(int &cnt)
{
    cnt = ui->sb_cnt->value();
}

void MainWindow::GetInterval(int &sec)
{
    sec = ui->sb_ival->value();
}

bool MainWindow::GetFileOut(void)
{
    return ui->cb_file->isChecked();
}

void MainWindow::on_pushButton_2_clicked()
{
    worker->msgq_push(1);
}

void MainWindow::on_pushButton_3_clicked()
{
    worker->msgq_push(2);
}

void MainWindow::on_pushButton_4_clicked()
{
    worker->msgq_push(3);
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
