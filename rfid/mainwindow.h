#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mwworker.h"
#include <string>

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void AddCardMsg(string &s);
    void GetSerial(string &s);
    void ShowErrCnt(int cnt);

private slots:
    void ShowStatus(string s);

private:
    Ui::MainWindow *ui;
    mwWorker *worker;
};

#endif // MAINWINDOW_H
