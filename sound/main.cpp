#include "mainwindow.h"
#include <QApplication>

#include "KeyReader.h"

KeyReader *pKbd = NULL;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pKbd = new KeyReader;

    MainWindow w;
    w.show();

    pKbd->start();

    return a.exec();
}
