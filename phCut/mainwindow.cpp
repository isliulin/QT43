#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "FaceDetec.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    facedetec = new FaceDetec;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbOk_clicked()
{
    QByteArray fc = "test.xml";
    QByteArray fi = "test2.jpg";
    QPoint center;

    facedetec->loadCode(fc);
    facedetec->loadImage(fi);
    facedetec->detecFace(center);

    QImage img;

    img.load(fi);
    img = img.scaled(ui->phView->size(), Qt::KeepAspectRatio);
    ui->phView->setPixmap(QPixmap::fromImage(img));
    ui->phView->moveCenter(center.x(), center.y());
}
