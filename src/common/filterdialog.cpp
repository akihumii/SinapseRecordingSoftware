#include "filterdialog.h"

FilterDialog::FilterDialog(Data *_dataProcessor){
    dataProcessor = _dataProcessor;
    createLayout();
}

void FilterDialog::createLayout(){
    QHBoxLayout *mainLayout = new QHBoxLayout;

    channel = new QGroupBox("Filter Setting");

    channelLayout = new QVBoxLayout;

    filterLabel = new QLabel("Notch Filter");
    notchFilter = new QComboBox;
    notchFilter->addItem("Disabled");
    notchFilter->addItem("50Hz");
    notchFilter->addItem("60Hz");
    connect(notchFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(on_notchFilter_changed(int)));

    notchLayout = new QHBoxLayout;
    notchLayout->addWidget(filterLabel);
    notchLayout->addWidget(notchFilter);

    lownhighLayout = new QHBoxLayout;

    highpassLayout = new QVBoxLayout;
    filterEnable[1] = new QCheckBox(tr("High pass Filter (Cutoff Freq in Hz)"));
    connect(filterEnable[1], SIGNAL(toggled(bool)), this, SLOT(on_highpassEnable_toggled(bool)));
    cutoffLabel[1] = new QLabel("Cutoff Freq (Hertz)");
    cutoffFreq[1] = new QLineEdit;

    cutoffFreq[1]->setEnabled(false);
    connect(cutoffFreq[1], SIGNAL(returnPressed()), this, SLOT(on_highpassFreq_changed()));
    highpassLayout->addWidget(filterEnable[1]);
    highpassLayout->addWidget(cutoffFreq[1]);
    lownhighLayout->addLayout(highpassLayout);

    channelLayout->addLayout(notchLayout);
    channelLayout->addLayout(lownhighLayout);

    channel->setLayout(channelLayout);
    mainLayout->addWidget(channel);
    setLayout(mainLayout);

    mainLayout->setSizeConstraint( QLayout::SetFixedSize );

    loadPrevSetting();
}

void FilterDialog::loadPrevSetting(){
    if(dataProcessor->isNotchFilterEnabled()){
        if(dataProcessor->currentNotchFreq() == 50){
            notchFilter->setCurrentIndex(1);
        }
        else if(dataProcessor->currentNotchFreq() == 60){
            notchFilter->setCurrentIndex(2);
        }
    }
    else{
        notchFilter->setCurrentIndex(0);
    }

    if(dataProcessor->isHighpassFilterEnabled()){
        filterEnable[1]->setChecked(true);
        cutoffFreq[1]->setText(QString::number(dataProcessor->currentHighpassFreq()));
    }
    else{
        filterEnable[1]->setChecked(false);
    }
}

FilterDialog::~FilterDialog(){

}

void FilterDialog::on_highpassEnable_toggled(bool enableFlag){
        cutoffLabel[1]->setEnabled(enableFlag);
        cutoffFreq[1]->setEnabled(enableFlag);
        if(dataProcessor->currentHighpassFreq() != 0){
            cutoffFreq[1]->setText(QString::number(dataProcessor->currentHighpassFreq()));
            dataProcessor->setHighpassFilter(dataProcessor->currentHighpassFreq(), samplingFreq);
        }
        dataProcessor->setHighpassFilterEnabled(enableFlag);
}

void FilterDialog::on_notchFilter_changed(int Index){
        if(Index == 0){
            dataProcessor->setNotchFilterEnabled(false);
        }
        else{
            dataProcessor->setNotchFilterEnabled(true);
            if(Index == 1){
                dataProcessor->setNotchFilter(50.0, 20.0, samplingFreq);
            }
            if(Index == 2){
                dataProcessor->setNotchFilter(60.0, 20.0, samplingFreq);
            }
        }
}

void FilterDialog::on_highpassFreq_changed(){
    if(cutoffFreq[1]->text().toInt() > 99 && cutoffFreq[1]->text().toInt() < 7501){
        dataProcessor->setHighpassFilter(cutoffFreq[1]->text().toDouble(), samplingFreq);
    }
    else{
        QMessageBox::warning(        this,
                                     tr("Invalid Frequency"),
                                     tr("Please enter a valid cutoff frequency ranging from 100 to 7500"));
    }
}
