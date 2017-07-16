#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class FaceDetec;

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
    void on_pbOk_clicked();

private:
    Ui::MainWindow *ui;
    FaceDetec *facedetec;
};

#endif // MAINWINDOW_H
