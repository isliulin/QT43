#ifndef ESCALE_H
#define ESCALE_H

#include <QThread>
#include <QtSerialPort/QSerialPort>
#include <time.h>

class EScale : public QThread
{
    Q_OBJECT

public:
    EScale();
    ~EScale();

public:
    void Open();
    void Close();
    bool isConnected();
    bool isZeroDone();
    void getWeight(float &nw, float &tw);
    void startZero(int sec);

private:
    void run();

    bool ReadWeight(void);
    int write(unsigned char *buf, int size);
    int read(unsigned char *buf, int size);
    bool parseWeight(unsigned char *buf, int size, float &nw, float &tw);

    bool devInit(char *name);

private:
    QSerialPort *Dev;
    bool isRun;
    float nWeight;
    float tWeight;
    bool Connected;
    bool ZeroDone;
    bool ZeroStart;
    int  ZeroTime;
    time_t ZStartTime;
};

#endif // ESCALE_H
