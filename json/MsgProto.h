#ifndef MSGPROTO_H
#define MSGPROTO_H

#include <stdint.h>

#include <QByteArray>
#include <QString>
#include <QJsonObject>

#pragma pack(1)
typedef struct
{
    uint8_t stx;
    uint8_t devid;
    uint8_t msgid;
    uint8_t stat;
    uint8_t type;
    uint8_t needack;
    uint8_t needret;
    uint8_t cmd;
    int len;
    uint8_t data[];
}MsgHdr;
#pragma pack()

typedef enum
{
    MC_DEVLOGIN = 1,
}MsgCmd;

class MsgProto
{
public:
    MsgProto();

public:
    QByteArray* HeartBeatReq();
    QByteArray* DevLoginReq();
    QByteArray* WorkersLoginReq();
    QByteArray* InitReq();
    QByteArray* RestoreReq();
    QByteArray* DownloadReq();
    QByteArray* FarmerLoginReq();
    QByteArray* CancelReq();
    QByteArray* SortReq();
    QByteArray* EndSortReq();
    QByteArray* WeightReq();
    QByteArray* WeightDelUndoReq();
    QByteArray* FarmerConfirmReq();
    QByteArray* WeighterReq();

private:
    void InitMsgHdr();
    QByteArray *MakeMsg(MsgCmd cmd);

private:
    QJsonObject root;
    QByteArray _buf;
    static QString devsn;
};

#endif // MSGPROTO_H
