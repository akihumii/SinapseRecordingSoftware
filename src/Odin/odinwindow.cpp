#include "odinwindow.h"

namespace Odin {

OdinWindow::OdinWindow(){
    QString version(APP_VERSION);
    setWindowTitle(tr("Odin Stimulator Software V") + version);
    serialOdin = new SerialOdin(this);
    socketOdin = new SocketOdin;
    commandOdin = new CommandOdin(serialOdin, socketOdin);
    connect(socketOdin, SIGNAL(odinDisconnected()), this, SLOT(on_odinDisconnected()));
    connect(serialOdin, SIGNAL(odinDisconnected()), this, SLOT(on_odinDisconnected()));

    createLayout();
    createStatusBar();
//    connectOdin();
}

void OdinWindow::createLayout(){
    stimParameters = new QGroupBox(tr("Stimulator Parameters"));
    paraLabels[0] = new QLabel(tr(" "));
    paraLabels[1] = new QLabel(tr("Threshold Enabled: "));
    paraLabels[2] = new QLabel(tr("Amplitude(mA): "));
    paraLabels[3] = new QLabel(tr("Pulse Duration(us): "));
    paraLabels[4] = new QLabel(tr("Frequency(Hz): "));

    for(int i = 0; i < 5; i++){
        stimParaLayout[i] = new QHBoxLayout;
        paraLabels[i]->setMinimumWidth(100);
        stimParaLayout[i]->addWidget(paraLabels[i]);
    }

    for(int i = 0; i < 4; i++){
        chnLabels[i] = new QLabel;
        chnLabels[i]->setText("Channel " + QString::number(i+1));
        stimParaLayout[0]->addWidget(chnLabels[i]);
    }

    for(int i = 0; i < 4; i++){
        thresholdEnable[i] = new QCheckBox;
        stimParaLayout[1]->addWidget(thresholdEnable[i]);
        connect(thresholdEnable[i], SIGNAL(toggled(bool)), this, SLOT(on_thresholdEnable_toggled()));
    }

    for(int i = 0; i < 4; i++){
        amplitudeSpinBox[i] = new QDoubleSpinBox;
        amplitudeSpinBox[i]->setMinimum(0.0);
        amplitudeSpinBox[i]->setMaximum(20.0);
        amplitudeSpinBox[i]->setSingleStep(0.1);
        amplitudeSpinBox[i]->setValue(0.0);
        stimParaLayout[2]->addWidget(amplitudeSpinBox[i]);
        connect(amplitudeSpinBox[i], SIGNAL(editingFinished()), this, SLOT(on_amplitude_Changed()));
    }

    for(int i = 0; i < 4; i++){
        pulseDurationSpinBox[i] = new QSpinBox;
        pulseDurationSpinBox[i]->setMinimum(20);
        pulseDurationSpinBox[i]->setMaximum(1260);
        pulseDurationSpinBox[i]->setValue(200);
        stimParaLayout[3]->addWidget(pulseDurationSpinBox[i]);
        connect(pulseDurationSpinBox[i], SIGNAL(editingFinished()), this, SLOT(on_pulseDuration_Changed()));
    }

    frequencySpinBox = new QSpinBox;
    frequencySpinBox->setMinimumWidth(300);
    frequencySpinBox->setMinimum(17);
    frequencySpinBox->setMaximum(250);
    frequencySpinBox->setValue(50);
    connect(frequencySpinBox, SIGNAL(editingFinished()), this, SLOT(on_frequency_Changed()));
    stimParaLayout[4]->addWidget(frequencySpinBox);

    QVBoxLayout *stimParaMainLayout = new QVBoxLayout;
    for(int i = 0; i < 5; i ++){
        stimParaMainLayout->addLayout(stimParaLayout[i]);
    }

    stimParameters->setLayout(stimParaMainLayout);

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
        int i = 0;
        do{
            i++;
            socketOdin->doConnect("192.168.4."+QString::number(i), 30000);
            qDebug() << i;
        } while(!socketOdin->isConnected() && i < 6);
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
    start = !start;
    if(start){
        commandOdin->initialiseCommand();
        sendButton->setText("Stop Odin!");
    }
    else{
        commandOdin->sendStop();
        sendButton->setText("Start Odin!");
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
    qDebug()<<"Testing threshold";
}

void OdinWindow::on_amplitude_Changed(){
    for(int i = 0; i < 4; i++){
        if(amplitudeSpinBox[i]->text().toDouble() !=  commandOdin->getAmplitude(i)){
            commandOdin->setChannelEnabled(i, (amplitudeSpinBox[i]->text().toDouble() == 0.0));
            commandOdin->setAmplitude(i, amplitudeSpinBox[i]->text().toDouble());
            qDebug() << "Set channel " << i << "amplitude to : " << amplitudeSpinBox[i]->text().toDouble();
            if(start){
                commandOdin->sendAmplitude(i);
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
        }
    }
}

void OdinWindow::on_odinDisconnected(){
    QMessageBox::warning(this, tr("Odin Disconnected!"), tr("Please restart the program after reconnecting to Odin"));
}

OdinWindow::~OdinWindow(){
    socketOdin->sendDisconnectSignal();
    socketOdin->doDisconnect();
}

}
