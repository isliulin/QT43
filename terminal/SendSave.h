#ifndef SENDSAVE_H
#define SENDSAVE_H

#include <QDialog>

namespace Ui {
class SendSave;
}

class SSWorker;
class QTableWidgetItem;

class SendSave : public QDialog
{
    Q_OBJECT

public:
    explicit SendSave(QWidget *parent = 0);
    ~SendSave();

    void tableAddRow(QString &name, QString &type, QString &value, QString &endline);

signals:
    void dataSend(const QByteArray &data);

private:
    void tableInit();
    void dataMake(QByteArray &buf, QString &type, QString &value, QString &endline);

private slots:
    void on_send1_clicked();

    void on_send2_clicked();

    void on_send3_clicked();

    void on_add_clicked();

    void on_tbSave_itemChanged(QTableWidgetItem *item);

    void on_clear_clicked();

    void VHeaderClicked(int index);

private:
    Ui::SendSave *ui;
    SSWorker *worker;
};

#endif // SENDSAVE_H
