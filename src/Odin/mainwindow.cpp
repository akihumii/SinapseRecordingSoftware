#include "mainwindow.h"

MainWindow::MainWindow(){
    serialOdin = new SerialOdin(this);
    serialOdin->connectOdin();
    serialOdin->initOdin();
    socketOdin = new SocketOdin(this);
//    socketOdin->doConnect("10.10.10.1", 30000);
//    socketOdin->initOdin();
    commandOdin = new CommandOdin(serialOdin, socketOdin);

    createLayout();
}

void MainWindow::createLayout(){
    QHBoxLayout *modeLayout = new QHBoxLayout;
    ModeLabel = new QLabel(tr("Mode"));
    ModeComboBox = new QComboBox;
    ModeComboBox->addItem("FLAT Single");
    ModeComboBox->addItem("Graded Amplitude Single");
    ModeComboBox->addItem("Graded Frequency (High - Low)");
    ModeComboBox->addItem("Graded Frequency (Low - High)");
    ModeComboBox->addItem("Multi-channel Interleaved");
    ModeComboBox->addItem("Multi-channel Non-Interleaved");

    connect(ModeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_Mode_Changed(int)));

    modeLayout->addWidget(ModeLabel);
    modeLayout->addWidget(ModeComboBox);

    QHBoxLayout *channelLayout = new QHBoxLayout;
    channelLabel = new QLabel(tr("Channel"));
    channelComboBox = new QComboBox;
    channelComboBox->addItem("Channel A");
    channelComboBox->addItem("Channel B");
    channelComboBox->addItem("Channel C");
    channelComboBox->addItem("Channel D");

    channelLayout->addWidget(channelLabel);
    channelLayout->addWidget(channelComboBox);

    connect(channelComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_channel_Changed(int)));
//-------------------------------------------------------------------------------------------------//
    Parameters = new QGroupBox(tr("Stimulation Parameters"));
    Intensity = new QGroupBox(tr("Pulse Magnitude (0.3 to 9.3 mA)"));
    Timing = new QGroupBox(tr("Timing Parameters"));
    Looping = new QGroupBox(tr("Looping Parameters"));

    QVBoxLayout *intensityLayout1 = new QVBoxLayout;
    QVBoxLayout *intensityLayout2 = new QVBoxLayout;
    pulseMagLabel[0] = new QLabel(tr("Mag 1*^~"));
    pulseMagLabel[1] = new QLabel(tr("Mag 2*^"));
    pulseMagLabel[2] = new QLabel(tr("Mag 3*^"));
    pulseMagLabel[3] = new QLabel(tr("Mag 4*^"));
    pulseMagLabel[4] = new QLabel(tr("Mag 5^"));
    for(int i = 0; i < 5; i++){
        intensityLayout1->addWidget(pulseMagLabel[i]);
        pulseMag[i] = new QDoubleSpinBox;
        pulseMag[i]->setMinimum(0.3);
        pulseMag[i]->setMaximum(9.3);
        pulseMag[i]->setSingleStep(0.1);
        pulseMag[i]->setValue(2.0);
        pulseMag[i]->setMaximumWidth(50);
        pulseMag[i]->setAlignment(Qt::AlignHCenter);
        intensityLayout2->addWidget(pulseMag[i]);
        connect(pulseMag[i], SIGNAL(valueChanged(double)), this, SLOT(on_pulseMag_Changed()));
    }

    QHBoxLayout *intensityLayout = new QHBoxLayout;
    intensityLayout->addLayout(intensityLayout1);
    intensityLayout->addLayout(intensityLayout2);

    Intensity->setLayout(intensityLayout);

    QHBoxLayout *pulseDurationLayout = new QHBoxLayout;
    pulseDurationLabel = new QLabel(tr("Pulse Duration (20 - 1260 us): "));
    pulseDurationSpinBox = new QSpinBox;
    pulseDurationSpinBox->setMinimum(20);
    pulseDurationSpinBox->setMaximum(1260);
    pulseDurationSpinBox->setSingleStep(10);
    pulseDurationSpinBox->setValue(200);
    pulseDurationSpinBox->setMaximumWidth(50);

    connect(pulseDurationSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_pulseDuration_Changed()));

    pulseDurationLayout->addWidget(pulseDurationLabel);
    pulseDurationLayout->addWidget(pulseDurationSpinBox);

    QHBoxLayout *numPulseLayout = new QHBoxLayout;
    numPulseLabel = new QLabel(tr("Number of Pulses (in one train): "));
    numPulseLabel2 = new QLabel(tr("10x"));
    numPulseSpinBox = new QSpinBox;
    numPulseSpinBox->setMinimum(1);
    numPulseSpinBox->setMaximum(255);
    numPulseSpinBox->setValue(2);
    numPulseSpinBox->setMaximumWidth(50);

    connect(numPulseSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_numPulse_Changed()));

    numPulseLayout->addWidget(numPulseLabel);
    numPulseLayout->addWidget(numPulseLabel2);
    numPulseLayout->addWidget(numPulseSpinBox);

    QHBoxLayout *interPulseDurationLayout = new QHBoxLayout;
    interPulseDurationLabel = new QLabel(tr("Frequency (17-250Hz): "));
    interPulseDurationSpinBox = new QSpinBox;
    interPulseDurationSpinBox->setMinimum(17);
    interPulseDurationSpinBox->setMaximum(250);
    interPulseDurationSpinBox->setValue(75);
    interPulseDurationSpinBox->setMaximumWidth(50);

    connect(interPulseDurationSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_interPulseDuration_Changed()));

    interPulseDurationLayout->addWidget(interPulseDurationLabel);
    interPulseDurationLayout->addWidget(interPulseDurationSpinBox);

    QVBoxLayout *timingLayout = new QVBoxLayout;
    timingLayout->addLayout(pulseDurationLayout);
    timingLayout->addLayout(numPulseLayout);
    timingLayout->addLayout(interPulseDurationLayout);

    Timing->setLayout(timingLayout);

    QHBoxLayout *numPulseTrainLayout = new QHBoxLayout;
    numPulseTrainLabel = new QLabel(tr("Number of Pulse Trains"));
    numPulseTrainSpinBox = new QSpinBox;
    numPulseTrainSpinBox->setValue(50);
    numPulseTrainSpinBox->setMaximumWidth(50);
    numPulseTrainLayout->addWidget(numPulseTrainLabel);
    numPulseTrainLayout->addWidget(numPulseTrainSpinBox);

    QHBoxLayout *interPulseTrainDelayLayout = new QHBoxLayout;
    interPulseTrainDelayLabel = new QLabel(tr("Inter-Pulse Train Delay (ms)"));
    interPulseTrainDelaySpinBox = new QSpinBox;
    interPulseTrainDelaySpinBox->setMinimum(1000);
    interPulseTrainDelaySpinBox->setMaximum(50000);
    interPulseTrainDelaySpinBox->setValue(3000);
    interPulseTrainDelaySpinBox->setMaximumWidth(50);
    interPulseTrainDelayLayout->addWidget(interPulseTrainDelayLabel);
    interPulseTrainDelayLayout->addWidget(interPulseTrainDelaySpinBox);

    QVBoxLayout *loopingLayout = new QVBoxLayout;
    loopingLayout->addLayout(numPulseTrainLayout);
    loopingLayout->addLayout(interPulseTrainDelayLayout);

    Looping->setLayout(loopingLayout);

//-------------------------------------------------------------------------------------------------//
    QHBoxLayout *parametersLayout1 = new QHBoxLayout;
    parametersLayout1->addWidget(Intensity);
    parametersLayout1->addWidget(Timing);

    legend1 = new QLabel(tr("* Multi-Channel (Interleaved & Non-Interleaved)"));
    legend2 = new QLabel(tr("^ Amplitude Graded"));
    legend3 = new QLabel(tr("~ FLAT Single"));

    QVBoxLayout *parametersLayout = new QVBoxLayout;
    parametersLayout->addLayout(parametersLayout1);
    parametersLayout->addWidget(Looping);
    parametersLayout->addWidget(legend1);
    parametersLayout->addWidget(legend2);
    parametersLayout->addWidget(legend3);

    Parameters->setLayout(parametersLayout);

    testButton = new QPushButton(tr("Send!"));
    connect(testButton, SIGNAL(clicked(bool)), this, SLOT(sendCommand()));
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(modeLayout);
    mainLayout->addLayout(channelLayout);
    mainLayout->addWidget(Parameters);
    mainLayout->addWidget(testButton);
    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}

void MainWindow::sendCommand(){
    commandOdin->sendCommand();
}

void MainWindow::on_Mode_Changed(int Mode){
    commandOdin->setMode(Mode);
}

void MainWindow::on_channel_Changed(int channel){
    commandOdin->setChannelNumber((CHANNELNUMBER) channel);
}

void MainWindow::on_pulseMag_Changed(){
    for(int i = 0; i < 5; i++){
        commandOdin->setPulseMag(i, pulseMag[i]->value());
    }
}

void MainWindow::on_pulseDuration_Changed(){
    commandOdin->setPulseDuration(pulseDurationSpinBox->value());
}

void MainWindow::on_numPulse_Changed(){
    commandOdin->setPulseNum((char) numPulseSpinBox->value());
}

void MainWindow::on_interPulseDuration_Changed(){
    commandOdin->setInterPulseDuration((char) interPulseDurationSpinBox->value());
}

MainWindow::~MainWindow(){
}
