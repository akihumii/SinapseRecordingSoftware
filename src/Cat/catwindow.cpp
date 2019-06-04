#include "catwindow.h"

namespace Cat {

CatWindow::CatWindow(){
    QString version(APP_VERSION);
    setWindowTitle(tr("Cat Software V") + version);
    createLayout();
}

void CatWindow::createLayout(){
    QWidget *mainWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(createMethodsGroup());
    mainLayout->addWidget(createSettingsGroup());
    mainLayout->addWidget(createTrainingGroup());
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

        //E
        QLabel *powerLetter = new QLabel("E");
        thresholdingSpinBoxLayout->addWidget(powerLetter);

        //Powers
        thresholdingPowerSpinBox[i] = new QSpinBox;
        thresholdingPowerSpinBox[i]->setMinimum(-20);
        thresholdingPowerSpinBox[i]->setMaximum(10);
        thresholdingPowerSpinBox[i]->setValue(10);
        thresholdingSpinBoxLayout->addWidget(thresholdingPowerSpinBox[i]);

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

CatWindow::~CatWindow(){
}
}
