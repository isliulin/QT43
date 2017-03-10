#include "FaceWorker.h"

#include <QImage>

FaceWorker::FaceWorker(QObject *parent) : QObject(parent)
{
    moveToThread(&thread);
    image = NULL;
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
    case 2:
    {
        LoadImage(param);
    }break;
    }
}

void FaceWorker::LoadImage(QByteArray &file)
{
    if (image == NULL)
    {
        image = new QImage;
    }

    image->load(QString(file));
}

void FaceWorker::LoadCode(QByteArray &file)
{
    cascade.load(String(file.data()));
}
