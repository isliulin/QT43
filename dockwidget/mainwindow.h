#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void about(void);

    void on_new_s_triggered();

private:
    void menuInit(void);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
