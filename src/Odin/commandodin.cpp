#include "commandodin.h"

CommandOdin::CommandOdin(SerialOdin *serialOdin_){
    serialOdin = serialOdin_;
}

void CommandOdin::sendTestCommand(){
    QByteArray testCommandArray;
    for(int i = 0; i < 16; i++){
        testCommandArray.append(testCommand[i]);
    }
    serialOdin->writeCommand(testCommandArray);
}
