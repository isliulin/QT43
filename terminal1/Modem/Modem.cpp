#include "Modem.h"
#include "ui_modem.h"

Modem::Modem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Modem)
{
    ui->setupUi(this);
}

Modem::~Modem()
{
    delete ui;
}

void Modem::getFile(QString &name)
{

}

void Modem::showTransfer(int filesize, int remain, float speed)
{

}
