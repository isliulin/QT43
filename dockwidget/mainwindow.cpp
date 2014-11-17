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
