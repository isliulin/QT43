#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class FaceDetec;
class PhFinder;
class QLabel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void moveSelRect(QPoint &center);

private:
    void keyPressEvent(QKeyEvent *ke);

private slots:
    void phFinded(QString file);
    void faceDetec(int index);
    void saveCut();
    void doCut();

private slots:
    void on_btOk_clicked();

    void on_btSelDir_clicked();

    void on_btFindPh_clicked();

    void on_sizeLock_clicked(bool checked);

    void on_listFile_doubleClicked(const QModelIndex &index);

    void on_btNext_clicked();

    void on_btSave_clicked();

private:
    Ui::MainWindow *ui;
    FaceDetec *facedetec;
    PhFinder *finder;
    int curfile;
    QLabel *showCut;
    bool isEditing;
    bool isSaved;
    float scalefw;
    float scalefh;
    QString curFileName;
};

#endif // MAINWINDOW_H
