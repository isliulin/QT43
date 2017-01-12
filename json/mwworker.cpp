#include "mwworker.h"
#include "mainwindow.h"

#include <QTcpSocket>
#include "MsgProto.h"
#include "CsIntf.h"

mwworker::mwworker(QWidget *parent)
{
    ui = (MainWindow*)parent;
    isrun = false;
}

void mwworker::msgq_push(int msg)
{
    msgq.push(msg);
}

bool mwworker::msgq_get(int &msg)
{
    if (msgq.empty())
        return false;

    msg = msgq.front();
    msgq.pop();

    return true;
}

void mwworker::ShowStatus(const char *s)
{
   string ss = s;
}

void mwworker::run()
{
    intf = new CsIntf;

    isrun = true;

    intf->SetHost("192.168.0.139", 5555);

    while (isrun)
    {
        int uimsg = 0;
        QByteArray *arr = NULL;

        msgq_get(uimsg);

        switch (uimsg)
        {
        case 1:
        {
            intf->DevLogin();
        }
        break;
        }

        if (arr)
        {
            QString str(*arr);
            qDebug(str.toStdString().c_str());
        }
    }
}
