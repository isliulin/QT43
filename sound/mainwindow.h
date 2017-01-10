#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

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
    void on_play_clicked();

private:
    virtual void keyPressEvent(QKeyEvent *e);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
