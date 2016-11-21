#ifndef MWWORKER_H
#define MWWORKER_H

#include <QThread>

class mwWorker : public QThread
{
public:
    mwWorker(void *p);

private:
    void run();

public:
    bool isrun;
    void *param;
};

#endif // MWWORKER_H
