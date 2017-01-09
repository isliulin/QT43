#include "TftpsSetting.h"
#include "ui_TftpsSetting.h"

TftpsSetting::TftpsSetting(QWidget *parent) :
    Setting(parent),
    ui(new Ui::TftpsSetting)
{
    ui->setupUi(this);
}

TftpsSetting::~TftpsSetting()
{
    delete ui;
}

void TftpsSetting::getSetting(SessionSetting &ns)
{

}
