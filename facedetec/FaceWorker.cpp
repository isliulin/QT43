#include "FaceWorker.h"

FaceWorker::FaceWorker(QObject *parent) : QObject(parent)
{
    moveToThread(&thread);
}

void FaceWorker::start()
{
   thread.start();
}

void FaceWorker::DoWork(int cmd, QByteArray param)
{
    switch (cmd)
    {
    case 1:
    {
        LoadCode(param);
    }break;
    }
}

void FaceWorker::LoadCode(QByteArray file)
{
    cascade.load(String(file.toStdString()));
}
