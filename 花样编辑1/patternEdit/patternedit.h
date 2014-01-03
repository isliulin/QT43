#ifndef PATTERNEDIT_H
#define PATTERNEDIT_H

#include <QTimer>
#include <QLabel>
#include <QWidget>
#include <QModelIndex>
#include "pattworker.h"

class pattImageModel;
class pattImageDelegate;

namespace Ui {
class patternEdit;
}

class patternEdit : public QWidget
{
    Q_OBJECT

public:
    explicit patternEdit(QWidget *parent = 0);
    ~patternEdit();

signals:
    void viewMove(QTableView *tbPatt, int dir, int type);

private slots:
    void currentItem(const QModelIndex &, const QModelIndex &);
    void initTable();
    void updatePattPosition(int originRow, int originColumn);
    void update();

protected:
    void changeEvent(QEvent *e);
    bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::patternEdit *ui;
    pattImageModel *modelPatt;
    QModelIndex currentIndex;
    pattWorker worker;
    int imageState;
    QLabel *revColor;
    pattImageDelegate *delegate;
    QTimer *timer;
};

#endif // PATTERNEDIT_H
