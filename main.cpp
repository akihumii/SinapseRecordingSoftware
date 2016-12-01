#include "mainwindow_ma.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow_MA w;
    w.show();

    return a.exec();
}
