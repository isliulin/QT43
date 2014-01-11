#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fileManage/filemanage.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    fileManage *manage = new fileManage;

    /* 设置窗口关闭后自动删除占用的内存 */
    manage->setAttribute(Qt::WA_DeleteOnClose, true);
    manage->show();
}
