#include "FaceDetec.h"
#include "ui_FaceDetec.h"

#include "FaceWorker.h"
#include <QFileDialog>

FaceDetec::FaceDetec(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FaceDetec)
{
    ui->setupUi(this);

    worker = new FaceWorker;
    connect(this, SIGNAL(DoWork(int,QByteArray)), worker, SLOT(DoWork(int,QByteArray)));

    worker->start();
}

FaceDetec::~FaceDetec()
{
    delete ui;
}

void FaceDetec::on_pbload_clicked()
{
    QString file;

    file = QFileDialog::getOpenFileName(this, "boy", "./", "ALL FILE(*.*)");
    ui->fncode->setText(file);
    emit DoWork(1, file.toLocal8Bit());
}

void FaceDetec::on_phload_clicked()
{
    QString file;

    file = QFileDialog::getOpenFileName(this, "boy", "./", "ALL FILE(*.*)");
    ui->fnphoto->setText(file);
    emit DoWork(2, file.toLocal8Bit());
}

void FaceDetec::on_detec_clicked()
{
   emit DoWork(3);
}
