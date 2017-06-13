#include "measurementdialog.h"

MeasurementDialog::MeasurementDialog(SerialChannel *NeutrinoSerial_)
{
    NeutrinoSerial = NeutrinoSerial_;
    connect(NeutrinoSerial, SIGNAL(singleByteReady(double)), this, SLOT(updataData(double)));
    createLayout();
}

MeasurementDialog::~MeasurementDialog()
{
}

void MeasurementDialog::updataData(double data){
    if(data > max){
        max = data;
    }
    if(data < min){
        min = data;
    }
    avg = calcAverage(data);
    if(data_store.size() < 200){
        data_store.append(data);
    }
    else{
        pk2pk = findMax(data_store) - findMin(data_store);
        data_store.clear();
    }
    dataLabel[0]->setText(QString::number(min));
    dataLabel[1]->setText(QString::number(max));
    dataLabel[2]->setText(QString::number(avg));
    dataLabel[3]->setText(QString::number(pk2pk));
}

double MeasurementDialog::calcAverage(double data){
    totalPoints++;
    return avg + ((data-avg)/((double) totalPoints+1));
}

double MeasurementDialog::findMax(QVector<double> data){
    double temp = 0.0;
    for(int i = 0; i < data.size(); i++){
        if(data.at(i) > temp)
            temp = data.at(i);
    }
    return temp;
}

double MeasurementDialog::findMin(QVector<double> data){
    double temp = 1.2;
    for(int i = 0; i < data.size(); i++){
        if(data.at(i) < temp)
            temp = data.at(i);
    }
    return temp;
}

void MeasurementDialog::createLayout(){
    QVBoxLayout *labelLayout = new QVBoxLayout;
    QVBoxLayout *dataLayout = new QVBoxLayout;
    for(int i = 0; i < 4; i++){
        labels[i] = new QLabel(labelNames[i]);
        labelLayout->addWidget(labels[i]);

        dataLabel[i] = new QLabel(defaultData[i]);
        dataLayout->addWidget(dataLabel[i]);
    }
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(labelLayout);
    mainLayout->addLayout(dataLayout);
    setLayout(mainLayout);
//    mainLayout->setSizeConstraint( QLayout::SetFixedSize );
}
