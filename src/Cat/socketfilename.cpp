#include "socketfilename.h"

namespace Cat {
SocketFilename::SocketFilename(){
}

void SocketFilename::writeCommand(QByteArray command){
    if(this->isConnected()){
        socketAbstract->write(command);
        qDebug() << "Sent filename via TCP socket: " << (quint8) command.at(0);
    }
}

}
