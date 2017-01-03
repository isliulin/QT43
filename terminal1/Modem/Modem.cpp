#include "Modem.h"
#include "ui_modem.h"

#include "Ymodem.h"

Modem::Modem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Modem)
{
    ui->setupUi(this);

    ym = new Ymodem(this);
}

Modem::~Modem()
{
    delete ui;
}

void Modem::getFile(QString &name)
{
    name = filename;
}

void Modem::setFile(QString &name)
{
    filename = name;
}

void Modem::showTransfer(int filesize, int remain, float speed)
{

}

void Modem::startTransfer(char type)
{
    show();
    ym->start();
}

void Modem::putData(const QByteArray &data)
{
    ym->put(data);
}
