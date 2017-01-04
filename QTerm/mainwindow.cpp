#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    term = new QTermWidget;

    setCentralWidget(term);

    test = new Dialog;

    connect(test, &test->outData, term, &term->putData);

    statusBar()->addWidget(ui->test);
    resize(600, term->lineHeight()*24);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_test_clicked()
{
    test->show();
}
