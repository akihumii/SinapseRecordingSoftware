#ifndef GRAPHDIALOG_H
#define GRAPHDIALOG_H

#include <QDialog>
#include "qtincludes.h"
#include "../common/qcustomplot.h"

class GraphDialog : public QDialog
{
    Q_OBJECT

public:
    GraphDialog(bool *dataSelected, QVector<double> *channelData, int X_axis, double xSteps, QWidget *parent = 0);
    GraphDialog(bool *dataSelected, QVector<double> *channelData, quint64 X_axis, quint64 ADC_xAxis, QWidget *parent = 0);
    GraphDialog(QComboBox *A[], QComboBox *B[], QCheckBox *diffEnable[], QVector<double> *channelData, int X_axis, QWidget *parent = 0);
    GraphDialog(int preThreshold, int postThreshold, int greaterlesser, int maxSpikes, double threshold, QVector<double> *channelData, int channelSelected, QWidget *parent = 0);
    ~GraphDialog();

private:
    void createGraph(int i);
    int count = 0;
    QCustomPlot *dataGraph[12];
    bool graphExist[12] = {false, false, false, false, false, false, false, false, false, false, false, false};
    QVector<double> extractData(int numPreThreshold, int numPostThreshold, QVector<double> data, int Index);
    QVector<double> averageData(QVector<double> *allData, int size);

private slots:
    void selectionChanged();
    void mouseWheel();
    void setAllRange(QCPRange range);
};

#endif // GRAPHDIALOG_H
