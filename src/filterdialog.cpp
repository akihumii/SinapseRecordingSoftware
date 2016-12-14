#include "filterdialog.h"

FilterDialog::FilterDialog(DataProcessor_MA *_dataProcessor){
    dataProcessor = _dataProcessor;
    createLayout();
//    on_channelFilter_toggled();
}

void FilterDialog::createLayout(){
    QHBoxLayout *mainLayout = new QHBoxLayout;

    channel = new QGroupBox("Filter Setting");

    channelLayout = new QVBoxLayout;

//    channelFilterEnable = new QCheckBox(tr("Enable Filter"));
//    connect(channelFilterEnable, SIGNAL(toggled(bool)), this, SLOT(on_channelFilter_toggled()));

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
    filterEnable[1] = new QCheckBox(tr("High pass"));
    connect(filterEnable[1], SIGNAL(toggled(bool)), this, SLOT(on_highpassEnable_toggled(bool)));
    cutoffLabel[1] = new QLabel("Cutoff Freq");
    cutoffFreq[1] = new QLineEdit;

    cutoffFreq[1]->setEnabled(false);
    connect(cutoffFreq[1], SIGNAL(returnPressed()), this, SLOT(on_highpassFreq_changed()));
    highpassLayout->addWidget(filterEnable[1]);
    highpassLayout->addWidget(cutoffLabel[1]);
    highpassLayout->addWidget(cutoffFreq[1]);

//    QValidator *lowpassValidator = new QIntValidator(0, 7500, this);

//    lowpassLayout = new QVBoxLayout;
//    filterEnable[0] = new QCheckBox(tr("Low pass"));
//    connect(filterEnable[0], SIGNAL(toggled(bool)), this, SLOT(on_lowpassEnable_toggled(bool)));
//    cutoffLabel[0] = new QLabel("Cutoff Freq");
//    cutoffFreq[0] = new QLineEdit;
//    cutoffFreq[0]->setValidator(lowpassValidator);
//    connect(cutoffFreq[0], SIGNAL(returnPressed()), this, SLOT(on_lowpassFreq_changed()));
//    lowpassLayout->addWidget(filterEnable[0]);
//    lowpassLayout->addWidget(cutoffLabel[0]);
//    lowpassLayout->addWidget(cutoffFreq[0]);

//    lownhighLayout->addLayout(lowpassLayout);
//    channelLayout->addWidget(channelFilterEnable);
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

//    if(dataProcessor->isFilterEnabled()){
//        channelFilterEnable->setChecked(true);
//    }
//    else{
//        channelFilterEnable->setChecked(false);
//    }

//    if(dataProcessor->isLowpassFilterEnabled()){
//        filterEnable[0]->setChecked(true);
//        cutoffFreq[0]->setText(QString::number(dataProcessor->currentLowpassFreq()));
//    }
//    else{
//        filterEnable[0]->setChecked(false);
//    }
}

FilterDialog::~FilterDialog(){

}

//void FilterDialog::on_channelFilter_toggled(){
//    if(channelFilterEnable->isChecked()){
//        filterLabel->setEnabled(true);
//        notchFilter->setEnabled(true);

////        filterEnable[0]->setEnabled(true);
////        cutoffLabel[0]->setEnabled(true);
////        cutoffFreq[0]->setEnabled(true);

//        filterEnable[1]->setEnabled(true);
//        cutoffLabel[1]->setEnabled(true);
//        cutoffFreq[1]->setEnabled(true);
//    }
//    else{
//        filterLabel->setEnabled(false);
//        notchFilter->setEnabled(false);

////        filterEnable[0]->setEnabled(false);
////        cutoffLabel[0]->setEnabled(false);
////        cutoffFreq[0]->setEnabled(false);

//        filterEnable[1]->setEnabled(false);
//        cutoffLabel[1]->setEnabled(false);
//        cutoffFreq[1]->setEnabled(false);
//        dataProcessor->setHighpassFilterEnabled(false);
//        dataProcessor->setLowpassFilterEnabled(false);
//        dataProcessor->setNotchFilterEnabled(false);
//    }
////    if(dataProcessor->isLowpassFilterEnabled()){
////        on_lowpassEnable_toggled(true);
////    }
////    else{
////        on_lowpassEnable_toggled(false);
////    }
//    if(dataProcessor->isHighpassFilterEnabled()){
//        on_highpassEnable_toggled(true);
//    }
//    else{
//        on_highpassEnable_toggled(false);
//    }
//}

void FilterDialog::on_highpassEnable_toggled(bool enableFlag){
        cutoffLabel[1]->setEnabled(enableFlag);
        cutoffFreq[1]->setEnabled(enableFlag);
        if(dataProcessor->currentHighpassFreq() != 0){
            cutoffFreq[1]->setText(QString::number(dataProcessor->currentHighpassFreq()));
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
                dataProcessor->setNotchFilter(50.0, 20.0, 19000.0);
            }
            if(Index == 2){
                dataProcessor->setNotchFilter(60.0, 20.0, 19000.0);
            }
        }
}

void FilterDialog::on_highpassFreq_changed(){
    if(cutoffFreq[1]->text().toInt() > 99 && cutoffFreq[1]->text().toInt() < 7501){
        dataProcessor->setHighpassFilter(cutoffFreq[1]->text().toDouble(), 20000);
        qDebug() << cutoffFreq[1]->text().toInt();
    }
    else{
        QMessageBox::warning(        this,
                                     tr("Invalid Frequency"),
                                     tr("Please enter a valid cutoff frequency ranging from 100 to 7500"));
    }
}

//void FilterDialog::on_lowpassFreq_changed(){
//    dataProcessor->setLowpassFilter(cutoffFreq[0]->text().toDouble(), 20000);
//}

//void FilterDialog::on_lowpassEnable_toggled(bool enableFlag){
//        cutoffLabel[0]->setEnabled(enableFlag);
//        cutoffFreq[0]->setEnabled(enableFlag);
//        if(dataProcessor->currentLowpassFreq() != 0){
//            cutoffFreq[0]->setText(QString::number(dataProcessor->currentLowpassFreq()));
//        }
//        dataProcessor->setLowpassFilterEnabled(enableFlag);
//}
