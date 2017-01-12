#include "CsIntf.h"

#include <QTcpSocket>
#include "MsgProto.h"

CsIntf::CsIntf()
{
   addr = "127.0.0.0";
   port = 5555;
}

CsIntf::~CsIntf()
{

}

void CsIntf::SetHost(QString a, int p)
{
    addr = a;
    port = p;
}

bool CsIntf::DevLogin()
{
    QByteArray *buf;
    MsgProto mp;

    buf = mp.DevLoginReq();

    write(buf);

    return true;
}

int CsIntf::write(QByteArray *buf)
{
    int size = 0;

    if (sk.state() != QTcpSocket::ConnectedState)
    {
        sk.connectToHost(addr, port);
        if (!sk.waitForConnected())
        {
            return 0;
        }
    }

    size = sk.write(*buf);
    sk.flush();

    return size;
}
