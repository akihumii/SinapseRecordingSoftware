#ifndef DYNOMOMETER_H
#define DYNOMOMETER_H

#include <QObject>
#include "../common/socketabstract.h"
#include "../common/qtincludes.h"

class Dynomometer : public SocketAbstract {
    Q_OBJECT
public:
    Dynomometer();
private:
    QTcpServer *server;
    QTcpSocket *socketDyno;
    QVector<double> dynoData;

signals:
    void dynoDataReady(double data);

private slots:
    void on_dynoPort_connect();
    void getDynoData();

public slots:

};

#endif // DYNOMOMETER_H
