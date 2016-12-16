#ifndef MSGPROTO_H
#define MSGPROTO_H

#include <stdint.h>
#include <string>

using namespace std;

#pragma pack(1)
typedef struct
{
    unsigned char sflag1;
    unsigned char sn[14];
    unsigned char sflag2;
    unsigned char cmd;
    unsigned short size; //从控制域到校验和前一个字节的长度
}proto_hdr_t;

typedef struct
{
    proto_hdr_t hdr;
    int size;
    int msgid;
    char data[1];
    //end
}textprint_t;

typedef struct
{
    proto_hdr_t hdr;
    char status;
}devlogin_t;
#pragma pack()

class msgproto
{
public:
    msgproto();

    bool in(char *buf, int size, int &cmd);

public:
    void makeprint(string &text, char *out, int &size);
    bool makeheartbeat(char *out, int &size);
    bool makelogin(char *out, int &size);

private:
    void proto_head_init(proto_hdr_t *hdr, unsigned char cmd, short size);
    uint8_t chksum(uint8_t *data, int size);
    void proto_end(uint8_t *data, int &size);
};

#endif // MSGPROTO_H
