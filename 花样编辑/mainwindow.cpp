#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "patternEdit/patternedit.h"

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

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_pb_patternEdit_clicked()
{
    patternEdit *edit = new patternEdit;
    /* 设置窗口关闭后自动删除占用的内存 */
    edit->setAttribute(Qt::WA_DeleteOnClose, true);
    edit->show();
}
