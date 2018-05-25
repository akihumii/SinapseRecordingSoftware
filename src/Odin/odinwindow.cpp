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

    createLayout();
    createStatusBar();
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

void OdinWindow::createLayout(){
    stimParameters = new QGroupBox(tr("Stimulator Parameters"));
    paraLabels[0] = new QLabel(tr(" "));
    paraLabels[1] = new QLabel(tr("Channel Enabled: "));
    paraLabels[2] = new QLabel(tr("Threshold Enable: "));
    paraLabels[3] = new QLabel(tr("Amplitude(mA): "));
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
        stimParaLayout[1]->addWidget(channelEnable[i]);
        connect(channelEnable[i], SIGNAL(toggled(bool)), this, SLOT(on_channelEnable_toggled()));

        thresholdEnable[i] = new QCheckBox;
        stimParaLayout[2]->addWidget(thresholdEnable[i]);
        connect(thresholdEnable[i], SIGNAL(toggled(bool)), this, SLOT(on_thresholdEnable_toggled()));

        amplitudeSpinBox[i] = new QDoubleSpinBox;
        amplitudeSpinBox[i]->setMinimum(0.0);
        amplitudeSpinBox[i]->setMaximum(19.0);
        amplitudeSpinBox[i]->setSingleStep(0.1);
        amplitudeSpinBox[i]->setValue(0.0);
        stimParaLayout[3]->addWidget(amplitudeSpinBox[i]);
        connect(amplitudeSpinBox[i], SIGNAL(editingFinished()), this, SLOT(on_amplitude_Changed()));

        pulseDurationSpinBox[i] = new QSpinBox;
        pulseDurationSpinBox[i]->setMinimum(20);
        pulseDurationSpinBox[i]->setMaximum(1260);
        pulseDurationSpinBox[i]->setValue(200);
        stimParaLayout[4]->addWidget(pulseDurationSpinBox[i]);
        connect(pulseDurationSpinBox[i], SIGNAL(editingFinished()), this, SLOT(on_pulseDuration_Changed()));
    }

    frequencySpinBox = new QSpinBox;
    frequencySpinBox->setMinimumWidth(300);
    frequencySpinBox->setMinimum(17);
    frequencySpinBox->setMaximum(250);
    frequencySpinBox->setValue(50);
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
    buttonThresholdLayout->addWidget(thresholdSelectAll);
    buttonThresholdLayout->addWidget(thresholdSelectNone);
    connect(thresholdSelectAll, SIGNAL(clicked(bool)), this, SLOT(on_thresholdSelectAll_clicked()));
    connect(thresholdSelectNone, SIGNAL(clicked(bool)), this, SLOT(on_thresholdSelectNone_clicked()));

    stimParaMainLayout->addLayout(buttonChannelLayout);
    stimParaMainLayout->addLayout(buttonThresholdLayout);

    stimParameters->setLayout(stimParaMainLayout);

    delayParameters = new QGroupBox(tr("Delay Parameters"));
    delayEnabledCheckBox = new QCheckBox;
    connect(delayEnabledCheckBox, SIGNAL(toggled(bool)), this, SLOT(on_delayEnabled_toggled()));
    QLabel *delayLabel = new QLabel(tr("Turn off after (seconds) : "));
    delaySpinBox = new QSpinBox;
    delaySpinBox->setValue(0);
    delaySpinBox->setMinimumWidth(220);
    delaySpinBox->setMaximum(60000);

    QHBoxLayout *delayLayout = new QHBoxLayout;
    delayLayout->addWidget(delayEnabledCheckBox);
    delayLayout->addWidget(delayLabel);
    delayLayout->addWidget(delaySpinBox);

    delayParameters->setLayout(delayLayout);

    thresholdParameters = new QGroupBox(tr("Threshold Parameters"));
    thresholdLabels[0] = new QLabel(tr("Step Size (mA):"));
    thresholdLabels[1] = new QLabel(tr("Upper Threshold (V):"));
    thresholdLabels[2] = new QLabel(tr("Lower Threshold (V):"));
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
    upperThresholdSpinBox->setMaximum(2.0);
    upperThresholdSpinBox->setSingleStep(0.1);
    upperThresholdSpinBox->setValue(2.0);
    connect(upperThresholdSpinBox, SIGNAL(editingFinished()), this, SLOT(on_upperThreshold_editted()));
    thresholdSpinBoxLayout->addWidget(upperThresholdSpinBox);

    lowerThresholdSpinBox = new QDoubleSpinBox;
    lowerThresholdSpinBox->setMinimum(0.0);
    lowerThresholdSpinBox->setMaximum(2.0);
    lowerThresholdSpinBox->setSingleStep(0.1);
    lowerThresholdSpinBox->setValue(0.0);
    connect(lowerThresholdSpinBox, SIGNAL(editingFinished()), this, SLOT(on_lowerThreshold_editted()));
    thresholdSpinBoxLayout->addWidget(lowerThresholdSpinBox);

    debounceSpinBox = new QSpinBox;
    debounceSpinBox->setMinimum(0);
    debounceSpinBox->setMaximum(2000);
    debounceSpinBox->setSingleStep(100);
    debounceSpinBox->setValue(40);
    connect(debounceSpinBox, SIGNAL(editingFinished()), this, SLOT(on_debounce_editted()));
    thresholdSpinBoxLayout->addWidget(debounceSpinBox);

    QHBoxLayout *thresholdMainLayout = new QHBoxLayout;
    thresholdMainLayout->addLayout(thresholdLabelLayout);
    thresholdMainLayout->addLayout(thresholdSpinBoxLayout);
    thresholdParameters->setLayout(thresholdMainLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    sendButton = new QPushButton(tr("Start Odin!"));
    connect(sendButton, SIGNAL(clicked(bool)), this, SLOT(sendCommand()));
    recordButton = new QPushButton;
    recordButton->setShortcut(QKeySequence(tr("Ctrl+T")));
    connect(recordButton, SIGNAL(clicked(bool)), this, SLOT(on_record_clicked()));
    recordButton->setMaximumWidth(20);
    recordButton->setIcon(QIcon(QDir::currentPath()+"/recordStart.png"));
    recordButton->setIconSize(QSize(15,15));
    buttonLayout->addWidget(sendButton);
    buttonLayout->addWidget(recordButton);

    QWidget *mainWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(stimParameters);
    mainLayout->addWidget(delayParameters);
    mainLayout->addWidget(thresholdParameters);
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
//        serialOdin->connectOdin();
        connectionStatus.clear();
        if(serialOdin->isOdinSerialConnected()){
            connectionStatus.append("Connected to Odin at " + serialOdin->getConnectedPort());
            sendButton->setEnabled(true);
            statusBarLabel->setText(connectionStatus);
            return true;
        }
        else{
//            connectionStatus.append("Connection to Odin failed");
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
            connectionStatus.append("Connection to Odin failed! Restart this program after connecting Odin.");
            QMessageBox::information(this, "Failed to connect!", "No Odin device detected.. \n"
                                                                 "Check your connections and run the program again..");
            statusBarLabel->setText(connectionStatus);
            return false;
        }
    }
    return false;
}

void OdinWindow::sendCommand(){
    QTimer::singleShot((1240), [=] {
        start = !start;
    });
    if(!start){
        commandOdin->sendStart();
        strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
        emit commandSent(lastSentCommand);
        QTimer::singleShot((800), [=] {
            commandOdin->sendFrequency();
            strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
            emit commandSent(lastSentCommand);
        });
        for(int i = 0; i < 4; i++){
//            QTimer::singleShot((1000+i*200), [=] {
            QTimer::singleShot((840+i*40), [=] {
                    commandOdin->sendPulseDuration(i);
                    strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
                    emit commandSent(lastSentCommand);
            });
        }
        for(int i = 0; i < 4; i++){
//            QTimer::singleShot((1800+i*200), [=] {
            QTimer::singleShot((1000+i*40), [=] {
                commandOdin->sendAmplitude(i);
                strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
                emit commandSent(lastSentCommand);
            });
        }
        QTimer::singleShot((1160), [=] {
            commandOdin->sendThresholdEnable();
        });
        QTimer::singleShot((1200), [=] {
            commandOdin->sendChannelEnable();
        });
        sendButton->setText("Stop Odin!");
        if(delayEnabledCheckBox->isChecked() && start){
            loopingThread->delay = delaySpinBox->value()*1000 + 1800;
            loopingThread->start();
        }
    }
    else{
        pauseOdin();
        QTimer::singleShot((1000), [=] {
            commandOdin->sendStop();
            count = 0;
            strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
            emit commandSent(lastSentCommand);
            sendButton->setText("Start Odin!");
            delayEnabledCheckBox->setChecked(false);
        });
    }
}

void OdinWindow::pauseOdin(){
    for(int i = 0; i < 4; i++){
        amplitudeSpinBox[i]->setValue(0.0);
        thresholdEnable[i]->setChecked(false);
        channelEnable[i]->setChecked(false);
        commandOdin->setAmplitude(i, 0.0);
        QTimer::singleShot((i*40), [=] {
            commandOdin->sendAmplitude(i);
            strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
            emit commandSent(lastSentCommand);
        });
    }
}

void OdinWindow::on_delayEnabled_toggled(){
    if(delayEnabledCheckBox->isChecked() && start){
        loopingThread->delay = delaySpinBox->value()*1000;
        loopingThread->start();
    }
}

void OdinWindow::on_record_clicked(){
    record = !record;
    if(record){
        recordButton->setIcon(QIcon(QDir::currentPath()+"/recordStop.png"));
    }
    else{
        recordButton->setIcon(QIcon(QDir::currentPath()+"/recordStart.png"));
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
}

void OdinWindow::on_channelSelectAll_clicked(){
    for(int i = 0; i < 4; i ++){
        channelEnable[i]->setChecked(true);
        commandOdin->setChannelEnabled(i, true);
    }
    commandOdin->sendChannelEnable();
}

void OdinWindow::on_channelSelectNone_clicked(){
    for(int i = 0; i < 4; i ++){
        channelEnable[i]->setChecked(false);
        commandOdin->setChannelEnabled(i, false);
    }
    commandOdin->sendChannelEnable();
}

void OdinWindow::on_thresholdSelectAll_clicked(){
    for(int i = 0; i < 4; i ++){
        thresholdEnable[i]->setChecked(true);
        commandOdin->setThresholdEnable((char) 15);
    }
    commandOdin->sendThresholdEnable();
}

void OdinWindow::on_thresholdSelectNone_clicked(){
    for(int i = 0; i < 4; i ++){
        thresholdEnable[i]->setChecked(false);
        commandOdin->setThresholdEnable((char) 0);
    }
    commandOdin->sendThresholdEnable();
}

void OdinWindow::on_amplitude_Changed(){
    for(int i = 0; i < 4; i++){
        if(amplitudeSpinBox[i]->text().toDouble() !=  commandOdin->getAmplitude(i)){
//            commandOdin->setChannelEnabled(i, (amplitudeSpinBox[i]->text().toDouble() != 0.0));
            commandOdin->setAmplitude(i, amplitudeSpinBox[i]->text().toDouble());
            qDebug() << "Set channel " << i << "amplitude to : " << amplitudeSpinBox[i]->text().toDouble();
            if(start){
                on_delayEnabled_toggled();
                commandOdin->sendAmplitude(i);
                strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
                emit commandSent(lastSentCommand);
            }
        }
    }
}

void OdinWindow::on_pulseDuration_Changed(){
    for(int i = 0; i < 4; i++){
        if(pulseDurationSpinBox[i]->text().toInt() !=  commandOdin->getPulseDuration(i)){
            commandOdin->setPulseDuration(i, pulseDurationSpinBox[i]->text().toInt());
            qDebug() << "Set channel " << i << "pulse duration to : " << pulseDurationSpinBox[i]->text().toInt();
            if(start){
                commandOdin->sendPulseDuration(i);
                strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
                emit commandSent(lastSentCommand);
                QTimer::singleShot(40, [=] {
                        commandOdin->sendFrequency();
                        strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
                        emit commandSent(lastSentCommand);
                });
            }
        }
    }
}

void OdinWindow::on_frequency_Changed(){
    if(frequencySpinBox->text().toInt() != commandOdin->getFrequency()){
        commandOdin->setFrequency(frequencySpinBox->text().toInt());
        qDebug() << "Set frequency to : " << frequencySpinBox->text().toInt();
        if(start){
            commandOdin->sendFrequency();
            strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
            emit commandSent(lastSentCommand);
        }
    }
}

void OdinWindow::on_debounce_editted(){
    emit debounceEditted(debounceSpinBox->text().toInt());
}

void OdinWindow::on_upperThreshold_editted(){
    emit upperThresholdEditted(upperThresholdSpinBox->text().toDouble());
}

void OdinWindow::on_lowerThreshold_editted(){
    emit lowerThresholdEditted(lowerThresholdSpinBox->text().toDouble());
}

void OdinWindow::on_stepSize_editted(){
    commandOdin->setStepSize(stepSizeSpinBox->text().toDouble());
}

void OdinWindow::on_upperThreshold_crossed(){
    for(int i = 0; i < 4; i++){
        if(thresholdEnable[i]->isChecked() && start){
//            QTimer::singleShot(i*100, [=] {
                if((amplitudeSpinBox[i]->text().toDouble() - stepSizeSpinBox->text().toDouble()) >= 0.0){
//                    qDebug() << "Decreasing amplitude for channel " << i << "to the value of : " << amplitudeSpinBox[i]->text().toDouble() - stepSizeSpinBox->text().toDouble() << " from " << amplitudeSpinBox[i]->text().toDouble();
                    amplitudeSpinBox[i]->setValue(amplitudeSpinBox[i]->text().toDouble() - stepSizeSpinBox->text().toDouble());
                    commandOdin->setAmplitude(i, amplitudeSpinBox[i]->text().toDouble());
//                    on_amplitude_Changed();
                }
                else{
                    amplitudeSpinBox[i]->setValue(0.0);
                    commandOdin->setAmplitude(i, 0.0);
                }
//            });
        }
    }
//    if(amplitudeSpinBox[0]->text().toDouble() > 0){
    if(count > 0 && start){
        commandOdin->sendStepSizeDecrease();
        count--;
    }
//    }
    if(numChannelsEnabled != commandOdin->getNumChannelEnabled()){
        QTimer::singleShot(200, [=] {
                commandOdin->sendFrequency();
                strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
                emit commandSent(lastSentCommand);
        });
        numChannelsEnabled = commandOdin->getNumChannelEnabled();
    }
}

void OdinWindow::on_lowerThreshold_crossed(){
    for(int i = 0; i < 4; i++){
        if(thresholdEnable[i]->isChecked() && start){
//            QTimer::singleShot(i*100, [=] {
                if((amplitudeSpinBox[i]->text().toDouble() + stepSizeSpinBox->text().toDouble()) <= 19.0){
//                    qDebug() << "Increasing amplitude for channel " << i << "to the value of : " << amplitudeSpinBox[i]->text().toDouble() + stepSizeSpinBox->text().toDouble() << " from " << amplitudeSpinBox[i]->text().toDouble();
                    amplitudeSpinBox[i]->setValue(amplitudeSpinBox[i]->text().toDouble() + stepSizeSpinBox->text().toDouble());
                    commandOdin->setAmplitude(i, amplitudeSpinBox[i]->text().toDouble());
//                    on_amplitude_Changed();
                }
                else{
                    amplitudeSpinBox[i]->setValue(19.0);
                    commandOdin->setAmplitude(i, 19.0);
                }
//            });
        }
    }
//    if(amplitudeSpinBox[0]->text().toDouble() < 19.0){
    if(count < 100 && start){
        commandOdin->sendStepSizeIncrease();
        count++;
    }
    if(numChannelsEnabled != commandOdin->getNumChannelEnabled()){
        QTimer::singleShot(40, [=] {
                commandOdin->sendFrequency();
                strcpy(lastSentCommand, commandOdin->getlastSentCommand().data());
                emit commandSent(lastSentCommand);
        });
        numChannelsEnabled = commandOdin->getNumChannelEnabled();
    }
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

}
