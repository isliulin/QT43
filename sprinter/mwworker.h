#ifndef MWWORKER_H
#define MWWORKER_H

#include <QThread>
#include <QWidget>

#include <string>
#include <queue>

using namespace std;

class MainWindow;

class mwworker : public QThread
{
    Q_OBJECT

public:
    mwworker(QWidget *parent);

    void msgq_push(int msg);

signals:
    void ShowStatus(string s);

private:
    void run();
    bool msgq_get(int &msg);

private:
    MainWindow *ui;
    bool isrun;
    queue <int> msgq;
};

#endif // MWWORKER_H
