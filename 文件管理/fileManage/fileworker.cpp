#include "fileworker.h"
#include <QFileSystemModel>

fileWorker::fileWorker(QObject *parent) :
    QObject(parent)
{
    this->moveToThread(&thread);
}

void fileWorker::listFile(QTreeView *tree, QString path)
{
    QFileSystemModel *model;

    model = (QFileSystemModel*)tree->model();
    model->setRootPath(path);
    tree->setRootIndex(model->index(path));
}

void fileWorker::start(void)
{
    thread.start();
}

void fileWorker::exit(void)
{
    thread.quit();
    thread.wait();
}
