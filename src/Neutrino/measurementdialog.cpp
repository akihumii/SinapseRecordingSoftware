#include "measurementdialog.h"

MeasurementDialog::MeasurementDialog(SerialChannel *NeutrinoSerial_, SocketNeutrino *socketNeutrino_)
{
    NeutrinoSerial = NeutrinoSerial_;
    socketNeutrino = socketNeutrino_;
    connect(NeutrinoSerial, SIGNAL(singleByteReady(double)), this, SLOT(updataData(double)));
    connect(socketNeutrino, SIGNAL(singleByteReady(double)), this, SLOT(updataData(double)));
    createLayout();
}

MeasurementDialog::~MeasurementDialog()
{
}

void MeasurementDialog::updataData(double data){
    if(data > max && data < 1.201){
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
    dataLabel[0]->setText("<b><FONT SIZE = 6>" + QString::number(min) + "</b>");
    dataLabel[1]->setText("<b><FONT SIZE = 6>" + QString::number(max) + "</b>");
    dataLabel[2]->setText("<b><FONT SIZE = 6>" + QString::number(avg) + "</b>");
    dataLabel[3]->setText("<b><FONT SIZE = 6>" + QString::number(pk2pk) + "</b>");
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
    recordButton = new QPushButton(tr("Start Recording!"));
    resetStatistic = new QPushButton(tr("Reset"));
    connect(recordButton, SIGNAL(clicked(bool)), this, SLOT(on_record_clicked()));
    connect(resetStatistic, SIGNAL(clicked(bool)), this, SLOT(on_reset_clicked()));
    QHBoxLayout *dispLayout = new QHBoxLayout;
    dispLayout->addLayout(labelLayout);
    dispLayout->addLayout(dataLayout);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(dispLayout);
    mainLayout->addWidget(recordButton);
    mainLayout->addWidget(resetStatistic);
    setLayout(mainLayout);
//    mainLayout->setSizeConstraint( QLayout::SetFixedSize );
}

void MeasurementDialog::on_record_clicked(){
    if(NeutrinoSerial->isConnected()){
        NeutrinoSerial->setRecordEnabled(!record);
    }
    if(socketNeutrino->isConnected()){
        socketNeutrino->setRecordEnabled(!record);
    }
    if(record){
        recordButton->setText("Start Recording!");
    }
    else{
        recordButton->setText("Stop Recording!");
    }
    record = !record;
}

void MeasurementDialog::on_reset_clicked(){
    max = 0;
    min = 1.2;
    avg = 0.5;
    pk2pk = 0;
    totalPoints = 0;
}
