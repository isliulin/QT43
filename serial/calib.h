#ifndef CALIB_H
#define CALIB_H

/* 数据包类型定义 */
#define DT_CALIB_REQ    0xD
#define DT_CALIB_RES    0xE

/* 信号采集通道定义 */
#define CHN_CA    0x00
#define CHN_CB    0x01
#define CHN_CC    0x02
#define CHN_CN    0x03
#define CHN_VA    0x04
#define CHN_VB    0x05
#define CHN_VC    0x06
#define CHN_CABC  0x07
#define CHN_VABC  0x08

/* 接线方式定义 */
#define PHAWIRE_33    0x00
#define PHAWIRE_34    0x01

/* 操作码定义 */
#define CALCMD_DO       0x00
#define CALCMD_ENTER    0x01
#define CALCMD_EXIT     0x02
#define CALCMD_SAM_REQ  0x03 //读取一个通道的样本
#define CALCMD_SAM_RES  0x04 //上传的样本数据
#define CALCAM_SAM_SET  0x05 //样本设置

/* 数据包头 */
typedef struct
{
    unsigned char dtype:4;
    unsigned char id:4;
    unsigned char size;
    unsigned short chksum;
}pkthead_t;

/* 校准请求和校准后的结果反馈 */
typedef struct
{
    pkthead_t hdr;

    unsigned char chn;     //通道
    unsigned char seg;     //区段
    unsigned char phawire; //接线方式
    unsigned char cmd;     //操作码

    float nvalue;          //标称值
    char datemval[4];      //日期或测量值
}calib_t;

/* 校准样本 */
typedef struct
{
    pkthead_t hdr;

    unsigned char chn;     //通道
    unsigned char seg;     //区段
    unsigned char phawire; //接线方式
    unsigned char cmd;     //操作码

    float mval[7];  //测量值
    float nval[7];  //标称值
}calib_sam_t;

#endif // CALIB_H
