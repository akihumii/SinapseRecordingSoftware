#include "mainwindow.h"

MainWindow::MainWindow(){
    QString version(APP_VERSION);
    setWindowTitle(tr("NeuroModulation Software V") + version);
    command = new Command;
    serialShuHao = new SerialShuHao;
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    createLayout();
//    serialShuHao->connectShuHao();
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

MainWindow::~MainWindow(){

}

void MainWindow::createLayout(){
    QHBoxLayout *channelLayout = new QHBoxLayout;
    QLabel *channelLabel = new QLabel(tr("Channel: "));
    channelLabel->setFixedWidth(200);
    channelComboBox = new QComboBox;
    for(int i = 0; i < 4; i++){
        channelComboBox->addItem("Channel " + QString::number(i));
    }
    connect(channelComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_channelComboBox_changed(int)));
    channelLayout->addWidget(channelLabel);
    channelLayout->addWidget(channelComboBox);

    QHBoxLayout *phaseTypeLayout = new QHBoxLayout;
    QLabel *phaseTypeLabel = new QLabel(tr("Phase Type:"));
    phaseTypeLabel->setFixedWidth(200);
    phaseTypeComboBox = new QComboBox;
    phaseTypeComboBox->addItem("None");
    phaseTypeComboBox->addItem("MonoPhasic");
    phaseTypeComboBox->addItem("BiPhasic");
    connect(phaseTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_phaseComboBox_changed(int)));
    phaseTypeLayout->addWidget(phaseTypeLabel);
    phaseTypeLayout->addWidget(phaseTypeComboBox);

    QHBoxLayout *polarityLayout = new QHBoxLayout;
    QLabel *polarityLabel = new QLabel(tr("Polarity Type: "));
    polarityLabel->setFixedWidth(200);
    polarityComboBox = new QComboBox;
    polarityComboBox->addItem("None");
    polarityComboBox->addItem("Anodic Pulse First");
    polarityComboBox->addItem("Cathodic Pulse First");
    connect(polarityComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_polarityComboBox_changed(int)));
    polarityLayout->addWidget(polarityLabel);
    polarityLayout->addWidget(polarityComboBox);

    QHBoxLayout *anodicAmpLayout = new QHBoxLayout;
    QLabel *anodicAmpLabel = new QLabel(tr("Anodic Amplitude (uA):"));
    anodicAmpLabel->setFixedWidth(200);
    anodicAmpLineEdit = new QLineEdit;
    connect(anodicAmpLineEdit, SIGNAL(textEdited(QString)), this, SLOT(on_anodicAmp_editted()));
    anodicAmpLayout->addWidget(anodicAmpLabel);
    anodicAmpLayout->addWidget(anodicAmpLineEdit);

    QHBoxLayout *anodicDurationLayout = new QHBoxLayout;
    QLabel *anodicDurationLabel = new QLabel(tr("Anodic Duration (us):"));
    anodicDurationLabel->setFixedWidth(200);
    anodicDurationLineEdit = new QLineEdit;
    connect(anodicDurationLineEdit, SIGNAL(textEdited(QString)), this, SLOT(on_anodicDuration_editted()));
    anodicDurationLayout->addWidget(anodicDurationLabel);
    anodicDurationLayout->addWidget(anodicDurationLineEdit);

    QHBoxLayout *cathodicAmpLayout = new QHBoxLayout;
    QLabel *cathodicAmpLabel = new QLabel(tr("Cathodic Amplitude (uA):"));
    cathodicAmpLabel->setFixedWidth(200);
    cathodicAmpLineEdit = new QLineEdit;
    connect(cathodicAmpLineEdit, SIGNAL(textEdited(QString)), this, SLOT(on_cathodicAmp_editted()));
    cathodicAmpLayout->addWidget(cathodicAmpLabel);
    cathodicAmpLayout->addWidget(cathodicAmpLineEdit);

    QHBoxLayout *cathodicDurationLayout = new QHBoxLayout;
    QLabel *cathodicDurationLabel = new QLabel(tr("Cathodic Duration (us):"));
    cathodicDurationLabel->setFixedWidth(200);
    cathodicDurationLineEdit = new QLineEdit;
    connect(cathodicDurationLineEdit, SIGNAL(textEdited(QString)), this, SLOT(on_cathodicDuration_editted()));
    cathodicDurationLayout->addWidget(cathodicDurationLabel);
    cathodicDurationLayout->addWidget(cathodicDurationLineEdit);

    QHBoxLayout *numPulseLayout = new QHBoxLayout;
    QLabel *numPulseLabel = new QLabel(tr("Pulse Number:"));
    numPulseLabel->setFixedWidth(200);
    numPulseLineEdit = new QLineEdit;
    connect(numPulseLineEdit, SIGNAL(textEdited(QString)), this, SLOT(on_numPulse_editted()));
    numPulseLayout->addWidget(numPulseLabel);
    numPulseLayout->addWidget(numPulseLineEdit);

    QHBoxLayout *adjDurationLayout = new QHBoxLayout;
    QLabel *adjDurationLabel = new QLabel(tr("Adjacent Duration (ms):"));
    adjDurationLabel->setFixedWidth(200);
    adjDurationLineEdit = new QLineEdit;
    connect(adjDurationLineEdit, SIGNAL(textEdited(QString)), this, SLOT(on_adjDuration_editted()));
    adjDurationLayout->addWidget(adjDurationLabel);
    adjDurationLayout->addWidget(adjDurationLineEdit);

    QHBoxLayout *interPhaseIntervalLayout = new QHBoxLayout;
    QLabel *interPhaseIntervalLabel = new QLabel(tr("Interphase Interval (us):"));
    interPhaseIntervalLabel->setFixedWidth(200);
    interPhaseIntervalLineEdit = new QLineEdit;
    connect(interPhaseIntervalLineEdit, SIGNAL(textEdited(QString)), this, SLOT(on_interPhaseInterval_editted()));
    interPhaseIntervalLayout->addWidget(interPhaseIntervalLabel);
    interPhaseIntervalLayout->addWidget(interPhaseIntervalLineEdit);

    sendCommandButton = new QPushButton(tr("Send Command!"));
    connect(sendCommandButton, SIGNAL(clicked(bool)), this, SLOT(on_sendCommand_clicked()));

    startStopButton = new QPushButton(tr("Send Start"));
    connect(startStopButton, SIGNAL(clicked(bool)), this, SLOT(on_startStop_clicked()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(channelLayout);
    mainLayout->addLayout(phaseTypeLayout);
    mainLayout->addLayout(polarityLayout);
    mainLayout->addLayout(anodicAmpLayout);
    mainLayout->addLayout(anodicDurationLayout);
    mainLayout->addLayout(cathodicAmpLayout);
    mainLayout->addLayout(cathodicDurationLayout);
    mainLayout->addLayout(numPulseLayout);
    mainLayout->addLayout(adjDurationLayout);
    mainLayout->addLayout(interPhaseIntervalLayout);
    mainLayout->addWidget(sendCommandButton);
    mainLayout->addWidget(startStopButton);

    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
    mainLayout->setSizeConstraint( QLayout::SetFixedSize );
}

void MainWindow::on_channelComboBox_changed(int index){
    qDebug() << "Channel combo box changed index to: " << index;
    switch (index){
        case 0:{
            command->setChannel(CHANNEL_0);
            break;
        }
        case 1:{
            command->setChannel(CHANNEL_1);
            break;
        }
        case 2:{
            command->setChannel(CHANNEL_2);
            break;
        }
        case 3:{
            command->setChannel(CHANNEL_3);
            break;
        }
        default:{
            command->setChannel(CHANNEL_0);
            break;
        }
    }
    qDebug() << "Channel set to: " << command->getChannel();
}

void MainWindow::on_phaseComboBox_changed(int index){
    qDebug() << "Phase Type combo box changed index to: " << index;
    switch (index){
        case 0:{
            command->setPhaseType(PHASE_TYPE_NONE);
            break;
        }
        case 1:{
            command->setPhaseType(MONOPHASIC);
            break;
        }
        case 2:{
            command->setPhaseType(BIPHASIC);
            break;
        }
        default:{
            command->setPhaseType(PHASE_TYPE_NONE);
            break;
        }
    }
    qDebug() << "Phase Type set to: " << command->getPhaseType();
}

void MainWindow::on_polarityComboBox_changed(int index){
    qDebug() << "Polarity combo box changed index to: " << index;
    switch (index){
        case 0:{
            command->setPolarity(POLARITY_NONE);
            break;
        }
        case 1:{
            command->setPolarity(ANODIC_PULSE_FIRST);
            break;
        }
        case 2:{
            command->setPolarity(CATHODIC_PULSE_FIRST);
            break;
        }
        default:{
            command->setPolarity(POLARITY_NONE);
            break;
        }
    }
    qDebug() << "Polarity set to: " << command->getPolarity();
}

void MainWindow::on_anodicAmp_editted(){
    if(anodicAmpLineEdit->text().toInt() <= 1000 && anodicAmpLineEdit->text().toInt() >= 0){
        command->setAnodicAmp(anodicAmpLineEdit->text());
        qDebug() << "Anodic Amplitude set to: " << command->getAnodicAmp();
    }
    else{
        QMessageBox::warning(this, "Value error!", "Please key in a value between 0 to 1000!");
        anodicAmpLineEdit->clear();
    }
}

void MainWindow::on_anodicDuration_editted(){
    if(anodicDurationLineEdit->text().toInt() <= 1000 && anodicDurationLineEdit->text().toInt() >= 0){
        command->setAnodicDuration(anodicDurationLineEdit->text());
        qDebug() << "Anodic Duration set to: " << command->getAnodicDuration();
    }
    else{
        QMessageBox::warning(this, "Value error!", "Please key in a value between 0 to 1000!");
        anodicDurationLineEdit->clear();
    }
}

void MainWindow::on_cathodicAmp_editted(){
    if(cathodicAmpLineEdit->text().toInt() <= 1000 && cathodicAmpLineEdit->text().toInt() >= 0){
        command->setCathodicAmp(cathodicAmpLineEdit->text());
        qDebug() << "Cathodic Amplitude set to: " << command->getCathodicAmp();
    }
    else{
        QMessageBox::warning(this, "Value error!", "Please key in a value between 0 to 1000!");
        cathodicAmpLineEdit->clear();
    }
}

void MainWindow::on_cathodicDuration_editted(){
    if(cathodicDurationLineEdit->text().toInt() <= 1000 && cathodicDurationLineEdit->text().toInt() >= 0){
        command->setCathodicDuration(cathodicDurationLineEdit->text());
        qDebug() << "Cathodic Duration set to: " << command->getCathodicDuration();
    }
    else{
        QMessageBox::warning(this, "Value error!", "Please key in a value between 0 to 1000!");
        cathodicDurationLineEdit->clear();
    }
}

void MainWindow::on_numPulse_editted(){
    if(numPulseLineEdit->text().toInt() <= 1000 && numPulseLineEdit->text().toInt() >= 0){
        command->setNumPulse(numPulseLineEdit->text());
        qDebug() << "Number of Pulses set to: " << command->getNumPulse();
    }
    else{
        QMessageBox::warning(this, "Value error!", "Please key in a value between 0 to 1000!");
        numPulseLineEdit->clear();
    }
}

void MainWindow::on_adjDuration_editted(){
    if(adjDurationLineEdit->text().toInt() < 1000 && adjDurationLineEdit->text().toInt() > 0){
        command->setAdjDuration(adjDurationLineEdit->text());
        qDebug() << "Adjacent Duration set to: " << command->getAdjDuration();
    }
    else{
        QMessageBox::warning(this, "Value error!", "Please key in a value between 0 to 1000!");
        adjDurationLineEdit->clear();
    }
}

void MainWindow::on_interPhaseInterval_editted(){
    if(interPhaseIntervalLineEdit->text().toInt() < 1000 && interPhaseIntervalLineEdit->text().toInt() > 0){
        command->setInterPhaseInterval(interPhaseIntervalLineEdit->text());
        qDebug() << "Adjacent Duration set to: " << command->getInterPhaseInterval();
    }
    else{
        QMessageBox::warning(this, "Value error!", "Please key in a value between 0 to 1000!");
        interPhaseIntervalLineEdit->clear();
    }
}

void MainWindow::on_sendCommand_clicked(){
    sendStart();
    QTimer::singleShot(300, [=] {
        serialShuHao->writeCommand(command->constructCommand());
    });
    QTimer::singleShot(6000, [=] {
            sendStop();
    });
}

void MainWindow::on_startStop_clicked(){
    if(!start){
        sendStart();
        start = !start;
    }
    else{
        sendStop();
        start = !start;
    }
}

void MainWindow::sendStop(){
    serialShuHao->sendStop();
    sendCommandButton->setEnabled(true);
    startStopButton->setText("Send Start");
}

void MainWindow::sendStart(){
    serialShuHao->sendStart();
    sendCommandButton->setEnabled(false);
    startStopButton->setText("Send Stop");
}

void MainWindow::acceptConnection(){
    qDebug() << "Accepted new connection";
    tcpServerConnection = tcpServer->nextPendingConnection();
    connect(tcpServerConnection, SIGNAL(readyRead()), this, SLOT(echo()));
}

void MainWindow::echo(){
    QByteArray temp;
    temp = tcpServerConnection->readAll();
    tcpServerConnection->close();
}
