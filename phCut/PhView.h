#ifndef PHVIEW_H
#define PHVIEW_H

#include <QLabel>

namespace Ui {
class PhView;
}

class PhView : public QLabel
{
    Q_OBJECT

public:
    explicit PhView(QWidget *parent = 0);
    ~PhView();

    void moveCenter(int x, int y);
    void moveX(int x);
    QRect getCutRect(bool hide = false);
    void setSize(int w, int h);
    void showImage(QImage &img);
    void getScaleFactor(float &sfw, float &sfh);
    void showCut(QImage &img);
    QImage* getCutImage(QImage &oimg, int w, int h);
    bool saveCut(QString file);

signals:
    void doubleClicked();

private slots:
    void _cutOk();

private:
    Ui::PhView *ui;
    float scalefw;
    float scalefh;
    QImage cutimg;
};

#endif // PHVIEW_H
