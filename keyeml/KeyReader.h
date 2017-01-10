#ifndef KEYREADER_H
#define KEYREADER_H

#include <QThread>

class QIODevice;

class KeyReader : public QThread
{
    Q_OBJECT

public:
    KeyReader();
    ~KeyReader();

private:
    bool open(const char *name);
    int read(char *buf, int size);
    bool wait(int msec);
    bool mapkey(int input, int &keyval, QString &text);

private:
    void run();
    int fd;
    bool isrun;
    int curmap;
};

#endif // KEYREADER_H
