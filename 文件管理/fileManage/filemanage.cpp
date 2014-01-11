#include "filemanage.h"
#include "ui_filemanage.h"
#include <QTimer>
#include <QFileSystemModel>

fileManage::fileManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fileManage)
{
    ui->setupUi(this);

    ui->trv_hdd->setModel(&model[0]);
    ui->trv_udisk->setModel(&model[1]);

    worker.start();
    // 延迟初始化表格
    QTimer::singleShot(100, this, SLOT(initTree()));
}

fileManage::~fileManage()
{
    worker.exit();
    delete ui;
}

void fileManage::initTree()
{
    // 显示硬盘文件
    worker.listFile(ui->trv_hdd, QString("/root"));
    // 显示U盘文件
    worker.listFile(ui->trv_udisk, QString("/media"));
}
