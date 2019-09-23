#ifndef SOCKETFILENAME_H
#define SOCKETFILENAME_H

#include "../common/qtincludes.h"
#include "../common/socketabstract.h"

namespace Cat {

class SocketFilename : public SocketAbstract{
    Q_OBJECT

public:
    SocketFilename();
    void writeCommand(QByteArray command);
};

}
#endif // SOCKETFILENAME_H
