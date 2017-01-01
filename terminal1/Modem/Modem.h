#ifndef MODEM_H
#define MODEM_H

#include <QDialog>

namespace Ui {
class Modem;
}

class Modem : public QDialog
{
    Q_OBJECT

public:
    explicit Modem(QWidget *parent = 0);
    ~Modem();

    void getFile(QString &name);
    void showTransfer(int filesize, int remain, float speed);

Q_SIGNALS:
    void outData(const QByteArray &data);

private:
    Ui::Modem *ui;
};

#endif // MODEM_H
