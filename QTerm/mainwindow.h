#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "QTermWidget/QTermWidget.h"
#include "dialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_test_clicked();

private:
    Ui::MainWindow *ui;
    QTermWidget *term;
    Dialog *test;
};

#endif // MAINWINDOW_H
