#include "pastedialog.h"
#include "ui_pastedialog.h"

pasteDialog::pasteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pasteDialog)
{
    ui->setupUi(this);
    ui->spinBox_times->setMinimum(1);
}

pasteDialog::~pasteDialog()
{
    delete ui;
}

void pasteDialog::changeEvent(QEvent *e)
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
