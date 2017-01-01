#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_home_clicked()
{
    QByteArray data = "he\x1B[1;1Hllo";

    emit outData(data);
}

void Dialog::on_start_clicked()
{
    QByteArray data = "\r行首";

    emit outData(data);
}

void Dialog::on_up_clicked()
{
    QByteArray data = "\x1B[A";

    emit outData(data);
}

void Dialog::on_down_clicked()
{
    QByteArray data = "\x1B[B";

    emit outData(data);
}

void Dialog::on_left_clicked()
{
    QByteArray data = "\x1B[C";

    emit outData(data);
}

void Dialog::on_right_clicked()
{
    QByteArray data = "\x1B[D";

    emit outData(data);
}

void Dialog::on_reset_clicked()
{
    QByteArray data = "\x1B[myes";

    emit outData(data);
}

void Dialog::on_color_clicked()
{
    QByteArray data;
    QString b,f,t;

    b = ui->back->currentText();
    f = ui->fore->currentText();
    t = "\x1B[" + b + ";" + f + "mtest";
    data = t.toLocal8Bit();

    emit outData(data);
}

void Dialog::on_lend_clicked()
{
    QByteArray data = "\x1B[K";

    emit outData(data);
}

void Dialog::on_lstart_clicked()
{
    QByteArray data = "\x1B[1K";

    emit outData(data);
}

void Dialog::on_lentire_clicked()
{
    QByteArray data = "\x1B[2K";

    emit outData(data);
}

void Dialog::on_ldown_clicked()
{
    QByteArray data = "\x1B[J";

    emit outData(data);
}

void Dialog::on_lup_clicked()
{
    QByteArray data = "\x1B[1J";

    emit outData(data);
}

void Dialog::on_screen_clicked()
{
    QByteArray data = "\x1B[2J";

    emit outData(data);
}
