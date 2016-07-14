#ifndef LOGINWORKER_H
#define LOGINWORKER_H

#include <QThread>

class LoginWorker : public QThread
{
    Q_OBJECT

public:
    LoginWorker(void *parent = 0);

    void setParent(void *p);
    void exit();

protected:
    void run();

private:
    void *_parent;
    bool isRun;
};

#endif // LOGINWORKER_H
