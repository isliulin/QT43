#include "printer.h"

#include <QTextCodec>

#define  BARCODE    "BARCODE %d,%d,\"128\",%d,1,0,3,3,\"%s\"\n"
#define  TEXT       "TEXT %d,%d,\"FONT001\",0,2,2,\"%s\"\n\n"
#define  TEXTLV     "TEXT %d,%d,\"FONT001\",0,5,5,\"%s\"\n\n"

printer::printer()
{
}

void printer::utf8ToGb2312(string& strUtf8)
{
    QTextCodec* utf8Codec= QTextCodec::codecForName("utf-8");
    QTextCodec* gb2312Codec = QTextCodec::codecForName("gb2312");

    QString strUnicode= utf8Codec->toUnicode(strUtf8.c_str());
    QByteArray ByteGb2312= gb2312Codec->fromUnicode(strUnicode);
    strUtf8 = ByteGb2312.data();
}

void printer::utogb(string &s)
{
    QTextCodec* gb2312Codec = QTextCodec::codecForName("gb2312");
    QString strUnicode = QString::fromStdString(s);

    QByteArray ByteGb2312= gb2312Codec->fromUnicode(strUnicode);
    s = ByteGb2312.data();
}

void printer::utoutf8(string &s)
{
    QTextCodec* gb2312Codec = QTextCodec::codecForName("utf-8");
    QString strUnicode = QString::fromStdString(s);

    QByteArray ByteGb2312= gb2312Codec->fromUnicode(strUnicode);
    s = ByteGb2312.data();
}

bool printer::print(string &s, int width)
{
    string buf;
    char pw[128];
    SPrinterParam sPrinterParam;
    SPrinterParam *Param = &sPrinterParam;
    OCP_Text text;
    OCP_Text *Text = &text;
    int x;
    int qrx, qry;

    Text->BarCode = "00001111222233334444555566667777";
    Text->Line1 = "产地:会理海潮烟点 品种:红花大金元";
    Text->Line2 = "净重:39.98Kg 成件:2016-05-27 11:14";
    Text->Line3 = "成件人:张三丰 库管:孙悟空 打印次数:2";
    Text->Line4 = "等级:C1F";
    Text->Line5 = "扎把:散烟";

    utoutf8(s);
    utf8ToGb2312(Text->BarCode);
    utf8ToGb2312(Text->Line1);
    utf8ToGb2312(Text->Line2);
    utf8ToGb2312(Text->Line3);
    utf8ToGb2312(Text->Line4);
    utf8ToGb2312(Text->Line5);

    sPrinterParam.BarCodeH = 55;
    sPrinterParam.BarCodeV = 250;
    sPrinterParam.Pint_Density = 7;
    sPrinterParam.BarCodeWidth = 150;
    sPrinterParam.Font_H = 55;
    sPrinterParam.Font_V = 470;
    sPrinterParam.Pint_Speed = 3;
    sPrinterParam.Offset = 0;
    sPrinterParam.Size_X = 100;
    sPrinterParam.Size_Y = 80;

    buf = "SIZE 100 mm,80 mm\n";
    buf += "GAP 3 mm,0\n";
    buf += "OFFSET 5 mm\n";
    buf += "SPEED 6\n";
    buf += "DENSITY 7\n";
    buf += "DIRECTION 1\n";
    buf += "REFERENCE 0,0\n";
    buf += "SET PRINTKEY OFF\n";
    buf += "CLS\n";

    x = Param->BarCodeH;

    sprintf(pw, TEXT, x, Param->Font_V - 0x40, Text->BarCode.c_str());
    buf += pw;
    sprintf(pw, TEXTLV, x, Param->BarCodeV, Text->Line4.c_str());
    buf += pw;
    sprintf(pw, TEXTLV, x, Param->BarCodeV + 0x60, Text->Line5.c_str());
    buf += pw;
    sprintf(pw, TEXT, x, Param->Font_V + 32, Text->Line1.c_str());
    buf += pw;
    sprintf(pw, TEXT, x, Param->Font_V + 72, Text->Line2.c_str());
    buf += pw;
    sprintf(pw, TEXT, x, Param->Font_V + 112, Text->Line3.c_str());
    buf += pw;


    qrx = 520;
    qry = Param->BarCodeV - 0x40;
    sprintf(pw, "DMATRIX %d,%d,%d,%d,x4,\"", qrx, qry, width*8, width*8);
    buf += pw;
    buf += s;
    buf += "\"\n";
    buf += "PRINT 1\n";
    buf += "CUT\n";

    write((char*)buf.c_str(), buf.size());

    return true;
}

string printer::format(string &s)
{
    string buf;
    char pw[128];
    SPrinterParam sPrinterParam;
    SPrinterParam *Param = &sPrinterParam;
    OCP_Text text;
    OCP_Text *Text = &text;
    int x;
    int qrx, qry;

    Text->BarCode = "00001111222233334444555566667777";
    Text->Line1 = "产地:会理海潮烟点 品种:红花大金元";
    Text->Line2 = "净重:39.98Kg 成件:16-05-27 11:14";
    Text->Line3 = "成件人:张三丰 库管:孙悟空 打印次数:2";
    Text->Line4 = "等级:C1F";
    Text->Line5 = "扎把:散烟";

    utoutf8(s);
    utf8ToGb2312(Text->BarCode);
    utf8ToGb2312(Text->Line1);
    utf8ToGb2312(Text->Line2);
    utf8ToGb2312(Text->Line3);
    utf8ToGb2312(Text->Line4);
    utf8ToGb2312(Text->Line5);

    sPrinterParam.BarCodeH = 55;
    sPrinterParam.BarCodeV = 250;
    sPrinterParam.Pint_Density = 7;
    sPrinterParam.BarCodeWidth = 150;
    sPrinterParam.Font_H = 55;
    sPrinterParam.Font_V = 470;
    sPrinterParam.Pint_Speed = 3;
    sPrinterParam.Offset = 0;
    sPrinterParam.Size_X = 100;
    sPrinterParam.Size_Y = 80;

    buf = "SIZE 100 mm,80 mm\n";
    buf += "GAP 3 mm,0\n";
    buf += "OFFSET 5 mm\n";
    buf += "SPEED 6\n";
    buf += "DENSITY 7\n";
    buf += "DIRECTION 1\n";
    buf += "REFERENCE 0,0\n";
    buf += "SET PRINTKEY OFF\n";
    buf += "CLS\n";

    x = Param->BarCodeH;

    sprintf(pw, TEXT, x, Param->Font_V - 0x40, Text->BarCode.c_str());
    buf += pw;
    sprintf(pw, TEXTLV, x, Param->BarCodeV, Text->Line4.c_str());
    buf += pw;
    sprintf(pw, TEXTLV, x, Param->BarCodeV + 0x60, Text->Line5.c_str());
    buf += pw;
    sprintf(pw, TEXT, x, Param->Font_V + 32, Text->Line1.c_str());
    buf += pw;
    sprintf(pw, TEXT, x, Param->Font_V + 72, Text->Line2.c_str());
    buf += pw;
    sprintf(pw, TEXT, x, Param->Font_V + 112, Text->Line3.c_str());
    buf += pw;

    qrx = 520;
    qry = Param->BarCodeV - 0x40;
    sprintf(pw, "DMATRIX %d,%d,%d,%d,x4,\"", qrx, qry, 8*8, 8*8);
    buf += pw;
    buf += s;
    buf += "\"\n";
    buf += "PRINT 1\n";
    buf += "CUT\n";

    return buf;
}

bool printer::open(const char *name, int br)
{
    bool ret = true;

    Dev.setPortName(name);
    Dev.setBaudRate(br);
    Dev.setDataBits(QSerialPort::Data8);
    Dev.setParity(QSerialPort::NoParity);
    Dev.setStopBits(QSerialPort::OneStop);
    Dev.setFlowControl(QSerialPort::NoFlowControl);

    ret = Dev.open(QIODevice::ReadWrite);

    return ret;
}

void printer::close()
{
    Dev.close();
}

int printer::write(char *buf, int size)
{
    int len;

    len = Dev.write(buf, size);
    Dev.waitForBytesWritten(300);

    return len;
}

int printer::read(char *buf, int size)
{
    int len;

    Dev.waitForReadyRead(20);
    if (Dev.bytesAvailable() == 0)
    {
       return 0;
    }

    len = Dev.read(buf, size);

    return len;
}
