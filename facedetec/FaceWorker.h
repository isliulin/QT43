#ifndef FACEWORKER_H
#define FACEWORKER_H

#include <QObject>
#include <QThread>

#include <opencv2/opencv.hpp>
using namespace cv;

class QImage;

class FaceWorker : public QObject
{
    Q_OBJECT

public:
    explicit FaceWorker(QObject *parent = 0);

    void start();

signals:

public slots:
    void DoWork(int cmd, QByteArray param);

private:
    void LoadCode(QByteArray &file);
    void LoadImage(QByteArray &file);
    void FaceDetec();

private:
    QThread thread;
    cv::CascadeClassifier cascade;
    cv::Mat image;
};

#endif // FACEWORKER_H
