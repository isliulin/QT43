#include "PhFinder.h"
#include <QThread>

#include <QDir>
#include <QDirIterator>

PhFinder::PhFinder(QObject *parent)
    : QObject(parent),
      filecnt(0)
{
    thread = new QThread;
    connect(this, SIGNAL(findReq(QString,int)),
            this, SLOT(findPhoto(QString,int)), Qt::QueuedConnection);
    moveToThread(thread);
}

void PhFinder::start()
{
    thread->start();
}

void PhFinder::findPhoto(QString path, int max)
{
    QString file;
    QDir dir(path);

    if(!dir.exists())
    {
        return;
    }

    filecnt = 0;

    QStringList filters;
    filters<<QString("*.jpeg")<<QString("*.jpg");

    QDirIterator dir_iterator(path,
                              filters,
                              QDir::Files | QDir::NoSymLinks,
                              QDirIterator::Subdirectories);

    while(dir_iterator.hasNext())
    {
        dir_iterator.next();

        QFileInfo file_info = dir_iterator.fileInfo();

        file = file_info.absoluteFilePath();
        if (file_info.isDir() && (filecnt > max))
        {
            break;
        }
        else if (file_info.size() > 30*1024)
        {
            filecnt ++;
            emit finded(file);
        }

        thread->msleep(20);
    }
}
