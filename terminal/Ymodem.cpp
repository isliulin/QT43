#include "Ymodem.h"
#include "console.h"
#include "crc.h"

#include <QFile>
#include <QFileInfo>

Ymodem::Ymodem(Console *parent)
{
    Stage = msFirst;
    sn = 0;
    isrun = false;
    ui = parent;
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
    msgq_push(data.at(0));
}

int Ymodem::makeFirstRsp(string &name, int size, char *buf, QByteArray &byte)
{
    int len = 133;
    ymhead_t *pkt;
    uint16_t *sum;

    pkt = (ymhead_t*)buf;
    pkt->start = 0x01;
    pkt->sn = 0;
    pkt->nsn = 0xFF;
    memset(pkt->data, 0, sizeof(pkt->data));
    strcpy(pkt->data, name.c_str());
    sprintf(&pkt->data[name.size() + 1], "%d", size);

    sum = (uint16_t*)(buf + 131);
    *sum = crc16(pkt->data, 128);

    byte.resize(len);
    memcpy(byte.data(), buf, len);

    return len;
}

int Ymodem::makeNextRsp(char *data, int size, char *buf, QByteArray &byte)
{
    int len = 0;
    ymhead_t *pkt;
    uint16_t *sum;

    byte.resize(3 + 1024 + 2);
    pkt = (ymhead_t *)byte.data();
    sn ++;
    pkt->start = 0x02;
    pkt->sn = sn;
    pkt->nsn = 0xFF - sn;
    memcpy(pkt->data, data, size);
    if (size < 1024)
    {
        memset(&pkt->data[size], 0, 1024 - size);
    }
    len = 1024 + 3;
    sum = (uint16_t*)(((char*)pkt) + len);
    *sum = crc16(pkt->data, 1024);
    len += 2;

    return len;
}

uint16_t Ymodem::crc16(char *data, int size)
{
    uint16_t sum;

    sum = crc16_ccitt(0, (uint8_t*)data, size);
    sum = ((sum >> 8) | (sum << 8));

    return sum;
}

void Ymodem::msgq_push(int msg)
{
    msgq.push(msg);
    char ch[4];
    sprintf(ch, "%02X", msg);
    qDebug(ch);
}

bool Ymodem::msgq_get(int &msg)
{
    if (msgq.empty())
        return false;

    msg = msgq.front();
    msgq.pop();

    return true;
}

int Ymodem::makeFinishRsp(QByteArray &byte)
{
    int len = 133;
    ymhead_t *pkt;
    uint16_t *sum;

    byte.resize(len);

    pkt = (ymhead_t*)byte.data();
    pkt->start = 0x01;
    pkt->sn = 0;
    pkt->nsn = 0xFF;
    memset(pkt->data, 0, sizeof(pkt->data));

    sum = (uint16_t*)(byte.data() + 131);
    *sum = crc16(pkt->data, 128);

    return len;
}

int Ymodem::makeEotRsp(QByteArray &byte)
{
   byte.resize(1);

   byte[0] = mcEOT;

   return 1;
}

void Ymodem::run()
{
    QString filename;
    char buf[1200];
    char fbuf[1024];
    bool isread = false;
    QByteArray byte;
    int filesize = 0;
    QFile file;
    string stext;
    int remain = 0;

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

    msgq_push(mcREQ);

    isrun = true;
    while (isrun)
    {
        int msg = 0;

        msgq_get(msg);

        switch (Stage)
        {
        case msFirst:
        {
            switch (msg)
            {
            case mcREQ:
            {
                QFileInfo info(filename);

                stext = info.fileName().toStdString();
                remain = file.size();
                filesize = remain;
                makeFirstRsp(stext, remain, buf, byte);
                ui->getData(byte);
            }
            break;
            case mcACK:
            {
                Stage = msReady;
            }
            break;
            }
        }
        break;
        case msReady:
        {
            switch (msg)
            {
            case mcREQ:
            {
                Stage = msData;
            }
            break;
            }
        }
        break;
        case msData:
        {
            if (!isread)
            {
                int size;

                size = file.read(fbuf, 1024);
                remain -= size;
                makeNextRsp(fbuf, size, buf, byte);

                isread = true;

                if (remain == 0)
                {
                    Stage = msEnding;
                }
                ui->getData(byte);
            }

            switch (msg)
            {
            case mcACK:
                isread = false;
                ui->showTransfer(filesize, remain, 1);
                break;
            }
        }
        break;
        case msRepeat:
        {
            ui->getData(byte);
        }
        break;
        case msEnding:
        {
            switch (msg)
            {
            case mcACK:
                makeEotRsp(byte);
                ui->getData(byte);
                Stage = msFinish;
                break;
            }
        }
        break;
        case msFinish:
        {
            switch (msg)
            {
            case mcACK:
                makeFinishRsp(byte);
                ui->getData(byte);
                goto err;
                break;
            case mcNAK:
                makeEotRsp(byte);
                ui->getData(byte);
                break;
            }
        }
        break;
        default:
            break;
        }

        msleep(10);
    }

err:
    ui->showMsg("\n退出Ymodem\n");
}
