#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QVariant>
#include <QDockWidget>

#define VERSION    "1.0.0"

#include "NewSession/NewSession.h"
#include "SerialAssistant/SerialAssistant.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget* p = takeCentralWidget();
    if(p)
        delete p;
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
        SessionSetting set;

        ns->getSetting(set);

        addSession(set);
    }
    delete ns;
}

void MainWindow::addSession(SessionSetting &set)
{
    QTreeWidgetItem *child;

    child = addSessionProject(set);

    addSessionWindow(set, child);
    int cnt = dwlist.count();
    if (cnt > 1)
    {
        QDockWidget *f, *s;

        f = dwlist.at(cnt - 2);
        s = dwlist.at(cnt - 1);
        tabifyDockWidget(f, s);
    }
}

QTreeWidgetItem* MainWindow::addSessionProject(SessionSetting &set)
{
    QTreeWidgetItem *type = new QTreeWidgetItem;
    bool addtype = true;

    for (int i = 0; i < ui->twProject->topLevelItemCount(); i ++)
    {
        QTreeWidgetItem *tmptype = ui->twProject->topLevelItem(i);

        if (tmptype->text(0) == set["type"])
        {
            delete type;
            type = tmptype;
            addtype = false;
            break;
        }
    }

    type->setText(0, set["type"]);
    if (addtype)
    {
        ui->twProject->addTopLevelItem(type);
    }

    QTreeWidgetItem *child = new QTreeWidgetItem;
    child->setText(0, set["name"]);
    type->addChild(child);

    return child;
}

void MainWindow::addSessionWindow(SessionSetting &set, QTreeWidgetItem *item)
{
    QDockWidget *dock;
    QVariant var;

    dock= new QDockWidget(set["name"], this);
    dock->setAllowedAreas(Qt::RightDockWidgetArea);

    var.setValue(dock);

    dwlist.append(dock);

    if (set["type"] == "串口终端")
    {
        item->setData(0,Qt::UserRole, var);

        return;
    }

    if (set["type"] == "串口助手")
    {
        SerialAssistant *win = new SerialAssistant(dock);

        addDockWidget(Qt::RightDockWidgetArea, dock);

        item->setData(0,Qt::UserRole, var);
        dock->setWidget(win);
        return;
    }

    dwlist.removeLast();
    delete dock;
}
