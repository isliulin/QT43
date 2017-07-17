#ifndef FACEDETEC_H
#define FACEDETEC_H

#include <QObject>
#include <QRect>
#include <QImage>

#include <opencv2/opencv.hpp>
using namespace cv;

class FaceDetec : public QObject
{
    Q_OBJECT

public:
    explicit FaceDetec(QObject *parent = 0);

    bool loadCode(QByteArray &file);
    QImage* loadImage(QString &file);
    bool detecFace(QPoint &center);
    QImage* qimage();

signals:

public slots:

private:
    CascadeClassifier cascade;
    Mat cvimg;
    QImage qimg;
};

#endif // FACEDETEC_H
