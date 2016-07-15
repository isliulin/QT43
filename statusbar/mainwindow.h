#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include <QCloseEvent>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void closeEvent(QCloseEvent *event);

private:
    void statusBarInit();

private:
    Ui::MainWindow *ui;
    QLabel lb1;
    QLabel lb2;
    QLabel lb3;
    QPixmap icon;
};

#endif // MAINWINDOW_H
