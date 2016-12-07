#ifndef KCREADER_H
#define KCREADER_H

#include <QThread>
#include <QtSerialPort/QSerialPort>

#include <stdint.h>
#include <string>

using namespace std;

#define LEVEL_INFOR_MAX 72

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

typedef struct
{
    uint8_t cansell: 2;    /* 是否允许交售(0是, 1否,2合同类别异常，3扎把类型异常) */
    uint8_t deleted: 1;    /* 是否被删除(0否,1是) */
    uint8_t contract:2;    /* 合同类别 */
    uint8_t bundle:  2;    /* 扎把类型 */
    uint8_t reserve: 1;    /* 保留(默认000000b) */
}inforFlags;

typedef struct levInfor
{
    uint8_t ucLevel;            /* 烟叶等级 注4 */
    inforFlags sFlags;                /* 定级标志 */
    unsigned short usWeight;          /* 该等级重量不会超过655公斤 注2 */
    uint32_t   levelTime;         /* 定级时间，从1970年到目前的秒数 */
    unsigned short   uiTime;          /* 磅码时间，从1970年到目前的秒数, 与定级时间的差值 */
    unsigned short usTare;            /* 皮重不会超过655公斤(单位:10g) 注2*/
}levInfor;

/* 此结构与定级键盘共用，不能修改 */
typedef struct
{
    uint8_t cstatus;                        /* status 1个字节的状态数据 */
    char usContract[19];           /* 19个ascii字符的合同号 */
    char usAssChecker[16];         /* 16个初检员ID */
    uint32_t uiSerial;                  /* 流水号 */
    uint32_t uiTotalAccnt;              /* 总金额 注2 */
    uint32_t uiTotalWeight;             /* 总重量 注2 */
    uint32_t uiHTL[4];                  /* 合同量 注2*/
    uint32_t uiHTYL[4];                 /* 合同余量 注2 */
    uint32_t uiDRYL;                    /* 当日余量 注2 */
    uint8_t ucBreedGrades[104];     /* 品种，分等级信息 */
    uint32_t uiLevInforNum;             /* 定级级别信息个数 */
    levInfor sLevelInfor[LEVEL_INFOR_MAX];  /* 级别信息详细情况 注3 */
}kccont_t;

typedef struct
{
    uint8_t status;
    uint32_t ctremain;
    uint8_t namelen;
    char name[1];
}kcshowct_t;
#pragma pack()

class KCReader : public QThread
{
    Q_OBJECT

public:
    KCReader();
    ~KCReader();

    bool Open(const char *name, int br = 115200);
    void Close();

    bool RecvProcess(int &msg);

    void SetDevId(uint8_t devid);
    bool DingJiRecover();
    bool DingJiRestart();
    bool DingJiStandby();
    bool DingJiWaitFarmer();
    bool ShuaKaShowCont(float ctremain, string &name);
    bool ShuaKaGet();
    bool ShuaKaEnd();
    bool InitCont(uint8_t status, uint8_t *lv, int num);
    bool DingJiSetCont();
    bool CardNumGet(string &card);
    bool LevelCodeGet(string &lv);

private:
    bool DevInit(const char *name, int br = 115200);
    bool DingJiProcess(uint8_t cmd, uint8_t *buf, int len, uint8_t msgid);
    bool ShuaKaProcess(uint8_t cmd, uint8_t *buf, int len);
    void RecvLevel(uint8_t msgid, uint8_t status, uint8_t *lv, int num);

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
    string CardNum;
    string LevelCode;
    kccont_t Cont;
};

#endif // KCREADER_H
