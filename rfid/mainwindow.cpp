#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    worker = new mwWorker(this);
    connect(worker, SIGNAL(ShowStatus(string)), this, SLOT(ShowStatus(string)));

    worker->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddCardMsg(string &s)
{
    QTextCodec *codec = QTextCodec::codecForName("GB2312");

    QString qs = codec->toUnicode(s.c_str(), s.size());

    ui->te_card->append(qs);
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

void MainWindow::ShowErrCnt(int cnt)
{
    QString qs;

    qs.sprintf("%d", cnt);
    ui->lb_errcnt->setText(qs);
}
