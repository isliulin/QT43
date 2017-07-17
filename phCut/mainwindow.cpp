#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "FaceDetec.h"
#include "PhFinder.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    curfile(0)
{
    ui->setupUi(this);

    facedetec = new FaceDetec;
    finder = new PhFinder;
    connect(finder, SIGNAL(finded(QString)), this, SLOT(phFinded(QString)));
    finder->start();

    QByteArray code("test.xml");

    facedetec->loadCode(code);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::phFinded(QString file)
{
    QString cnt;

    ui->listFile->addItem(file);
    cnt = cnt.sprintf("%d", ui->listFile->count());
    ui->phCnt->setText(cnt);
}

void MainWindow::faceDetec(int index)
{
    QImage img;
    QString file;
    QPoint center;
    QSize sizei, sizev;

    if (index >= ui->listFile->count())
        return;

    ui->listFile->setCurrentRow(index);
    file = ui->listFile->item(index)->text();
    facedetec->loadImage(file);
    img = facedetec->qimage()->scaled(ui->phView->size(), Qt::KeepAspectRatio);
    sizei = facedetec->qimage()->size();
    sizev = img.size();
    ui->phView->setPixmap(QPixmap::fromImage(img));
    if (facedetec->detecFace(center))
    {
        moveSelRect(center, sizei, sizev);
    }
    else
    {
        statusBar()->showMessage("检测失败，需要手动处理", 1500);
    }

    QString cnt;

    cnt = cnt.sprintf("%d", curfile);
    ui->phDone->setText(cnt);
}

void MainWindow::on_pbOk_clicked()
{

}

void MainWindow::moveSelRect(QPoint &center, QSize &imgs, QSize views)
{
    float r;

    r = (float)views.width()/imgs.width();
    ui->phView->moveX(center.x()*r);
}

void MainWindow::on_btSelDir_clicked()
{
    QString dir;

    dir = QFileDialog::getExistingDirectory(this, "选择照片目录", "./");
    if (!dir.isEmpty())
    {
       ui->leFolder->setText(dir);
    }
}

void MainWindow::on_btFindPh_clicked()
{
    QString path;

    path = ui->leFolder->text();
    if (path.isEmpty())
    {
        statusBar()->showMessage("请指定目录", 1500);
    }
    else
    {
        ui->listFile->clear();
        curfile = 0;
        finder->findReq(path, 1000);
    }
}

void MainWindow::on_sizeLock_clicked(bool checked)
{

}

void MainWindow::on_listFile_doubleClicked(const QModelIndex &index)
{
    QString file;

    file = ui->listFile->item(index.row())->text();
    QImage img;

    img.load(file);

    img = img.scaled(ui->phView->size(), Qt::KeepAspectRatio);
    ui->phView->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_btNext_clicked()
{
    if (curfile < ui->listFile->count())
    {
        ui->btNext->setEnabled(false);

        faceDetec(curfile);
        curfile ++;
        if (curfile == ui->listFile->count())
            curfile = 0;

        ui->btNext->setEnabled(true);
    }
}
