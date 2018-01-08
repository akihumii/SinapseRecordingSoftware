#include "socketneutrino.h"

SocketNeutrino::SocketNeutrino(Command *NeutrinoCommand_, DataProcessor *NeutrinoData_, Channel *NeutrinoChannel_){
    NeutrinoCommand = NeutrinoCommand_;
    NeutrinoData = NeutrinoData_;
    NeutrinoChannel = NeutrinoChannel_;

    File = new QFile;

    connect(socketAbstract, SIGNAL(readyRead()), this, SLOT(ReadCommand()));
}

void SocketNeutrino::ReadCommand(){
//    if(wifiEnabled){
        switch (NeutrinoCommand->getOPMode()){
            case 2:{
                if(NeutrinoData->isPlotEnabled()){
                    if(Mode_8Bit == true){
//                        qDebug() << "Got 8 bit data";
                        NeutrinoData->MultiplexChannelData(NeutrinoData->ParseFrameMarkers8bits(socketAbstract->read(maxSize)));
                    }
                    break;
                }
            }
            case 3:{
                if(NeutrinoData->isPlotEnabled()){
                    if(Mode_8Bit == false){
//                        qDebug() << "Got 10 bit data";
                        NeutrinoData->MultiplexChannelData(NeutrinoData->ParseFrameMarkers10bits(socketAbstract->read(maxSize)));
                    }
                    break;
                }
            }
            case 5:{
                emit singleByteReady(NeutrinoData->signalReconstruction((char) socketAbstract->read(1).at(0)));
                if(record){
                    QByteArray temp;
                    temp = socketAbstract->read(maxSize);
                    for(int i = 0; i < temp.size(); i++){
                        *out << (uint8_t) temp.at(i) << "\n";
                    }
                }
                else{
                    socketAbstract->read(maxSize);
                }
                break;
            }
            case 6:{
                QByteArray temp;
                temp = socketAbstract->read(2);
                emit singleByteReady(NeutrinoData->signalReconstruction((char) temp.at(0), (char) temp.at(1)));
//                socketAbstract->read(maxSize);
                break;
            }
            case 7:{
                if(record){
                    QByteArray temp;
                    temp = socketAbstract->read(maxSize);
                    for(int i = 0; i < temp.size(); i++){
                        *out << (uint8_t) temp.at(i) << "\n";
                    }
                    emit singleByteReady(NeutrinoData->signalReconstruction((char) temp.at(0)));
                }
                else{
                    QByteArray temp;
                    temp = socketAbstract->read(maxSize);
                    emit singleByteReady(NeutrinoData->signalReconstruction((char) temp.at(0)));
                }
                break;
            }
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
            case 10:{
                QByteArray temp = socketAbstract->read(2048);
                currentByte = (char) (temp.at(temp.size()-1) + temp.at(temp.size()-2) + temp.at(temp.size()-3) + temp.at(temp.size()-4) + temp.at(temp.size()-5) + temp.at(temp.size()-6) + temp.at(temp.size()-7) + temp.at(temp.size()-8) + temp.at(temp.size()-9) + temp.at(temp.size()-10))/10;
                break;
            }
            case 11:{
                QByteArray temp = socketAbstract->read(2048);
                currentByte = (char) (temp.at(temp.size()-1) + temp.at(temp.size()-2) + temp.at(temp.size()-3) + temp.at(temp.size()-4) + temp.at(temp.size()-5) + temp.at(temp.size()-6) + temp.at(temp.size()-7) + temp.at(temp.size()-8) + temp.at(temp.size()-9) + temp.at(temp.size()-10))/10;
                break;
            }
        default:
            break;
        }
//    }
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
        if(File->open(QIODevice::WriteOnly|QIODevice::Text)){
            qDebug()<< "File opened";
        }
        else{
            qDebug() << "File failed to open";
        }
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
    lastSentCommand = Command;
    if(socketAbstract->state() == QAbstractSocket::ConnectedState){
        if(Command.size()>5){
            if(Command.at(6) == (char) WL_8){
                Mode_8Bit = true;
                NeutrinoData->setBitMode(true);
                NeutrinoData->setPlotEnabled(true);
                NeutrinoData->clearallChannelData();
                qDebug() << "8 Bit Mode";
                NeutrinoChannel->setNumChannels(getNumChannels(Command));
            }
            else if(Command.at(6) == (char) WL_10){
                Mode_8Bit = false;
                NeutrinoData->setBitMode(false);
                NeutrinoData->setPlotEnabled(true);
                NeutrinoData->clearallChannelData();
                qDebug() << "10 Bit Mode";
                NeutrinoChannel->setNumChannels(getNumChannels(Command));
            }
            else {
                NeutrinoData->setPlotEnabled(false);
            }
        }
        socketAbstract->write(Command);         //write the command itself
//        qDebug() << Command;
        return socketAbstract->waitForBytesWritten();
    }
    else
        return false;
}

int SocketNeutrino::getNumChannels(QByteArray lastCommand){
    numChannels = 0;
    for(int i=7;i<17;i++){
        if (lastCommand.at(i) == (const char) CHANNEL_ON){
            numChannels++;
        }
    }
    return numChannels;
}

QByteArray SocketNeutrino::getlastCommand(){
    return lastSentCommand;
}
