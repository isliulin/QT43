#include "MsgProto.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

QString MsgProto::devsn("BM000000000");

MsgProto::MsgProto()
{

}

void MsgProto::InitMsgHdr()
{
    _buf.resize(sizeof(MsgHdr) - 1);
    _buf.fill(0);

    _buf[0] = 0xA5;

    root["DeviceNo"] = devsn;
}

QByteArray* MsgProto::MakeMsg(MsgCmd cmd)
{
    QJsonDocument doc;
    MsgHdr *hdr = (MsgHdr *)_buf.data();

    doc.setObject(root);
    hdr->len = _buf.size() - sizeof(MsgHdr) - 1;
    hdr->cmd = (uint8_t)cmd;

    _buf += doc.toJson();

    return &_buf;
}

QByteArray* MsgProto::DevLoginReq()
{
    InitMsgHdr();

    root["ScalesId"] = "11111111";
    root["SiteCode"] = "xxxx";
    root["SwVersion"] = "xxxx";

    return MakeMsg(MC_DEVLOGIN);
}

QByteArray* MsgProto::WorkersLoginReq()
{
    QJsonObject obj;
    QJsonDocument doc;

    obj["RatingJobCardNum"] = "bm111111";
    obj["RatingName"] = "11111111";
    obj["WeightName"] = "xxxx";
    obj["PrimerName"] = "xxxx";

    doc.setObject(obj);
    _buf = doc.toJson();

    return &_buf;
}

QByteArray* MsgProto::InitReq()
{
    QJsonObject obj;
    QJsonDocument doc;

    obj["ParametersVersion"] = "bm111111";

    doc.setObject(obj);
    _buf = doc.toJson();

    return &_buf;
}

QByteArray* MsgProto::RestoreReq()
{
    QJsonObject obj;
    QJsonDocument doc;

    obj["SaleId"] = "bm111111";

    doc.setObject(obj);
    _buf = doc.toJson();

    return &_buf;
}

QByteArray* MsgProto::DownloadReq()
{
    QJsonObject obj;
    QJsonDocument doc;

    obj["SaleId"] = "bm111111";
    obj["PageNum"] = "xxx";

    doc.setObject(obj);
    _buf = doc.toJson();

    return &_buf;
}

QByteArray* MsgProto::FarmerLoginReq()
{
    QJsonObject obj;
    QJsonDocument doc;

    obj["SaleId"] = "bm111111";
    obj["PageNum"] = "xxx";

    doc.setObject(obj);
    _buf = doc.toJson();

    return &_buf;
}

QByteArray* MsgProto::CancelReq()
{
    QJsonObject obj;
    QJsonDocument doc;

    obj["SaleId"] = "bm111111";

    doc.setObject(obj);
    _buf = doc.toJson();

    return &_buf;
}

QByteArray* MsgProto::SortReq()
{
    QJsonObject obj;
    QJsonDocument doc;

    obj["BasketNo"] = "bm111111";
    obj["LevelID"] = "bm111111";
    obj["LevelCode"] = "bm111111";
    obj["RatingTime"] = "bm111111";

    doc.setObject(obj);
    _buf = doc.toJson();

    return &_buf;
}

QByteArray* MsgProto::EndSortReq()
{
    QJsonObject obj;
    QJsonDocument doc;

    obj["BasketNo"] = "bm111111";
    obj["LevelID"] = "bm111111";
    obj["LevelCode"] = "bm111111";
    obj["RatingTime"] = "bm111111";

    doc.setObject(obj);
    _buf = doc.toJson();

    return &_buf;
}

QByteArray* MsgProto::WeightReq()
{
    QJsonObject obj;
    QJsonDocument doc;

    obj["ScalesId"] = "bm111111";
    obj["ValidResult"] = "bm111111";
    obj["BasketNo"] = "bm111111";
    obj["LevelId"] = "bm111111";
    obj["LevelCode"] = "bm111111";
    obj["Tare"] = "bm111111";
    obj["Weight"] = "bm111111";
    obj["WeightState"] = "bm111111";
    obj["WeightTime"] = "bm111111";
    obj["SaleId"] = "bm111111";

    doc.setObject(obj);
    _buf = doc.toJson();

    return &_buf;
}

QByteArray* MsgProto::WeightDelUndoReq()
{
    QJsonObject obj;
    QJsonDocument doc;

    obj["SaleId"] = "bm111111";
    obj["BasketNo"] = "bm111111";
    obj["OpType"] = "bm111111";

    doc.setObject(obj);
    _buf = doc.toJson();

    return &_buf;
}

QByteArray* MsgProto::FarmerConfirmReq()
{
    QJsonObject obj;
    QJsonDocument doc;

    obj["SaleId"] = "bm111111";
    obj["BankCardNum"] = "bm111111";

    doc.setObject(obj);
    _buf = doc.toJson();

    return &_buf;
}

QByteArray* MsgProto::WeighterReq()
{
    QJsonObject obj;
    QJsonDocument doc;

    obj["SaleId"] = "bm111111";
    obj["BankCardNum"] = "bm111111";

    doc.setObject(obj);
    _buf = doc.toJson();

    return &_buf;
}

QByteArray* MsgProto::HeartBeatReq()
{
    QJsonObject obj;
    QJsonDocument doc;

    obj["RatingState"] = "bm111111";
    obj["ScalesIdState"] = "bm111111";
    obj["SellCardState"] = "bm111111";

    doc.setObject(obj);
    _buf = doc.toJson();

    return &_buf;
}
