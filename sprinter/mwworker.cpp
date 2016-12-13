#include "mwworker.h"
#include "mainwindow.h"
#include "libqrencode/qrencode.h"

mwworker::mwworker(QWidget *parent)
{
    ui = (MainWindow*)parent;
    isrun = false;
}

void mwworker::run()
{
    isrun = true;
    QRcode *code;

    code = QRcode_encodeString("sissi", 2, QR_ECLEVEL_L, QR_MODE_8, 0);

    ui->DrawQrCode();
    while (isrun)
    {

    }

    QRcode_free(code);
}
