#ifndef MEASUREMENTDIALOG_H
#define MEASUREMENTDIALOG_H

#include <QDialog>
#include "../common/qtincludes.h"
#include "serialchannel.h"

class MeasurementDialog : public QDialog
{
    Q_OBJECT

public:
    MeasurementDialog(SerialChannel *NeutrinoSerial_);
    ~MeasurementDialog();

public slots:
    void updataData(double data);

private:
    SerialChannel *thorSerial;

    QLabel *labels[4];
    QString labelNames[4] = {   "Minimum:",
                                "Maximum:",
                                "Average:",
                                "Pk to Pk:"
    };
    QLabel *dataLabel[4];
    QString defaultData[4] = { "1.2",
                               "0",
                               "1.5",
                               "0"
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
