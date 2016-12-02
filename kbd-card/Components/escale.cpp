#include "escale.h"

#define WEIGHTLENGTH    19
#define SCALEIDLENGTH   11

EScale::EScale()
{
    Dev = NULL;
    isRun = false;
    Connected = false;
}

EScale::~EScale()
{
    Close();
}

int EScale::write(unsigned char *buf, int size)
{
    return Dev->write((char*)buf, size);
}

int EScale::read(unsigned char *buf, int size)
{
    if (!Dev->waitForReadyRead(100))
        return 0;

    return Dev->read((char*)buf, size);
}

void EScale::Open()
{
    if (isRun)
        return;

    start();
}

void EScale::Close()
{
    isRun = false;
    wait();
}

bool EScale::isConnected()
{
    return Connected;
}

bool EScale::ReadWeight(void)
{
    unsigned char wcmd[] = {0x02, 0x52, 0x44, 0x53, 0x01, 0xEA, 0x0D};
    unsigned char wrsp[WEIGHTLENGTH];
    int size;

    write(wcmd, sizeof(wcmd));
    size = read(wrsp, WEIGHTLENGTH);

    return parseWeight(wrsp, size, nWeight, tWeight);
}

void EScale::getWeight(float &nw, float &tw)
{
    nw = nWeight;
    tw = tWeight;
}

bool EScale::isZeroDone()
{
    return ZeroDone;
}

void EScale::startZero(int sec)
{
    ZStartTime = time(NULL);
    ZeroDone = false;
    ZeroStart = true;
}

bool EScale::parseWeight(unsigned char *buf, int size, float &nw, float &tw)
{
    char w[16];
    int i, j;
    char status = 1;
    float weight = 0;
    float tare = 0;
    bool ret = false;

    if (size != WEIGHTLENGTH)
    {
        nw = 0;
        tw = 0;
        Connected = false;
        return false;
    }

    if (buf[0] != 0x02 || buf[1] != 0x01 || buf[2] != 'N' || buf[18] != 0x0D)
        goto err;

    if (buf[16] & 0x8) /* 称重溢出 */
        goto err;

    if (!(buf[16] & 0x2)) /* 称重不稳定 */
        goto err;

    /* 当前重量 */
    for (i = 0, j = 0; i < 7; i++)
    {
        if (buf[9-i] == 0x20)
            continue;
        w[j++] = buf[9-i];
    }
    w[j++] = '\0';
    weight = (float)atof(w);

    j = 0;
    /* 皮重 */
    for (i = 0; i < 6; i++)
    {
        if (buf[15-i] == 0x20)
            continue;
        w[j++] = buf[15-i];
    }
    w[j++] = '\0';
    tare = (float)atof(w);

    status = 0;
    ret = true;

err:
    Connected = true;
    if (status == 0)
    {
        nw = weight;
        tw = tare;
    }
    else
    {
        nw = 0;
        tw = 0;
    }

    return ret;
}

bool EScale::devInit(char *name)
{
    Dev->setPortName(name);
    Dev->setBaudRate(9600);
    Dev->setDataBits(QSerialPort::Data8);
    Dev->setParity(QSerialPort::NoParity);
    Dev->setStopBits(QSerialPort::OneStop);
    Dev->setFlowControl(QSerialPort::NoFlowControl);

    return Dev->open(QIODevice::ReadWrite);
}

void EScale::run()
{
    bool ret;
    time_t tm;

    isRun = true;
    Dev = new QSerialPort;

INIT:
    if (!isRun)
        goto EXIT;

    if (!devInit("COM4"))
    {
        msleep(200);
        goto INIT;
    }

    while (isRun)
    {
        ReadWeight();

        if (ZeroStart)
        {
            if (Connected)
            {
                tm = time(NULL);

                if (nWeight != 0)
                {
                    ZStartTime = tm;
                }

                if (tm - ZStartTime >= ZeroTime)
                {
                    ZeroDone = true;
                    ZeroStart = 0;
                }
            }
        }
        msleep(500);
    }

EXIT:
    Dev->close();
    delete Dev;
    Dev = NULL;
}
