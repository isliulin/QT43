#include "kcreader.h"

#define WAITMS    10

KCReader::KCReader()
{
    DevId = 0;
    MsgIdToDJ = 0;
    MsgIdFmDj = 0;
    MsgIdSK = 0;

    Retry = 400/WAITMS;
}

KCReader::~KCReader()
{
    Dev.close();
}

bool KCReader::Open(const char *name, int br)
{
    bool ret = true;

    Dev.setPortName(name);
    Dev.setBaudRate(br);
    Dev.setDataBits(QSerialPort::Data8);
    Dev.setParity(QSerialPort::NoParity);
    Dev.setStopBits(QSerialPort::OneStop);
    Dev.setFlowControl(QSerialPort::NoFlowControl);

    ret = Dev.open(QIODevice::ReadWrite);
    if (ret)
    {
       // start();
    }

    return ret;
}

void KCReader::Close()
{
    Dev.close();
}

void KCReader::run()
{
    char rxbuf[256];
    int size;

    isrun = true;

    while (isrun)
    {
        if (!Dev.waitForReadyRead(100))
            continue;

        size = Dev.read(rxbuf, sizeof(rxbuf));

        QString lstr;

        for (int i = 0; i < size; i ++)
        {
            QString tmp;

            tmp.sprintf("%0X ", rxbuf[i]);
            lstr += tmp;
        }
        string str = lstr.toStdString();
        qDebug(str.c_str());
    }
}

void KCReader::SetDevId(uint8_t devid)
{
    DevId = devid;
}

bool KCReader::DingJiRecover()
{
    ToDingJi(0x03, ++MsgIdToDJ, NULL, 0);

    return true;
}

bool KCReader::ShuaKaShowCont(float ctremain, string &name)
{
    kcshowct_t *ct;
    uint8_t buf[64];
    int len;

    ct = (kcshowct_t*)buf;

    ct->status = 0;
    ct->ctremain = (uint32_t)(ctremain * 100);
    ct->namelen = name.size();

    sprintf(ct->name, "%s", name.c_str());

    len = sizeof(*ct) + ct->namelen;

    ToShuaKa(0x15, ++MsgIdSK, buf, len);

    return true;
}

bool KCReader::DingJiRestart()
{
    ToDingJi(0x0B, ++MsgIdToDJ, NULL, 0);

    return true;
}

bool KCReader::DingJiStandby()
{
    uint8_t stat = STANDBY;

    ToDingJi(0x05, ++MsgIdToDJ, &stat, 1);

    return true;
}

int KCReader::Write(char *buf, int size)
{
    int len;

    Dev.clear();
    len = Dev.write((char*)buf, size);

    return len;
}

int KCReader::Read(char *buf, int size)
{
    int len = 0;
    char *pbuf = (char*)buf;
    int cnt = 0;

    do
    {
        int n;

        n = Dev.read(pbuf, size - len);

        if (n == -1)
            break;

        if (n == 0)
        {
            Dev.waitForReadyRead(WAITMS);
            if (cnt ++ > Retry)
                break;

            continue;
        }

        pbuf += n;
        len += n;
        cnt = 0;
    }while (len < size);

    return len;
}

void KCReader::ToDingJi(uint8_t cmd, uint8_t msgid, uint8_t *buf, int size)
{
    ReqSend(KCDEV_DINGJI, KCDEV_MASTER, cmd, msgid, buf, size);
}

void KCReader::ToShuaKa(uint8_t cmd, uint8_t msgid, uint8_t *buf, int size)
{
    ReqSend(KCDEV_SHUAKA, KCDEV_MASTER, cmd, msgid, buf, size);
}

void KCReader::ReqSend(uint8_t to, uint8_t from, uint8_t cmd,
                       uint8_t rsf, uint8_t *buf, int len)
{
    uint8_t tmp[256];
    kcmsg_hdr_t *msg;
    int size = 0;

    msg = (kcmsg_hdr_t*)tmp;

    msg->stx = 0xAA;
    msg->to = to;
    msg->from = from;
    msg->devid = DevId;
    msg->rsf = rsf;
    msg->cmd = cmd;
    msg->len = len;

    if (buf && len)
    {
        memcpy(msg->data, buf, len);
        size = len;
    }
    else
    {
        len = 0;
    }

    size = sizeof(*msg) - 1 + len;
    msg->data[len] = BCC(&tmp[1], size - 1);

    Write((char*)tmp, size + 1);
}

uint8_t KCReader::BCC(uint8_t *buf, int size)
{
    uint32_t tmp = 0;

    for (int i = 0; i < size; i ++)
    {
        tmp += buf[i];
    }

    return (uint8_t)tmp;
}
