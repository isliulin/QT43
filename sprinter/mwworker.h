#ifndef MWWORKER_H
#define MWWORKER_H

#include <QThread>
#include <QWidget>

class MainWindow;

class mwworker : public QThread
{
    Q_OBJECT

public:
    mwworker(QWidget *parent);

private:
    void run();

private:
    MainWindow *ui;
    bool isrun;
};

#endif // MWWORKER_H
