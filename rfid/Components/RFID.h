#ifndef _RFID_H_
#define _RFID_H_

#include <QtSerialPort/QSerialPort>
#include <string>

using namespace std;

#define ZM704_FLAG    0xAA55

#define M1_CARD_SECRET_CODE_A       0x60
#define M1_CARD_SECRET_CODE_B       0x61

#define M1_S50_EEPROM_MAX_SECTION 	16   // 最大扇区数
#define M1_S50_EEPROM_MAX_BLOCK 	(M1_S50_EEPROM_MAX_SECTION*4)    //  最大块数
#define M1_S50_EEPROM_BLOCK_BYTE_SIZE   16      // 每个块的字节数


#pragma pack(1)
typedef struct 
{
	unsigned short flag;
    unsigned char addr;
    unsigned char len;
	unsigned short cmd;
	unsigned char data[1];
}zm704_hdr_t;

typedef struct
{
    unsigned short flag;
    unsigned char addr;
    unsigned char len;
    unsigned char data[1];
}zm704_rsp_t;
#pragma pack()

typedef enum
{
    ZM_CMD_SCAN_CARD_AUTO = 0x0221,
    ZM_CMD_GET_INFO       = 0x0201,
	ZM_CMD_AUTHEN         = 0x0301,
	ZM_CMD_READ_BLOCK     = 0x0302,
	ZM_CMD_ENTER_CPUCARD  = 0x0401,
	ZM_CMD_SEND_COS       = 0x04F1,
	ZM_CMD_GET_DEVINFO    = 0x0130,
}zm704_cmd_t;

typedef enum
{
    TYPE_A = 0x60,
	TYPE_B = 0x61
}RfidCrcType;

typedef struct  
{
	unsigned short type1;
	unsigned char type2;
	unsigned int uid;
}RfidInfo;

class RFID 
{
public:
	RFID(void);
	~RFID(void);

    bool Open(const char *name, int br = 115200);
    void Close();

    bool Connected(void);
    string ErrMsg();
    QString readText();

protected:
    bool CardScan();
	bool Authen(unsigned char blkn, unsigned char type, unsigned char *pwd, short size);
    bool BlockRead(unsigned char blkn, unsigned char *buf, short size);    
	bool CpuCardMode();
	bool CosSend(unsigned char *c, short size);

    bool ReqSend(short cmd, unsigned char *data, short size);
	int AckRecv(unsigned char *buf, short size);

    void SetRetry(int retry);

protected:
    string errmsg;

private:
    QSerialPort Dev;
    int Retry;
	int Read(unsigned char *buf, short size);
	int Write(unsigned char *buf, short size);
	unsigned char Crc8(unsigned char *buf, short size);
    QString recvstr;
    bool iscon;
};
#endif
