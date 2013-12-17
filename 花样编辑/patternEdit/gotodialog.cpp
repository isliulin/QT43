#include "gotodialog.h"
#include "ui_gotodialog.h"

gotoDialog::gotoDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gotoDialog)
{
    ui->setupUi(this);
    /* 限定用户输入的最小值 */
    ui->spinBox_x->setMinimum(1);
    ui->spinBox_y->setMinimum(1);
}

gotoDialog::~gotoDialog()
{
    delete ui;
}

void gotoDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void gotoDialog::setXYMax(unsigned x, unsigned y)
{
    ui->spinBox_x->setMaximum(x);
    ui->spinBox_y->setMaximum(y);
}

void gotoDialog::getXY(int *x, int *y)
{
    if (x != 0)
    {
        *x = ui->spinBox_x->value();
    }

    if (y != 0)
    {
        *y = ui->spinBox_y->value();
    }
}
