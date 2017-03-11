#include "FaceWorker.h"

#include <QImage>

FaceWorker::FaceWorker(QObject *parent) : QObject(parent)
{
    moveToThread(&thread);
}

void FaceWorker::start()
{
   thread.start();
}

void FaceWorker::DoWork(int cmd, QByteArray param)
{
    switch (cmd)
    {
    case 1:
    {
        LoadCode(param);
    }break;
    case 2:
    {
        LoadImage(param);
    }break;
    case 3:
    {
        FaceDetec();
    }break;
    }
}

void FaceWorker::FaceDetec()
{
    std::vector<Rect> faces;
    Mat frame_gray;

    cvtColor(image, frame_gray, CV_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

    for (int i = 0; i < faces.size(); i++)
    {
        Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
        ellipse(image, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
    }

    imshow("window_name", image);

}

void FaceWorker::LoadImage(QByteArray &file)
{
    image = cv::imread(String(file.data()), cv::IMREAD_UNCHANGED);
}

void FaceWorker::LoadCode(QByteArray &file)
{
    cascade.load(String(file.data()));
}
