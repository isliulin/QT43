#include "PhView.h"
#include "ui_phview.h"

#include <QMouseEvent>
#include <QRect>

#include <QDebug>

PhView::PhView(QWidget *parent) :
    QLabel(parent),
    ui(new Ui::PhView)
{
    ui->setupUi(this);

    connect(ui->cutRect, SIGNAL(doubleClick()), this, SLOT(_cutOk()));
}

bool PhView::saveCut(QString file)
{
    return cutimg.save(file);
}

PhView::~PhView()
{
    delete ui;
}

void PhView::moveCenter(int x, int y)
{
    int w, h;

    w = ui->cutRect->rect().width();
    h = ui->cutRect->rect().height();

    ui->cutRect->move(x - w/2, y - h/2);
}

void PhView::moveX(int x)
{
    QPoint pos;
    int w;

    w = ui->cutRect->width();
    pos = ui->cutRect->pos();
    pos.setX(x - w/2);
    ui->cutRect->move(pos);
}

QRect PhView::getCutRect(bool hide)
{
    if (hide)
    {
        ui->cutRect->hide();
    }
    return ui->cutRect->rectCut();
}

void PhView::_cutOk()
{
    emit doubleClicked();
}

void PhView::showImage(QImage &oimg)
{
    QImage vimg;

    setAlignment(Qt::AlignLeft);
    vimg = oimg.scaled(size(), Qt::KeepAspectRatio);
    setPixmap(QPixmap::fromImage(vimg));
    ui->cutRect->show();

    scalefw = (float)vimg.width()/oimg.width();
    scalefh = (float)vimg.height()/oimg.height();
}

QImage* PhView::getCutImage(QImage &oimg, int width, int height)
{
    int x, y, w, h;
    QRect cr;

    cr = ui->cutRect->rectCut();

    x = cr.x() / scalefw;
    y = cr.y() / scalefh;
    w = cr.width() / scalefw;
    h = cr.height() / scalefh;
    if (w < width)
        w = width;
    if (h < height)
        h = height;

    cutimg = oimg.copy(x, y, w, h).scaled(width, height, Qt::KeepAspectRatio);

    return &cutimg;
}

void PhView::showCut(QImage &img)
{
    QImage vimg;

    ui->cutRect->hide();
    vimg = img.scaled(size(), Qt::KeepAspectRatio);
    setAlignment(Qt::AlignHCenter);
    setPixmap(QPixmap::fromImage(vimg));
}

void PhView::getScaleFactor(float &sfw, float &sfh)
{
    sfw = scalefw;
    sfh = scalefh;
}

void PhView::setSize(int w, int h)
{
    ui->cutRect->resize(w, h);
}
