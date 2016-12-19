#ifndef YMODEM_H
#define YMODEM_H

#include <QThread>
#include <queue>

using namespace std;
class Console;

class Ymodem : public QThread
{
    Q_OBJECT

public:
    Ymodem(Console *parent);

    void put(const QByteArray &data);

    int makeFirstRsp(const char *name, char *buf);
    int makeNextRsp(char *data, int size, char *buf);
    void close();

private:
    enum modemState
    {
        msWaitReq,
        msWaitAck,
        msFirst,
        msNext,
        msRepeat,
        msEnding
    };

    typedef struct
    {
        uint8_t start;
        uint8_t sn;
        uint8_t nsn;
        char data[128];
        //short crc;
    }ymhead_t;

private:
    void run();
    void msgq_push(int msg);
    bool msgq_get(int &msg);
    uint16_t crc16(char *data, int size);

private:
    bool isrun;
    Console *ui;
    uint8_t sn;
    queue <int> msgq;
};

#endif // YMODEM_H
