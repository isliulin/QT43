#ifndef PATTERNWORKER_H
#define PATTERNWORKER_H

#include <QObject>
#include <QtCore>
#include <QStandardItemModel>
#include <QTableView>

#define    MODEL_PATT 1
#define    MODEL_CTRL 2

class patternWorker : public QObject
{
    Q_OBJECT
public:
    explicit patternWorker(QObject *parent = 0);

    void start();
    void exit();

private:
    void setPage(QStandardItemModel *model, int startRow, int startColumn);

signals:
    void addItem(int model, int row, int column, QStandardItem *item);
    void setPattData(const QByteArray &, const QByteArray &);
    void updatePattPosition(int originColumn, int originRow);

public slots:
    void doWork();
    void viewMove(QTableView *tbPatt, int dir, int type);
    void readFile(void *model_patt, void *model_ctrl);
    void initTable(QTableView *patt, QTableView *ctrl);

private:
    volatile int exit_notify;
    const QStandardItemModel *modelPatt;
    const QStandardItemModel *modelCtrl;

public:
    QThread thread;
    QByteArray baPatt;
    QByteArray baCtrl;
    struct
    {
        unsigned short row;
        unsigned short column;
    }startIndex;
};

#endif // PATTERNWORKER_H
