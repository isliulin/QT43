#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "KeyReader.h"
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //kbd = new KeyReader;
    //kbd->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QKeyEvent *evtp = new QKeyEvent(QEvent::KeyPress, Qt::Key_A, Qt::NoModifier,"a");
    QKeyEvent *evtr = new QKeyEvent(QEvent::KeyRelease, Qt::Key_A, Qt::NoModifier,"a");

ui->lineEdit->setFocus();
    QObject *recv = (QObject *)qApp->thread();
    //recv = QApplication::focusObject();
    if (recv)
    {

        qDebug("rrr");
        QApplication::postEvent(qApp, evtp);
        QApplication::postEvent(qApp, evtr);
    }
}
