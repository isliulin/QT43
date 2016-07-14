#include "loginworker.h"
#include "framelogin.h"

LoginWorker::LoginWorker(void *parent)
{
    _parent = parent;
    isRun = false;
}

void LoginWorker::setParent(void *p)
{
    _parent = p;
}

void LoginWorker::exit()
{
    isRun = false;
    wait();
}

void LoginWorker::run()
{
    FrameLogin *view;
    int n = 0;

    if (_parent == 0)
        return;
    isRun = true;
    view = (FrameLogin*)_parent;

    view->ShowMsg("thread start");

    while (isRun)
    {
        n ++;
        sleep(1);
        view->ShowNum(n);
    }
}
