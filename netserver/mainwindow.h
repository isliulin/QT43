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

    void DrawQrCode(uint8_t *data, int width);
    void GetSerial(string &s);
    void UpdateSerial();
    void GetText(string &s);
    void GetWidth(int &w);

private slots:
    void ShowStatus(string s);

private slots:
    void on_print_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    mwworker *worker;
};

#endif // MAINWINDOW_H
