#include "mwworker.h"
#include "mainwindow.h"
#include "libqrencode/qrencode.h"
#include "printer.h"
#include "msgproto.h"
#include <QTcpSocket>

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
    QTcpSocket *client;
    msgproto pkt;
    char *rxbuf, *txbuf;
    int size;
    printer pt;

    isrun = true;

    server = new QTcpServer;
    if (!server->listen(QHostAddress::Any, 5555))
    {
        qDebug("err");
        return;
    }

    server->waitForNewConnection(-1);
    client = server->nextPendingConnection();

    rxbuf = new char[1024];
    txbuf = new char[1024];

    while (isrun)
    {
        int uimsg;

        if (client->waitForReadyRead())
        {
            size = client->read(rxbuf, 1024);
        }

        msgq_get(uimsg);

        switch (uimsg)
        {
        case 2:
        {
            string text;

            ui->GetText(text);
            size = text.size();
            if (size == 0)
            {
                emit ShowStatus("请输入内容");
            }
            else
            {
                text = pt.format(text);
                pkt.make(8, txbuf, (char*)text.c_str(), size);
                client->write(txbuf, size);
                client->flush();
            }
        }
        break;
        }
    }
}
