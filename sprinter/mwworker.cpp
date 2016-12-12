#include "mwworker.h"
#include "mainwindow.h"

mwworker::mwworker(QWidget *parent)
{
    ui = parent;
    isrun = false;
}

void mwworker::run()
{
    isrun = true;

    while (isrun)
    {

    }
}
