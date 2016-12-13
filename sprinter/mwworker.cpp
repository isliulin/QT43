#include "mwworker.h"
#include "mainwindow.h"
#include "libqrencode/qrencode.h"
#include "printer.h"

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

void mwworker::run()
{
    isrun = true;
    QRcode *code;
    printer pt;
    string name, tmp;
    bool ret;
    string text;

    ui->UpdateSerial();

    while (isrun)
    {
        msleep(500);

        ui->GetSerial(name);
        if (!name.empty())
        {
            ret = pt.open(name.c_str());

            if (ret)
                goto NEXT;

            tmp = "打开" + name + "失败";

            emit ShowStatus(tmp);
        }
    }

NEXT:

    while (isrun)
    {
        int msg = 0;

        msgq_get(msg);

        switch (msg)
        {
        case 1:
            if (text.empty())
            {
                emit ShowStatus("请输入内容");
            }
            else
            {
                int w;

                ui->GetWidth(w);
                pt.print(text, w);
            }
            break;
        case 2:
            ui->GetText(text);
            if (text.empty())
            {
                emit ShowStatus("请输入内容");
            }
            else
            {
                code = QRcode_encodeString(text.c_str(), 2, QR_ECLEVEL_L, QR_MODE_8, 0);
                ui->DrawQrCode(code->data, code->width);
                QRcode_free(code);
            }
            break;
        }

        sleep(1);
    }
}
