#ifndef MWWORKER_H
#define MWWORKER_H

#include <QThread>
#include <string>
#include <queue>

using namespace std;

class mwWorker : public QThread
{
    Q_OBJECT

public:
    mwWorker(void *p);

    void clear();
    void msgq_push(int msg);

private:
    bool msgq_get(int &msg);

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
    int tolcnt;
    queue <int> msgq;
};

#endif // MWWORKER_H
