#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "mwworker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void ShowValue(float nw, float tw);
    void ShowEStatus(char *s);

private:
    Ui::MainWindow *ui;
    mwWorker *worker;
};

#endif // MAINWINDOW_H
