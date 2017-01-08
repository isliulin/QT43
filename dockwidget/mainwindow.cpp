#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

#define VERSION    "1.0.0"

#include "NewSession/NewSession.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->dockWidget->setWidget(ui->twProject);
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

void MainWindow::on_new_s_triggered()
{
    NewSession *ns = new NewSession;

    ns->show();
    ns->exec();

    if (ns->result() == 1)
    {
        NewSesSetting set;

        ns->getSetting(set);

        QTreeWidgetItem *stype = new QTreeWidgetItem;
        stype->setText(0, set["type"]);
        ui->twProject->addTopLevelItem(stype);
        QTreeWidgetItem *child = new QTreeWidgetItem(QStringList()<<"A0");
        stype->addChild(child);
    }
    delete ns;
}
