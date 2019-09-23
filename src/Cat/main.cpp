#include "catwindow.h"
#include <QApplication>

using namespace Cat;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CatWindow w;
    w.setFixedSize(w.sizeHint());
//    w.showMaximized();
//    w.show();

    return a.exec();
}
