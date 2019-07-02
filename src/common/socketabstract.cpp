#include "socketabstract.h"

SocketAbstract::SocketAbstract(){
    socketAbstract = new QTcpSocket(this);
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(updateRate()));
    timer->start(1000);
}

bool SocketAbstract::doConnect(QString ipAddress, int port){
    int connectionTries = 0;
    while(socketAbstract->state() != QAbstractSocket::ConnectedState){
        qDebug() << "Connecting...";
        socketAbstract->connectToHost(ipAddress, port);
        qDebug() << "Waiting...";
        if(!socketAbstract->waitForConnected(1000)){
            qDebug() << "Error: " << socketAbstract->errorString();
        }
        connectionTries++;
        if(connectionTries>2)
            return false;
    }
    return true;
}

void SocketAbstract::doDisconnect(){
    if(socketAbstract->state() == QAbstractSocket::ConnectedState){
        socketAbstract->flush();
        socketAbstract->disconnectFromHost();
        socketAbstract->close();
    }
}

bool SocketAbstract::isConnected(){
    return (socketAbstract->state() == QAbstractSocket::ConnectedState);
}

QString SocketAbstract::getError(){
    return socketAbstract->errorString();
}

int SocketAbstract::getRate(){
    return rate;
}

void SocketAbstract::write(QByteArray command){
    socketAbstract->write(command);
}

void SocketAbstract::setChecked(bool flag){
    checked = flag;
}

void SocketAbstract::updateRate(){
    rate = bytesRead*8/1000;
    if(rate == 0 && checked){
        checked = false;
    }
    bytesRead = 0;
}

void SocketAbstract::sendDisconnectSignal(){
    qDebug() << "Closing ESP";
    QByteArray closingMSG = "DISCONNECT!!!!!!";
    socketAbstract->write(closingMSG);
    socketAbstract->waitForBytesWritten(1000);
}
