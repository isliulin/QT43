#ifndef FACEWORKER_H
#define FACEWORKER_H

#include <QObject>
#include <QThread>

#include <opencv2/objdetect.hpp>
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

private:
    QThread thread;
    cv::CascadeClassifier cascade;
    QImage *image;
};

#endif // FACEWORKER_H
