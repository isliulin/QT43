#include "mainwindow.h"
#include <QApplication>
#include "KeyReader.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MainWindow w;
    w.show();
    KeyReader kbd;
    kbd.start();
    return a.exec();
}
