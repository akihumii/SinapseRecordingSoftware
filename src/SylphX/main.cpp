#include "treedialog.h"

#include <QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

//    SylphX::MainWindow w;
//    w.setMinimumSize(1366,768);
    SylphX::TreeDialog treeDialog;
    treeDialog.show();
//    w.showMaximized();

    return a.exec();
}
