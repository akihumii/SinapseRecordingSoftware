#include "mainwindow.h"

MainWindow::MainWindow(){
    QString version(APP_VERSION);
    setWindowTitle(tr("Odin Stimulator Software V") + version);
    serialOdin = new SerialOdin(this);
    socketOdin = new SocketOdin(this);
    commandOdin = new CommandOdin(serialOdin, socketOdin);
    loopingThread = new LoopingThread();
    QThread *thread = new QThread;
    loopingThread->moveToThread(thread);
    connect(loopingThread, SIGNAL(finishedSending()), this, SLOT(on_finishedSending()));
    connect(loopingThread, SIGNAL(commandSent()), this, SLOT(on_commandSent()));
    connect(socketOdin, SIGNAL(odinDisconnected()), this, SLOT(on_odinDisconnected()));
    connect(serialOdin, SIGNAL(odinDisconnected()), this, SLOT(on_odinDisconnected()));
    mbox = new QMessageBox;
    mboxStop = new QMessageBox;
    pulsePlot = new PulsePlot;

    createLayout();
    createStatusBar();
//    fileMenu = menuBar()->addMenu(tr("&Connect"));
    connectAction = new QAction(tr("&Connect"), this);
//    connectAction->setShortcut(tr("Ctrl+C"));
//    connect(connectAction, SIGNAL(triggered()), this, SLOT(on_ConnectMenu_triggered()));
//    fileMenu->addAction(connectAction);
    connectOdin();
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

    Parameters = new QGroupBox(tr("Stimulation Parameters"));
    Intensity = new QGroupBox(tr("Pulse Magnitude (0.3 to 9.3 mA)"));
    Timing = new QGroupBox(tr("Timing Parameters"));
    Looping = new QGroupBox(tr("Looping Parameters"));

    QVBoxLayout *intensityLayout1 = new QVBoxLayout;
    QVBoxLayout *intensityLayout2 = new QVBoxLayout;
    pulseMagLabel[0] = new QLabel(tr("Magnitude 1 *^~"));
    pulseMagLabel[1] = new QLabel(tr("Magnitude 2 *^"));
    pulseMagLabel[2] = new QLabel(tr("Magnitude 3 *^"));
    pulseMagLabel[3] = new QLabel(tr("Magnitude 4 *^"));
    pulseMagLabel[4] = new QLabel(tr("Magnitude 5 ^"));
    for(int i = 0; i < 5; i++){
        intensityLayout1->addWidget(pulseMagLabel[i]);
        pulseMag[i] = new QDoubleSpinBox;
        pulseMag[i]->setMinimum(0.0);
        pulseMag[i]->setMaximum(9.3);
        pulseMag[i]->setSingleStep(0.1);
        pulseMag[i]->setValue(2.0);
        pulseMag[i]->setMaximumWidth(50);
        pulseMag[i]->setAlignment(Qt::AlignHCenter);
        pulseMag[i]->setDisabled(true);
        intensityLayout2->addWidget(pulseMag[i]);
        connect(pulseMag[i], SIGNAL(valueChanged(double)), this, SLOT(on_pulseMag_Changed()));
    }
    pulseMag[0]->setDisabled(false);

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
    numPulseTrainSpinBox->setMaximum(50000);
    numPulseTrainSpinBox->setMaximumWidth(50);
    numPulseTrainLayout->addWidget(numPulseTrainLabel);
    numPulseTrainLayout->addWidget(numPulseTrainSpinBox);

    connect(numPulseTrainSpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_numPulseTrain_Changed()));

    QHBoxLayout *interPulseTrainDelayLayout = new QHBoxLayout;
    interPulseTrainDelayLabel = new QLabel(tr("Inter-Pulse Train Delay (ms)"));
    interPulseTrainDelaySpinBox = new QSpinBox;
    interPulseTrainDelaySpinBox->setMinimum(1000);
    interPulseTrainDelaySpinBox->setMaximum(50000);
    interPulseTrainDelaySpinBox->setValue(3000);
    interPulseTrainDelaySpinBox->setMaximumWidth(50);
    interPulseTrainDelayLayout->addWidget(interPulseTrainDelayLabel);
    interPulseTrainDelayLayout->addWidget(interPulseTrainDelaySpinBox);

    connect(interPulseTrainDelaySpinBox, SIGNAL(valueChanged(int)), this, SLOT(on_interPulseTrainDelay_Changed()));

    QVBoxLayout *loopingLayout = new QVBoxLayout;
    loopingLayout->addLayout(numPulseTrainLayout);
    loopingLayout->addLayout(interPulseTrainDelayLayout);

    Looping->setLayout(loopingLayout);

    QHBoxLayout *parametersLayout1 = new QHBoxLayout;
    parametersLayout1->addWidget(Intensity);
    parametersLayout1->addWidget(Timing);

    legend = new QLabel(tr("<FONT SIZE = 2>*Multi-Channel (Interleaved & Non-Interleaved)   ^Amplitude Graded   ~FLAT Single</b></b>"));

    QVBoxLayout *parametersLayout = new QVBoxLayout;
    parametersLayout->addLayout(parametersLayout1);
    parametersLayout->addWidget(legend);
    parametersLayout->addWidget(Looping);

    Parameters->setLayout(parametersLayout);

    sendButton = new QPushButton(tr("Send!"));
    connect(sendButton, SIGNAL(clicked(bool)), this, SLOT(sendCommand()));

    multiChannel = new QGroupBox(tr("Multi-Channel Settings"));

    sequence = new QGroupBox(tr("Sequence of Channels for Tetanic Pulses"));
    channelSeqLabel[0] = new QLabel(tr("Channel A"));
    channelSeqLabel[1] = new QLabel(tr("Channel B"));
    channelSeqLabel[2] = new QLabel(tr("Channel C"));
    channelSeqLabel[3] = new QLabel(tr("Channel D"));
    QVBoxLayout *multiChannelleftLayout = new QVBoxLayout;
    for(int i = 0; i < 4; i++){
        channelSeqLayout[i] = new QHBoxLayout;
        channelSeqLayout[i]->addWidget(channelSeqLabel[i]);
        channelSeqComboBox[i] = new QComboBox;
        channelSeqComboBox[i]->setFixedWidth(90);
        for(int j = 0; j < 4; j++){
            channelSeqComboBox[i]->addItem(QString::number(j+1));
        }
        channelSeqComboBox[i]->setCurrentIndex(i);
        channelSeqComboBox[i]->setMaximumWidth(40);
        channelSeqLayout[i]->addWidget(channelSeqComboBox[i]);
        multiChannelleftLayout->addLayout(channelSeqLayout[i]);

        connect(channelSeqComboBox[i], SIGNAL(currentIndexChanged(int)), this, SLOT(on_channelSeq_Changed()));
    }

    sequence->setLayout(multiChannelleftLayout);
    for(int i = 0; i < 4; i++){
        zoneleftLayout[i] = new QHBoxLayout;
        zoneLabel[i] = new QLabel("Zone " + QString::number(i+1));
        zoneSelector[i] = new QComboBox;
        zoneSelector[i]->setFixedWidth(90);
        zoneSelector[i]->addItem("Time A");
        zoneSelector[i]->addItem("Time B");
        zoneSelector[i]->addItem("Time C");
        zoneSelector[i]->addItem("Time D");
        zoneleftLayout[i]->addWidget(zoneLabel[i]);
        zoneleftLayout[i]->addWidget(zoneSelector[i]);

        connect(zoneSelector[i], SIGNAL(currentIndexChanged(int)), this, SLOT(on_zoneSelector_Changed()));
    }

    for(int i = 0; i < 3; i ++){
        zonerightLayout[i] = new QHBoxLayout;
        zoneLabel[i+4] = new QLabel("Zone " + QString::number(i+5));
        zoneSelector[i+4] = new QComboBox;
        zoneSelector[i+4]->setFixedWidth(90);
        zoneSelector[i+4]->addItem("Time A");
        zoneSelector[i+4]->addItem("Time B");
        zoneSelector[i+4]->addItem("Time C");
        zoneSelector[i+4]->addItem("Time D");
        zonerightLayout[i]->addWidget(zoneLabel[i+4]);
        zonerightLayout[i]->addWidget(zoneSelector[i+4]);

        connect(zoneSelector[i+4], SIGNAL(currentIndexChanged(int)), this, SLOT(on_zoneSelector_Changed()));
    }
    maskLabel = new QLabel(tr("Zone Mask"));
    zonerightLayout[3] = new QHBoxLayout;
    maskSelector = new QComboBox;
    maskSelector->setFixedWidth(90);
    maskSelector->addItem("Mask None");
    maskSelector->addItem("Mask Zone 1");
    maskSelector->addItem("Mask Zone 2");
    maskSelector->addItem("Mask Zone 3");
    maskSelector->addItem("Mask Zone 4");
    maskSelector->addItem("Mask Zone 5");
    maskSelector->addItem("Mask Zone 6");
    maskSelector->addItem("Mask Zone 7");
    maskSelector->addItem("Mask All Zone");

    connect(maskSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(on_zoneMask_Changed()));

    zonerightLayout[3]->addWidget(maskLabel);
    zonerightLayout[3]->addWidget(maskSelector);

    QVBoxLayout *zoneleftmain = new QVBoxLayout;
    for(int i = 0; i < 4; i ++){
        zoneleftmain->addLayout(zoneleftLayout[i]);
    }

    QVBoxLayout *zonerightmain = new QVBoxLayout;
    for(int i = 0; i < 4; i ++){
        zonerightmain->addLayout(zonerightLayout[i]);
    }

    QHBoxLayout *zoneLayout = new QHBoxLayout;
    zoneLayout->addLayout(zoneleftmain);
    zoneLayout->addLayout(zonerightmain);

    zoneDuration = new QGroupBox(tr("Zone Duration Selector"));
    zoneDuration->setLayout(zoneLayout);

    QHBoxLayout *multiChannelmainLayout = new QHBoxLayout;
    multiChannelmainLayout->addWidget(sequence);
    multiChannelmainLayout->addWidget(zoneDuration);

    multiChannel->setLayout(multiChannelmainLayout);
    pulseGraph = new QCustomPlot;

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(modeLayout);
    mainLayout->addLayout(channelLayout);
    mainLayout->addWidget(Parameters);
    mainLayout->addWidget(multiChannel);
    mainLayout->addWidget(pulseGraph);
    mainLayout->addWidget(sendButton);
    QWidget *mainWidget = new QWidget;
    multiChannel->setDisabled(true);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
    mainLayout->setSizeConstraint( QLayout::SetFixedSize );

    pulseGraph->setMinimumHeight(120);
    pulseGraph->yAxis->setAutoTickStep(false);
    pulseGraph->xAxis->setTickStep((double) 0.000001);
    pulseGraph->setInteractions(QCP::iRangeDrag);
    pulseGraph->axisRect()->setAutoMargins(QCP::msNone);
    pulseGraph->yAxis->setRange(-2.2, 4.8, Qt::AlignLeft);
    pulseGraph->yAxis->setTickStep(2.0);
    pulseGraph->addGraph();
    plotPulse();
}

void MainWindow::createStatusBar(){
    statusBarLabel = new QLabel;
    statusBarMainWindow = statusBar();
    statusBarMainWindow->addPermanentWidget(statusBarLabel, 1);
    statusBarMainWindow->setSizeGripEnabled(false);  // fixed window size
    statusBarLabel->setText("Odin initialised");
}

bool MainWindow::connectOdin(){
    portInfo = QSerialPortInfo::availablePorts();
    connectionStatus.clear();
    if(portInfo.size()>0){
        serialOdin->connectOdin();
        connectionStatus.clear();
        if(serialOdin->isOdinSerialConnected()){
            connectionStatus.append("Connected to Odin!!");
            sendButton->setEnabled(true);
            return true;
        }
        else{
            connectionStatus.append("Connection to Odin failed");
        }
        statusBarLabel->setText(connectionStatus);
    }
    if(!serialOdin->isOdinSerialConnected()){
        socketOdin->doConnect("10.10.10.1", 30000);
        connectionStatus.clear();
        if(socketOdin->isConnected()){
            connectionStatus.append("Connected to Odin WiFi Module at 10.10.10.1/30000");
            statusBarLabel->setText(connectionStatus);
            sendButton->setEnabled(true);
            return true;
        }
        else{
            sendButton->setDisabled(true);
            connectionStatus.append("Connection to Odin failed! Restart this program after connecting Odin.");
            QMessageBox::information(this, "Failed to connect!", "No Odin device detected.. \n"
                                                                 "Check your connections and run the program again..");
            statusBarLabel->setText(connectionStatus);
            return false;
        }
    }
    return false;
}

void MainWindow::sendCommand(){
    start = !start;
    commandOdin->constructCommand();
    setDelay();
    if(start){
        commandOdin->sendStart();
        loopingThread->send = true;
        loopingThread->start();
        numPulseTrainSpinBox->setDisabled(true);
        interPulseTrainDelaySpinBox->setDisabled(true);
        sendButton->setText("Stop!");
    }
    else{
        commandOdin->sendStop();
        loopingThread->send = false;
        loopingThread->quit();
        mboxStop->setText("Stopping Odin.. Please wait...");
        mboxStop->setStandardButtons(0);
        mboxStop->show();
    }
}

void MainWindow::on_Mode_Changed(int Mode){
    if(ModeComboBox->currentIndex() < 4){
        multiChannel->setDisabled(true);
        channelComboBox->setEnabled(true);
    }
    else{
        multiChannel->setEnabled(true);
        channelComboBox->setDisabled(true);
        if(ModeComboBox->currentIndex() == 5){
            zoneDuration->setDisabled(true);
        }
        else{
            zoneDuration->setEnabled(true);
        }
    }
    for(int i = 0; i < 5; i++){
        pulseMag[i]->setEnabled(true);
    }
    if(ModeComboBox->currentIndex() == 0 || ModeComboBox->currentIndex() == 2 || ModeComboBox->currentIndex() == 3){
        for(int i = 1; i < 5; i++){
            pulseMag[i]->setDisabled(true);
        }
    }
    else if(ModeComboBox->currentIndex() == 4 || ModeComboBox->currentIndex() == 5){
        pulseMag[4]->setDisabled(true);
    }

    commandOdin->setMode(Mode);
}

void MainWindow::on_channel_Changed(int channel){
    commandOdin->setChannelNumber((CHANNELNUMBER) channel);
}

void MainWindow::on_pulseMag_Changed(){
    for(int i = 0; i < 5; i++){
        commandOdin->setPulseMag(i, pulseMag[i]->value());
        pulsePlot->setAmplitude(pulseMag[0]->value());
    }
    pulseGraph->yAxis->setTickStep((double) pulseMag[0]->value());
    pulseGraph->yAxis->setRange(-(pulseMag[0]->value()+0.2), 2*(pulseMag[0]->value()+0.2), Qt::AlignLeft);
    plotPulse();
}

void MainWindow::on_pulseDuration_Changed(){
    pulsePlot->setPulseDuration(pulseDurationSpinBox->value());
    plotPulse();
    commandOdin->setPulseDuration(pulseDurationSpinBox->value());
}

void MainWindow::on_numPulse_Changed(){
    commandOdin->setPulseNum((char) numPulseSpinBox->value());
    setDelay();
}

void MainWindow::on_interPulseDuration_Changed(){
    commandOdin->setInterPulseDuration((char) interPulseDurationSpinBox->value());
    setDelay();
}

void MainWindow::on_channelSeq_Changed(){
    int *temp = commandOdin->getMultiChannelSequence();
    for(int i = 0; i < 4; i++){
        if(channelSeqComboBox[i]->currentIndex() != temp[i]){
            for(int j = 0; j < 4; j++){
                if(i != j && channelSeqComboBox[j]->currentIndex() == channelSeqComboBox[i]->currentIndex()){
                    channelSeqComboBox[j]->setCurrentIndex(temp[i]);
                }
            }
        }
    }
    commandOdin->setMultiChannelSequence((CHANNELNUMBER) channelSeqComboBox[0]->currentIndex(),
                                         (CHANNELNUMBER) channelSeqComboBox[1]->currentIndex(),
                                         (CHANNELNUMBER) channelSeqComboBox[2]->currentIndex(),
                                         (CHANNELNUMBER) channelSeqComboBox[3]->currentIndex());
}

void MainWindow::on_zoneSelector_Changed(){
    for(int i = 0; i < 7; i++){
        commandOdin->setTimeZone(i, (TIMEZONE) zoneSelector[i]->currentIndex());
    }
}

void MainWindow::on_zoneMask_Changed(){
    if(maskSelector->currentIndex() == 0){
        commandOdin->setZoneMask((ZONEMASK) 0b01010101);
    }
    else{
        commandOdin->setZoneMask((ZONEMASK) ~(1<<(maskSelector->currentIndex())));
    }
}

void MainWindow::on_numPulseTrain_Changed(){
    loopingThread->num = numPulseTrainSpinBox->value();
}

void MainWindow::on_interPulseTrainDelay_Changed(){
    setDelay();
}

void MainWindow::on_finishedSending(){
    numPulseTrainSpinBox->setEnabled(true);
    interPulseTrainDelaySpinBox->setEnabled(true);
    sendButton->setText("Start!");
    mboxStop->hide();
    mbox->setText("Finished sending commands " + QString::number(commandCount, 10) + " times");
    mbox->show();
    commandOdin->sendStop();
    commandCount = 0;
    start = false;
}

void MainWindow::on_commandSent(){
    commandCount++;
    commandOdin->sendCommand();
    if(commandOdin->getlastSentCommand().size() > 0){
    QString lastCommand;
    lastCommand.append("Command: ");
    for(int i = 0; i < commandOdin->getlastSentCommand().size(); i++){
        if((quint8) commandOdin->getlastSentCommand().at(i) < 16){
            lastCommand.append("0x0" + QString::number((quint8) commandOdin->getlastSentCommand().at(i), 16).toUpper());
        }
        else{
            lastCommand.append("0x" + QString::number((quint8) commandOdin->getlastSentCommand().at(i), 16).toUpper());
        }
        lastCommand.append(" ");
    }
    lastCommand.append(" | ");
    lastCommand.append("Sent: " + QString::number(commandCount,10));

    statusBarLabel->setText(lastCommand);
    }
}

void MainWindow::plotPulse(){
    pulsePlot->updateYvalues();
    qDebug() << "Setting data";

    pulseGraph->graph()->setData(pulsePlot->retrieveXaxis(), pulsePlot->retrieveYaxis());
    pulseGraph->xAxis->setRange(0, 2550*0.000001, Qt::AlignLeft);
    pulseGraph->replot();
}

void MainWindow::setDelay(){
    if(ModeComboBox->currentIndex() == 0){
        loopingThread->delay = interPulseTrainDelaySpinBox->value() + (int) (( 1.0 / (interPulseDurationSpinBox->value()) * 1000.0) * 10.0 *  numPulseSpinBox->value());
    }
    else if(ModeComboBox->currentIndex() == 1){
        loopingThread->delay = interPulseTrainDelaySpinBox->value() + (int) (( 5.0 / (interPulseDurationSpinBox->value()) * 1000.0) * 10.0 *  numPulseSpinBox->value());
    }
    else if(ModeComboBox->currentIndex() == 2 || ModeComboBox->currentIndex() == 3){
        loopingThread->delay = interPulseTrainDelaySpinBox->value() + (int) (( 1.0 / (5*interPulseDurationSpinBox->value()) * 1000.0) * 10.0 *  numPulseSpinBox->value() +
                                                                             ( 1.0 / (4*interPulseDurationSpinBox->value()) * 1000.0) * 10.0 *  numPulseSpinBox->value() +
                                                                             ( 1.0 / (3*interPulseDurationSpinBox->value()) * 1000.0) * 10.0 *  numPulseSpinBox->value() +
                                                                             ( 1.0 / (2*interPulseDurationSpinBox->value()) * 1000.0) * 10.0 *  numPulseSpinBox->value() +
                                                                             ( 1.0 / (interPulseDurationSpinBox->value()) * 1000.0) * 10.0 *  numPulseSpinBox->value());
    }
    else if(ModeComboBox->currentIndex() == 5){
        if(pulseMag[2]->value() == 0.0 && pulseMag[3]->value() == 0.0){
            qDebug() << (quint8) pulseMag[3]->value();
            loopingThread->delay = interPulseTrainDelaySpinBox->value() + (int) (( 2.0 / (interPulseDurationSpinBox->value()) * 1000.0) * 10.0 *  numPulseSpinBox->value());
        }
        else if(pulseMag[3]->value() == 0.0){
            qDebug() << (quint8) pulseMag[3]->value();
            loopingThread->delay = interPulseTrainDelaySpinBox->value() + (int) (( 3.0 / (interPulseDurationSpinBox->value()) * 1000.0) * 10.0 *  numPulseSpinBox->value());
        }
        else{
            loopingThread->delay = interPulseTrainDelaySpinBox->value() + (int) (( 4.0 / (interPulseDurationSpinBox->value()) * 1000.0) * 10.0 *  numPulseSpinBox->value());
        }
    }
    else{
        loopingThread->delay = interPulseTrainDelaySpinBox->value() + 500;
    }
}

void MainWindow::on_odinDisconnected(){
    QMessageBox::warning(this, tr("Odin Disconnected!"), tr("Please restart the program after reconnecting to Odin"));
    this->close();
}

void MainWindow::on_ConnectMenu_triggered(){
    IPDialog ipDialog;
    ipDialog.exec();
}

MainWindow::~MainWindow(){
}
