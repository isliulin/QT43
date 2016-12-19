#include "Ymodem.h"
#include "console.h"

Ymodem::Ymodem(Console *parent)
{
    isrun = false;
    ui =parent;
}

void Ymodem::close()
{
   if (!isFinished())
   {
       isrun = false;
       wait();
   }
}

void Ymodem::put(const QByteArray &data)
{
    if (data.at(0) == 'C')
    {
        msgq_push(1);
    }
}

int Ymodem::makeFirstRsp(const char *name, char *buf)
{
    int len = 0;

    return len;
}

int Ymodem::makeNextRsp(char *data, int size, char *buf)
{
    int len = 0;

    return len;
}

void Ymodem::msgq_push(int msg)
{
    msgq.push(msg);
}

bool Ymodem::msgq_get(int &msg)
{
    if (msgq.empty())
        return false;

    msg = msgq.front();
    msgq.pop();

    return true;
}

void Ymodem::run()
{
    QString filename;
    enum modemState state = msWaitReq;
    char buf[1200];
    char fbuf[1024];
    bool isread = false;
    QByteArray byte;
    int filesize;
    QFile file;
    string stext;

    ui->showMsg("\n已启动Ymodem\n");
    ui->getFile(filename);
    if (filename.isEmpty())
    {
       stext = "错误:文件名为空";
       emit ui->showStatus(stext);
       goto err;
    }

    file.setFileName(filename);
    if (!file.open(QFile::ReadOnly))
    {
        stext = "错误:打开文件失败";
        emit ui->showStatus(stext);
        goto err;
    }

    isrun = true;
    while (isrun)
    {
        int msg;

        switch (state)
        {
        case msWaitReq:
        case msWaitAck:
        {
            if (!isread)
            {
                stext = "读取文件";
                emit ui->showStatus(stext);
                filesize = file.read(fbuf, 1024);
                isread = true;
            }
            msleep(50);
        }
        break;
        case msFirst:
        {
            int size = makeFirstRsp("test.bin", buf);

            byte.clear();
            for (int i = 0; i < size; i ++)
                byte.append(buf[i]);

            ui->getData(byte);
            state = msWaitAck;
        }
        break;
        case msNext:
        {
            int size = makeNextRsp(fbuf, filesize, buf);

            byte.clear();
            for (int i = 0; i < size; i ++)
                byte.append(buf[i]);

            ui->getData(byte);
            state = msWaitAck;
        }
        break;
        case msRepeat:
        {
            ui->getData(byte);
            state = msWaitAck;
        }
        break;
        }

        if (msgq_get(msg))
        {
            switch (state)
            {
            case msWaitReq:
            {
                if (msg = 1)
                {
                    state = msFirst;
                }
            }
            break;
            case msWaitAck:
            {
                if (msg = 1)
                {
                    state = msFirst;
                }
            }
            break;
            default:
                break;
            }
        }
    }

err:
    ui->showMsg("\n退出Ymodem\n");
}
