#ifndef PRINTER_H
#define PRINTER_H

#include <QtSerialPort/QSerialPort>
#include <string>

using namespace std;

typedef struct
{
    uint8_t Size_X;                       //Ö½ÕÅ¿í¶È
    uint8_t Size_Y;                       //Ö½ÕÅ³¤¶È
    uint8_t Offset;                       //Ö½ÕÅÆ«ÒÆÎ»ÖÃ
    uint8_t Pint_Speed;                   //´òÓ¡ËÙ¶È
    uint8_t Pint_Density;                 //´òÓ¡Éî¶È
    uint8_t BarCodeH;                    //ÌõÂë×ø±êË®Æ½Î»ÖÃ
    uint8_t BarCodeV;                    //ÌõÂë×ø±ê´¹Ö±Î»ÖÃ
    uint8_t BarCodeWidth;                //ÌõÂë¿í¶È
    uint8_t Font_H;                      //×ÖÌåµÄË®Æ½Î»ÖÃ
    uint16_t Font_V;                      //×ÖÌåµÄ´¹Ö±Î»ÖÃ
}SPrinterParam;

typedef struct
{
    string BarCode;
    string Line1;
    string Line2;
    string Line3;
    string Line4;
}OCP_Text;

class printer
{
public:
    printer();

    bool open(const char *name, int br = 9600);
    void close();

    bool print(string &s, int width);

private:
    int write(char *buf, int size);
    int read(char *buf, int size);
    void utf8ToGb2312(string& strUtf8);
    void utogb(string &s);

private:
    QSerialPort Dev;
};

#endif // PRINTER_H
