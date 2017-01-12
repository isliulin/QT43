#ifndef CSINTF_H
#define CSINTF_H

#include <QTcpSocket>

class CsIntf
{
public:
    CsIntf();
    ~CsIntf();

    void SetHost(QString a, int p);

public:
    bool DevLogin();

private:
    int write(QByteArray *buf);

private:
    QTcpSocket sk;
    QString addr;
    int port;
};

#endif // CSINTF_H
