#include "graphdialog.h"

GraphDialog::GraphDialog(bool *dataSelected, QVector<double> *channelData, int X_axis, QWidget *parent) : QDialog(parent){
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QVector<double> x;
    for(int i = 0; i < X_axis; i++){
        x.append(i*0.000056);
    }
    for(int i = 0; i < 10; i++){
        if(dataSelected[i]){
            createGraph(i);
            mainLayout->addWidget(dataGraph[i]);
            dataGraph[i]->graph()->setData(x, channelData[i]);
            dataGraph[i]->replot();
        }
    }
    setLayout(mainLayout);
}

GraphDialog::GraphDialog(QComboBox *A[5], QComboBox *B[5], QCheckBox *diffEnable[5], QVector<double> *channelData, int X_axis, QWidget *parent) : QDialog(parent){
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QVector<double> x;
    for(int i = 0; i < X_axis; i++){
        x.append(i*0.000056);
    }
    QVector<double> diffData[5];
    for(int i = 0; i < 5; i++){
        if(diffEnable[i]->isChecked()){
            for(int j = 0; j < X_axis; j++){
                diffData[i].append(channelData[(A[i]->currentIndex())].at(j)
                                    - channelData[(B[i]->currentIndex())].at(j));
            }
            createGraph(i);
            mainLayout->addWidget(dataGraph[i]);
            dataGraph[i]->graph()->setData(x, diffData[i]);
            dataGraph[i]->replot();
        }
    }
    setLayout(mainLayout);
}

GraphDialog::GraphDialog(int preThreshold, int postThreshold, double threshold, QVector<double> *channelData, int channelSelected, QWidget *parent) : QDialog(parent){
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QVector<double> x;
    int timeFrameMS = postThreshold - preThreshold;
    qDebug() << timeFrameMS;
    int numPreThreshold =  preThreshold/0.056;
    qDebug() << "# pre threshold : " << numPreThreshold;
    int numPostThreshold = postThreshold/0.056;
    qDebug() << "# post threshold : " << numPostThreshold;
    for(int i = 0; i < (int) timeFrameMS/0.056; i++){
        x.append(i*0.000056);
    }
    int count = 0;
    for(int i = 0; i < channelData[channelSelected].size(); i++){
        if(channelData[channelSelected].at(i) >= threshold){
            indices[count] = i;
            count ++;
            i += numPostThreshold;
            qDebug() << "Crossed threshold at : " << i;
        }
    }
    QVector<double> allData[count];
    createGraph(0);
    for(int i = 0; i < count; i++){
        allData[i] = extractData(numPreThreshold, numPostThreshold, channelData[channelSelected], indices[i]);
        dataGraph[0]->graph()->setData(x, allData[i]);
        dataGraph[0]->yAxis->setRange(-0.21, 1.42, Qt::AlignLeft);
        dataGraph[0]->xAxis->setRange(0, (double) timeFrameMS/1000.0, Qt::AlignLeft);
        dataGraph[0]->replot();
        dataGraph[0]->addGraph();
    }
    mainLayout->addWidget(dataGraph[0]);
    setLayout(mainLayout);

    qDebug() << count;
}

QVector<double> GraphDialog::extractData(int numPreThreshold, int numPostThreshold, QVector<double> data, int Index){
    QVector<double> extractedData;
    for(int i = (Index + numPreThreshold); i < (Index + numPostThreshold); i++){
        extractedData.append(data.at(i));
    }
    return extractedData;
}

GraphDialog::~GraphDialog(){
}

void GraphDialog::createGraph(int i){
    dataGraph[i] = new QCustomPlot;
    dataGraph[i]->addGraph();
    dataGraph[i]->setInteractions(QCP::iRangeDrag);
    dataGraph[i]->axisRect()->setRangeDrag(Qt::Horizontal);
    dataGraph[i]->yAxis->setRange(-0.21, 1.42, Qt::AlignLeft);
}
