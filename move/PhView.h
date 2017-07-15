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

private:
    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);

private:
    Ui::PhView *ui;
    QPoint lastPos;
};

#endif // PHVIEW_H
