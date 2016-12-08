#include "mwworker.h"
#include "mainwindow.h"

#include <QTextStream>
#include <QFile>
#include <QTextCodec>

#include "kcreader.h"
#include "RfCardReader.h"

mwWorker::mwWorker(void *p)
{
    param = p;
    isrun = false;
    errcnt = 0;
    suscnt = 0;
    tolcnt = 0;
}

void mwWorker::clear()
{
    errcnt = 0;
    suscnt = 0;
    tolcnt = 0;
}

void mwWorker::msgq_push(int msg)
{
    msgq.push(msg);
}

bool mwWorker::msgq_get(int &msg)
{
    if (msgq.empty())
        return false;

    msg = msgq.front();
    msgq.pop();

    return true;
}

void mwWorker::run()
{
    isrun = true;

    MainWindow *ui = (MainWindow *)param;
    string name;
    bool ret;
    string tmp;
    int mode;
    QFile data("kclog.txt");
    KCReader theKc;
    RfCardReader theNfc;
    uint8_t lvid[3] = {0x26, 0x25, 0x24};

    data.open(QFile::WriteOnly | QIODevice::Truncate);

    ui->UpdateSerial();
    theKc.SetDevId(1);

    while (isrun)
    {
        msleep(500);

        ui->GetSerial(name);
        if (!name.empty())
        {
            ret = theKc.Open(name.c_str());

            if (ret)
                goto NEXT;

            tmp = "打开" + name + "失败";

            emit ShowStatus(tmp);
        }
    }

NEXT:
    queue <int> cc;
    msgq.swap(cc);

    theKc.InitCont(2, lvid, 1);

    while (isrun)
    {
        int cnt;
        int ival;
        int msg;
        int kcmsg;

        ui->GetCntLimit(cnt);
        if (cnt != 0 && (tolcnt >= cnt))
        {
            tmp = "已达指定次数,重试请清除";
            emit ShowStatus(tmp);

            msleep(1000);
            continue;
        }

        ui->GetInterval(ival);
        ui->GetMode(mode);

        if (msgq_get(msg))
        {
            switch (msg)
            {
            case 1:
                ret = theKc.DingJiRestart();
                break;
            case 2:
                theKc.DingJiStandby();
                break;
            case 4:
            {
                string name = "NameTest";
                theKc.ShuaKaShowCont(100.92, name);
            }
                break;
            case 5:
                ret = theKc.ShuaKaWaitFarmer();
                if (ret)
                {
                    tmp = "收到交烟刷卡应答";
                }
                else
                {
                    tmp = "交烟刷卡无应答";
                }
                emit ShowStatus(tmp);
                break;
            case 6:
                theKc.DingJiSetCont();
                break;
            case 7:
                theKc.ShuaKaEnd();
                break;
            case 8:
                theKc.ShuaKaEnd();
                break;
            case 9:
                ret = theKc.DingJiWaitFarmer();
                if (ret)
                {
                    tmp = "收到定级键盘应答";
                }
                else
                {
                    tmp = "定级键盘无应答";
                }
                emit ShowStatus(tmp);
                break;
            }
        }

        theKc.RecvProcess();

        ret = theKc.CardNumGet(tmp);
        if (ret)
        {
            ui->AddCardMsg(tmp);

            if (ui->GetFileOut())
            {
                tmp += "\r\n";
                data.write(tmp.c_str());
                data.flush();
            }
        }

        if (theKc.LevelCodeGet(tmp))
        {
            ui->AddCardMsg(tmp);
        }

        ui->ShowErrCnt(errcnt, suscnt);

        while (isrun && ival)
        {
            sleep(1);
            ival --;
        }
    }
}
