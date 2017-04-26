#ifndef COMMANDODIN_H
#define COMMANDODIN_H

#include "serialodin.h"
#include "socketodin.h"

class CommandOdin
{
public:
    CommandOdin(SerialOdin *serialOdin_, SocketOdin *socketOdin_);
    void sendTestCommand();
private:
    SerialOdin *serialOdin;
    SocketOdin *socketOdin;


    quint8 testCommand[16] = {
        0xAA,
        0x55,
        0x0C,
        0x2D,
        0x25,
        0x02,
        0x35,
        0xE4,
        0x00,
        0x00,
        0x00,
        0x55,
        0x00,
        0x00,
        0x00,
        0x00
    };
};

#endif // COMMANDODIN_H
