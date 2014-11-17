#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

#define VERSION    "1.0.0"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->dockWidget->setWidget(ui->treeWidget);
    project = childitemCreate(0);
    project->setText(0, QString("HMI"));
    image = childitemCreate(project);
    image->setText(0, "0");
    image->setText(1, "文件名");

    menuInit();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::about(void)
{
    QMessageBox mbox;

    mbox.about(this, tr("关于"), tr("版本号: " VERSION));
}

void MainWindow::menuInit(void)
{
    connect(ui->act_about, SIGNAL(triggered()), this, SLOT(about()));
}

QTreeWidgetItem *MainWindow::childitemCreate(QTreeWidgetItem *item)
{
    QTreeWidgetItem *childItem;

    if (item)
    {
        childItem = new QTreeWidgetItem(item);
    }
    else
    {
        childItem = new QTreeWidgetItem(ui->treeWidget);
    }

    return childItem;
}
