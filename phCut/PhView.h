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

private:
    Ui::PhView *ui;
};

#endif // PHVIEW_H
