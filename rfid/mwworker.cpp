#include "mwworker.h"
#include "mainwindow.h"
#include "RfCardReader.h"

#include <QTextStream>
#include <QFile>
#include <QTextCodec>

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

void mwWorker::run()
{
    isrun = true;

    MainWindow *ui = (MainWindow *)param;
    string name;
    RfCardReader theNfc;
    bool ret;
    WorkerInfo wi;
    string tmp;
    int mode;
    QFile data("rflog.txt");

    data.open(QFile::WriteOnly | QIODevice::Truncate);

    ui->UpdateSerial();

    while (isrun)
    {
        msleep(500);

        ui->GetSerial(name);
        if (!name.empty())
        {
            ret = theNfc.Open(name.c_str());
            if (ret)
                goto NEXT;

            tmp = "打开" + name + "失败";

            emit ShowStatus(tmp);
        }
    }

NEXT:

    while (isrun)
    {
        int cnt;
        int ival;

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

        switch (mode)
        {
        case 1:
            ret = theNfc.WorkerCardRead(wi);
            tmp = wi.Name + " " + wi.Number;
            break;
        case 2:
            ret = theNfc.CreditCardRead(tmp);
            if (!ret)
            {
                ret = theNfc.M1BankCardRead(tmp);
            }
            break;
        case 3:
        {
            string str;

            ret = theNfc.M1BankCardRead(tmp);
            str = theNfc.readText().toStdString();
            str += "\r\n";
            data.write(str.c_str());
            data.flush();
        }
            break;
        case 4:
            ret = theNfc.CreditCardRead(tmp);
            break;
        }

        tolcnt ++;

        if (ret)
        {
            suscnt ++;
            ui->AddCardMsg(tmp);

            if (ui->GetFileOut())
            {
                tmp += "\r\n";
                data.write(tmp.c_str());
                data.flush();
            }
        }
        else
        {
            tmp = theNfc.ErrMsg();
            errcnt ++;

            emit ShowStatus(tmp);
        }

        ui->ShowErrCnt(errcnt, suscnt);

        while (isrun && ival)
        {
            sleep(1);
            ival --;
        }
    }
}
