#include "graphdialog.h"

GraphDialog::GraphDialog(bool *dataSelected, QVector<double> *channelData, int X_axis, QWidget *parent) : QDialog(parent){
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QVector<double> x;
    for(int i = 0; i < X_axis; i++){
        x.append(i*0.000056);
    }
    for(int i = 0; i < 12; i++){
        if(dataSelected[i]){
            createGraph(i);
            mainLayout->addWidget(dataGraph[i]);
            dataGraph[i]->graph()->setData(x, channelData[i]);
            dataGraph[i]->replot();
        }
    }
    setLayout(mainLayout);
}

GraphDialog::GraphDialog(QComboBox *A[5],
                            QComboBox *B[5],
                            QCheckBox *diffEnable[5],
                            QVector<double> *channelData,
                            int X_axis,
                            QWidget *parent) : QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QVector<double> x;
    for(int i = 0; i < X_axis; i++){
        x.append(i*0.000202);
    }
    QVector<double> diffData[6];
    for(int i = 0; i < 6; i++){
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

GraphDialog::GraphDialog(int preThreshold,
                         int postThreshold,
                         int greaterlesser,
                         int maxSpikes,
                         double threshold,
                         QVector<double> *channelData,
                         int channelSelected,
                         QWidget *parent) : QDialog(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QVector<double> x;
    int timeFrameMS = postThreshold - preThreshold;
    int numPreThreshold =  preThreshold/0.202;
    int numPostThreshold = postThreshold/0.202;
    for(int i = 0; i < (int) timeFrameMS/0.202; i++){
        x.append(i*0.000202);
    }
    int indices[maxSpikes];
    count = 0;
    for(int i = 0; i < channelData[channelSelected].size(); i++){
        if(channelData[channelSelected].at(i) >= threshold && greaterlesser == 0 && count < maxSpikes){
            indices[count] = i;
            count ++;
            if((i + numPostThreshold) < channelData[channelSelected].size()){
                i += numPostThreshold;
            }
        }
        if(channelData[channelSelected].at(i) <= threshold && greaterlesser == 1 && count < maxSpikes){
            indices[count] = i;
            count ++;
            if((i + numPostThreshold) < channelData[channelSelected].size()){
                i += numPostThreshold;
            }
        }
    }
    QVector<double> allData[count];
    createGraph(0);
    for(int i = 0; i < count; i++){
        allData[i] = extractData(numPreThreshold, numPostThreshold, channelData[channelSelected], indices[i]);
        dataGraph[0]->graph()->setData(x, allData[i]);
        dataGraph[0]->yAxis->setRange(-0.005, 0.01, Qt::AlignLeft);
        dataGraph[0]->xAxis->setRange(0, (double) timeFrameMS/1000.0, Qt::AlignLeft);
        dataGraph[0]->replot();
        dataGraph[0]->addGraph();
    }
    if(count > 0){
        dataGraph[0]->graph()->setData(x, averageData(allData, count));
        dataGraph[0]->yAxis->setRange(-0.005, 0.01, Qt::AlignLeft);
        dataGraph[0]->xAxis->setRange(0, (double) timeFrameMS/1000.0, Qt::AlignLeft);
        QPen avgPen;
        avgPen.setColor(Qt::red);
        avgPen.setWidth(3);
        dataGraph[0]->graph()->setPen(avgPen);
        dataGraph[0]->replot();
    }

    mainLayout->addWidget(dataGraph[0]);
    setLayout(mainLayout);
}

QVector<double> GraphDialog::extractData(int numPreThreshold, int numPostThreshold, QVector<double> data, int Index){
    QVector<double> extractedData;

    for(int i = (Index + numPreThreshold); i < (Index + numPostThreshold); i++){
        if(i > 0 && i < data.size()){
            extractedData.append(data.at(i));
        }
    }
    return extractedData;
}

GraphDialog::~GraphDialog(){
}

void GraphDialog::createGraph(int i){
    graphExist[i] = true;
    dataGraph[i] = new QCustomPlot;
    dataGraph[i]->addGraph();
    dataGraph[i]->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes);
    dataGraph[i]->axisRect()->setRangeDrag(Qt::Horizontal);
    connect(dataGraph[i]->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(setAllRange(QCPRange)));
    dataGraph[i]->yAxis->setRange(-0.005, 0.01, Qt::AlignLeft);
    connect(dataGraph[i], SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
    connect(dataGraph[i], SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
}

QVector<double> GraphDialog::averageData(QVector<double> *allData, int size){
    QVector<double> averagedData;
    for(int j = 0; j < allData[0].size(); j++){
        double temp = 0;
        for(int i = 0; i < size; i++){
            temp += allData[i].at(j);

        }
        temp = temp / size;
        averagedData.append(temp);
    }
    return averagedData;
}

void GraphDialog::selectionChanged(){
    for(int i = 0; i < 12; i++){
        if(graphExist[i]){
            // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
            if (dataGraph[i]->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || dataGraph[i]->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
              dataGraph[i]->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || dataGraph[i]->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
            {
            dataGraph[i]->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
            dataGraph[i]->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
            }
            // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
            if (dataGraph[i]->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || dataGraph[i]->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
              dataGraph[i]->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || dataGraph[i]->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
            {
            dataGraph[i]->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
            dataGraph[i]->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
            }
        }
    }
}

void GraphDialog::mouseWheel(){
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed
    for(int i = 0; i < 12; i++){
        if(graphExist[i]){
            if (dataGraph[i]->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
            dataGraph[i]->axisRect()->setRangeZoom(dataGraph[i]->xAxis->orientation());
            else if (dataGraph[i]->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
            dataGraph[i]->axisRect()->setRangeZoom(dataGraph[i]->yAxis->orientation());
            else
            dataGraph[i]->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
        }
    }
}

void GraphDialog::setAllRange(QCPRange range){
    for(int i = 0; i < 12; i++){
        if(graphExist[i]){
            qDebug() << "Setting range: " << i;
            dataGraph[i]->xAxis->setRange(range);
            dataGraph[i]->replot();
        }
    }
}
