#include "patternworker.h"
#include <QFile>

patternWorker::patternWorker(QObject *parent) :
    QObject(parent)
{
    connect(&thread, SIGNAL(started()), this, SLOT(doWork()));
    this->moveToThread(&thread);
    exit_notify = 0;
}

void patternWorker::start()
{
    thread.start();
}

void patternWorker::exit()
{
    exit_notify = 1;
    thread.quit();
    thread.wait();
}

void patternWorker::doWork()
{
    int row, column;
    QFile fpatt; // 花样文件
    QFile fctrl; // 控制文件
    QByteArray dpatt;
    QByteArray dctrl;

    /* 读取花样文件 */
    fpatt.setFileName("/root/mq/flower2/7x7.dat");
    if (!fpatt.open(QIODevice::ReadWrite))
    {
        qDebug("file fail\n");
    }
    dpatt = fpatt.readAll();
    if (!dpatt.isEmpty())
    {
        qDebug("SS %d\n", dpatt.at(0));
    }

    /* 根据花样文件创建表格 */
    for (row = 0; row < 25; row ++)
    {
        for (column = 0; column < 100; column ++)
        {
            QStandardItem *item = new QStandardItem();
            item->setForeground(QBrush(Qt::white));
            emit addItem(MODEL_PATT, row, column, item);
        }
    }
    /* 创建控制文件表格 */
    for (row = 0; row < 25; row ++)
    {
        for (column = 0; column < 5; column ++)
        {
            QStandardItem *item = new QStandardItem();
            item->setForeground(QBrush(Qt::white));
            emit addItem(MODEL_CTRL, row, column, item);
        }
    }
}
