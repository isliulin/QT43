#ifndef MODEM_H
#define MODEM_H

#include <QDialog>

namespace Ui {
class Modem;
}

class Ymodem;

class Modem : public QDialog
{
    Q_OBJECT

public:
    explicit Modem(QWidget *parent = 0);
    ~Modem();

    void getFile(QString &name);
    void showTransfer(int filesize, int remain, float speed);
    void startTransfer(char type = 'C');

public Q_SLOTS:
    void putData(const QByteArray &data);

Q_SIGNALS:
    void outData(const QByteArray &data);

private:
    Ui::Modem *ui;
    Ymodem *ym;
};

#endif // MODEM_H
