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

    void GetText(string &s);
    void ShowTip(const char *s);

private slots:
    void ShowStatus(string s);

private slots:
    void on_print_clicked();

private:
    Ui::MainWindow *ui;
    mwworker *worker;
};

#endif // MAINWINDOW_H
