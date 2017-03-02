#include "mainwindow.h"
#include <QApplication>
#include "qtincludes.h"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    QString filename =  QFileDialog::getOpenFileName(
                                                Q_NULLPTR,
                                                "Open Implant Data",
                                                QDir::currentPath(),
                                                "All files (*.*)");
    qDebug() << filename;
    MainWindow w(filename);
//    w.setMinimumSize(1366,768);
    w.show();

    return a.exec();
}
