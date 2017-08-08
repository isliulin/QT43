#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "projectfile.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    prjFile = new ProjectFile;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    prjFile->AddType("串口终端");
}

void MainWindow::on_pushButton_3_clicked()
{
    prjFile->Save();
}

void MainWindow::on_pushButton_4_clicked()
{
    Session ses;

    ses.type = "串口终端";
    ses.name = "huihua1";

    ses.param["speed"] = "115200";
    ses.param["dev"] = "com1";

    prjFile->AddSession(ses);
}

void MainWindow::on_pushButton_5_clicked()
{
    SesList sl;
    Session ses;

    prjFile->GetSessionList(sl);

    for (int i = 0; i < sl.count(); i ++)
    {
        ses = sl.at(i);
        qDebug("%s", ses.type.toStdString().c_str());
    }
}

void MainWindow::on_pushButton_clicked()
{
    prjFile->Load("project.xml");
}
