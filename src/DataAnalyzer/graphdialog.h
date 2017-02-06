#ifndef GRAPHDIALOG_H
#define GRAPHDIALOG_H

#include <QDialog>
#include "qtincludes.h"
#include "../common/qcustomplot.h"

class GraphDialog : public QDialog
{
    Q_OBJECT

public:
    GraphDialog(bool *dataSelected, QVector<double> *channelData, int X_axis, QWidget *parent = 0);
    GraphDialog(QComboBox *A[], QComboBox *B[], QCheckBox *diffEnable[], QVector<double> *channelData, int X_axis, QWidget *parent = 0);
    GraphDialog(int preThreshold, int postThreshold, double threshold, QVector<double> *channelData, int channelSelected, QWidget *parent = 0);
    ~GraphDialog();

private:
    void createGraph(int i);
    int indices[30];
    QCustomPlot *dataGraph[10];
    QVector<double> extractData(int numPreThreshold, int numPostThreshold, QVector<double> data, int Index);
};

#endif // GRAPHDIALOG_H
