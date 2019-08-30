#ifndef SOCKETFORCE_H
#define SOCKETFORCE_H

#include "../common/qtincludes.h"
#include "../common/socketabstract.h"

namespace SylphX {

class SocketForce : public SocketAbstract{
    Q_OBJECT

public:
    SocketForce();
    void streamData(double data);
    void streamData(QVector<double> data);

private:
    QDataStream *out;
};

}
#endif // SOCKETFORCE_H
