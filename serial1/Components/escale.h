#ifndef ESCALE_H
#define ESCALE_H

#include <QThread>
#include <QtSerialPort/QSerialPort>

class EScale : public QThread
{
    Q_OBJECT

public:
    EScale();
    ~EScale();

public:
    void Open();
    void Close();

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
};

#endif // ESCALE_H
