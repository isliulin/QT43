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
    QByteArray fi = "test1.jpg";
    QPoint center;
    QSize imgs, views;

    facedetec->loadCode(fc);
    facedetec->loadImage(fi);
    facedetec->detecFace(center);

    QImage img;

    img.load(fi);
    imgs = img.size();
    img = img.scaled(ui->phView->size(), Qt::KeepAspectRatio);
    views = img.size();
    ui->phView->setPixmap(QPixmap::fromImage(img));

    moveSelRect(center, imgs, views);
}

void MainWindow::moveSelRect(QPoint &center, QSize &imgs, QSize views)
{
    float r;

    r = (float)views.width()/imgs.width();
    ui->phView->moveX(center.x()*r);
}
