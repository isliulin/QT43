#ifndef FILEWORKER_H
#define FILEWORKER_H

#include <QObject>
#include <QThread>
#include <QTreeView>

class fileWorker : public QObject
{
    Q_OBJECT
public:
    explicit fileWorker(QObject *parent = 0);
    
    void start();
    void exit();
    void listFile(QTreeView *tree, QString path);

signals:
    
public slots:

private:
    QThread thread;
};

#endif // FILEWORKER_H
