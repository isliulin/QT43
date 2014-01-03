#ifndef PATTWORKER_H
#define PATTWORKER_H

#include <QObject>
#include <QThread>
#include <QTableView>

class pattWorker : public QObject
{
    Q_OBJECT

public:
    explicit pattWorker(QObject *parent = 0);

    void start();
    void exit();

private:
    void setPage(QAbstractItemModel *model, int destRow, int destColumn);

signals:
    void updatePattPosition(int originRow, int originColumn);

public slots:
    void initTable(QTableView *tvPatt, QTableView *tvCtrl);
    void viewMove(QTableView *tbPatt, int dir, int type);

private slots:
    void loadPattern(void);

public:
    unsigned short pattWidth;
    unsigned short pattHeight;

private:
    QThread thread;
    QByteArray baPatt;
    QByteArray baCtrl;
    struct
    {
        unsigned short row;
        unsigned short column;
    }originIndex;
};

#endif // PATTWORKER_H
