#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "FaceDetec.h"
#include "PhFinder.h"
#include <QFileDialog>
#include <QLabel>
#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    curfile(0),
    isEditing(false),
    isSaved(true),
    isCuted(true)
{
    ui->setupUi(this);

    facedetec = new FaceDetec;
    finder = new PhFinder;
    connect(finder, SIGNAL(finded(QString)), this, SLOT(phFinded(QString)));
    connect(finder, SIGNAL(finished()), this, SLOT(findFinished(QString)));
    finder->start();

    QByteArray code("frontface.xml");

    facedetec->loadCode(code);

    connect(ui->phView, SIGNAL(doubleClicked()), this, SLOT(doCut()));
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

bool MainWindow::faceDetec(int index)
{
    QPoint center;
    float sfw, sfh;

    if (index >= ui->listFile->count())
        return false;

    if (!isSaved)
    {
        statusBar()->showMessage("照片未保存", 2000);
        return false;
    }

    if (!isCuted)
    {
        QMessageBox::StandardButton sb;

        sb = QMessageBox::information(this,
                                 "下一张",
                                 "要放弃剪裁这张图片吗?",
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::No);

        if (sb == QMessageBox::No)
            return false;
    }
    isCuted = false;

    ui->listFile->setCurrentRow(index);
    curFileName = ui->listFile->item(index)->text();
    facedetec->loadImage(curFileName);

    ui->phView->showImage(*facedetec->qimage());
    ui->phView->getScaleFactor(sfw, sfh);
    ui->phView->setSize(358*sfw, 441*sfh);
    if (facedetec->detecFace(center))
    {
        moveSelRect(center);
    }
    else
    {
        statusBar()->showMessage("检测失败，需要手动处理", 1500);
    }

    QString cnt;

    cnt = cnt.sprintf("%d", curfile + 1);
    ui->phDone->setText(cnt);

    return true;
}

void MainWindow::on_btOk_clicked()
{
    doCut();
}

void MainWindow::doCut()
{
    QRect cutr;
    QImage *img;

    if (isEditing)
        return;

    if (curFileName.isEmpty())
        return;

    isCuted = true;
    isSaved = false;
    isEditing = true;
    img = ui->phView->getCutImage(*facedetec->qimage(), 358, 441);
    ui->phView->showCut(*img);
}

void MainWindow::moveSelRect(QPoint &center)
{
    float sfw;
    float sfh;

    ui->phView->getScaleFactor(sfw, sfh);
    ui->phView->moveCenter(center.x()*sfw, center.y()*sfh);
}

void MainWindow::saveCut()
{
    if (!isEditing)
    {
        statusBar()->showMessage("已保存", 1500);
        return;
    }

    ui->phView->saveCut(curFileName);
    isSaved = true;
    isEditing = false;
    curFileName.clear();

    statusBar()->showMessage("已保存", 1500);
}

void MainWindow::on_btSelDir_clicked()
{
    QString dir;

    dir = QFileDialog::getExistingDirectory(this, "选择照片目录", "./");
    if (!dir.isEmpty())
    {
       ui->leFolder->setText(dir);
       ui->btFindPh->setEnabled(true);
       ui->btFindPh->setText(QString("查找图片"));
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
        finder->findReq(path, 5000, ui->minFleSize->value() * 1024);
        ui->btFindPh->setEnabled(false);
        ui->btFindPh->setText(QString("查找中.."));
    } 
}

void MainWindow::findFinished()
{
    ui->btFindPh->setText(QString("查找结束"));
}

void MainWindow::on_sizeLock_clicked(bool checked)
{

}

void MainWindow::on_listFile_doubleClicked(const QModelIndex &index)
{

}

void MainWindow::on_btNext_clicked()
{
    if (curfile < ui->listFile->count())
    {
        ui->btNext->setEnabled(false);

        if (faceDetec(curfile))
            curfile ++;

        ui->btNext->setEnabled(true);
    }
}

void MainWindow::on_btSave_clicked()
{
    saveCut();
}

void MainWindow::keyPressEvent(QKeyEvent *ke)
{
    if (ke->key() == Qt::Key_S)
    {
        on_btSave_clicked();
    }
    else if (ke->key() == Qt::Key_D)
    {
        on_btNext_clicked();
    }
}
