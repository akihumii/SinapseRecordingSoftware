#include "mainwindow.h"

MainWindow::MainWindow(){
    server= new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(on_predictionPort_connect()));

    server->listen(QHostAddress::LocalHost, 45454)? qDebug() << "Server is started" : qDebug() << "Server could not started";

    QHBoxLayout *mainLayout = new QHBoxLayout;
    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}

void MainWindow::motionDetected(){
    float d;
    QDataStream stream(socketAbstract->readAll());
    stream.setByteOrder(QDataStream::BigEndian);
    stream >> d;
//    if (d > max){
//            max = d;
//            qDebug() << "New max: " << max;
//    }
//    if (d < min){
//            min = d;
//            qDebug() << "New min: " << min;
//    }
    qDebug() << d;
}
void MainWindow::on_predictionPort_connect(){
    socketAbstract = new QTcpSocket(this);
    socketAbstract = server->nextPendingConnection();
    connect(socketAbstract, SIGNAL(disconnected()), socketAbstract, SLOT(deleteLater()));
    connect(socketAbstract, SIGNAL(readyRead()), this, SLOT(motionDetected()));
    qDebug() << "Connected";
    min = 0.0;
    max = 0.0;
}

MainWindow::~MainWindow(){

}
