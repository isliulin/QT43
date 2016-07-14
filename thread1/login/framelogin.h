#ifndef FRAMELOGIN_H
#define FRAMELOGIN_H

#include <QFrame>

#include "loginworker.h"

namespace Ui {
class FrameLogin;
}

class FrameLogin : public QFrame
{
    Q_OBJECT

public:
    explicit FrameLogin(QWidget *parent = 0);
    ~FrameLogin();

    void ShowMsg(const char *s);
    void ShowNum(int n);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::FrameLogin *ui;
    LoginWorker worker;
};

#endif // FRAMELOGIN_H
