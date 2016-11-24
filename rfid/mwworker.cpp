#include "mwworker.h"
#include "mainwindow.h"
#include "RfCardReader.h"

mwWorker::mwWorker(void *p)
{
    param = p;
    isrun = false;
    errcnt = 0;
    suscnt = 0;
}

void mwWorker::clear()
{
    errcnt = 0;
    suscnt = 0;
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
        msleep(1000);

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
            ret = theNfc.M1BankCardRead(tmp);
            break;
        case 4:
            ret = theNfc.CreditCardRead(tmp);
            break;
        }

        if (ret)
        {
            suscnt ++;
            ui->AddCardMsg(tmp);
        }
        else
        {
            tmp = "读卡失败";
            errcnt ++;

            emit ShowStatus(tmp);
        }

        ui->ShowErrCnt(errcnt, suscnt);
    }
}
