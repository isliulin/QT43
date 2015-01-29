#ifndef CALIB_H
#define CALIB_H

/* 数据包类型定义 */
#define DT_STATUS_RSP   0x08 //操作状态反馈
#define DT_CALIB_REQ    0x0D //校准请求
#define DT_CALIB_RSP    0x0F //校准结果应答(返回测量数据)

/* 校正通道定义 */
#define CHN_CA    0x00 //A相电流
#define CHN_CB    0x01 //B相电流
#define CHN_CC    0x02 //C相电流
#define CHN_CN    0x03 //N相电流
#define CHN_VA    0x04 //A相电压
#define CHN_VB    0x05 //B相电压
#define CHN_VC    0x06 //C相电压
#define CHN_CABC  0x07 //三相电流一起校正
#define CHN_VABC  0x08 //三相电压一起校正
#define CHN_PHA   0x09 //电流电压A相位
#define CHN_PHB   0x0A //电流电压B相位
#define CHN_PHC   0x0B //电流电压C相位
#define CHN_PA    0x0C //A相有功功率
#define CHN_PB    0x0D //B相有功功率
#define CHN_PC    0x0E //C相有功功率

/* 校准操作码定义 */
#define CALCMD_DO       0x01 //执行校准
#define CALCMD_ENTER    0x02 //进入校准模式
#define CALCMD_EXIT     0x03 //退出校准模式
#define CALCMD_SAM_GET  0x04 //读取一个通道的样本
#define CALCMD_SAM_DAT  0x05 //上传的样本数据
#define CALCAM_SAM_SET  0x06 //样本设置

/* 客户端编号定义 */
#define CID_LOCAL       0x01 //本地访问
#define CID_REMOTE      0x02 //远程访问

#pragma pack(1)
/* 数据包头 */
typedef struct
{
    unsigned char dtype;  /* 数据包用途 */
    unsigned char size;   /* 整个数据包大小 */
    unsigned short chksum;/* 对整个包的校验和 */
    unsigned char sid;    /* 传感器编号 */
    unsigned char cid;    /* 客户端编号 */
}pkthead_t;

/* 操作状态应答(对不需要返回数据的操作的响应) */
typedef struct
{
    pkthead_t hdr;

    unsigned char dtype;  //被响应的操作码
    unsigned char status; //结果(0成功 其他值自定)

    short value;          //附加值
}status_rsp_t;

/* 校准请求和校准后的结果反馈 */
typedef struct
{
    pkthead_t hdr;

    unsigned char chn:4;     //通道
    unsigned char seg:4;     //区段
    unsigned char rsv:4;     //保留
    unsigned char cmd:4;     //操作码

    float nvalue;            //标称值
    float xvalue;            //测量值或密码
}calib_t;

/* 校准样本返回 */
typedef struct
{
    pkthead_t hdr;

    unsigned char chn:4;     //通道号
    unsigned char seg:4;     //样本中包含的区段总数
    unsigned char rsv:4;     //保留
    unsigned char cmd:4;     //操作码（固定为CALCMD_SAM_DAT）

    float         data[0];   //数据区(标称值在前测量值在后)
}calib_sam_t;

/* modbus数据帧 */
typedef struct
{
    unsigned char addr;
    unsigned char funcode;
    calib_t data;
    unsigned short crc16;
}mbadu_t;

#pragma pack()

#endif // CALIB_H
