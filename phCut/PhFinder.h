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
    void findReq(QString dir, int max);
    void finded(QString file);

public slots:
    void findPhoto(QString dir, int max);

private:
    QThread *thread;
    uint32_t filecnt;
};

#endif // PHFINDER_H
