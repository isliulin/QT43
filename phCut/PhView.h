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

private:
    Ui::PhView *ui;
};

#endif // PHVIEW_H
