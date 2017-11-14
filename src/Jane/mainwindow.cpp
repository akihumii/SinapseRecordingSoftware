#include "mainwindow.h"
#include "amwflash_mainwindow.h"

MainWindow::MainWindow()
{
    amwFlash_init = new amwFlash_mainWindow;
    thorParam = new Stimulator;
    data = new DataProcessor(thorChannel);
    thorCommand = new Command(thorParam);
    serialThor = new SerialChannel(this, thorCommand, data, thorChannel);
    socketThor = new SocketThor(this, thorCommand, data, thorChannel);
    createLayout();
    createAction();
    connectThor();
//    on_wired_triggered();


    if(thorCommand -> haveLastCommand()){
        loadLastCommand();
    }
    else{
        loadDefault();
        DCLExit_RadioButton -> setChecked(true);
    }
}

MainWindow::~MainWindow()
{
    thorCommand -> setLastCommand(true);
}

void MainWindow::createLayout()
{
    QLabel *modeLabel = new QLabel;
    modeLabel -> setText("Mode  : ");

    createModeComboBox();
    QLabel *chipIDLabel = new QLabel;
    chipIDLabel -> setText("Chip ID: ");
    chipIDComboBox = new QComboBox;
    for(int i = 0; i < 8; i++)
        chipIDComboBox -> addItem("Chip " + QString::number(i));

    int width = modeComboBox -> minimumSizeHint().width();
    modeComboBox -> setMinimumWidth(width);
    chipIDComboBox -> setMinimumWidth(width);

    QHBoxLayout *modeLayout = new QHBoxLayout;
    modeLayout -> addWidget(modeLabel);
    modeLayout -> addWidget(modeComboBox);

    QHBoxLayout *chipIDLayout = new QHBoxLayout;
    chipIDLayout -> addWidget(chipIDLabel);
    chipIDLayout -> addWidget(chipIDComboBox);

    berGroupBox = new QGroupBox(tr("Data BER Assessment"));
    QHBoxLayout *BERLayout = new QHBoxLayout;
    for(int i = 0; i < 8; i++){
        BER_byte[i] = new QLineEdit;
        BER_byte[i] -> setInputMask("HH");
        BER_byte[i] -> setFixedWidth(45);
        BER_byte[i] -> setAlignment(Qt::AlignCenter);
        BERLayout -> addWidget(BER_byte[i]);
    }
    berGroupBox -> setLayout(BERLayout);

    triggerGroupBox = new QGroupBox(tr("DEFXYabc"));
    QHBoxLayout *triggerBitLayout = new QHBoxLayout;
    for(int i = 0; i < 8; i++){
        OP_bit[i] = new QLineEdit;
        OP_bit[i] -> setInputMask("B");
        OP_bit[i] -> setFixedWidth(45);
        OP_bit[i] -> setAlignment(Qt::AlignCenter);
        triggerBitLayout -> addWidget(OP_bit[i]);
    }
    triggerGroupBox -> setLayout(triggerBitLayout);

    bioImpGroupBox = new QGroupBox(tr("Bio Impedance"));
    QVBoxLayout *BioImpLayout = new QVBoxLayout;
    for(int i = 0; i < 6; i++){
        BioImpData[i] = new QCheckBox;
        BioImpData[i] -> setText(BioImpName[i]);
        BioImpLayout -> addWidget(BioImpData[i]);
    }
    bioImpGroupBox -> setLayout(BioImpLayout);

    dclGroupBox = new QGroupBox(tr("Digital Command Loopback"));
    QVBoxLayout *DCLLayout = new QVBoxLayout;
    DCLEnter_RadioButton = new QRadioButton(tr("Enter"));
    DCLExit_RadioButton = new QRadioButton(tr("Exit"));
    DCLLayout -> addWidget(DCLEnter_RadioButton);
    DCLLayout -> addWidget(DCLExit_RadioButton);
    DCLLayout -> setAlignment(Qt::AlignTop);
    dclGroupBox -> setLayout(DCLLayout);

    QHBoxLayout *BIOnDCL = new QHBoxLayout;
    BIOnDCL -> addWidget(bioImpGroupBox);
    BIOnDCL -> addWidget(dclGroupBox);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    sendCommandButton = new QPushButton(tr("Send Command"));
    chipResetButton = new QPushButton(tr("Chip Reset"));
    buttonLayout -> addWidget(sendCommandButton);
    buttonLayout -> addWidget(chipResetButton);

    amwFlashButton = new QPushButton(tr("Programming Mode (Under development)"));
    amwFlashButton->setEnabled(false);

    timeStartMapper = new QSignalMapper(this);
    timeStopMapper = new QSignalMapper(this);
    multipleStartMapper = new QSignalMapper(this);
    multipleStopMapper = new QSignalMapper(this);
    createStimulatorParamWidget();

    mainLayout = new QVBoxLayout();
    mainLayout -> addLayout(modeLayout);
    mainLayout -> addLayout(chipIDLayout);
    mainLayout -> addWidget(berGroupBox);
    mainLayout -> addWidget(triggerGroupBox);
    mainLayout -> addWidget(stimParaGroupBox);
    mainLayout -> addLayout(BIOnDCL);
    mainLayout -> addLayout(buttonLayout);
    mainLayout -> addWidget(amwFlashButton);

    createSubsequenceWidget();
    createJTAGWidget();

    topLeftLayout = new QVBoxLayout();
    topLeftLayout -> addWidget(subSequenceGroupBox);
    topLeftLayout -> addWidget(JTAGTabWidget);

    allLayout = new QHBoxLayout();
    allLayout -> addLayout(mainLayout);
    allLayout -> addLayout(topLeftLayout);
    on_mode_changed(0);

    QWidget *mainWidget = new QWidget;
    allLayout->setSizeConstraint( QLayout::SetFixedSize );
    mainWidget->setLayout(allLayout);
    this->setCentralWidget(mainWidget);
}

void MainWindow::createAction()
{    
    connect(modeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_mode_changed(int)));
    connect(chipIDComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_chipID_changed(int)));
    for(int i = 0; i < 8; i++){
        connect(BER_byte[i], SIGNAL(textEdited(QString)), this, SLOT(on_BER_TextEditted()));
    }

    for(int i = 0; i < 8; i++){
        connect(OP_bit[i], SIGNAL(textEdited(QString)), this, SLOT(on_OP_Trigger_TextEditted()));
    }

    for(int i = 0; i < 6; i++){
        connect(BioImpData[i], SIGNAL(toggled(bool)), this, SLOT(on_BioImp_toggled()));
    }
    connect(DCLEnter_RadioButton, SIGNAL(toggled(bool)), this, SLOT(on_DCL_toggled()));
    connect(DCLExit_RadioButton, SIGNAL(toggled(bool)), this, SLOT(on_DCL_toggled()));
    connect(sendCommandButton, SIGNAL(clicked()), this, SLOT(on_sendCommand_clicked()));
    connect(chipResetButton, SIGNAL(clicked()), this, SLOT(on_chipReset_clicked()));
    connect(amwFlashButton, SIGNAL(clicked()), amwFlash_init, SLOT(show()));
//    connect(graphButton, SIGNAL(clicked()), graph_init, SLOT(show()));

    connect(timeStartMapper, SIGNAL(mapped(int)), this, SLOT(on_subSeqTimeStartEdit_changed(int)));
    connect(timeStopMapper, SIGNAL(mapped(int)), this, SLOT(on_subSeqTimeStopEdit_changed(int)));
    connect(multipleStartMapper, SIGNAL(mapped(int)), this, SLOT(on_subSeqMultipleStartComboBox_selected(int)));
    connect(multipleStopMapper, SIGNAL(mapped(int)), this, SLOT(on_subSeqMultipleStopComboBox_selected(int)));

    for(int i = 0; i < 8; i++){
        connect(subSeqCheckBox[i], SIGNAL(toggled(bool)), this, SLOT(on_subSequenceChannel_selected()));
        connect(subSeqParamSpinBox[i], SIGNAL(valueChanged(int)), this, SLOT(on_subSeqParamSpinBox_changed()));
        connect(subSeqTimeStartEdit[i], SIGNAL(textChanged(QString)), timeStartMapper, SLOT(map()));
        connect(subSeqMultipleStartComboBox[i], SIGNAL(currentIndexChanged(int)), multipleStartMapper, SLOT(map()));
        connect(subSeqTimeStopEdit[i], SIGNAL(textChanged(QString)), timeStopMapper, SLOT(map()));
        connect(subSeqMultipleStopComboBox[i], SIGNAL(currentIndexChanged(int)), multipleStopMapper, SLOT(map()));
    }

    connect(paramSetSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(on_paramSetSelect_changed()));
    for (int i = 0; i<6;i++){
        connect(paramEdit[i],SIGNAL(textEdited(QString)),this,SLOT(on_paramValueChange()));
    }

}

void MainWindow::loadLastCommand(){

}

void MainWindow::loadDefault(){

}

void MainWindow::on_mode_changed(int mode){
    thorCommand -> setOPMode(mode);

    dclGroupBox -> setEnabled(false);
    berGroupBox -> setEnabled(false);
    subSequenceGroupBox -> setEnabled(false);
    stimParaGroupBox -> setEnabled(false);
    bioImpGroupBox -> setEnabled(false);
    JTAGTabWidget -> setEnabled(false);
    triggerGroupBox -> setEnabled(false);


    switch (mode){
    case 0:{
        dclGroupBox -> setEnabled(true);
        break;
    }
    case 1:{
        berGroupBox -> setEnabled(true);
        break;
    }
    case 2:{
        JTAGTabWidget -> setEnabled(true);
        break;
    }
    case 3:{
        stimParaGroupBox -> setEnabled(true);
        break;
    }
    case 4:{
        subSequenceGroupBox -> setEnabled(true);
        break;
    }
    case 5:{
        triggerGroupBox -> setEnabled(true);
        break;
    }
    case 6:{
        JTAGTabWidget -> setEnabled(true);
        break;
    }
    case 8:{
        bioImpGroupBox -> setEnabled(true);
        JTAGTabWidget -> setEnabled(true);
        break;
    }
    default:{
        break;
    }
    }

}

void MainWindow::on_chipID_changed(int IDNum)
{
    thorCommand -> setChipID(IDNum);
}

void MainWindow::on_BER_TextEditted(){
    for(int i = 0; i < 8; i++){
        thorCommand -> updateBER(i, BER_byte[i] -> text());
    }
}

void MainWindow::on_OP_Trigger_TextEditted()
{
    for(int i = 0; i < 8; i++){
        thorCommand -> updateTriggerCmd(i, OP_bit[i] -> text());
    }
}

void MainWindow::on_BioImp_toggled()
{
    for(int i = 0; i < 6; i++){
        if(BioImpData[i] -> isChecked())
            thorCommand -> setBioImpBit(i);
        else{
            thorCommand -> clearBioImpBit(i);
        }
    }
}

void MainWindow::on_DCL_toggled()
{
    if(DCLEnter_RadioButton -> isChecked()){
        thorCommand -> setDCLMode(DCL_ENTER);
    }
    else{
        thorCommand -> setDCLMode(DCL_EXIT);
    }
}

void MainWindow::on_sendCommand_clicked(){

    thorCommand -> constructCommand();
    qDebug() << "Constructed: " << thorCommand->getCmd().toHex();
    if(serialThor->isConnected()){
        qDebug()<< "send via wired: " << thorCommand->getCmd().toHex();
        serialThor->writeCommand(thorCommand->getCmd());
    }
    if(socketThor->isConnected()){
        qDebug()<< "send via wifi: " << thorCommand->getCmd().toHex();
        socketThor->writeCommand(thorCommand->getCmd());
    }
    if( (modeComboBox->currentIndex() == 7) ||
        (modeComboBox->currentIndex() == 8)
       ){
        MeasurementDialog measurementDialog(serialThor);
        measurementDialog.exec();
    }
}

void MainWindow::on_chipReset_clicked()
{
    qDebug() << thorCommand -> resetCommand().toHex();
    if(serialThor->isConnected()){
        serialThor->writeCommand(thorCommand->getCmd());
    }
}

void MainWindow::on_JTAG_toggled()
{
    for(int i=0;i<48;i++){
        if(JTAG[i]->isChecked()){
            thorCommand->setJTAGBit(i);
        }
        else{
            thorCommand->clearJTAGBit(i);
        }
    }
    for (int i=0; i<6;i++){
        qDebug()<<thorCommand->getJTAG(i);
    }

}

void MainWindow::on_subSequenceChannel_selected()
{
    for(int i = 0; i < 8; i++){
        if(subSeqCheckBox[i] -> isChecked()){
            thorParam -> setSubSeqSelect(i, true);
        }
        else{
            thorParam -> setSubSeqSelect(i, false);
        }
    }
}

void MainWindow::on_subSeqParamSpinBox_changed()
{
    for(int i = 0; i < 8; i++){
        thorParam -> setSubSeqParam(i, subSeqParamSpinBox[i] -> value());
    }
}

void MainWindow::on_paramSetSelect_changed(){
    for(int i = 0; i < 6; i++){
        paramEdit[i]->setText(QString::number(thorParam->getStimParam(paramSetSelect->currentIndex(), i), 16));
    }
}

void MainWindow::on_paramValueChange()
{
    for(int i = 0; i < 6; i++){
        thorParam->setStimParam(paramSetSelect->currentIndex(), i, paramEdit[i]->text().toInt(&ok,16));
    }
}

void MainWindow::on_subSeqTimeStartEdit_changed(int channel)
{
    thorParam -> setSubSeqTimeStart(channel, subSeqTimeStartEdit[channel] -> text().toInt());
}

void MainWindow::on_subSeqMultipleStartComboBox_selected(int index)
{
    thorParam -> setSubSeqMultipleStart(index, subSeqMultipleStartComboBox[index] -> currentIndex());
}

void MainWindow::on_subSeqTimeStopEdit_changed(int channel)
{
    thorParam -> setSubSeqTimeStop(channel, subSeqTimeStopEdit[channel] -> text().toInt());
}

void MainWindow::on_subSeqMultipleStopComboBox_selected(int index)
{
    thorParam -> setSubSeqMultipleStop(index, subSeqMultipleStopComboBox[index] -> currentIndex());
}


void MainWindow::on_wired_triggered(){
    serialThor->serialenabled = true;
    if(serialThor->doConnect()){
        qDebug() << "ready via USB";
    }
    //    socketThor->wifiEnabled = false;
}



void MainWindow::connectThor()
{
    portInfo = QSerialPortInfo::availablePorts();
    connectionStatus.clear();
    if(portInfo.size()>0){
        serialThor->doConnect();
        connectionStatus.clear();
        if(serialThor->isConnected()){
            connectionStatus.append("Connected to Thor!!");
        }
        else{
            connectionStatus.append("Connection to Thor failed");
        }
        qDebug()<<connectionStatus;
//        statusBarLabel->setText(connectionStatus);
    }
    if(!serialThor->isConnected()){
        qDebug()<<"start wifi";
        socketThor->doConnect("192.168.4.1", 8888);
        connectionStatus.clear();
        if(socketThor->isConnected()){
            connectionStatus.append("Connected to Thor WiFi Module at 192.168.42.1/8888");
        }
        else{
            connectionStatus.append("Connection to Thor failed! Restart this program after connecting Thor.");
            QMessageBox::information(this, "Failed to connect!", "No Thor device detected.. \n"
                                                                 "Check your connections and run the program again..");
        }
        qDebug()<< connectionStatus;
//        statusBarLabel->setText(connectionStatus);
    }
}

void MainWindow::createModeComboBox(){
    modeComboBox = new QComboBox;
    modeComboBox -> addItem("Digital Command Loopback");
    modeComboBox -> addItem("Data BER Assessment");
    modeComboBox -> addItem("JTAG Programming Mode");
    modeComboBox -> addItem("Stimulator Parameter Mode");
    modeComboBox -> addItem("Stimulator Sequence Program Mode");
    modeComboBox -> addItem("Stimulator Trigger Command");
    modeComboBox -> addItem("Oscillator Clock Tuning Mode");
    modeComboBox -> addItem("Analog Measurement Mode (8 Bits)");
    modeComboBox -> addItem("Bioimpedance Measurement Mode (8 Bits)");
    modeComboBox -> setCurrentIndex(0);
}

void MainWindow::createSubsequenceWidget()
{
    subSequenceGroupBox = new QGroupBox(tr("Subsequence Settings"));
    subSequenceLayout = new QVBoxLayout();
    for(int i = 0; i < 8; i++){
        subSeqLine[i] = new QHBoxLayout;
        subSeqCheckBox[i] = new QCheckBox("Channel " + QString::number(i+1) + ":", this);
        QLabel *chooseParamLabel = new QLabel(tr("Parameter:  "));

        subSeqParamSpinBox[i] = new QSpinBox;
        subSeqParamSpinBox[i] -> setMinimum(0);
        subSeqParamSpinBox[i] -> setMaximum(16);
        subSeqParamSpinBox[i] -> setSingleStep(1);
        subSeqParamSpinBox[i] -> setValue(1);
        subSeqParamSpinBox[i] -> setMaximumWidth(100);
        subSeqParamSpinBox[i] -> setAlignment(Qt::AlignCenter);

        QLabel *timeStartLabel = new QLabel(tr("Start:  "));
        subSeqTimeStartEdit[i] = new QLineEdit;
        subSeqTimeStartEdit[i] -> setAlignment(Qt::AlignCenter);
        subSeqTimeStartEdit[i] -> setMaximumWidth(100);
        timeStartMapper -> setMapping(subSeqTimeStartEdit[i], i);

        subSeqMultipleStartComboBox[i] = new QComboBox;
        subSeqMultipleStartComboBox[i] -> addItem("X1");
        subSeqMultipleStartComboBox[i] -> addItem("X2");
        subSeqMultipleStartComboBox[i] -> addItem("X4");
        subSeqMultipleStartComboBox[i] -> addItem("X8");
        subSeqMultipleStartComboBox[i] -> addItem("X16");
        subSeqMultipleStartComboBox[i] -> addItem("X32");
        subSeqMultipleStartComboBox[i] -> addItem("X64");
        subSeqMultipleStartComboBox[i] -> addItem("X128");
        multipleStartMapper -> setMapping(subSeqMultipleStartComboBox[i], i);

        QLabel *timeStopLabel = new QLabel(tr("Stop:  "));
        subSeqTimeStopEdit[i] = new QLineEdit;
        subSeqTimeStopEdit[i] -> setAlignment(Qt::AlignCenter);
        subSeqTimeStopEdit[i] -> setMaximumWidth(100);
        timeStopMapper -> setMapping(subSeqTimeStopEdit[i], i);

        subSeqMultipleStopComboBox[i] = new QComboBox;
        subSeqMultipleStopComboBox[i] -> addItem("X1");
        subSeqMultipleStopComboBox[i] -> addItem("X2");
        subSeqMultipleStopComboBox[i] -> addItem("X4");
        subSeqMultipleStopComboBox[i] -> addItem("X8");
        subSeqMultipleStopComboBox[i] -> addItem("X16");
        subSeqMultipleStopComboBox[i] -> addItem("X32");
        subSeqMultipleStopComboBox[i] -> addItem("X64");
        subSeqMultipleStopComboBox[i] -> addItem("X128");
        multipleStopMapper -> setMapping(subSeqMultipleStopComboBox[i], i);

        subSeqLine[i] -> addWidget(subSeqCheckBox[i]);
        subSeqLine[i] -> addWidget(chooseParamLabel);
        subSeqLine[i] -> addWidget(subSeqParamSpinBox[i]);
        subSeqLine[i] -> addWidget(timeStartLabel);
        subSeqLine[i] -> addWidget(subSeqTimeStartEdit[i]);
        subSeqLine[i] -> addWidget(subSeqMultipleStartComboBox[i]);
        subSeqLine[i] -> addWidget(timeStopLabel);
        subSeqLine[i] -> addWidget(subSeqTimeStopEdit[i]);
        subSeqLine[i] -> addWidget(subSeqMultipleStopComboBox[i]);

        subSequenceLayout -> addLayout(subSeqLine[i]);
    }
    subSequenceLayout -> setAlignment(Qt::AlignTop);

    subSequenceGroupBox ->setLayout(subSequenceLayout);
}

void MainWindow::createStimulatorParamWidget(){
    stimParaGroupBox = new QGroupBox(tr("Stimulator parameter"));
    StimulatorParamLayout = new QVBoxLayout();
    paramLine = new QHBoxLayout;
    paramSetSelect = new QComboBox;
    paramSetSelect->setFixedWidth(70);
    QHBoxLayout *labelsLayout = new QHBoxLayout;
    for(int i = 0; i < 7; i++){
        stimLabel[i] = new QLabel(StimParamNames[i]);
        stimLabel[i]->setFixedWidth(50);
        stimLabel[i]->setAlignment(Qt::AlignLeft);
        labelsLayout->addWidget(stimLabel[i]);
    }
    stimLabel[0]->setFixedWidth(70);
    StimulatorParamLayout->addLayout(labelsLayout);
    for(int i = 0; i < 16; i++){
        paramSetSelect->addItem("Set " + QString::number(i+1));
    }
    paramLine->addWidget(paramSetSelect);
    for(int i = 0; i < 6; i++){
        paramEdit[i] = new QLineEdit;
        paramEdit[i]->setAlignment(Qt::AlignCenter);
        paramEdit[i]->setInputMask("HH");
        paramEdit[i]->setFixedWidth(50);
        paramEdit[i]->setText(QString::number(00));
        paramLine->addWidget(paramEdit[i]);
    }

    StimulatorParamLayout->addLayout(paramLine);

    StimulatorParamLayout -> setAlignment(Qt::AlignTop);

    stimParaGroupBox -> setLayout(StimulatorParamLayout);
}

void MainWindow::createJTAGWidget(){
    JTAGTabWidget = new QTabWidget;

    DATAMOD = new QWidget;
    OSCINT = new QWidget;
    BIOIMPEDANCE = new QWidget;
    VTUNEDAX = new QWidget;
    JTAGREGISTER = new QWidget;

    JTAGTabWidget->addTab(DATAMOD, tr("DATAMOD"));
    JTAGTabWidget->addTab(OSCINT,tr("OSCINT"));
    JTAGTabWidget->addTab(BIOIMPEDANCE,tr("BIOIMPEDANCE"));
    JTAGTabWidget->addTab(VTUNEDAX,tr("VTUNEDAX"));
    JTAGTabWidget->addTab(JTAGREGISTER,tr("JTAG REGISTER"));

    for(int i=0; i<48; i++){
        JTAG[i] = new QCheckBox(JTAGNames[i], this);
        connect(JTAG[i], SIGNAL(toggled(bool)), this, SLOT(on_JTAG_toggled()));
    }
    QVBoxLayout *DATAMODLayout = new QVBoxLayout;
    for(int i=0; i<5;i++){
        DATAMODLayout->addWidget(JTAG[i]);
    }
    DATAMOD->setLayout(DATAMODLayout);

    QVBoxLayout *OSCINTLayout = new QVBoxLayout;
    for(int i=5;i<13;i++){
        OSCINTLayout->addWidget(JTAG[i]);
    }
    OSCINT->setLayout(OSCINTLayout);

    QVBoxLayout *BIOIMPEDANCELayout = new QVBoxLayout;
    for(int i=13;i<20;i++){
        BIOIMPEDANCELayout->addWidget(JTAG[i]);
    }
    BIOIMPEDANCE->setLayout(BIOIMPEDANCELayout);

    QVBoxLayout *VTUNEDAXLayout = new QVBoxLayout;
    for(int i=20;i<25;i++){
        VTUNEDAXLayout->addWidget(JTAG[i]);
    }
    VTUNEDAX->setLayout(VTUNEDAXLayout);


    QVBoxLayout *JTAGReg_1 = new QVBoxLayout;
    QVBoxLayout *JTAGReg_2 = new QVBoxLayout;
    QVBoxLayout *JTAGReg_3 = new QVBoxLayout;
    JTAGReg_1->setAlignment(Qt::AlignTop);
    JTAGReg_2->setAlignment(Qt::AlignTop);
    JTAGReg_3->setAlignment(Qt::AlignTop);

    QHBoxLayout *JTAGREGISTERLayout = new QHBoxLayout;
    for(int i=25;i<33;i++){
        JTAGReg_1->addWidget(JTAG[i]);
    }
    for(int i=33;i<41;i++){
        JTAGReg_2->addWidget(JTAG[i]);
    }
    for(int i=41;i<48;i++){
        JTAGReg_3->addWidget(JTAG[i]);
    }

    JTAGREGISTERLayout->addLayout(JTAGReg_1);
    JTAGREGISTERLayout->addLayout(JTAGReg_2);
    JTAGREGISTERLayout->addLayout(JTAGReg_3);

    JTAGREGISTER->setLayout(JTAGREGISTERLayout);
}
