#ifndef PATTERN_H
#define PATTERN_H

#pragma pack(1)

/* 花样色码文件格式 */
typedef struct
{
    unsigned short width;
    unsigned short height;
    char reserve[12];
    struct
    {
        unsigned char right:4;
        unsigned char left:4;
    }color[1];
}patt_dat_t;





#endif // PATTERN_H
