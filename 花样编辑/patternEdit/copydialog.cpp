#include "copydialog.h"
#include "ui_copydialog.h"

copyDialog::copyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::copyDialog)
{
    ui->setupUi(this);
    ui->spinBox_sx->setMinimum(1);
    ui->spinBox_sy->setMinimum(1);
    ui->spinBox_ex->setMinimum(1);
    ui->spinBox_ey->setMinimum(1);
}

copyDialog::~copyDialog()
{
    delete ui;
}

void copyDialog::changeEvent(QEvent *e)
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
