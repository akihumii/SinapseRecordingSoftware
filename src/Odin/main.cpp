#include "odinwindow.h"
#include <QApplication>

using namespace Odin;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedSize(w.sizeHint());
    w.show();

    return a.exec();
}
