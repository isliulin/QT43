#include "framelogin.h"
#include "ui_framelogin.h"


FrameLogin::FrameLogin(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FrameLogin)
{
    ui->setupUi(this);

    worker.setParent(this);
    worker.start();
}

FrameLogin::~FrameLogin()
{
    worker.exit();
    delete ui;
}

void FrameLogin::ShowMsg(const char *s)
{
    ui->label->setText(QString(s));
}

void FrameLogin::ShowNum(int n)
{
    QString s;

    s.sprintf("%d", n);
    ui->label->setText(s);
}

void FrameLogin::on_pushButton_clicked()
{
    if (!worker.isRunning())
    {
        worker.start();
    }
}

void FrameLogin::on_pushButton_2_clicked()
{
    worker.exit();
}
