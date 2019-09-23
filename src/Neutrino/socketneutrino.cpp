#include "socketneutrino.h"

SocketNeutrino::SocketNeutrino(Command *NeutrinoCommand_, DataProcessor *NeutrinoData_, Channel *NeutrinoChannel_){
    NeutrinoCommand = NeutrinoCommand_;
    NeutrinoData = NeutrinoData_;
    NeutrinoChannel = NeutrinoChannel_;

    File = new QFile;

    connect(socketAbstract, SIGNAL(readyRead()), this, SLOT(readData()));
}

void SocketNeutrino::readData(){
    switch (NeutrinoCommand->getOPMode()){
        case 2:{
            if(NeutrinoData->isPlotEnabled()){
                if(NeutrinoData->getBitMode() == WORDLENGTH_8){
                    NeutrinoData->ParseFrameMarkers8bits(socketAbstract->read(maxSize));
                }
            }
            break;
        }
        case 3:{
            if(NeutrinoData->isPlotEnabled()){
                if(NeutrinoData->getBitMode() == WORDLENGTH_10){
                    NeutrinoData->MultiplexChannelData(NeutrinoData->ParseFrameMarkers10bits(socketAbstract->read(maxSize)));
                }
            }
            break;
        }
        case 4:{
            qDebug() << (quint8) socketAbstract->read(maxSize).at(0);
            break;
        }
        case 5:{
            QByteArray temp;
            emit singleByteReady(NeutrinoData->signalReconstruction((char) socketAbstract->read(1).at(0)));
            temp = socketAbstract->read(maxSize);
            if(record){
                for(int i = 0; i < temp.size(); i++){
                    *out << (uint8_t) temp.at(i) << "\n";
                }
            }
            break;
        }
        case 7:{
            QByteArray temp;
            temp = socketAbstract->read(maxSize);
            if(record){
                for(int i = 0; i < temp.size(); i++){
                    *out << (uint8_t) temp.at(i) << "\n";
                }
            }
            emit singleByteReady(NeutrinoData->signalReconstruction((char) temp.at(0)));
            break;
        }
        case 6:
        case 8:{
            QByteArray temp;
            temp = socketAbstract->read(2);
            emit singleByteReady(NeutrinoData->signalReconstruction((char) temp.at(0), (char) temp.at(1)));
            break;
        }
        case 9:{
            emit singleByteReady(NeutrinoData->signalReconstruction((char) socketAbstract->read(1).at(0)));
            socketAbstract->read(maxSize);
            break;
        }
        case 10:
        case 11:{
            QByteArray temp;
            temp = socketAbstract->read(2048);
            quint32 tempChar = temp.at(0) + temp.at(1) + temp.at(2) + temp.at(3);
            tempChar = tempChar/4;
            currentByte = (char) tempChar;
            break;
        }
        default:
            break;
    }
}

void SocketNeutrino::setRecordEnabled(bool flag){
    if(flag){
        if(NeutrinoCommand->getOPMode() == 5){
            fileName = directory + QDateTime::currentDateTime().toString("'Analog_'yyyyMMdd_HHmmss'.csv'");
        }
        else if(NeutrinoCommand->getOPMode() == 7){
            fileName = directory + QDateTime::currentDateTime().toString("'BioImpedance_'yyyyMMdd_HHmmss'.csv'");
        }
        File->setFileName(fileName);
        File->open(QIODevice::WriteOnly|QIODevice::Text)? qDebug()<< "File opened" : qDebug() << "File failed to open";
        out = new QTextStream(File);
    }
    else{
        File->close();
        qDebug() << "File closed";
    }
    record = flag;
}

char SocketNeutrino::getCurrentByte(){
    return currentByte;
}

bool SocketNeutrino::writeCommand(QByteArray Command){
    if(socketAbstract->state() == QAbstractSocket::ConnectedState){
        socketAbstract->write(Command);
        return socketAbstract->waitForBytesWritten();
    }
    else
        return false;
}
