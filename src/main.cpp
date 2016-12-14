#include "mainwindow_ma.h"
#include "mainwindow_ka.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QCoreApplication::addLibraryPath("./");
    QApplication a(argc, argv);
    MainWindow_MA w;
    w.show();
    w.setMinimumSize(1366,768);

    return a.exec();
}
