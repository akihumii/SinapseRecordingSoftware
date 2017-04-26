#include "commandodin.h"

CommandOdin::CommandOdin(SerialOdin *serialOdin_, SocketOdin *socketOdin_){
    serialOdin = serialOdin_;
    socketOdin = socketOdin_;
}

void CommandOdin::sendTestCommand(){
    QByteArray commandByte;
    for(int i = 0; i < 16; i++){
        commandByte.append(testCommand[i]);
    }
//    serialOdin->writeCommand(testCommandArray);
    socketOdin->writeCommand(commandByte);
}
