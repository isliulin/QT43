#ifndef PASTEDIALOG_H
#define PASTEDIALOG_H

#include <QDialog>

namespace Ui {
class pasteDialog;
}

class pasteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit pasteDialog(QWidget *parent = 0);
    ~pasteDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::pasteDialog *ui;
};

#endif // PASTEDIALOG_H
