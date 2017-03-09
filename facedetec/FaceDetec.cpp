#include "FaceDetec.h"
#include "ui_FaceDetec.h"

#include "FaceWorker.h"

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
    QByteArray file = "E:/test.jpg";

    emit DoWork(1, file);
}
