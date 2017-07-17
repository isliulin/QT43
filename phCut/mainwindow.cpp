#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "FaceDetec.h"
#include "PhFinder.h"
#include <QFileDialog>
#include <QLabel>
#include <QDebug>

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

    showCut = new QLabel(this);
    showCut->hide();
    showCut->resize(ui->phView->size());
    showCut->move(ui->phView->pos());
    showCut->setFrameShape(QFrame::Box);
    showCut->setAlignment(Qt::AlignLeading|Qt::AlignHCenter|Qt::AlignVCenter);

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

void MainWindow::on_btSave_clicked()
{
    QRect rc;
    QImage *img;
    QString fn("save.jpg");
    QSize sizei, sizev;
    int x, y, w, h;
    float sw, sh;

    rc = ui->phView->getCutRect();

    img = facedetec->qimage();
    sizei = img->size();
    sizev = ui->phView->pixmap()->size();

    sw = (float)sizei.width()/sizev.width();
    sh = (float)sizei.height()/sizev.height();
    x = rc.x() * sw;
    y = rc.y() * sh;
    w = rc.width() * sw;
    h = rc.height() *sh;
    img->copy(x, y, w, h).save(fn);
}
