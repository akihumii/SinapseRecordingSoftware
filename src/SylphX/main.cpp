#include "../Odin/odinwindow.h"
#include "mainwindow.h"
#include <QApplication>

using namespace SylphX;
using namespace Odin;

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    SylphX::MainWindow x;
    x.setFixedSize(x.sizeHint());
    x.show();

    Odin::MainWindow w;
    w.setMinimumSize(1366,768);
    w.showMaximized();


    return a.exec();
}
