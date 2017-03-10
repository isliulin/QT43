#ifndef FACEDETEC_H
#define FACEDETEC_H

#include <QWidget>

namespace Ui {
class FaceDetec;
}

class FaceWorker;

class FaceDetec : public QWidget
{
    Q_OBJECT

public:
    explicit FaceDetec(QWidget *parent = 0);
    ~FaceDetec();

signals:
    void DoWork(int cmd, QByteArray param = QByteArray());

private slots:
    void on_pbload_clicked();

    void on_phload_clicked();

    void on_detec_clicked();

private:
    Ui::FaceDetec *ui;
    FaceWorker *worker;
};

#endif // FACEDETEC_H
