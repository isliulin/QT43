#include "mwworker.h"
#include "mainwindow.h"
#include "RfCardReader.h"

mwWorker::mwWorker(void *p)
{
    param = p;
    isrun = false;
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

        if (theNfc.WorkerCardRead(wi))
        {
            tmp = wi.Name + " " + wi.Number;
            ui->AddCardMsg(tmp);
        }
        else
        {
            tmp = "读卡失败";

            emit ShowStatus(tmp);
        }
    }
}
