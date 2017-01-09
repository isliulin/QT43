#ifndef TFTPSSETTING_H
#define TFTPSSETTING_H

#include "Setting.h"

namespace Ui {
class TftpsSetting;
}

class TftpsSetting : public Setting
{
    Q_OBJECT

public:
    explicit TftpsSetting(QWidget *parent = 0);
    ~TftpsSetting();

    void getSetting(SessionSetting &ns);

private:
    Ui::TftpsSetting *ui;
};

#endif // TFTPSSETTING_H
