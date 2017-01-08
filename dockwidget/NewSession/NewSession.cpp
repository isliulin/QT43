#include "NewSession.h"
#include "ui_NewSession.h"

NewSession::NewSession(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewSession)
{
    ui->setupUi(this);
}

NewSession::~NewSession()
{
    delete ui;
}

void NewSession::getSetting(NewSesSetting &s)
{
    QString val = "xxx";

    nss["type"] = ui->sesType->currentItem()->text();
    nss["name"] = "name";
    nss["param"] = "ss";

    s = nss;
}
