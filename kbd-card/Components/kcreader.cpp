#include "kcreader.h"

#define WAITMS    10

KCReader::KCReader()
{
    DevId = 0;
    MsgIdToDJ = 0;
    MsgIdFmDj = 0;
    MsgIdSK = 0;

    Retry = 400/WAITMS;

    memset(&Cont, 0, sizeof(Cont));
}

KCReader::~KCReader()
{
    isrun = false;
}

bool KCReader::Open(const char *name, int br)
{
    bool ret = true;

    ret = DevInit(name);

    return ret;
}

void KCReader::Close()
{

}

bool KCReader::DevInit(const char *name, int br)
{
    bool ret = true;

    Dev.setPortName(name);
    Dev.setBaudRate(br);
    Dev.setDataBits(QSerialPort::Data8);
    Dev.setParity(QSerialPort::NoParity);
    Dev.setStopBits(QSerialPort::OneStop);
    Dev.setFlowControl(QSerialPort::NoFlowControl);

    ret = Dev.open(QIODevice::ReadWrite);

    return ret;
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

bool KCReader::ShuaKaGet()
{
    uint8_t buf[2] = {0, 1};

    CardNum.clear();

    ToShuaKa(0x11, ++MsgIdSK, buf, 2);

    return true;
}

bool KCReader::ShuaKaEnd()
{
    uint8_t buf[2] = {0, 0xFF};

    ToShuaKa(0x11, ++MsgIdSK, buf, 2);

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

bool KCReader::InitCont(uint8_t status, uint8_t *lv, int num)
{
    Cont.cstatus = status;

    memcpy(Cont.usContract, "62121111111111111111", 19);
    memcpy(Cont.usAssChecker, "0000000000000000", 16);
    Cont.uiSerial = 1;

    for (int i = 0; i < num; i ++)
    {
        Cont.sLevelInfor[i].ucLevel = lv[i];
    }
    Cont.uiLevInforNum = num;

    return true;
}

bool KCReader::DingJiSetCont()
{
    int size;

    size = sizeof(Cont) - (sizeof(levInfor) * (LEVEL_INFOR_MAX - Cont.uiLevInforNum));

    ToDingJi(0x05, ++MsgIdToDJ, (uint8_t*)&Cont, size);

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

bool KCReader::DingJiWaitFarmer()
{
    uint8_t stat = STANDBY;

    ToDingJi(0x09, ++MsgIdToDJ, &stat, 1);

    return true;
}

bool KCReader::ShuaKaProcess(uint8_t cmd, uint8_t *buf, int len)
{
    switch (cmd)
    {
    case 0x12: /* 收购设备命令刷卡设备可以开始烟农刷卡的响应 */
        qDebug("Shuaka");
        break;
    case 0x16: /* 刷卡设备向收购设备传递卡号 */
        /* 1状态+19合同号+1逗号+16初检员卡号 */
        if (len == (1+19+16+1))
        {
            char card[24] = {0};

            memcpy(&card[0], &buf[1], 19);
            CardNum = card;
        }
        break;
    }

    return true;
}

void KCReader::RecvLevel(uint8_t msgid, uint8_t status, uint8_t *lv, int num)
{
    ToDingJi(0x07, msgid, NULL, 0);

    if (msgid == MsgIdFmDj)
        return;

    MsgIdFmDj = msgid;

    if (status == LEVING)
    {
        LevelCode = "B1L";
        Cont.uiLevInforNum = num;
        for (int i = 0; i < num; i ++)
        {
            Cont.sLevelInfor[i].ucLevel = lv[i];
        }
    }
    else if (status == ENDLEVING)
    {
        Cont.uiLevInforNum = num;
        for (int i = 0; i < num; i ++)
        {
            Cont.sLevelInfor[i].ucLevel = lv[i];
        }
    }
}

bool KCReader::LevelCodeGet(string &lv)
{
    if (LevelCode.empty())
        return false;

    lv = LevelCode;
    LevelCode.clear();

    return true;
}

bool KCReader::DingJiProcess(uint8_t cmd, uint8_t *buf, int len, uint8_t msgid)
{

    switch (cmd)
    {
    case 0x08:
        RecvLevel(msgid, buf[0], &buf[1], len - 1);
        break;
    case 0x06:
        DingJiSetCont();
        break;
    }

    return true;
}

bool KCReader::RecvProcess(int &msg)
{
    int size;
    char buf[256];
    bool ret = false;
    kcmsg_hdr_t *hdr;

    size = Read(buf, sizeof(buf));
    if (size == 0)
        return ret;

    hdr = (kcmsg_hdr_t*)buf;

    if (hdr->from == KCDEV_DINGJI)
    {
        ret = DingJiProcess(hdr->cmd, hdr->data, hdr->len, hdr->rsf);
    }
    else if (hdr->from == KCDEV_SHUAKA)
    {
        ret = ShuaKaProcess(hdr->cmd, hdr->data, hdr->len);
    }

    return ret;
}

bool KCReader::CardNumGet(string &card)
{
    if (CardNum.empty())
        return false;

    card = CardNum;
    CardNum.clear();

    return true;
}

int KCReader::Write(char *buf, int size)
{
    int len;

    len = Dev.write(buf, size);
    Dev.waitForBytesWritten(300);

    return len;
}

int KCReader::Read(char *buf, int size)
{
    int len = 0;
    char *pbuf = (char*)buf;
    int cnt = 0;
    kcmsg_hdr_t *msg;
    int dlen;
    uint8_t chk;

    //等待数据
    Dev.waitForReadyRead(WAITMS);
    if (Dev.bytesAvailable() == 0)
    {
       return 0;
    }
    Dev.waitForReadyRead(WAITMS);

    //读取数据头
    len = Dev.read(buf, sizeof(kcmsg_hdr_t));
    if (len != sizeof(kcmsg_hdr_t))
        return 0;

    //检查帧头
    msg = (kcmsg_hdr_t*)buf;
    dlen = msg->len;
    if (msg->stx != 0xAA || (dlen + len) > size)
    {
        Dev.clear(QSerialPort::Input);
        return 0;
    }

    pbuf = buf + len;
    len = 0;

    while (len < dlen)
    {
        int n;

        n = Dev.read(pbuf, dlen - len);

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
    }

    len += sizeof(kcmsg_hdr_t);
    chk = buf[len - 1];

    if (chk != BCC((uint8_t*)&buf[1], len - 2))
        len = 0;

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
    uint8_t *tmp;
    kcmsg_hdr_t *msg;
    int size = 0;

    tmp = new uint8_t[2048];
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

    delete tmp;
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
