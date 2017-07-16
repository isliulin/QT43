#ifndef FACEDETEC_H
#define FACEDETEC_H

#include <QObject>
#include <QRect>

#include <opencv2/opencv.hpp>
using namespace cv;

class FaceDetec : public QObject
{
    Q_OBJECT

public:
    explicit FaceDetec(QObject *parent = 0);

    bool loadCode(QByteArray &file);
    void loadImage(QByteArray &file);
    bool detecFace(QPoint &center);

signals:

public slots:

private:
    CascadeClassifier cascade;
    Mat cvimg;
};

#endif // FACEDETEC_H
