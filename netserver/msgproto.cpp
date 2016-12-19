#include "msgproto.h"

msgproto::msgproto()
{

}

bool msgproto::in(char *buf, int size, int &cmd)
{
    proto_hdr_t *hdr;
    bool ret = false;

    hdr = (proto_hdr_t*)buf;

    if (hdr->sflag1 == 0x68 && hdr->sflag2 == 0x68)
    {
        cmd = hdr->cmd;
        ret = true;
    }

    return ret;
}

bool msgproto::makeheartbeat(char *out, int &size)
{
    proto_hdr_t *hdr;

    hdr = (proto_hdr_t *)out;
    proto_head_init(hdr, 0xA1, 0);
    size = sizeof(proto_hdr_t);
    proto_end((uint8_t*)out, size);

    return true;
}

bool msgproto::makelogin(char *out, int &size)
{
    devlogin_t *lg;

    lg = (devlogin_t *)out;
    lg->status = 0;
    proto_head_init(&lg->hdr, 0xAF, 1);
    size = sizeof(devlogin_t);
    proto_end((uint8_t*)out, size);

    return true;
}

void msgproto::makeprint(string &text, char *out, int &size)
{
    textprint_t *pt;
    int len;

    pt = (textprint_t*)out;
    len = text.size();
    proto_head_init(&pt->hdr, 0x08, len + 8);
    pt->msgid = 1;
    pt->size = len;
    memcpy(pt->data, text.c_str(), len);
    size = len + sizeof(textprint_t) - sizeof(pt->data);

    proto_end((uint8_t*)out, size);
}

uint8_t msgproto::chksum(uint8_t *data, int size)
{
    uint8_t sum = 0;

    for (int i = 0; i < size; i ++)
    {
        sum += data[i];
    }

    return sum;
}

void msgproto::proto_head_init(proto_hdr_t *hdr, unsigned char cmd, short size)
{
    memcpy(hdr->sn, "sn123456789012", 14);
    hdr->cmd = cmd;
    hdr->size = size + 3;
    hdr->sflag1 = 0x68;
    hdr->sflag2 = 0x68;
}

void msgproto::proto_end(uint8_t *data, int &size)
{
    data[size] = chksum(data, size);
    size ++;
    data[size++] = 0x16;
}
