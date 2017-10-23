#ifndef MEASUREMENTDIALOG_H
#define MEASUREMENTDIALOG_H

#include <QDialog>
#include "../common/qtincludes.h"
#include "serialchannel.h"
#include "socketneutrino.h"

class MeasurementDialog : public QDialog
{
    Q_OBJECT

public:
    MeasurementDialog(SerialChannel *NeutrinoSerial_, SocketNeutrino *socketNeutrino_);
    ~MeasurementDialog();

public slots:
    void updataData(double data);

private:
    SerialChannel *NeutrinoSerial;
    SocketNeutrino *socketNeutrino;

    QLabel *labels[4];
    QString labelNames[4] = {   "<b><FONT SIZE = 6> Minimum: </b>",
                                "<b><FONT SIZE = 6> Maximum: </b>",
                                "<b><FONT SIZE = 6> Average: </b>",
                                "<b><FONT SIZE = 6> Pk to Pk: </b>"
    };
    QLabel *dataLabel[4];
    QString defaultData[4] = { "<b><FONT SIZE = 6> 1.2 </b>",
                               "<b><FONT SIZE = 6> 0 </b>",
                               "<b><FONT SIZE = 6> 1.5 </b>",
                               "<b><FONT SIZE = 6> 0 </b>"
                             };

    double min = 1.2;
    double max = 0;
    double avg = 0.5;
    double pk2pk = 0;
    quint64 totalPoints = 0;
    QVector<double> data_store;

    void createLayout();
    double calcAverage(double data);
    double findMin(QVector<double> data);
    double findMax(QVector<double> data);
};

#endif // MEASUREMENTDIALOG_H
