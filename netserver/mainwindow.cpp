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

void MainWindow::DrawQrCode(uint8_t *data, int width)
{
    int scale = 2;
    int imgsize = scale * width;
    QImage img = QImage(imgsize, imgsize, QImage::Format_RGB16);
    QPainter painter(&img);

    QColor bg(Qt::white);
    painter.setBrush(bg);
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, imgsize, imgsize);
    QColor fg(Qt::black);
    painter.setBrush(fg);

    for(int y = 0; y < width; y ++)
    {
        for(int x = 0; x < width; x++)
        {
            uint8_t b = data[y * width + x];
            if(b & 0x01)
            {
                QRectF r(x * scale, y * scale, scale, scale);
                painter.drawRects(&r, 1);
            }
        }
    }

    QPixmap map = QPixmap::fromImage(img);

    ui->qrcode->setPixmap(map);
}

void MainWindow::UpdateSerial()
{
    foreach (const QSerialPortInfo &qspinfo, QSerialPortInfo::availablePorts())
    {
        ui->cb_tty->addItem(qspinfo.portName());
    }
}

void MainWindow::GetText(string &s)
{
    QString qs;

    qs = ui->text->toPlainText();

    s = qs.toStdString();
}

void MainWindow::GetWidth(int &w)
{
    w = ui->width->value();
}

void MainWindow::GetSerial(string &s)
{
    QString qs;

    qs = ui->cb_tty->currentText();

    s = qs.toStdString();
}

void MainWindow::on_print_clicked()
{
    worker->msgq_push(1);
}

void MainWindow::on_pushButton_clicked()
{
    worker->msgq_push(2);
}
