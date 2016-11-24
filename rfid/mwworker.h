#ifndef MWWORKER_H
#define MWWORKER_H

#include <QThread>
#include <string>

using namespace std;

class mwWorker : public QThread
{
    Q_OBJECT

public:
    mwWorker(void *p);

    void clear();

signals:
    void ShowStatus(string s);

private:
    void run();

public:
    bool isrun;

private:
    void *param;
    int errcnt;
    int suscnt;
};

#endif // MWWORKER_H
