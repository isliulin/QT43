#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "login/framelogin.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(this, SIGNAL(sWindowSwitch(int)), this, SLOT(WindowSwitch(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::WindowSwitch(int wid)
{
    switch (wid)
    {
    case 0:
        FrameLogin *form;

        form = new FrameLogin;
        //form->setAttribute(Qt::WA_DeleteOnClose, true);
        //form->show();
        setCentralWidget(form);
        //ui->view->show();
        break;
    }
}
