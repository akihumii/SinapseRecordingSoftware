#include "odinwindow.h"

namespace Odin {

OdinWindow::OdinWindow(){
    QString version(APP_VERSION);
    setWindowTitle(tr("Odin Stimulator Software V") + version);
    serialOdin = new SerialOdin(this);
    socketOdin = new SocketOdin;
    commandOdin = new CommandOdin(serialOdin, socketOdin);
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    loopingThread = new LoopingThread();
    QThread *thread = new QThread;
    loopingThread->moveToThread(thread);
    connect(loopingThread, SIGNAL(finishedSending()), this, SLOT(pauseOdin()));
    connect(socketOdin, SIGNAL(odinDisconnected()), this, SLOT(on_odinDisconnected()));
    connect(serialOdin, SIGNAL(odinDisconnected()), this, SLOT(on_odinDisconnected()));

    configurationFile = new ConfigurationFile;
    configurationFile->setFilenameSettingsMostRecent(filenameMostRecent);
    connect(configurationFile, SIGNAL(writeSettingsSignal()), this, SLOT(writeSettings()));
    connect(configurationFile, SIGNAL(readSettingsSignal()), this, SLOT(on_read_settings_changed()));
    filenameSettingsAllMapper = new QSignalMapper(this);
    connect(filenameSettingsAllMapper, SIGNAL(mapped(int)), this, SLOT(on_read_settings_selected_changed(int)));

    createStatusBar();
    createActions();
    mainWidget = new QWidget;
    configurationFile->readMostRecentSettings();
    firstLoadingFlag = false;
    createLayout();
    connectOdin();

    qDebug() << tcpServer->serverPort() << tcpServer->serverAddress().toString();
    while (!tcpServer->isListening() && !tcpServer->listen(QHostAddress::LocalHost, 13567)) {
            QMessageBox::StandardButton ret = QMessageBox::critical(this,
                                            tr("Loopback"),
                                            tr("Unable to start the test: %1.")
                                            .arg(tcpServer->errorString()),
                                            QMessageBox::Retry
                                            | QMessageBox::Cancel);
            if (ret == QMessageBox::Cancel)
                return;
    }
}

void OdinWindow::createActions(){
    //FileMenu
    openSettingsAction = new QAction(tr("&Open"));
    openSettingsAction->setShortcut(tr("Ctrl+O"));
    connect(openSettingsAction, SIGNAL(triggered(bool)), configurationFile, SLOT(on_read_settings_changed()));

    saveSettingsAction = new QAction(tr("&Save"));
    saveSettingsAction->setShortcut(tr("Ctrl+S"));
    connect(saveSettingsAction, SIGNAL(triggered(bool)), this, SLOT(writeSettings()));

    saveSettingsAsAction = new QAction(tr("S&ave As"));
    saveSettingsAsAction->setShortcut(tr("Ctrl+A"));
    connect(saveSettingsAsAction, SIGNAL(triggered(bool)), configurationFile, SLOT(on_write_settings_changed()));

    //GUIMenu
    sylphxAction = new QAction(tr("SylphX Control Panel"));
    sylphxAction->setShortcut(tr("Ctrl+R"));
    connect(sylphxAction, SIGNAL(triggered(bool)), this, SLOT(on_sylphx_triggered()));

    catAction = new QAction(tr("Ca&t Control Panel"));
    catAction->setShortcut(tr("Ctrl+T"));
    connect(catAction, SIGNAL(triggered(bool)), this, SLOT(on_cat_triggered()));

    //Add to menu
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openSettingsAction);
    openSettingsRecentAction = fileMenu->addMenu(tr("O&pen Recent"));
    for(int i = 0; i < 10; i++){
        recentFilenameSettings[i] = new QAction;
        connect(recentFilenameSettings[i], SIGNAL(triggered(bool)), filenameSettingsAllMapper, SLOT(map()));
        filenameSettingsAllMapper->setMapping(recentFilenameSettings[i], i);
    }

    fileMenu->addAction(saveSettingsAction);
    fileMenu->addAction(saveSettingsAsAction);

    GUIMenu = menuBar()->addMenu(tr("G&UI"));
    GUIMenu->addAction(sylphxAction);
    GUIMenu->addAction(catAction);
}

void OdinWindow::createLayout(){
    stimParameters = new QGroupBox(tr("Stimulator Parameters"));
    paraLabels[0] = new QLabel(tr(" "));
    paraLabels[1] = new QLabel(tr("Channel Enabled: "));
    paraLabels[2] = new QLabel;
//    paraLabels[2] = new QLabel(tr("Threshold Enable: "));
    paraLabels[3] = new QLabel(tr("Amplitude(uA): "));
    paraLabels[4] = new QLabel(tr("Pulse Duration(us): "));
    paraLabels[5] = new QLabel(tr("Frequency(Hz): "));

    for(int i = 0; i < 6; i++){
        stimParaLayout[i] = new QHBoxLayout;
        paraLabels[i]->setMinimumWidth(100);
        stimParaLayout[i]->addWidget(paraLabels[i]);
    }

    for(int i = 0; i < 4; i++){
        chnLabels[i] = new QLabel;
        chnLabels[i]->setText("Channel " + QString::number(i+1));
        stimParaLayout[0]->addWidget(chnLabels[i]);

        channelEnable[i] = new QCheckBox;
        channelEnable[i]->setChecked(commandOdin->getChannelEnabled() >> i);
        stimParaLayout[1]->addWidget(channelEnable[i]);
        connect(channelEnable[i], SIGNAL(toggled(bool)), this, SLOT(on_channelEnable_toggled()));

        thresholdEnable[i] = new QCheckBox;
//        stimParaLayout[2]->addWidget(thresholdEnable[i]);
        connect(thresholdEnable[i], SIGNAL(toggled(bool)), this, SLOT(on_thresholdEnable_toggled()));

        amplitudeSpinBox[i] = new QDoubleSpinBox;
        amplitudeSpinBox[i]->setMinimum(0.0);
        amplitudeSpinBox[i]->setMaximum(19.0);
        amplitudeSpinBox[i]->setSingleStep(1.0);
        amplitudeSpinBox[i]->setValue(commandOdin->getAmplitude(i));
        stimParaLayout[3]->addWidget(amplitudeSpinBox[i]);
        connect(amplitudeSpinBox[i], SIGNAL(editingFinished()), this, SLOT(on_amplitude_Changed()));

        pulseDurationSpinBox[i] = new QSpinBox;
        pulseDurationSpinBox[i]->setMinimum(20);
        pulseDurationSpinBox[i]->setMaximum(1000);
        pulseDurationSpinBox[i]->setValue(commandOdin->getPulseDuration(i));
        stimParaLayout[4]->addWidget(pulseDurationSpinBox[i]);
        connect(pulseDurationSpinBox[i], SIGNAL(editingFinished()), this, SLOT(on_pulseDuration_Changed()));
    }

    frequencySpinBox = new QSpinBox;
    frequencySpinBox->setMinimumWidth(360);
    frequencySpinBox->setMinimum(10);
    frequencySpinBox->setMaximum(200);
    frequencySpinBox->setValue(commandOdin->getFrequency());
    connect(frequencySpinBox, SIGNAL(editingFinished()), this, SLOT(on_frequency_Changed()));
    stimParaLayout[5]->addWidget(frequencySpinBox);

    QVBoxLayout *stimParaMainLayout = new QVBoxLayout;
    for(int i = 0; i < 6; i ++){
        stimParaMainLayout->addLayout(stimParaLayout[i]);
    }

    QHBoxLayout *buttonChannelLayout = new QHBoxLayout;
    channelSelectAll = new QPushButton(tr("Channel Select All"));
    channelSelectNone = new QPushButton(tr("Channel Select None"));
    buttonChannelLayout->addWidget(channelSelectAll);
    buttonChannelLayout->addWidget(channelSelectNone);
    connect(channelSelectAll, SIGNAL(clicked(bool)), this, SLOT(on_channelSelectAll_clicked()));
    connect(channelSelectNone, SIGNAL(clicked(bool)), this, SLOT(on_channelSelectNone_clicked()));

    QHBoxLayout *buttonThresholdLayout = new QHBoxLayout;
    thresholdSelectAll = new QPushButton(tr("Threshold Select All"));
    thresholdSelectNone = new QPushButton(tr("Threshold Select None"));
//    buttonThresholdLayout->addWidget(thresholdSelectAll);
//    buttonThresholdLayout->addWidget(thresholdSelectNone);
    connect(thresholdSelectAll, SIGNAL(clicked(bool)), this, SLOT(on_thresholdSelectAll_clicked()));
    connect(thresholdSelectNone, SIGNAL(clicked(bool)), this, SLOT(on_thresholdSelectNone_clicked()));

    stimParaMainLayout->addLayout(buttonChannelLayout);
    stimParaMainLayout->addLayout(buttonThresholdLayout);

    stimParameters->setLayout(stimParaMainLayout);

    delayParameters = new QGroupBox(tr("Delay Parameters"));
    delayEnabledCheckBox = new QCheckBox;
    delayEnabledCheckBox->setChecked(delayFlag);
    connect(delayEnabledCheckBox, SIGNAL(toggled(bool)), this, SLOT(on_delayEnabled_toggled()));
    QLabel *delayLabel = new QLabel(tr("Turn off after (seconds) : "));
    delaySpinBox = new QSpinBox;
    delaySpinBox->setValue(delayValue);
    delaySpinBox->setMinimumWidth(220);
    delaySpinBox->setMaximum(60000);

    QHBoxLayout *delayLayout = new QHBoxLayout;
    delayLayout->addWidget(delayEnabledCheckBox);
    delayLayout->addWidget(delayLabel);
    delayLayout->addWidget(delaySpinBox);

    delayParameters->setLayout(delayLayout);

    thresholdParameters = new QGroupBox(tr("Threshold Parameters"));
    thresholdLabels[0] = new QLabel(tr("Step Size (mA):"));
    thresholdLabels[1] = new QLabel(tr("Channel A Threshold (mV):"));
    thresholdLabels[2] = new QLabel(tr("Channel B Threshold (mV):"));
    thresholdLabels[3] = new QLabel(tr("Debounce delay (ms):"));

    QVBoxLayout *thresholdLabelLayout = new QVBoxLayout;
    for(int i = 0; i < 4; i++){
        thresholdLabelLayout->addWidget(thresholdLabels[i]);
    }

    QVBoxLayout *thresholdSpinBoxLayout = new QVBoxLayout;

    stepSizeSpinBox = new QSpinBox;
    stepSizeSpinBox->setMinimum(1);
    stepSizeSpinBox->setMaximum(20);
    stepSizeSpinBox->setSingleStep(1);
    stepSizeSpinBox->setValue(1);
    connect(stepSizeSpinBox, SIGNAL(editingFinished()), this, SLOT(on_stepSize_editted()));
    thresholdSpinBoxLayout->addWidget(stepSizeSpinBox);

    upperThresholdSpinBox = new QDoubleSpinBox;
    upperThresholdSpinBox->setMinimum(0.0);
    upperThresholdSpinBox->setMaximum(10.0);
    upperThresholdSpinBox->setSingleStep(1.0);
    upperThresholdSpinBox->setValue(10.0);
    connect(upperThresholdSpinBox, SIGNAL(editingFinished()), this, SLOT(on_upperThreshold_editted()));
    thresholdSpinBoxLayout->addWidget(upperThresholdSpinBox);

    lowerThresholdSpinBox = new QDoubleSpinBox;
    lowerThresholdSpinBox->setMinimum(0.0);
    lowerThresholdSpinBox->setMaximum(10.0);
    lowerThresholdSpinBox->setSingleStep(1.0);
    lowerThresholdSpinBox->setValue(10.0);
    connect(lowerThresholdSpinBox, SIGNAL(editingFinished()), this, SLOT(on_lowerThreshold_editted()));
    thresholdSpinBoxLayout->addWidget(lowerThresholdSpinBox);

    debounceSpinBox = new QSpinBox;
    debounceSpinBox->setMinimum(20);
    debounceSpinBox->setMaximum(5000);
    debounceSpinBox->setSingleStep(100);
    debounceSpinBox->setValue(1000);
    connect(debounceSpinBox, SIGNAL(editingFinished()), this, SLOT(on_debounce_editted()));
    thresholdSpinBoxLayout->addWidget(debounceSpinBox);

    QHBoxLayout *thresholdMainLayout = new QHBoxLayout;
    thresholdMainLayout->addLayout(thresholdLabelLayout);
    thresholdMainLayout->addLayout(thresholdSpinBoxLayout);
    thresholdParameters->setLayout(thresholdMainLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    sendParameterButton = new QPushButton(tr("SP"));
    connect(sendParameterButton, SIGNAL(clicked(bool)), this, SLOT(sendParameter()));
    sendParameterButton->setMaximumWidth(40);
    sendButton = new QPushButton(tr("Start Odin!"));
    connect(sendButton, SIGNAL(clicked(bool)), this, SLOT(sendCommand()));
    modeButton = new QPushButton(tr("H"));
    modeButton->setShortcut(QKeySequence(tr("Ctrl+Q")));
    connect(modeButton, SIGNAL(clicked(bool)), this, SLOT(on_currentMode_clicked()));
    modeButton->setMaximumWidth(20);
//    modeButton->setIcon(QIcon(QDir::currentPath()+"/recordStart.png"));
//    modeButton->setIconSize(QSize(15,15));
    buttonLayout->addWidget(sendParameterButton);
    buttonLayout->addWidget(sendButton);
    buttonLayout->addWidget(modeButton);

    mainWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(stimParameters);
    mainLayout->addWidget(delayParameters);
//    mainLayout->addWidget(thresholdParameters);
    mainLayout->addLayout(buttonLayout);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
    mainLayout->setSizeConstraint( QLayout::SetFixedSize );

}

void OdinWindow::createStatusBar(){
    statusBarLabel = new QLabel;
    statusBarMainWindow = statusBar();
    statusBarMainWindow->addPermanentWidget(statusBarLabel, 1);
    statusBarMainWindow->setSizeGripEnabled(false);  // fixed window size
}

bool OdinWindow::connectOdin(){
    portInfo = QSerialPortInfo::availablePorts();
    connectionStatus.clear();
    if(portInfo.size()>0){
        connectionStatus.clear();
        serialOdin->connectOdin();
        if(serialOdin->isOdinSerialConnected()){
            connectionStatus.append("Connected to Odin at " + serialOdin->getConnectedPort());
            sendButton->setEnabled(true);
            statusBarLabel->setText(connectionStatus);
            return true;
        }
        else{
            statusBarLabel->setText(connectionStatus);
        }
    }
    if(!serialOdin->isOdinSerialConnected()){
        socketOdin->doConnect("192.168.4.1", 30000);
        if(socketOdin->isConnected()){
            connectionStatus.append("Connected to Odin WiFi Module at 192.168.4.1/30000");
            statusBarLabel->setText(connectionStatus);
            return true;
        }
        else{
            connectionStatus.append("Connection to Odin failed! Command now being sent through UDP socket!");
            QMessageBox::information(this, "Failed to connect!", "No Odin device detected.. \n"
                                                                 "Check your connections and run the program again..");
            statusBarLabel->setText(connectionStatus);
            return false;
        }
    }
    return false;
}

void OdinWindow::sendParameter(){
    int delay = 40;

    for(int i = 0; i < 4; i++){  // send pulse duration
        QTimer::singleShot((STARTDELAY+delay+i*delayInterval), [=] {
            commandOdin->sendPulseDuration(i);
            strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
            emit commandSent(lastSentCommand);
        });
    }
    delay += 4*delayInterval;
    for(int i = 0; i < 4; i++){  // send amplitude
        QTimer::singleShot((STARTDELAY+delay+i*delayInterval), [=] {
            commandOdin->setAmplitude(i, amplitudeSpinBox[i]->text().toDouble());
            qDebug() << "Set channel " << i << "amplitude to : " << amplitudeSpinBox[i]->text().toDouble();

            commandOdin->sendAmplitude(i);
            strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
            emit commandSent(lastSentCommand);
        });
    }
    delay += 4*delayInterval;
    QTimer::singleShot((STARTDELAY+delay), [=] {  // send frequency
        commandOdin->sendFrequency();
        strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
        emit commandSent(lastSentCommand);
    });
    delay += delayInterval;
//    QTimer::singleShot((STARTDELAY+delay), [=] {  // send step size utilzing sending step size increase
//        commandOdin->sendStepSizeIncrease();
//        strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
//        emit commandSent(lastSentCommand);
//    });
//    delay += delayInterval;
//        QTimer::singleShot((STARTDELAY+360), [=] {
//            commandOdin->sendThresholdEnable();
//        });
//    QTimer::singleShot((STARTDELAY+400), [=] {
//        commandOdin->sendChannelEnable();
//    });
//    QTimer::singleShot((STARTDELAY+440), [=] {
//        commandOdin->sendThresholdUpper();
//        strcpy(rpiCommand, commandOdin->getlastRpiCommand().data());
//        emit commandSent(rpiCommand);
//    });
//    QTimer::singleShot((STARTDELAY+480), [=] {
//        commandOdin->sendThresholdLower();
//        strcpy(rpiCommand, commandOdin->getlastRpiCommand().data());
//        emit commandSent(rpiCommand);
//    });
//    QTimer::singleShot((STARTDELAY+520), [=] {
//        commandOdin->sendDebounceDelay();
//        strcpy(rpiCommand, commandOdin->getlastRpiCommand().data());
//        emit commandSent(rpiCommand);
//    });
//    for(int i = 0; i < 4; i++){
//        QTimer::singleShot((STARTDELAY+440+i*40), [=] {
//            commandOdin->sendThreshold(i);
//            strcpy(lastSentCommand, commandOdin->getlastRpiCommand().data());
//            emit commandSent(lastSentCommand);
//        });
//    }
//    for(int i = 0; i < 4; i++){
//        QTimer::singleShot((STARTDELAY+600+i*40), [=] {
//            commandOdin->sendThresholdPower(i);
//            strcpy(lastSentCommand, commandOdin->getlastRpiCommand().data());
//            emit commandSent(lastSentCommand);
//        });
//    }
    QTimer::singleShot((STARTDELAY+delay), [=] {
        QMessageBox::information(this, "Done!", "Stimulator parameters have been sent...");
        mainWidget->setEnabled(true);
    });
    delay += delayInterval;
    sendParameterButton->setEnabled(false);
    QTimer::singleShot((STARTDELAY+delay), [=] {
        sendParameterButton->setEnabled(true);
    });
    delay += delayInterval;
}

void OdinWindow::sendCommand(){
    sendButton->setEnabled(false);
    QTimer::singleShot((STARTDELAY+1000), [=] {
        start = !start;
        sendButton->setEnabled(true);
    });
    if(!start){
        int delay = 40;
        commandOdin->sendStart();
        strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
        emit commandSent(lastSentCommand);
        for(int i = 0; i < 4; i++){  // send pulse duration
            QTimer::singleShot((STARTDELAY+delay+i*40), [=] {
                    commandOdin->sendPulseDuration(i);
                    strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
                    emit commandSent(lastSentCommand);
            });
        }
        delay += 4*40;
        for(int i = 0; i < 4; i++){  // send amplitudes
            QTimer::singleShot((STARTDELAY+delay+i*60), [=] {
                commandOdin->setAmplitude(i, amplitudeSpinBox[i]->text().toDouble());
                qDebug() << "Set channel " << i << "amplitude to : " << amplitudeSpinBox[i]->text().toDouble();

                commandOdin->sendAmplitude(i);
                strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
                emit commandSent(lastSentCommand);
            });
        }
        delay += 4*60;
//        QTimer::singleShot((STARTDELAY+360), [=] {
//            commandOdin->sendThresholdEnable();
//        });

        QTimer::singleShot((STARTDELAY+delay), [=] {  // send frequency
            commandOdin->sendFrequency();
            strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
            emit commandSent(lastSentCommand);
        });
        delay += delayInterval;
        QTimer::singleShot((STARTDELAY+delay), [=] {  // send channel enable
            for(int i = 0; i < 4; i++){
                commandOdin->setChannelEnabled(i, channelEnable[i]->isChecked());
                qDebug() << "Setting Channel Enable for channel " << i+1 << channelEnable[i]->isChecked();
            }
            commandOdin->sendChannelEnable();
            strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
            emit commandSent(lastSentCommand);

        });
        delay += delayInterval;
        QTimer::singleShot((STARTDELAY+delay), [=] {  // send start flag
            commandOdin->sendStartFlag();
            strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
            emit commandSent(lastSentCommand);
        });
        delay += delayInterval;
        sendButton->setText("Stop Odin!");
        QTimer::singleShot((STARTDELAY+delay), [=] {
            if(delayEnabledCheckBox->isChecked() && start){
                loopingThread->delay = delaySpinBox->value()*1000;
                loopingThread->start();
            }
        });
    }
    else{
        pauseOdin();
        QTimer::singleShot((500), [=] {
            commandOdin->sendStop();
            count = 0;
            strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
            emit commandSent(lastSentCommand);
            sendButton->setText("Start Odin!");
            delayEnabledCheckBox->setChecked(false);

            for(int i = 0; i < 4; i++){  // send amplitudes
                QTimer::singleShot((550+i*delayInterval), [=] {
                    commandOdin->setAmplitude(i, amplitudeSpinBox[i]->text().toDouble());
                    qDebug() << "Set channel " << i << "amplitude to : " << amplitudeSpinBox[i]->text().toDouble();

                    commandOdin->sendAmplitude(i);
                    strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
                    emit commandSent(lastSentCommand);
                });
            }
        });
    }
}

void OdinWindow::pauseOdin(){
    for(int i = 0; i < 4; i++){
        commandOdin->setChannelEnabled(i, false);
        qDebug() << "Setting Channel Enable for channel " << i+1 << false;
//        thresholdEnable[i]->setChecked(false);
//        channelEnable[i]->setChecked(false);
    }
    commandOdin->sendChannelEnable();
    strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
    emit commandSent(lastSentCommand);

    for(int i = 0; i < 4; i++){
        QTimer::singleShot((160+i*delayInterval), [=] {
//            amplitudeSpinBox[i]->setValue(0.0);
            commandOdin->setAmplitude(i, 0.0);
            commandOdin->sendAmplitude(i);
            strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
            emit commandSent(lastSentCommand);
        });
    }
}

void OdinWindow::on_delayEnabled_toggled(){
    delayFlag = delayEnabledCheckBox->isChecked();
    if(delayEnabledCheckBox->isChecked() && start){
        loopingThread->delay = delaySpinBox->value()*1000 + 1300;
        loopingThread->start();
    }
}

void OdinWindow::on_currentMode_clicked(){
    highcurrent = !highcurrent;
    if(highcurrent){
        for(int i = 0; i < 4; i++){
            amplitudeSpinBox[i]->setMinimum(0.0);
            amplitudeSpinBox[i]->setMaximum(19.0);
            amplitudeSpinBox[i]->setSingleStep(1.0);
            amplitudeSpinBox[i]->setValue(0.0);
        }
        modeButton->setText("H");
    }
    else{
        for(int i = 0; i < 4; i++){
            amplitudeSpinBox[i]->setMinimum(0.0);
            amplitudeSpinBox[i]->setMaximum(400.0);
            amplitudeSpinBox[i]->setSingleStep(100.0);
            amplitudeSpinBox[i]->setValue(0.0);
        }
        modeButton->setText("L");
    }
}

void OdinWindow::on_thresholdEnable_toggled(){
    char temp;
    temp = (char) thresholdEnable[3]->isChecked() << 3 |
            (char) thresholdEnable[2]->isChecked() << 2 |
            (char) thresholdEnable[1]->isChecked() << 1 |
            (char) thresholdEnable[0]->isChecked();
    commandOdin->setThresholdEnable(temp);
    commandOdin->sendThresholdEnable();
}

void OdinWindow::on_channelEnable_toggled(){
    for(int i = 0; i < 4; i++){
        commandOdin->setChannelEnabled(i, channelEnable[i]->isChecked());
        qDebug() << "Setting Channel Enable for channel " << i+1 << channelEnable[i]->isChecked();
    }
    commandOdin->sendChannelEnable();
    strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
    emit commandSent(lastSentCommand);
}

void OdinWindow::on_channelSelectAll_clicked(){
    for(int i = 0; i < 4; i ++){
        QTimer::singleShot((i*delayInterval), [=] {
            channelEnable[i]->setChecked(true);
            commandOdin->setChannelEnabled(i, true);
        });
    }
//    commandOdin->sendChannelEnable();
//    strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
//    emit commandSent(lastSentCommand);
}

void OdinWindow::on_channelSelectNone_clicked(){
    for(int i = 0; i < 4; i ++){
        QTimer::singleShot((i*delayInterval), [=] {
            channelEnable[i]->setChecked(false);
            commandOdin->setChannelEnabled(i, false);
        });
    }
//    commandOdin->sendChannelEnable();
//    strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
//    emit commandSent(lastSentCommand);
}

void OdinWindow::on_thresholdSelectAll_clicked(){
    for(int i = 0; i < 4; i ++){
        thresholdEnable[i]->setChecked(true);
        commandOdin->setThresholdEnable((char) 15);
    }
    commandOdin->sendThresholdEnable();
    thresholdIncreaseEnable = true;
    thresholdDecreaseEnable = true;
}

void OdinWindow::on_thresholdSelectNone_clicked(){
    for(int i = 0; i < 4; i ++){
        thresholdEnable[i]->setChecked(false);
        commandOdin->setThresholdEnable((char) 0);
    }
    commandOdin->sendThresholdEnable();
    thresholdIncreaseEnable = false;
    thresholdDecreaseEnable = false;
}

void OdinWindow::on_amplitude_Changed(){
    for(int i = 0; i < 4; i++){
        if(amplitudeSpinBox[i]->text().toDouble() !=  commandOdin->getAmplitude(i)){
            commandOdin->setAmplitude(i, amplitudeSpinBox[i]->text().toDouble());
            qDebug() << "Set channel " << i << "amplitude to : " << amplitudeSpinBox[i]->text().toDouble();
//            if(start){
                on_delayEnabled_toggled();
                commandOdin->sendAmplitude(i);
                strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
                emit commandSent(lastSentCommand);
//            }
        }
    }
}

void OdinWindow::on_pulseDuration_Changed(){
    for(int i = 0; i < 4; i++){
        if(pulseDurationSpinBox[i]->text().toInt() !=  commandOdin->getPulseDuration(i)){
            commandOdin->setPulseDuration(i, pulseDurationSpinBox[i]->text().toInt());
            qDebug() << "Set channel " << i << "pulse duration to : " << pulseDurationSpinBox[i]->text().toInt();
//            if(start){
                commandOdin->sendPulseDuration(i);
                strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
                emit commandSent(lastSentCommand);
                QTimer::singleShot(40, [=] {
                        commandOdin->sendFrequency();
                        strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
                        emit commandSent(lastSentCommand);
                });
//            }
        }
    }
}

void OdinWindow::on_frequency_Changed(){
    if(frequencySpinBox->text().toInt() != commandOdin->getFrequency()){
        commandOdin->setFrequency(frequencySpinBox->text().toInt());
        qDebug() << "Set frequency to : " << frequencySpinBox->text().toInt();
//        if(start){
            commandOdin->sendFrequency();
            strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
            emit commandSent(lastSentCommand);
//        }
    }
}

void OdinWindow::on_debounce_editted(){
    commandOdin->setDebounceDelay(debounceSpinBox->text().toInt());
    emit debounceEditted(debounceSpinBox->text().toInt());
}

void OdinWindow::on_upperThreshold_editted(){
    commandOdin->setThresholdUpper(upperThresholdSpinBox->text().toDouble());
    emit upperThresholdEditted(upperThresholdSpinBox->text().toDouble());
}

void OdinWindow::on_lowerThreshold_editted(){
    commandOdin->setThresholdLower(lowerThresholdSpinBox->text().toDouble());
    emit lowerThresholdEditted(lowerThresholdSpinBox->text().toDouble());
}

void OdinWindow::on_stepSize_editted(){
    commandOdin->setStepSize(stepSizeSpinBox->text().toDouble());
}

void OdinWindow::on_channelAThreshold_crossed(){
//    if(thresholdDecreaseEnable && start){
//        commandOdin->sendStepSizeDecrease();
//        strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
//        emit commandSent(lastSentCommand);
//    }
//    for(int i = 0; i < 4; i++){
//        if(thresholdEnable[i]->isChecked() && start){
//                if((amplitudeSpinBox[i]->text().toDouble() - stepSizeSpinBox->text().toDouble()) >= 0.0){
//                    amplitudeSpinBox[i]->setValue(amplitudeSpinBox[i]->text().toDouble() - stepSizeSpinBox->text().toDouble());
//                    commandOdin->setAmplitude(i, amplitudeSpinBox[i]->text().toDouble());
//                    thresholdIncreaseEnable = true;
//                    thresholdDecreaseEnable = true;
//                }
//                else{
//                    amplitudeSpinBox[i]->setValue(0.0);
//                    commandOdin->setAmplitude(i, 0.0);
//                    commandOdin->setCurrentAmplitude((unsigned char) 0);
//                    thresholdDecreaseEnable = false;
//                    thresholdIncreaseEnable = true;
//                }
//                lastSentAmplitude[i] = amplitudeSpinBox[i]->text().toDouble();
//        }
//    }
//    emit(amplitudeChanged(lastSentAmplitude));
    commandOdin->setChannelEnabled(0, true);
    commandOdin->setChannelEnabled(1, true);
    channelEnable[0]->setChecked(true);
    channelEnable[1]->setChecked(true);
    commandOdin->sendChannelEnable();
    QTimer::singleShot(1000, [=] {
        commandOdin->setChannelEnabled(0, false);
        commandOdin->setChannelEnabled(1, false);
        channelEnable[0]->setChecked(false);
        channelEnable[1]->setChecked(false);
        commandOdin->sendChannelEnable();
    });
}

void OdinWindow::on_channelBThreshold_crossed(){
//    if(thresholdIncreaseEnable && start){
//        commandOdin->sendStepSizeIncrease();
//        strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
//        emit commandSent(lastSentCommand);
//    }
//    for(int i = 0; i < 4; i++){
//        if(thresholdEnable[i]->isChecked() && start){
//                if((amplitudeSpinBox[i]->text().toDouble() + stepSizeSpinBox->text().toDouble()) <= 19.0){
//                    amplitudeSpinBox[i]->setValue(amplitudeSpinBox[i]->text().toDouble() + stepSizeSpinBox->text().toDouble());
//                    commandOdin->setAmplitude(i, amplitudeSpinBox[i]->text().toDouble());
//                    thresholdIncreaseEnable = true;
//                    thresholdDecreaseEnable = true;
//                }
//                else{
//                    amplitudeSpinBox[i]->setValue(19.0);
//                    commandOdin->setAmplitude(i, 19.0);
//                    commandOdin->setCurrentAmplitude((unsigned char) 240);
//                    thresholdIncreaseEnable = false;
//                    thresholdDecreaseEnable = true;
//                }
//                lastSentAmplitude[i] = amplitudeSpinBox[i]->text().toDouble();
//        }
//        lastSentAmplitude[i] = amplitudeSpinBox[i]->text().toDouble();
//    }
//    emit(amplitudeChanged(lastSentAmplitude));
    commandOdin->setChannelEnabled(2, true);
    commandOdin->setChannelEnabled(3, true);
    commandOdin->sendChannelEnable();
    channelEnable[2]->setChecked(true);
    channelEnable[3]->setChecked(true);
    QTimer::singleShot(1000, [=] {
        commandOdin->setChannelEnabled(2, false);
        commandOdin->setChannelEnabled(3, false);
        channelEnable[2]->setChecked(false);
        channelEnable[3]->setChecked(false);
        commandOdin->sendChannelEnable();
    });
}

void OdinWindow::on_odinDisconnected(){
    QMessageBox::warning(this, tr("Odin Disconnected!"), tr("Please restart the program after reconnecting to Odin"));
}

OdinWindow::~OdinWindow(){
    socketOdin->sendDisconnectSignal();
    socketOdin->doDisconnect();
}

void OdinWindow::acceptConnection(){
    qDebug() << "Accepted new connection";
    tcpServerConnection = tcpServer->nextPendingConnection();
    connect(tcpServerConnection, SIGNAL(readyRead()), this, SLOT(increaseCurrent()));
}

void OdinWindow::increaseCurrent(){
    QByteArray temp;
    temp = tcpServerConnection->readAll();
    delayEnabledCheckBox->setChecked(true);
    amplitudeSpinBox[0]->setValue(temp.toDouble());
    //Needa debug if should be sendCommand(); here or not.
    on_amplitude_Changed();
    tcpServerConnection->close();
}

void OdinWindow::on_write_settings_changed(){
    QSettings settings(filenameSettings, QSettings::IniFormat);

    settings.setValue("channelEnable", commandOdin->getChannelEnabled());  //channel enable
    settings.beginWriteArray("parameters");  //amplitudes & pulse duation
    for(int i = 0; i < 4; i++){
        settings.setArrayIndex(i);
        settings.setValue("amplitude", commandOdin->getAmplitude(i));
        settings.setValue("pulseDuration", commandOdin->getPulseDuration(i));
    }
    settings.endArray();
    settings.setValue("frequency", commandOdin->getFrequency());  //frequency
    settings.setValue("delayFlag", delayFlag);  //delay
    settings.setValue("delayValue", delayValue);
    if(filenameSettings.contains(filenameMostRecent)){
        qDebug() << "Writing most recent settings filename......";
        settings.setValue("filenameSettingsAll", filenameSettingsAll);
    }

    statusBarLabel->setText(tr("Save settings as: ") + filenameSettings);
    qDebug() << "all saved keys: " << settings.allKeys();
    qDebug() << "local filename now: " << filenameSettings;
    qDebug() << "configuratoin filename now: " << configurationFile->getFilenameSettings();
    updateFilenameSettingsAll();
}

void OdinWindow::readSettings(){
    QSettings settings(filenameSettings, QSettings::IniFormat);

    for(int i = 0; i < 4; i++){
        commandOdin->setChannelEnabled(i, settings.value("channelEnable").toInt() >> i);  //channel enable
    }
    settings.beginReadArray("parameters");  //amplitude & pulse duration
    for(int i = 0; i < 4; i++){
        settings.setArrayIndex(i);
        commandOdin->setAmplitude(i, settings.value("amplitude").toDouble());
        commandOdin->setPulseDuration(i, settings.value("pulseDuration").toInt());
    }
    settings.endArray();
    commandOdin->setFrequency(settings.value("frequency").toInt());  //frequency
    delayFlag = settings.value("delayFlag").toBool();  //delay
    delayValue = settings.value("delayValue").toInt();
    if(firstLoadingFlag){
        qDebug() << "Reading most recent settings filename......";
        filenameSettingsAll = settings.value("filenameSettingsAll").toStringList();
        indexRecentFilenameSettings = filenameSettingsAll.size();
        qDebug() << "Loaded filenameSettingsAll: " << filenameSettingsAll << "\nSize: " << indexRecentFilenameSettings;
        updateOpenSettingsRecent();
        mainWidget->setEnabled(false);
        updateFilenameSettingsAll();
    }
    else{
        createLayout();
        mainWidget->setEnabled(false);
        updateFilenameSettingsAll();
        sendParameter();

    }

//    statusBarLabel->setText(tr("Read settings: ") + configurationFile->getFilenameSettings());
    qDebug() << "local filename now: " << filenameSettings;
    qDebug() << "configuratoin filename now: " << configurationFile->getFilenameSettings();

}

void OdinWindow::updateFilenameSettingsAll(){
    if(!filenameSettings.contains(filenameMostRecent)){  //exclude the most recent filename
        if(!filenameSettingsAll.contains(filenameSettings)){  //check if it's contained in the recent filename list
            filenameSettingsAll.prepend(filenameSettings);  //add into filenameSettingsAll
            indexRecentFilenameSettings ++;
        }
        else{  //move that
            int index = filenameSettingsAll.indexOf(QRegExp("*"+filenameSettings+"*", Qt::CaseInsensitive, QRegExp::Wildcard));
            filenameSettingsAll.move(index, 0);
        }
        updateOpenSettingsRecent();
    }
}

void OdinWindow::updateOpenSettingsRecent(){
    qDebug() << "no. settings: " << indexRecentFilenameSettings << "\nno. actions: " << indexRecentFilenameAction;
    qDebug() << "filenameSettingsAll: " << filenameSettingsAll;
    if(indexRecentFilenameSettings >= 10){
        filenameSettingsAll.removeLast();
        indexRecentFilenameSettings --;
    }
    int diff = indexRecentFilenameSettings - indexRecentFilenameAction;
    if(diff > 0){
        for(int i = 0; i < diff; i++){
            openSettingsRecentAction->addAction(recentFilenameSettings[indexRecentFilenameAction + i]);  //add into list
        }
    }
    else if(diff < 0){
        for(int i = 0; i < (-diff); i++){
            openSettingsRecentAction->removeAction(recentFilenameSettings[indexRecentFilenameAction - i - 1]);  //remove from list
        }
    }
    indexRecentFilenameAction = indexRecentFilenameSettings;

    qDebug() << "no. settings: " << indexRecentFilenameSettings << "\nno. actions: " << indexRecentFilenameAction;

    for(int i = 0; i < indexRecentFilenameSettings; i++){
        recentFilenameSettings[i]->setText(filenameSettingsAll[i]);
    }
}

void OdinWindow::writeSettings(){
    filenameSettings = configurationFile->getFilenameSettings();
    on_write_settings_changed();
}

void OdinWindow::on_read_settings_changed(){
    if(!(firstLoadingFlag && filename.contains(filenameMostRecent))){
        filenameSettings = configurationFile->getFilenameSettings();
        qDebug() << "reading settings that is not the most recent settings...";
        readSettings();
    }
}

void OdinWindow::on_read_settings_selected_changed(int index){
    filenameSettingsTemp = filenameSettingsAll[index];
    QFile file(filenameSettingsTemp);
    qDebug() << "status of the file is: " << file.exists();
    if(file.exists()){
        filenameSettings = filenameSettingsAll[index];
        readSettings();
    }
    else{
        qDebug() << "file doesn't exist...!!!";
        indexTemp = index;
        QMessageBox removeFilenameBox;
        removeFilenameBox.setText(filenameSettingsTemp + " does not exist...\nRemove from list?");
        removeFilenameBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        removeFilenameBox.setDefaultButton(QMessageBox::Cancel);
        int ret = removeFilenameBox.exec();

        switch(ret){
            case QMessageBox::Ok:
                filenameSettingsAll.removeAt(indexTemp);  //remove filename in filenameSettingsAll
                indexRecentFilenameSettings --;
                updateOpenSettingsRecent();
                break;
            case QMessageBox::Cancel:
                break;
            default:
                break;
        }
    }
}

void OdinWindow::closeEvent(QCloseEvent *event){
    configurationFile->writeMostRecentSettings();
    event->accept();
}

void OdinWindow::on_sylphx_triggered(){
    emit showSylphXSignal();
}

void OdinWindow::on_cat_triggered(){
    emit showCatSignal();
}
}
