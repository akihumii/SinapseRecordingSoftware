#include "catwindow.h"

namespace Cat {

CatWindow::CatWindow(){
    QString version(APP_VERSION);
    setWindowTitle(tr("Cat Software V") + version);
    commandCat = new CommandCat;
    createLayout();
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
    int windowWidth = 70;
    QHBoxLayout *windowSubLayout[4];
    QLabel* windowDecodingLabel = new QLabel(tr("Decoding window size: "));
    windowDecodingSpinBox = new QSpinBox;
    windowDecodingSpinBox->setMaximumWidth(windowWidth);
    windowSubLayout[0] = new QHBoxLayout;
    windowSubLayout[0]->addWidget(windowDecodingLabel);
    windowSubLayout[0]->addWidget(windowDecodingSpinBox);

    QLabel *windowOverlapLabel = new QLabel(tr("Overlap window size: "));
    windowOverlapSpinBox = new QSpinBox;
    windowOverlapSpinBox->setMaximumWidth(windowWidth);
    windowSubLayout[1] = new QHBoxLayout;
    windowSubLayout[1]->addWidget(windowOverlapLabel);
    windowSubLayout[1]->addWidget(windowOverlapSpinBox);

    QLabel *windowSamplingFrequencyLabel = new QLabel(tr("Sampling frequency: "));
    windowSamplingFrequencySpinBox = new QSpinBox;
    windowSamplingFrequencySpinBox->setMaximumWidth(windowWidth);
    windowSubLayout[2] = new QHBoxLayout;
    windowSubLayout[2]->addWidget(windowSamplingFrequencyLabel);
    windowSubLayout[2]->addWidget(windowSamplingFrequencySpinBox);

    QLabel *extendStimLabel = new QLabel(tr("Extend Stimulation (s): "));
    extendStimSpinBox = new QSpinBox;
    extendStimSpinBox->setMaximumWidth(windowWidth);
    windowSubLayout[3] = new QHBoxLayout;
    windowSubLayout[3]->addWidget(extendStimLabel);
    windowSubLayout[3]->addWidget(extendStimSpinBox);


    //layout window
    QVBoxLayout *windowLayout = new QVBoxLayout();
    for(int i = 0; i < 4; i++){
        windowLayout->addLayout(windowSubLayout[i]);
    }

    //filtering
    QHBoxLayout *filteringSubLayout[3];
    QLabel* highpassLabel = new QLabel(tr("Highpass cutoff freq.: "));
    highpassSpinBox = new QSpinBox;
    highpassSpinBox->setMaximumWidth(windowWidth);
    filteringSubLayout[0] = new QHBoxLayout;
    filteringSubLayout[0]->addWidget(highpassLabel);
    filteringSubLayout[0]->addWidget(highpassSpinBox);

    QLabel *lowpassLabel = new QLabel(tr("Lowpass cutoff freq.: "));
    lowpassSpinBox = new QSpinBox;
    lowpassSpinBox->setMaximumWidth(windowWidth);
    filteringSubLayout[1] = new QHBoxLayout;
    filteringSubLayout[1]->addWidget(lowpassLabel);
    filteringSubLayout[1]->addWidget(lowpassSpinBox);

    QLabel *notchLabel = new QLabel(tr("Notch cutoff freq.: "));
    notchSpinBox = new QSpinBox;
    notchSpinBox->setMaximumWidth(windowWidth);
    filteringSubLayout[2] = new QHBoxLayout;
    filteringSubLayout[2]->addWidget(notchLabel);
    filteringSubLayout[2]->addWidget(notchSpinBox);

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
        connect(thresholdingPowerSpinBox[i], SIGNAL(editingFinished()), this, SLOT(on_threshold_power_Changed()));

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

void CatWindow::on_threshold_power_Changed(){
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

CatWindow::~CatWindow(){
}
}
