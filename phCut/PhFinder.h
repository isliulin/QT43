#ifndef PHFINDER_H
#define PHFINDER_H

#include <QObject>

class QThread;

class PhFinder : public QObject
{
    Q_OBJECT

public:
    explicit PhFinder(QObject *parent = 0);

    void start();

signals:
    void findReq(QString dir, int max, int mins);
    void finded(QString file);
    void finished();

public slots:
    void findPhoto(QString dir, int maxcnt, int mins = 0);

private:
    QThread *thread;
    int filecnt;
};

#endif // PHFINDER_H
