#include "socketforce.h"

namespace SylphX {

SocketForce::SocketForce(){
    out = new QDataStream(socketAbstract);
}

void SocketForce::streamData(double data){
    *out << data;
}

void SocketForce::streamData(QVector<double> data){
    for(int i = 0; i < data.size(); i++){
        *out << data.at(i);
    }
}
}
