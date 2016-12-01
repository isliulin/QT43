#ifndef KCREADER_H
#define KCREADER_H

#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <stdint.h>

#pragma pack(1)

typedef struct
{
    uint8_t stx;
    uint8_t to;
    uint8_t from;
    uint8_t devid;
    uint8_t rsf;
    uint8_t cmd;
    int     len;
    uint8_t data[1];
}kcmsg_hdr_t;

typedef enum
{
    KCDEV_NOTTNG = 0x00,
    KCDEV_MASTER = 0x01,
    KCDEV_DINGJI = 0x02,
    KCDEV_SHUAKA = 0x03,
}kcdev_t;

typedef enum
{
    INVALID        = 0x0, /* 无效状态 */
    STANDBY        = 0x1, /* 等待收购设备启动新的收烟流程 注1 */
    LEVING         = 0x2, /* 正在定级 */
    ENDLEVING      = 0x3, /* 定级结束 */
    WEIGHTEND      = 0x4, /* 称重结束 */
    SAVED          = 0x5, /* 数据已上传 */
    STORE          = 0x6, /* 存储后面的数据：收购设备要求定级设备存储信息*/
}kcstat_t;

#pragma pack()

class KCReader : public QThread
{
    Q_OBJECT

public:
    KCReader();
    ~KCReader();

    bool Open(const char *name, int br = 115200);
    void Close();

    void SetDevId(uint8_t devid);
    bool DingJiRecover();
    bool DingJiRestart();
    bool DingJiStandby();

private:
    void run();

    int Write(char *buf, int size);
    int Read(char *buf, int size);
    uint8_t BCC(uint8_t *buf, int size);

    void ReqSend(uint8_t to, uint8_t from, uint8_t cmd,
                 uint8_t rsf, uint8_t *buf, int len);

    void ToDingJi(uint8_t cmd, uint8_t msgid, uint8_t *buf, int size);
    void ToShuaKa(uint8_t cmd, uint8_t msgid, uint8_t *buf, int size);

private:
    int Retry;
    QSerialPort Dev;
    uint8_t DevId;
    uint8_t MsgIdToDJ;
    uint8_t MsgIdFmDj;
    uint8_t MsgIdSK;
    bool isrun;
};

#endif // KCREADER_H
