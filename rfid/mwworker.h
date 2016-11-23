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

signals:
    void ShowStatus(string s);

private:
    void run();

public:
    bool isrun;
    void *param;
};

#endif // MWWORKER_H
