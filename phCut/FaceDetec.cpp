#include "FaceDetec.h"

FaceDetec::FaceDetec(QObject *parent) : QObject(parent)
{

}

bool FaceDetec::loadCode(QByteArray &file)
{
    return cascade.load(String(file.data()));
}

void FaceDetec::loadImage(QByteArray &file)
{
    cvimg = imread(String(file.data()), cv::IMREAD_UNCHANGED);
}

bool FaceDetec::detecFace(QPoint &center)
{
    std::vector<Rect> faces;
    Mat frame_gray;

    cvtColor(cvimg, frame_gray, CV_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
    if (faces.size() == 0)
    {
        return false;
    }

    center.setX(faces[0].x + faces[0].width/2);
    center.setY(faces[0].y + faces[0].height/2);

    return true;
}
