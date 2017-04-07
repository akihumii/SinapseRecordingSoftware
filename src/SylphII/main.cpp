#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainWindow w;
    w.setMinimumSize(1366,768);
    w.showMaximized();

    return a.exec();
}
