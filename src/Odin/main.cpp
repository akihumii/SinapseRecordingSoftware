#include "odinwindow.h"
#include <QApplication>

using namespace Odin;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OdinWindow w;
    w.setFixedSize(w.sizeHint());
    w.showMaximized();

    return a.exec();
}
