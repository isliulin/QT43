#ifndef MSGPROTO_H
#define MSGPROTO_H

#include <stdint.h>

typedef struct
{
    unsigned char sflag1;
    unsigned char sn[14];
    unsigned char sflag2;
    unsigned char cmd;
    unsigned short size; //从控制域到校验和前一个字节的长度
}proto_hdr_t;

class msgproto
{
public:
    msgproto();

    bool in(char *buf, int size, int &cmd);
    void make(uint8_t cmd, char *out, char *data, int &size);
};

#endif // MSGPROTO_H
