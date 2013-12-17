#ifndef COPYDIALOG_H
#define COPYDIALOG_H

#include <QDialog>

namespace Ui {
class copyDialog;
}

class copyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit copyDialog(QWidget *parent = 0);
    ~copyDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::copyDialog *ui;
};

#endif // COPYDIALOG_H
