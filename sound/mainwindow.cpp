#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtMultimedia/QSound>
#include "KeyReader.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pKbd->setRecver(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_play_clicked()
{
    QString fn;

    fn = ui->pfile->text();
    if (fn.isEmpty())
        return;

    QSound::play(fn);
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_1)
    {
        on_play_clicked();
    }
}
