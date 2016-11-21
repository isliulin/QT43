#include "mwworker.h"
#include "mainwindow.h"
#include "escale.h"

mwWorker::mwWorker(void *p)
{
    param = p;
    isrun = false;
}

void mwWorker::run()
{
    MainWindow *ui;
    EScale es;
    float nw, tw;

    ui = (MainWindow*)param;
    isrun = true;
    es.Open();

    while (isrun)
    {
        msleep(500);

        es.getWeight(nw, tw);

        if (es.isConnected())
        {
            ui->ShowEStatus("已连接");
        }
        else
        {
            ui->ShowEStatus("未连接");
        }

        ui->ShowValue(nw, tw);
    }

    es.Close();
}
