#include "kcreader.h"

#define WAITMS    10

static const char *LevelName[] =
{
    "",
    // 1
    "C1F",
    // 2
    "C2F",
    // 3
    "C3F",
    // 4
    "C1L",
    // 5
    "C2L",
    // 6
    "B1F",
    // 7
    "B2F",
    // 8
    "B1L",
    // 9
    "B1R",
    // 10
    "H1F",
    // 11
    "X1F",
    // 12
    "C3L",
    // 13
    "C4F",
    // 14
    "C4L",
    // 15
    "X2F",
    // 16
    "X3F",
    // 17
    "X1L",
    // 18
    "X2L",
    // 19
    "B3F",
    // 20
    "B4F",
    // 21
    "B2L",
    // 22
    "B3L",
    // 23
    "B2R",
    // 24
    "B3R",
    // 25
    "H2F",
    // 26
    "X2V",
    // 27
    "C3V",
    // 28
    "B2V",
    // 29
    "B3V",
    // 30
    "S1",
    // 31
    "B4L",
    // 32
    "X3L",
    // 33
    "X4L",
    // 34
    "X4F",
    // 35
    "S2",
    // 36
    "CX1K",
    // 37
    "CX2K",
    // 38
    "B1K",
    // 39
    "B2K",
    // 40
    "B3K",
    // 41
    "GY1",
    // 42
    "GY2",
    // 43
    "JW",
};

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

bool KCReader::ShuaKaWaitFarmer()
{
    uint8_t buf[2] = {0x00, 0xFF};
    bool ret;

    CardNum.clear();

    ToShuaKa(0x11, ++MsgIdSK, buf, 2);
    ret = RecvProcess(0x12, 1000);

    return ret;
}

bool KCReader::ShuaKaShowCont(float ctremain, string &name)
{
    kcshowct_t *ct;
    uint8_t buf[64];
    int len;

    ct = (kcshowct_t*)buf;

    ct->status = 0;
    ct->ctremain = (uint32_t)(ctremain * 100);

    sprintf(ct->name, "%s", name.c_str());

    len = sizeof(*ct) + name.size();

    ToShuaKa(0x15, ++MsgIdSK, buf, len);

    return true;
}

bool KCReader::ShuaKaError(int err)
{
    kcshowct_t *ct;
    uint8_t buf[64];
    int len;

    ct = (kcshowct_t*)buf;

    ct->status = err;
    ct->ctremain = 0;

    len = sizeof(*ct);

    ToShuaKa(0x15, ++MsgIdSK, buf, len);

    return true;
}

bool KCReader::InitCont(uint8_t status, uint8_t *lv, int num)
{
    Cont.cstatus = status;

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
    bool ret;

    ToDingJi(0x09, ++MsgIdToDJ, &stat, 1);
    ret = RecvProcess(0x0A, 1000);

    return ret;
}

bool KCReader::ShuaKaProcess(uint8_t cmd, uint8_t *buf, int len)
{
    switch (cmd)
    {
    case 0x12: /* 收购设备命令刷卡设备可以开始烟农刷卡的响应 */
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

bool KCReader::RecvLevel(uint8_t msgid, uint8_t status, uint8_t *lv, int num)
{
    ToDingJi(0x07, msgid, NULL, 0);

    if (msgid == MsgIdFmDj)
        return false;

    MsgIdFmDj = msgid;

    if (status == LEVING)
    {
        LevelCode = LvIdToCode(lv[num-1]);

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

    return true;
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
    bool ret = false;

    switch (cmd)
    {
    case 0x08:
        ret = RecvLevel(msgid, buf[0], &buf[1], len - 1);
        break;
    case 0x06:
        ret = DingJiSetCont();
        break;
    }

    return ret;
}

bool KCReader::RecvProcess(int wcmd, int wmsec)
{
    int size;
    char buf[256];
    bool ret = false;
    kcmsg_hdr_t *hdr;

    Dev.waitForReadyRead(wmsec);

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

    if (!ret)
    {
        QString qs;
        string  ss;
        for (int i = 0; i < size; i ++)
        {
            QString t;

            t.sprintf("%02X ", buf[i]);
            qs += t;
        }

        ss = qs.toStdString();
        qDebug(ss.c_str());
    }

    if (wcmd == hdr->cmd)
    {
        ret = true;
    }
    else
    {
        ret = false;
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

    Dev.clear(QSerialPort::Input);
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

char* KCReader::LvIdToCode(uint8_t id)
{
    if (id > 43)
        id = 0;

    return (char*)LevelName[id];
}
