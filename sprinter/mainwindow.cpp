#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>

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

void MainWindow::DrawQrCode()
{
    QImage img = QImage(50,50,QImage::Format_RGB16);
    QPainter painter(&img);

    QColor bg(Qt::white);
    painter.setBrush(bg);
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, 50, 50);
    QColor fg(Qt::black);
    painter.setBrush(fg);

    QRectF r(10, 10, 1, 1);
    painter.drawRects(&r, 1);

    QPixmap map = QPixmap::fromImage(img);

    ui->qrcode->setPixmap(map);
}
