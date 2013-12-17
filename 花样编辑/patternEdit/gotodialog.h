#ifndef GOTODIALOG_H
#define GOTODIALOG_H

#include <QDialog>

namespace Ui {
class gotoDialog;
}

class gotoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit gotoDialog(QWidget *parent = 0);
    ~gotoDialog();

    void setXYMax(unsigned x, unsigned y);
    void getXY(int *x, int *y);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::gotoDialog *ui;
};

#endif // GOTODIALOG_H
