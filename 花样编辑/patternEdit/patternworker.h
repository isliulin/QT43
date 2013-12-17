#ifndef PATTERNWORKER_H
#define PATTERNWORKER_H

#include <QObject>
#include <QtCore>
#include <QStandardItemModel>


#define    MODEL_PATT 1
#define    MODEL_CTRL 2

class patternWorker : public QObject
{
    Q_OBJECT
public:
    explicit patternWorker(QObject *parent = 0);

    void start();
    void exit();

signals:
    void addItem(int model, int row, int column, QStandardItem *item);

public slots:
    void doWork();

private:
    volatile int exit_notify;

public:
    QThread thread;
};

#endif // PATTERNWORKER_H
