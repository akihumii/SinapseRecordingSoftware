#include "catwindow.h"

namespace Cat {

CatWindow::CatWindow(){
    QString version(APP_VERSION);
    setWindowTitle(tr("Cat Software V") + version);
    commandCat = new CommandCat;
    highpassValueSets = new QVector<double>;
    lowpassValueSets = new QVector<double>;
    notchValueSets = new QVector<double>;
    createLayout();
//    highpassCheckBox->setChecked(true);
//    notchCheckBox->setChecked(true);
    createStatusBar();
}

void CatWindow::createLayout(){
    QWidget *mainWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(createMethodsGroup());
    mainLayout->addWidget(createSettingsGroup());
    mainLayout->addWidget(createTrainingGroup());
    mainLayout->addWidget(createParametersGroup());
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
    mainLayout->setSizeConstraint( QLayout::SetFixedSize );

    sendParameters();
}

QGroupBox *CatWindow::createMethodsGroup(){
    QGroupBox *methodsGroup = new QGroupBox(tr("Mehtods"));

    QVBoxLayout *methodsParameterLayout = new QVBoxLayout;
    methodsParameterLayout->addWidget(createThreasholdingGroup());

    methodsGroup->setLayout(methodsParameterLayout);

    return methodsGroup;
}

QGroupBox *CatWindow::createSettingsGroup(){
    QGroupBox *groupSettings= new QGroupBox(tr("Settings"));

    //Labels
    QVBoxLayout *settingsLabelLayout = new QVBoxLayout;
    QLabel *settingsLabel[2];
    settingsLabel[0] = new QLabel(tr("S/M Channel: "));
    settingsLabel[1] = new QLabel(tr("Methods: "));
    for(int i = 0; i < 2; i++){
        settingsLabelLayout->addWidget(settingsLabel[i]);
    }

    //S/M Channel
    methodsSMChannelBox[0] = new QRadioButton(tr("&Single-channel"));
    methodsSMChannelBox[0]->setChecked(true);
    methodsSMChannelBox[1] = new QRadioButton(tr("&Multi-channel"));
    QHBoxLayout *SMChannelLayout = new QHBoxLayout;
    for(int i = 0; i < 2; i++){
        methodsSMChannelBox[i]->setMinimumWidth(150);
        SMChannelLayout->addWidget(methodsSMChannelBox[i]);
    }
    QGroupBox *groupSMChannel = new QGroupBox();
    groupSMChannel->setLayout(SMChannelLayout);

    //Methods
    methodsClassifyBox[0] = new QRadioButton(tr("&Tresholding"));
    methodsClassifyBox[0]->setChecked(true);
    methodsClassifyBox[1] = new QRadioButton(tr("&Features"));

    QHBoxLayout *methodsClassifyLayout = new QHBoxLayout;
    for(int i = 0; i < 2; i++){
        methodsClassifyBox[i]->setMinimumWidth(150);
        methodsClassifyLayout->addWidget(methodsClassifyBox[i]);
    }
    QGroupBox *groupMethodsClassify = new QGroupBox();
    groupMethodsClassify->setLayout(methodsClassifyLayout);


    //Layout
    QVBoxLayout *settingsRadioButtonLayout = new QVBoxLayout;
    settingsRadioButtonLayout->addWidget(groupSMChannel);
    settingsRadioButtonLayout->addWidget(groupMethodsClassify);

    QHBoxLayout *settingsLayout = new QHBoxLayout;
    settingsLayout->addLayout(settingsLabelLayout);
    settingsLayout->addLayout(settingsRadioButtonLayout);

    groupSettings->setLayout(settingsLayout);

    return groupSettings;
}

QGroupBox *CatWindow::createTrainingGroup(){
    QGroupBox *trainingGroup = new QGroupBox(tr("Training"));

    //number of class
    QLabel *numClassLabel = new QLabel(tr("No. of class"));
    numClassLabel->setMaximumWidth(100);
    numClassSpinBox = new QSpinBox;
    QHBoxLayout *numClassLayout = new QHBoxLayout;
    numClassLayout->addWidget(numClassLabel);
    numClassLayout->addWidget(numClassSpinBox);

    //control buttons
    trainingStart = new QPushButton(tr("Start"));
    trainingRedo = new QPushButton(tr("Redo Class"));
    trainingNext = new QPushButton(tr("Next Class"));
    trainingCancel = new QPushButton(tr("Cancel"));
    QHBoxLayout *trainingButtonsLayout = new QHBoxLayout;
    trainingButtonsLayout->addWidget(trainingStart);
    trainingButtonsLayout->addWidget(trainingRedo);
    trainingButtonsLayout->addWidget(trainingNext);
    trainingButtonsLayout->addWidget(trainingCancel);

    //save buttons
    trainingSave = new QPushButton(tr("Save File"));
    trainingSave->setMaximumWidth(100);
    trainingSaveDir = new QLabel("[display chosen saving location...]");
    QHBoxLayout *trainingSaveLayout = new QHBoxLayout;
    trainingSaveLayout->addWidget(trainingSave);
    trainingSaveLayout->addWidget(trainingSaveDir);

    //Layout
    QVBoxLayout *trainingLayout = new QVBoxLayout;
    trainingLayout->addLayout(numClassLayout);
    trainingLayout->addLayout(trainingButtonsLayout);
    trainingLayout->addLayout(trainingSaveLayout);

    trainingGroup->setLayout(trainingLayout);

    return trainingGroup;
}

QGroupBox *CatWindow::createParametersGroup(){
    QGroupBox *parametersGroup = new QGroupBox(tr("Parameters"));

    //window
    int windowWidth = 60;
    QHBoxLayout *windowSubLayout[4];
    QLabel* windowDecodingLabel = new QLabel(tr("Decoding window size (ms): "));
    windowDecodingSpinBox = new QSpinBox;
    windowDecodingSpinBox->setMaximumWidth(windowWidth);
    windowDecodingSpinBox->setMinimum(0);
    windowDecodingSpinBox->setMaximum(9999);
    windowDecodingSpinBox->setValue(200);
    windowSubLayout[0] = new QHBoxLayout;
    windowSubLayout[0]->addWidget(windowDecodingLabel);
    windowSubLayout[0]->addWidget(windowDecodingSpinBox);
    connect(windowDecodingSpinBox, SIGNAL(editingFinished()), this, SLOT(on_decoding_window_size_changed()));

    QLabel *windowOverlapLabel = new QLabel(tr("Overlap window size (ms): "));
    windowOverlapSpinBox = new QSpinBox;
    windowOverlapSpinBox->setMaximumWidth(windowWidth);
    windowOverlapSpinBox->setMinimum(0);
    windowOverlapSpinBox->setMaximum(9999);
    windowOverlapSpinBox->setValue(50);
    windowSubLayout[1] = new QHBoxLayout;
    windowSubLayout[1]->addWidget(windowOverlapLabel);
    windowSubLayout[1]->addWidget(windowOverlapSpinBox);
    connect(windowOverlapSpinBox, SIGNAL(editingFinished()), this, SLOT(on_overlap_window_size_changed()));

    QLabel *windowSamplingFrequencyLabel = new QLabel(tr("Sampling frequency (Hz): "));
    windowSamplingFrequencySpinBox = new QSpinBox;
    windowSamplingFrequencySpinBox->setMaximumWidth(windowWidth);
    windowSamplingFrequencySpinBox->setMinimum(0);
    windowSamplingFrequencySpinBox->setMaximum(30000);
    windowSamplingFrequencySpinBox->setValue(1250);
    windowSamplingFrequencySpinBox->setMaximumWidth(windowWidth);
    windowSubLayout[2] = new QHBoxLayout;
    windowSubLayout[2]->addWidget(windowSamplingFrequencyLabel);
    windowSubLayout[2]->addWidget(windowSamplingFrequencySpinBox);
    connect(windowSamplingFrequencySpinBox, SIGNAL(editingFinished()), this, SLOT(on_sampling_freq_changed()));

    QLabel *extendStimLabel = new QLabel(tr("Extend Stimulation (ms): "));
    extendStimSpinBox = new QSpinBox;
    extendStimSpinBox->setMaximumWidth(windowWidth);
    extendStimSpinBox->setMinimum(0);
    extendStimSpinBox->setMaximum(9999);
    extendStimSpinBox->setValue(0);
    windowSubLayout[3] = new QHBoxLayout;
    windowSubLayout[3]->addWidget(extendStimLabel);
    windowSubLayout[3]->addWidget(extendStimSpinBox);
    connect(extendStimSpinBox, SIGNAL(editingFinished()), this, SLOT(on_extend_stimulation_changed()));


    //layout window
    QVBoxLayout *windowLayout = new QVBoxLayout();
    for(int i = 0; i < 4; i++){
        windowLayout->addLayout(windowSubLayout[i]);
    }

    //filtering
    QHBoxLayout *filteringSubLayout[3];
    QLabel *highpassLabel = new QLabel(tr("Highpass cutoff freq.: "));
    highpassCheckBox = new QCheckBox;
    highpassSpinBox = new QSpinBox;
    highpassSpinBox->setMaximumWidth(windowWidth);
    highpassSpinBox->setMinimum(0);
    highpassSpinBox->setMaximum(windowSamplingFrequencySpinBox->text().toInt()/2 - 1);
    highpassSpinBox->setValue(100);
    highpassSpinBox->setEnabled(false);
    filteringSubLayout[0] = new QHBoxLayout;
    filteringSubLayout[0]->addWidget(highpassCheckBox);
    filteringSubLayout[0]->addWidget(highpassLabel);
    filteringSubLayout[0]->addWidget(highpassSpinBox);
    connect(highpassSpinBox, SIGNAL(editingFinished()), this, SLOT(on_highpass_cutoff_freq_changed()));
    connect(highpassCheckBox, SIGNAL(toggled(bool)), this, SLOT(on_highpass_cutoff_freq_checkbox_changed()));

    QLabel *lowpassLabel = new QLabel(tr("Lowpass cutoff freq.: "));
    lowpassCheckBox = new QCheckBox;
    lowpassSpinBox = new QSpinBox;
    lowpassSpinBox->setMaximumWidth(windowWidth);
    lowpassSpinBox->setMinimum(highpassSpinBox->text().toInt() + 1);
    lowpassSpinBox->setMaximum(windowSamplingFrequencySpinBox->text().toInt()/2 - 1);
    lowpassSpinBox->setValue(windowSamplingFrequencySpinBox->text().toInt()/2 - 1);
    lowpassSpinBox->setEnabled(false);
    filteringSubLayout[1] = new QHBoxLayout;
    filteringSubLayout[1]->addWidget(lowpassCheckBox);
    filteringSubLayout[1]->addWidget(lowpassLabel);
    filteringSubLayout[1]->addWidget(lowpassSpinBox);
    connect(lowpassSpinBox, SIGNAL(editingFinished()), this, SLOT(on_lowpass_cutoff_freq_changed()));
    connect(lowpassCheckBox, SIGNAL(toggled(bool)), this, SLOT(on_lowpass_cutoff_freq_checkbox_changed()));

    QLabel *notchLabel = new QLabel(tr("Notch cutoff freq.: "));
    notchCheckBox = new QCheckBox;
    notchSpinBox = new QSpinBox;
    notchSpinBox->setMaximumWidth(windowWidth);
    notchSpinBox->setMinimum(0);
    notchSpinBox->setMaximum(windowSamplingFrequencySpinBox->text().toInt()-1);
    notchSpinBox->setValue(50);
    notchSpinBox->setEnabled(false);
    filteringSubLayout[2] = new QHBoxLayout;
    filteringSubLayout[2]->addWidget(notchCheckBox);
    filteringSubLayout[2]->addWidget(notchLabel);
    filteringSubLayout[2]->addWidget(notchSpinBox);
    connect(notchSpinBox, SIGNAL(editingFinished()), this, SLOT(on_notch_cutoff_freq_changed()));
    connect(notchCheckBox, SIGNAL(toggled(bool)), this, SLOT(on_notch_cutoff_freq_checkbox_changed()));

    //layout filtering
    QVBoxLayout *filteringLayout = new QVBoxLayout();
    for(int i = 0; i < 3; i++){
        filteringLayout->addLayout(filteringSubLayout[i]);
    }

    //layout
    QHBoxLayout *parametersLayout = new QHBoxLayout();
    parametersLayout->addLayout(windowLayout);
    parametersLayout->addLayout(filteringLayout);

    parametersGroup->setLayout(parametersLayout);

    return parametersGroup;
}

QGroupBox *CatWindow::createThreasholdingGroup(){
    QGroupBox *groupThreasholding = new QGroupBox(tr("Thresholding Parameters"));

    QVBoxLayout *thresholdingSubLayout[4];
    QLabel *channelLabel[4];
    for(int i = 0; i < 4; i++){
        thresholdingSubLayout[i] = new QVBoxLayout;

        //Labels
        channelLabel[i] = new QLabel("Channel " + QString::number(i+1));
        thresholdingSubLayout[i]->addWidget(channelLabel[i]);

        //Digits
        QHBoxLayout *thresholdingSpinBoxLayout = new QHBoxLayout;
        thresholdingSpinBox[i] = new QSpinBox;
        thresholdingSpinBox[i]->setMinimum(0);
        thresholdingSpinBox[i]->setMaximum(99);
        thresholdingSpinBox[i]->setValue(1);
        thresholdingSpinBoxLayout->addWidget(thresholdingSpinBox[i]);
        connect(thresholdingSpinBox[i], SIGNAL(editingFinished()), this, SLOT(on_threshold_changed()));

        //E
        QLabel *powerLetter = new QLabel("E");
        thresholdingSpinBoxLayout->addWidget(powerLetter);

        //Powers
        thresholdingPowerSpinBox[i] = new QSpinBox;
        thresholdingPowerSpinBox[i]->setMinimum(-20);
        thresholdingPowerSpinBox[i]->setMaximum(10);
        thresholdingPowerSpinBox[i]->setValue(10);
        thresholdingSpinBoxLayout->addWidget(thresholdingPowerSpinBox[i]);
        connect(thresholdingPowerSpinBox[i], SIGNAL(editingFinished()), this, SLOT(on_threshold_power_changed()));

        thresholdingSubLayout[i]->addLayout(thresholdingSpinBoxLayout);
    }

    //Layout
    QHBoxLayout *thresholdingLayout = new QHBoxLayout;
    for(int i = 0; i < 4; i++){
        thresholdingLayout->addLayout(thresholdingSubLayout[i]);
    }

    groupThreasholding->setLayout(thresholdingLayout);

    return groupThreasholding;
}

void CatWindow::createStatusBar(){
    statusBarLabel = new QLabel;
    statusBarMainWindow = statusBar();
    statusBarMainWindow->addPermanentWidget(statusBarLabel, 1);
    statusBarMainWindow->setSizeGripEnabled(false);  //fixed window size
}

void CatWindow::sendParameters(){
    int delay = 0;
    int delayInterval = 60;
    for(int i = 0; i < 4; i++){  // send threhsold digits
        QTimer::singleShot((startDelay+delay+i*delayInterval), [=] {
            commandCat->sendThreshold(i);
            strcpy(lastSentCommand, commandCat->getlastRpiCommand().data());
            emit commandSent(lastSentCommand);
        });
    }
    delay += 4*delayInterval;
    for(int i = 0; i < 4; i++){  // send threshold power
        QTimer::singleShot((startDelay+delay+i*delayInterval), [=] {
            commandCat->sendThresholdPower(i);
            strcpy(lastSentCommand, commandCat->getlastRpiCommand().data());
            emit commandSent(lastSentCommand);
        });
    }
    delay += 4*delayInterval;
    QTimer::singleShot((startDelay+delay), [=] {  // send decoding window size
        commandCat->sendDecodingWindowSize();
        strcpy(lastSentCommand, commandCat->getlastRpiCommand().data());
        emit commandSent(lastSentCommand);
        });
    delay += delayInterval;
    QTimer::singleShot((startDelay+delay), [=] {  // send overlap window size
        commandCat->sendOverlapWindowSize();
        strcpy(lastSentCommand, commandCat->getlastRpiCommand().data());
        emit commandSent(lastSentCommand);
        });
    delay += delayInterval;
    QTimer::singleShot((startDelay+delay), [=] {  // send sampling freq
        commandCat->sendSamplingFreq();
        strcpy(lastSentCommand, commandCat->getlastRpiCommand().data());
        emit commandSent(lastSentCommand);
        });
    delay += delayInterval;
    QTimer::singleShot((startDelay+delay), [=] {  // send extend stimulation
        commandCat->sendExtendStimulation();
        strcpy(lastSentCommand, commandCat->getlastRpiCommand().data());
        emit commandSent(lastSentCommand);
        });
    delay += delayInterval;
    QTimer::singleShot((startDelay+delay), [=] {  // send highpass freq
        commandCat->sendHighpassCutoffFreq();
        strcpy(lastSentCommand, commandCat->getlastRpiCommand().data());
        emit commandSent(lastSentCommand);
        });
    delay += delayInterval;
    QTimer::singleShot((startDelay+delay), [=] {  // send lowpass freq
        commandCat->sendLowpassCutoffFreq();
        strcpy(lastSentCommand, commandCat->getlastRpiCommand().data());
        emit commandSent(lastSentCommand);
        });
    delay += delayInterval;
    QTimer::singleShot((startDelay+delay), [=] {  //send notch freq
        commandCat->sendNotchCutoffFreq();
        strcpy(lastSentCommand, commandCat->getlastRpiCommand().data());
        emit commandSent(lastSentCommand);
        });
    delay += delayInterval;
    QTimer::singleShot((startDelay+delay), [=] {
        QMessageBox::information(this, "Done!", "Classification parameters have been sent...");
        });
    delay += delayInterval;
}

void CatWindow::on_threshold_changed(){
    for(int i = 0; i < 4; i++){
        if(thresholdingSpinBox[i]->text().toInt() != commandCat->getThreshold(i)){
            commandCat->setThreshold(i, thresholdingSpinBox[i]->text().toInt());
            qDebug() << "Sent channel " << i << "threshold power to : " << thresholdingSpinBox[i]->text().toInt();
            commandCat->sendThreshold(i);
            strcpy(lastSentCommand, commandCat->getlastRpiCommand().data());
            emit commandSent(lastSentCommand);
        }
    }
}

void CatWindow::on_threshold_power_changed(){
    for(int i = 0; i < 4; i++){
        if(thresholdingPowerSpinBox[i]->text().toInt() != commandCat->getThresholdPower(i)){
            commandCat->setThresholdPower(i, thresholdingPowerSpinBox[i]->text().toInt());
            qDebug() << "Sent channel " << i << "threshold power to : " << thresholdingPowerSpinBox[i]->text().toInt();
            commandCat->sendThresholdPower(i);
            strcpy(lastSentCommand, commandCat->getlastRpiCommand().data());
            emit commandSent(lastSentCommand);
        }
    }
}

void CatWindow::on_decoding_window_size_changed(){
    if(windowDecodingSpinBox->text().toInt() != commandCat->getDecodingWindowSize()){
        commandCat->setDecodingWindowSize(windowDecodingSpinBox->text().toInt());
        qDebug() << "Sent decoding window size to : " << windowDecodingSpinBox->text().toInt();
        commandCat->sendDecodingWindowSize();
        strcpy(lastSentCommand, commandCat->getlastRpiCommand().data());
        emit commandSent(lastSentCommand);
    }
}

void CatWindow::on_overlap_window_size_changed(){
    if(windowOverlapSpinBox->text().toInt() != commandCat->getOverlapWindowSize()){
        commandCat->setOverlapWindowSize(windowOverlapSpinBox->text().toInt());
        qDebug() << "Sent overlap window size to : " << windowOverlapSpinBox->text().toInt();
        commandCat->sendOverlapWindowSize();
        strcpy(lastSentCommand, commandCat->getlastRpiCommand().data());
        emit commandSent(lastSentCommand);
    }
}

void CatWindow::on_sampling_freq_changed(){
    if(windowSamplingFrequencySpinBox->text().toInt() != commandCat->getSamplingFreq()){
        highpassSpinBox->setMaximum(lowpassSpinBox->text().toInt() - 1);
        lowpassSpinBox->setMinimum(highpassSpinBox->text().toInt() + 1);
        lowpassSpinBox->setMaximum(windowSamplingFrequencySpinBox->text().toInt()/2 - 1);
        notchSpinBox->setMaximum(windowSamplingFrequencySpinBox->text().toInt()-1);

        sendFilteringParameters();

        QTimer::singleShot(150, [=] {
            commandCat->setSamplingFreq(windowSamplingFrequencySpinBox->text().toInt());
            qDebug() << "Sent decoding window size to : " << windowSamplingFrequencySpinBox->text().toInt();
            commandCat->sendSamplingFreq();
            strcpy(lastSentCommand, commandCat->getlastRpiCommand().data());
            emit commandSent(lastSentCommand);
        });

    }
}

void CatWindow::on_extend_stimulation_changed(){
    if(extendStimSpinBox->text().toInt() != commandCat->getExtendStimulation()){
        commandCat->setExtendStimulation(extendStimSpinBox->text().toInt());
        qDebug() << "Sent extend stimulation to : " << extendStimSpinBox->text().toInt();
        commandCat->sendExtendStimulation();
        strcpy(lastSentCommand, commandCat->getlastRpiCommand().data());
        emit commandSent(lastSentCommand);
    }
}

void CatWindow::sendHighpassParameters(int value){
    commandCat->setHighpassCutoffFreq(value);
    qDebug() << "Sent highpass cutoff freq to : " << value;
    commandCat->sendHighpassCutoffFreq();
    strcpy(lastSentCommand, commandCat->getlastRpiCommand().data());
    emit commandSent(lastSentCommand);
}

void CatWindow::on_highpass_cutoff_freq_changed(){
    if(highpassSpinBox->text().toInt() != commandCat->getHighpassCutoffFreq()){
        sendHighpassParameters(highpassSpinBox->text().toInt());
        sendHighpassSignal(highpassSpinBox->text().toInt());
    }
}

void CatWindow::on_highpass_cutoff_freq_checkbox_changed(){
    if(highpassCheckBox->isChecked()){
        highpassSpinBox->setEnabled(true);
        sendFilteringParameters();
    }
    else{
        highpassSpinBox->setEnabled(false);
        sendHighpassParameters(0);
        sendHighpassSignal(0);
    }
}

void CatWindow::sendLowpassParameters(int value){
    commandCat->setLowpassCutoffFreq(value);
    qDebug() << "Sent lowpass cutoff freq to : " << value;
    commandCat->sendLowpassCutoffFreq();
    strcpy(lastSentCommand, commandCat->getlastRpiCommand().data());
    emit commandSent(lastSentCommand);
}

void CatWindow::on_lowpass_cutoff_freq_changed(){
    if(lowpassSpinBox->text().toInt() != commandCat->getLowpassCutoffFreq()){
        sendLowpassParameters(lowpassSpinBox->text().toInt());
        sendLowpassSignal(lowpassSpinBox->text().toInt());
    }
}

void CatWindow::on_lowpass_cutoff_freq_checkbox_changed(){
    if(lowpassCheckBox->isChecked()){
        lowpassSpinBox->setEnabled(true);
        sendFilteringParameters();
    }
    else{
        lowpassSpinBox->setEnabled(false);
        sendLowpassParameters(0);
        sendLowpassSignal(0);
    }
}

void CatWindow::sendNotchParameters(int value){
    commandCat->setNotchCutoffFreq(value);
    qDebug() << "Sent notch cutoff freq to : " << value;
    commandCat->sendNotchCutoffFreq();
    strcpy(lastSentCommand, commandCat->getlastRpiCommand().data());
    emit commandSent(lastSentCommand);

}

void CatWindow::on_notch_cutoff_freq_changed(){
    if(notchSpinBox->text().toInt() != commandCat->getNotchCutoffFreq()){
        sendNotchParameters(notchSpinBox->text().toInt());
        sendNotchSignal(notchSpinBox->text().toInt());
    }
}

void CatWindow::on_notch_cutoff_freq_checkbox_changed(){
    if(notchCheckBox->isChecked()){
        notchSpinBox->setEnabled(true);
        sendFilteringParameters();
    }
    else{
        notchSpinBox->setEnabled(false);
        sendNotchParameters(0);
        sendNotchSignal(0);
    }
}

void CatWindow::sendFilteringParameters(){
    highpassSpinBox->setMaximum(lowpassSpinBox->text().toInt() - 1);
    lowpassSpinBox->setMinimum(highpassSpinBox->text().toInt() + 1);
    lowpassSpinBox->setMaximum(windowSamplingFrequencySpinBox->text().toInt()/2 - 1);
    notchSpinBox->setMaximum(windowSamplingFrequencySpinBox->text().toInt()-1);

    QTimer::singleShot(0, [=] {
        int highpassValue;
        highpassCheckBox->isChecked()? highpassValue = highpassSpinBox->text().toInt() : highpassValue = 0;
        sendHighpassParameters(highpassValue);
        sendHighpassSignal(highpassValue);
    });
    QTimer::singleShot(150, [=] {
        int lowpassValue;
        lowpassCheckBox->isChecked()? lowpassValue = lowpassSpinBox->text().toInt() : lowpassValue = 0;
        sendLowpassParameters(lowpassValue);
        sendLowpassSignal(lowpassValue);
    });
    QTimer::singleShot(300, [=] {
        int notchValue;
        notchCheckBox->isChecked()? notchValue = notchSpinBox->text().toInt() : notchValue = 0;
        sendNotchParameters(notchValue);
        sendNotchSignal(notchValue);
    });
}

void CatWindow::sendHighpassSignal(double highpassValue){
    highpassValueSets->clear();
    highpassValueSets->append((double) highpassValue);
    highpassValueSets->append((double) windowSamplingFrequencySpinBox->text().toDouble());
    highpassValueSets->append((double) highpassCheckBox->isChecked());
    emit highpassSent(highpassValueSets);
}

void CatWindow::sendLowpassSignal(double lowpassValue){
    lowpassValueSets->clear();
    lowpassValueSets->append((double) lowpassValue);
    lowpassValueSets->append((double) windowSamplingFrequencySpinBox->text().toDouble());
    lowpassValueSets->append((double) lowpassCheckBox->isChecked());
    emit lowpassSent(lowpassValueSets);
}

void CatWindow::sendNotchSignal(double notchValue){
    notchValueSets->clear();
    notchValueSets->append((double) notchValue);
    notchValueSets->append((double) windowSamplingFrequencySpinBox->text().toDouble());
    notchValueSets->append((double) notchCheckBox->isChecked());
    emit notchSent(notchValueSets);
}

CatWindow::~CatWindow(){
}
}
