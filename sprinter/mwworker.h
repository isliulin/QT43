#ifndef MWWORKER_H
#define MWWORKER_H

#include <QThread>
#include <QWidget>

class mwworker : public QThread
{
    Q_OBJECT

public:
    mwworker(QWidget *parent);

private:
    void run();

private:
    QWidget *ui;
    bool isrun;
};

#endif // MWWORKER_H
