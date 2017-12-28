#include "commanddialog.h"

CommandDialog::CommandDialog(SocketNeutrino *socketNeutrino_,
                             Command *NeutrinoCommand_,
                             Channel *NeutrinoChannel_,
                             SerialChannel *NeutrinoSerial_,
                             DataProcessor *dataProcessor_){
    socketNeutrino = socketNeutrino_;
    NeutrinoChannel = NeutrinoChannel_;
    NeutrinoCommand = NeutrinoCommand_;
    NeutrinoSerial = NeutrinoSerial_;
    dataProcessor = dataProcessor_;
    setWindowTitle(tr("Neutrino Command"));
    createLayout();
    if(NeutrinoCommand->havelastCommand()){
        loadlastCommand();
    }
    else{
        loadDefault();
        Exit->setChecked(true);
    }
    on_SELALL_clicked();
    on_amplifierSelectAll_clicked();

    mboxWait = new QMessageBox;
    mboxWait->setText("Please wait... \nMeasurement in progress...");
    mboxWait->setStandardButtons(0);

    delayThread = new DelayThread();
    QThread *thread = new QThread;
    delayThread->moveToThread(thread);

    connect(this, SIGNAL(closeWaitingMBox()), this, SLOT(on_exitBioImpedanceMeasurement()));
}

CommandDialog::~CommandDialog(){
    NeutrinoCommand->setlastCommand(true);
}

void CommandDialog::createLayout(){
    ModeLabel = new QLabel;
    ModeLabel->setText("Mode");
    ModeComboBox = new QComboBox;
    ModeComboBox->addItem("Digital Command Loopback");
    ModeComboBox->addItem("Data BER Assessment");
    ModeComboBox->addItem("Selected Channel Out (8-bit)");
    ModeComboBox->addItem("Selected Channel Out (10-bit)");
    ModeComboBox->addItem("Oscillator Clock Tuning");
    ModeComboBox->addItem("Analog Measurement (8-bit)");
    ModeComboBox->addItem("Analog Measurement (10-bit)");
    ModeComboBox->addItem("Bioimpedance Measurement (8-bit)");
    ModeComboBox->addItem("Bioimpedance Measurement (10-bit)");
    ModeComboBox->addItem("Power Level Measurement");
    ModeComboBox->addItem("Full Bioimpedance Measurement (8-bit)");
    ModeComboBox->addItem("Auto Ranged Bioimpedance Measurement (8-bit)");

    connect(ModeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_Mode_Changed(int)));

    CIDLabel = new QLabel;
    CIDLabel->setText("CID");
    CIDComboBox = new QComboBox;
    for (int i = 0; i < 8; i++){
        CIDComboBox->addItem("Chip " + QString::number(i));
    }

    connect(CIDComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_ChipID_Changed(int)));

    QVBoxLayout *BioImpLayout = new QVBoxLayout;
    BioImpLabel = new QLabel;
    BioImpLabel->setText("Bio Impedance");
    BioImpLayout->addWidget(BioImpLabel);

    for(int i=0;i<6;i++){
        BioImpData[i] = new QCheckBox;
        BioImpData[i]->setText(BioImpNames[i]);
        BioImpLayout->addWidget(BioImpData[i]);
        connect(BioImpData[i], SIGNAL(toggled(bool)), this, SLOT(on_BioImp_toggled()));
    }

    SendCommand = new QPushButton(tr("Send Command"));
    connect(SendCommand, SIGNAL(clicked(bool)), this, SLOT(on_sendCommand_clicked()));

    BioImpLayout->addWidget(SendCommand);


    QVBoxLayout *DCL = new QVBoxLayout;
    DigComLoopback = new QLabel;
    DigComLoopback->setText("Digital Command Loopback");

    Exit = new QRadioButton("Exit", this);
    Enter = new QRadioButton("Enter", this);

    connect(Exit, SIGNAL(toggled(bool)), this, SLOT(on_DCL_toggled()));
    connect(Enter, SIGNAL(toggled(bool)), this, SLOT(on_DCL_toggled()));

    CMReset = new QPushButton(tr("CM Reset"));
    connect(CMReset, SIGNAL(clicked(bool)), this, SLOT(on_CMReset_clicked()));

    shortRefToGND = new QPushButton(tr("Short Ref to GND"));
    connect(shortRefToGND, SIGNAL(clicked(bool)), this, SLOT(on_shortRefToGND_clicked()));

    ChipReset = new QPushButton(tr("Chip Reset"));
    connect(ChipReset, SIGNAL(clicked(bool)), this, SLOT(on_chipReset_clicked()));

    DCL->addWidget(DigComLoopback);
    DCL->addWidget(Exit);
    DCL->addWidget(Enter);
    DCL->addSpacing(40);
    DCL->addWidget(shortRefToGND);
    DCL->addWidget(CMReset);
    DCL->addWidget(ChipReset);

    QHBoxLayout *BIOnDCL = new QHBoxLayout;
    BIOnDCL->addLayout(BioImpLayout);
    BIOnDCL->addLayout(DCL);

    QVBoxLayout *LabelLayout = new QVBoxLayout;
    LabelLayout->addWidget(ModeLabel);
    LabelLayout->addWidget(CIDLabel);

    QVBoxLayout *ComboBoxLayout = new QVBoxLayout;
    ComboBoxLayout->addWidget(ModeComboBox);
    ComboBoxLayout->addWidget(CIDComboBox);

    QHBoxLayout *LabelComboLayout = new QHBoxLayout;
    LabelComboLayout->addLayout(LabelLayout);
    LabelComboLayout->addLayout(ComboBoxLayout);

    QHBoxLayout *BERLayout = new QHBoxLayout;
    for (int i=0;i<8;i++){
        BER_byte[i] = new QLineEdit;
        BER_byte[i]->setInputMask("HH");
        BER_byte[i]->setMaximumWidth(35);
        BER_byte[i]->setAlignment(Qt::AlignHCenter);
        connect(BER_byte[i], SIGNAL(textEdited(QString)), this, SLOT(on_BER_textEdited()));
        BERLayout->addWidget(BER_byte[i]);
    }

    DataBERLabel = new QLabel;
    DataBERLabel->setText("Data BER Assessment");

    QVBoxLayout *DataBERLayout = new QVBoxLayout;
    DataBERLayout->addLayout(LabelComboLayout);
    DataBERLayout->addWidget(DataBERLabel);
    DataBERLayout->addLayout(BERLayout);
    DataBERLayout->addLayout(BIOnDCL);

    QVBoxLayout *ChannelLayout = new QVBoxLayout;
    QLabel *channelStreamLabel = new QLabel(tr("Channel Stream:"));
    ChannelLayout->addWidget(channelStreamLabel);
    for(int i=0;i<10;i++){
        SELCHN[i] = new QCheckBox("Channel "+QString::number(i+1), this);
        connect(SELCHN[i], SIGNAL(toggled(bool)), this, SLOT(on_SELCHN_toggled()));
        ChannelLayout->addWidget(SELCHN[i]);
    }
    SELALL = new QPushButton(tr("Select All"));
    SELNONE = new QPushButton(tr("Select None"));
    connect(SELALL, SIGNAL(clicked(bool)), this, SLOT(on_SELALL_clicked()));
    connect(SELNONE, SIGNAL(clicked(bool)), this, SLOT(on_SELNONE_clicked()));

    ChannelLayout->addWidget(SELALL);
    ChannelLayout->addWidget(SELNONE);

    QHBoxLayout *topLayout = new QHBoxLayout;

    topLayout->addLayout(DataBERLayout);
    topLayout->addLayout(ChannelLayout);
//------------------------------------------------------------------ TO ADD AMPLIFIER ENABLE CHECKBOXES -------------------------------------------------------//
    QVBoxLayout *AmplifierSelectLayout = new QVBoxLayout;
    QLabel *amplifierEnableLabel = new QLabel(tr("Amplifier Enable:"));
    AmplifierSelectLayout->addWidget(amplifierEnableLabel);
    for(int i = 0; i < 10; i++){
        amplifierSelect[i] = new QCheckBox("Amplifier " + QString::number(i+1), this);
        connect(amplifierSelect[i], SIGNAL(toggled(bool)), this, SLOT(on_amplifierSelect_toggled()));
        AmplifierSelectLayout->addWidget(amplifierSelect[i]);
    }
    amplifierSelectAll = new QPushButton(tr("Select All"));
    amplifierSelectNone = new QPushButton(tr("Select None"));
    connect(amplifierSelectAll, SIGNAL(clicked(bool)), this, SLOT(on_amplifierSelectAll_clicked()));
    connect(amplifierSelectNone, SIGNAL(clicked(bool)), this, SLOT(on_amplifierSelectNone_clicked()));

    AmplifierSelectLayout->addWidget(amplifierSelectAll);
    AmplifierSelectLayout->addWidget(amplifierSelectNone);

    topLayout->addLayout(AmplifierSelectLayout);
//------------------------------------------------------------------ TO ADD AMPLIFIER ENABLE CHECKBOXES -------------------------------------------------------//

    JTAGextension = new QPushButton(tr("Show JTAG"));
    connect(JTAGextension, SIGNAL(clicked(bool)), this, SLOT(on_JTAGextension_clicked()));

    JTAGreset = new QPushButton(tr("Restore default JTAG setting"));

    connect(JTAGreset, SIGNAL(clicked(bool)), this, SLOT(on_JTAGreset_clicked()));

    createJTAGLayout();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(JTAGextension);
    mainLayout->addWidget(JTAGreset);
    mainLayout->addWidget(JTAGTabWidget);

    JTAGreset->hide();
    JTAGTabWidget->hide();

    setLayout(mainLayout);
    mainLayout->setSizeConstraint( QLayout::SetFixedSize );
}

void CommandDialog::createJTAGLayout(){
    JTAGTabWidget = new QTabWidget;
    DATAMOD = new QWidget;
    OSCINT = new QWidget;
    BIOIMPEDANCE = new QWidget;
    NEUAMPChannel = new QWidget;
    NEUAMP = new QWidget;
    TUNEADC = new QWidget;
    SPILLOVER = new QWidget;
    JTAGTabWidget->addTab(DATAMOD, tr("DATAMOD"));
    JTAGTabWidget->addTab(OSCINT,tr("OSCINT"));
    JTAGTabWidget->addTab(BIOIMPEDANCE,tr("BIOIMPEDANCE"));
    JTAGTabWidget->addTab(NEUAMPChannel,tr("NEUAMP Channel"));
    JTAGTabWidget->addTab(NEUAMP,tr("NEUAMP"));
    JTAGTabWidget->addTab(TUNEADC,tr("TUNEADC"));
    JTAGTabWidget->addTab(SPILLOVER,tr("SPILLOVER"));

    for(int i=0; i<112; i++){
        JTAG[i] = new QCheckBox(JTAGNames[i], this);
        connect(JTAG[i], SIGNAL(toggled(bool)), this, SLOT(on_JTAG_toggled()));
    }
    QVBoxLayout *DATAMODLayout = new QVBoxLayout;
    for(int i=0; i<5;i++){
        DATAMODLayout->addWidget(JTAG[i]);
    }
    DATAMOD->setLayout(DATAMODLayout);

    QVBoxLayout *OSCINTLayout = new QVBoxLayout;
    for(int i=5;i<10;i++){
        OSCINTLayout->addWidget(JTAG[i]);
    }
    OSCINT->setLayout(OSCINTLayout);

    QVBoxLayout *BIOIMPEDANCELayout = new QVBoxLayout;
    for(int i=10;i<17;i++){
        BIOIMPEDANCELayout->addWidget(JTAG[i]);
    }
    BIOIMPEDANCE->setLayout(BIOIMPEDANCELayout);

    QHBoxLayout *NEUAMPChannelLayout10 = new QHBoxLayout;
    QHBoxLayout *NEUAMPChannelLayout9 = new QHBoxLayout;
    QHBoxLayout *NEUAMPChannelLayout8 = new QHBoxLayout;
    QHBoxLayout *NEUAMPChannelLayout7 = new QHBoxLayout;
    QHBoxLayout *NEUAMPChannelLayout6 = new QHBoxLayout;
    QHBoxLayout *NEUAMPChannelLayout5 = new QHBoxLayout;
    QHBoxLayout *NEUAMPChannelLayout4 = new QHBoxLayout;
    QHBoxLayout *NEUAMPChannelLayout3 = new QHBoxLayout;
    QHBoxLayout *NEUAMPChannelLayout2 = new QHBoxLayout;
    QHBoxLayout *NEUAMPChannelLayout1 = new QHBoxLayout;
    for(int i=17;i<22;i++){
        NEUAMPChannelLayout1->addWidget(JTAG[i]);
        NEUAMPChannelLayout2->addWidget(JTAG[i+5]);
        NEUAMPChannelLayout3->addWidget(JTAG[i+10]);
        NEUAMPChannelLayout4->addWidget(JTAG[i+15]);
        NEUAMPChannelLayout5->addWidget(JTAG[i+20]);
        NEUAMPChannelLayout6->addWidget(JTAG[i+25]);
        NEUAMPChannelLayout7->addWidget(JTAG[i+30]);
        NEUAMPChannelLayout8->addWidget(JTAG[i+35]);
        NEUAMPChannelLayout9->addWidget(JTAG[i+40]);
        NEUAMPChannelLayout10->addWidget(JTAG[i+45]);
    }
    QVBoxLayout *NEUAMPChannelLayout = new QVBoxLayout;
    NEUAMPChannelLayout->addLayout(NEUAMPChannelLayout1);
    NEUAMPChannelLayout->addLayout(NEUAMPChannelLayout2);
    NEUAMPChannelLayout->addLayout(NEUAMPChannelLayout3);
    NEUAMPChannelLayout->addLayout(NEUAMPChannelLayout4);
    NEUAMPChannelLayout->addLayout(NEUAMPChannelLayout5);
    NEUAMPChannelLayout->addLayout(NEUAMPChannelLayout6);
    NEUAMPChannelLayout->addLayout(NEUAMPChannelLayout7);
    NEUAMPChannelLayout->addLayout(NEUAMPChannelLayout8);
    NEUAMPChannelLayout->addLayout(NEUAMPChannelLayout9);
    NEUAMPChannelLayout->addLayout(NEUAMPChannelLayout10);
    NEUAMPChannel->setLayout(NEUAMPChannelLayout);

    QHBoxLayout *NEUAMPLayout8 = new QHBoxLayout;
    QHBoxLayout *NEUAMPLayout7 = new QHBoxLayout;
    QHBoxLayout *NEUAMPLayout6 = new QHBoxLayout;
    QHBoxLayout *NEUAMPLayout5 = new QHBoxLayout;
    QHBoxLayout *NEUAMPLayout4 = new QHBoxLayout;
    QHBoxLayout *NEUAMPLayout3 = new QHBoxLayout;
    QHBoxLayout *NEUAMPLayout2 = new QHBoxLayout;
    QHBoxLayout *NEUAMPLayout1 = new QHBoxLayout;
    for(int i=67;i<71;i++){
        NEUAMPLayout1->addWidget(JTAG[i]);
        NEUAMPLayout2->addWidget(JTAG[i+4]);
        NEUAMPLayout3->addWidget(JTAG[i+8]);
        NEUAMPLayout4->addWidget(JTAG[i+12]);
        NEUAMPLayout5->addWidget(JTAG[i+16]);
        NEUAMPLayout6->addWidget(JTAG[i+20]);
        NEUAMPLayout7->addWidget(JTAG[i+24]);
        NEUAMPLayout8->addWidget(JTAG[i+28]);
    }

    QVBoxLayout *NEUAMPLayout = new QVBoxLayout;
    NEUAMPLayout->addLayout(NEUAMPLayout1);
    NEUAMPLayout->addLayout(NEUAMPLayout2);
    NEUAMPLayout->addLayout(NEUAMPLayout3);
    NEUAMPLayout->addLayout(NEUAMPLayout4);
    NEUAMPLayout->addLayout(NEUAMPLayout5);
    NEUAMPLayout->addLayout(NEUAMPLayout6);
    NEUAMPLayout->addLayout(NEUAMPLayout7);
    NEUAMPLayout->addLayout(NEUAMPLayout8);
    NEUAMP->setLayout(NEUAMPLayout);

    QVBoxLayout *TUNEADCLayout = new QVBoxLayout;
    for(int i=99;i<104;i++){
        TUNEADCLayout->addWidget(JTAG[i]);
    }
    TUNEADC->setLayout(TUNEADCLayout);

    QVBoxLayout *SPILLOVERLayout = new QVBoxLayout;
    for(int i=104;i<112;i++){
        SPILLOVERLayout->addWidget(JTAG[i]);
    }
    SPILLOVER->setLayout(SPILLOVERLayout);
}

void CommandDialog::on_sendCommand_clicked(){
    for(int i=0;i<10;i++){
        if(SELCHN[i]->isChecked()){
            NeutrinoChannel->setChannelState(i, true);
        }
        else{
            NeutrinoChannel->setChannelState(i, false);
        }
    }
    if(ModeComboBox->currentIndex() == 10){
        runFullBioImpedanceMeasurement();
    }
    else if(ModeComboBox->currentIndex() == 11){
        runAutoRangedBioImpedanceMeasurement();
    }
    else{
        if(NeutrinoSerial->isConnected()){
            NeutrinoSerial->writeCommand(NeutrinoCommand->constructCommand());
        }
        if(socketNeutrino->isConnected()){
            socketNeutrino->writeCommand(NeutrinoCommand->constructCommand());
        }
    }
    if(ModeComboBox->currentIndex() == 5 || ModeComboBox->currentIndex() == 6 ||
            ModeComboBox->currentIndex() == 7 || ModeComboBox->currentIndex() == 8 ||
            ModeComboBox->currentIndex() == 9){
        MeasurementDialog measurementDialog(NeutrinoSerial, socketNeutrino);
        measurementDialog.exec();
        on_chipReset_clicked();
    }
    updateHeader();
}

void CommandDialog::runFullBioImpedanceMeasurement(){
    bioImpedanceData.clear();
    QTimer::singleShot(0, [=] {
        on_startBioImpedanceMeasurement();
        if(NeutrinoSerial->isConnected()){
            NeutrinoSerial->writeCommand(NeutrinoCommand->constructCommand());
        }
        if(socketNeutrino->isConnected()){
            socketNeutrino->writeCommand(NeutrinoCommand->constructCommand());
        }
    });
    QTimer::singleShot(3000, [=] {
        BioImpData[5]->setChecked(true);    // STEP 2: Check ETIRST
        on_BioImp_toggled();
        if(NeutrinoSerial->isConnected()){
            qDebug() << "Reset On:" << (quint8) NeutrinoSerial->getCurrentByte();
            bioImpedanceData.append(NeutrinoSerial->getCurrentByte());
            NeutrinoSerial->writeCommand(NeutrinoCommand->constructCommand());
        }
        if(socketNeutrino->isConnected()){
            qDebug() << "Reset On:" << (quint8) socketNeutrino->getCurrentByte();
            bioImpedanceData.append(socketNeutrino->getCurrentByte());
            socketNeutrino->writeCommand(NeutrinoCommand->constructCommand());
        }
    });
    for(int i = 0; i < 10; i++){
        for(int k = 0; k < 4; k++){
            for(int j = 0; j < 2; j++){
                QTimer::singleShot((6000 + 6000*k + 24000*i), [=] {
                    setCurrentType((CURRENT_TYPE) k);
                });
                QTimer::singleShot((6000 + 6000*k + 24000*i + 3000*j), [=] {
                    setInlineQuad(j);
                    BioImpData[5]->setChecked(true);    // STEP 2: Check ETIRST
                    on_BioImp_toggled();
                    on_JTAG_toggled();
                    if(NeutrinoSerial->isConnected()){
                        qDebug() << (quint8) NeutrinoSerial->getCurrentByte();
                        bioImpedanceData.append(NeutrinoSerial->getCurrentByte());
                        NeutrinoSerial->writeCommand(NeutrinoCommand->constructCommand());
                    }
                    if(socketNeutrino->isConnected()){
                        qDebug() << (quint8) socketNeutrino->getCurrentByte();
                        bioImpedanceData.append(socketNeutrino->getCurrentByte());
                        socketNeutrino->writeCommand(NeutrinoCommand->constructCommand());
                    }
                });
                QTimer::singleShot((6000 + 24000*i), [=] {
                    setBioImpedanceChannel(i);
                });
            }
        }
    }
    QTimer::singleShot(246000, [=] {
        if(NeutrinoSerial->isConnected()){
            qDebug() << (quint8) NeutrinoSerial->getCurrentByte();
            bioImpedanceData.append(NeutrinoSerial->getCurrentByte());
        }
        if(socketNeutrino->isConnected()){
            qDebug() << (quint8) socketNeutrino->getCurrentByte();
            bioImpedanceData.append(socketNeutrino->getCurrentByte());
        }
        on_exitBioImpedanceMeasurement();
        BioImpedance bioImpedance(bioImpedanceData, bioImpGain);
        qDebug() << "Total bytes:" << bioImpedanceData.size();
    });
}

void CommandDialog::setBioImpedanceChannel(int channel){
    switch (channel){
        case 0:{
            BioImpData[0]->setChecked(false);
            BioImpData[1]->setChecked(false);
            BioImpData[2]->setChecked(false);
            BioImpData[3]->setChecked(false);
            break;
        }
        case 1:{
            BioImpData[0]->setChecked(true);
            BioImpData[1]->setChecked(false);
            BioImpData[2]->setChecked(false);
            BioImpData[3]->setChecked(false);
            break;
        }
        case 2:{
            BioImpData[0]->setChecked(false);
            BioImpData[1]->setChecked(true);
            BioImpData[2]->setChecked(false);
            BioImpData[3]->setChecked(false);
            break;
        }
        case 3:{
            BioImpData[0]->setChecked(true);
            BioImpData[1]->setChecked(true);
            BioImpData[2]->setChecked(false);
            BioImpData[3]->setChecked(false);
            break;
        }
        case 4:{
            BioImpData[0]->setChecked(false);
            BioImpData[1]->setChecked(false);
            BioImpData[2]->setChecked(true);
            BioImpData[3]->setChecked(false);
            break;
        }
        case 5:{
            BioImpData[0]->setChecked(true);
            BioImpData[1]->setChecked(false);
            BioImpData[2]->setChecked(true);
            BioImpData[3]->setChecked(false);
            break;
        }
        case 6:{
            BioImpData[0]->setChecked(false);
            BioImpData[1]->setChecked(true);
            BioImpData[2]->setChecked(true);
            BioImpData[3]->setChecked(false);
            break;
        }
        case 7:{
            BioImpData[0]->setChecked(true);
            BioImpData[1]->setChecked(true);
            BioImpData[2]->setChecked(true);
            BioImpData[3]->setChecked(false);
            break;
        }
        case 8:{
            BioImpData[0]->setChecked(false);
            BioImpData[1]->setChecked(false);
            BioImpData[2]->setChecked(false);
            BioImpData[3]->setChecked(true);
            break;
        }
        case 9:{
            BioImpData[0]->setChecked(true);
            BioImpData[1]->setChecked(false);
            BioImpData[2]->setChecked(false);
            BioImpData[3]->setChecked(true);
            break;
        }
    }
    on_BioImp_toggled();
}

void CommandDialog::setInlineQuad(int input){
    if(input%2 != 0){
        JTAG[16]->setChecked(true);
        NeutrinoCommand->setJTAGbit(16);
    }
    else{
        JTAG[16]->setChecked(false);
        NeutrinoCommand->clearJTAGbit(16);
    }
}

void CommandDialog::setCurrentType(CURRENT_TYPE type){
    switch (type){
        case SMALL_CURRENT:{
            JTAG[14]->setChecked(false);
            JTAG[15]->setChecked(true);
            break;
        }
        case MEDIUM_CURRENT:{
            JTAG[14]->setChecked(true);
            JTAG[15]->setChecked(false);
            break;
        }
        case LARGE_CURRENT:{
            JTAG[14]->setChecked(true);
            JTAG[15]->setChecked(true);
            break;
        }
        default:{
            JTAG[14]->setChecked(false);
            JTAG[15]->setChecked(false);
            BioImpData[5]->setChecked(false);    // STEP 2: Check ETIRST
            on_BioImp_toggled();
            break;
        }
    }
    on_JTAG_toggled();
}

void CommandDialog::setBioImpedanceGain(GAIN gain){
    switch (gain){
        case MEDIUM_GAIN:{
            JTAG[11]->setChecked(false);            // A1 = 0
            JTAG[12]->setChecked(true);             // A0 = 1
            JTAG[13]->setChecked(false);            // SEL_LNA = 0
            qDebug() << "Setting Gain... SEL_LNA = A1 = 0 | A0 = 1";
            break;
        }
        case HIGH_GAIN:{
            JTAG[11]->setChecked(true);             // A1 = 1
            JTAG[12]->setChecked(true);             // A0 = 1
            JTAG[13]->setChecked(false);            // SEL_LNA = 0
            qDebug() << "Setting Gain... SEL_LNA = 0 | A1 = A0 = 1";
            break;
        }
        case SUPER_HIGH_GAIN:{
            JTAG[11]->setChecked(true);             // A1 = 1
            JTAG[12]->setChecked(true);             // A0 = 1
            JTAG[13]->setChecked(true);             // SEL_LNA = 1
            qDebug() << "Setting Gain... SEL_LNA = A1 = A0 = 1";
            break;
        }
        default:{
            JTAG[11]->setChecked(false);            // A0 = 0
            JTAG[12]->setChecked(false);            // A1 = 0
            JTAG[13]->setChecked(false);            // SEL_LNA = 0
            qDebug() << "Setting Gain... SEL_LNA = A1 = A0 = 0";
            break;
        }
    }
    on_JTAG_toggled();
}

void CommandDialog::runAutoRangedBioImpedanceMeasurement(){
    // Initialise default command with A0 = 1
    bioImpedance = new BioImpedance;
    setBioImpedanceGain(MEDIUM_GAIN);

    measureResetVoltage(bioImpedance);

    // Turn off Reset voltage command
    BioImpData[5]->setChecked(true);    // STEP 2: Check ETIRST
    on_BioImp_toggled();

    for(int i = 0; i < 10; i++){
        runMediumCurrentMeasurement(i, bioImpedance);
        runLowCurrentMeasurement(i, bioImpedance);
        runHighCurrentMeasurement(i, bioImpedance);
        if(bioImpedance->getHighCurrentHighGainQuad(i) || bioImpedance->getHighCurrentHighGainInline(i)){
            runHighCurrentHighGainMeasurement(i, bioImpedance);
        }
    }

    for(int i = 0; i < 10; i++){
        qDebug() << "Final Inline Value for Channel " << i + 1 << " is Voltage: " << bioImpedance->getFinalInline(i);
        qDebug() << "Final Quad Value for Channel " << i + 1 << " is Voltage: " << bioImpedance->getFinalQuad(i);
    }

    on_exitBioImpedanceMeasurement();
}

void CommandDialog::measureResetVoltage(BioImpedance *bioImpedance){
    on_startBioImpedanceMeasurement();

    // Send reset voltage command
    if(NeutrinoSerial->isConnected()){
        NeutrinoSerial->writeCommand(NeutrinoCommand->constructCommand());
    }
    if(socketNeutrino->isConnected()){
        socketNeutrino->writeCommand(NeutrinoCommand->constructCommand());
    }

    delay3seconds();

    // Read reset Voltage
    if(NeutrinoSerial->isConnected()){
        qDebug() << "Reset Voltage:" << (((quint8) NeutrinoSerial->getCurrentByte() / 255.0) * 1.2);
        bioImpedance->setResetVoltage(NeutrinoSerial->getCurrentByte());
    }
    if(socketNeutrino->isConnected()){
        qDebug() << "Reset Voltage:" << (((quint8) socketNeutrino->getCurrentByte() / 255.0) * 1.2);
        bioImpedance->setResetVoltage(socketNeutrino->getCurrentByte());
    }
}

void CommandDialog::runMediumCurrentMeasurement(int i, BioImpedance *bioImpedance){
    // Set Medium Current
    setCurrentType(MEDIUM_CURRENT);
    setBioImpedanceGain(MEDIUM_GAIN);
    setBioImpedanceChannel(i);
    for(int j = 0; j < 2; j++){
        setInlineQuad(j);
        if(NeutrinoSerial->isConnected()){
            NeutrinoSerial->writeCommand(NeutrinoCommand->constructCommand());
        }
        if(socketNeutrino->isConnected()){
            socketNeutrino->writeCommand(NeutrinoCommand->constructCommand());
        }

        delay3seconds();
        if(j == 0){
            double inlineVoltage;
            if(NeutrinoSerial->isConnected()){
                inlineVoltage = (((quint8)NeutrinoSerial->getCurrentByte()/255.0)*1.2);
            }
            if(socketNeutrino->isConnected()){
                inlineVoltage = (((quint8)socketNeutrino->getCurrentByte()/255.0)*1.2);
            }
            if((inlineVoltage - bioImpedance->getResetVoltage()) < ((resolution/255.0)*1.2)){
                bioImpedance->setHighCurrentInline(i, true);
                qDebug() << "Channel " << i+1 << "'s Inline Voltage is not enough at Medium Current, Voltage is " << inlineVoltage;
            }
            else{
                bioImpedance->setTempInline(i, inlineVoltage);
                qDebug() << "Channel " << i+1 << "'s Inline Voltage may be enough, Voltage is " << inlineVoltage;
                bioImpedance->setLowCurrentInline(i, true);
            }
        }
        else{
            double quadVoltage;
            if(NeutrinoSerial->isConnected()){
                quadVoltage = (((quint8)NeutrinoSerial->getCurrentByte()/255.0)*1.2);
            }
            if(socketNeutrino->isConnected()){
                quadVoltage = (((quint8)socketNeutrino->getCurrentByte()/255.0)*1.2);
            }
            if((bioImpedance->getResetVoltage() - quadVoltage) < ((resolution/255.0)*1.2)){
                bioImpedance->setHighCurrentQuad(i, true);
                qDebug() << "Channel " << i+1 << "'s Quad Voltage is not enough at Medium Current, Voltage is " << quadVoltage;
            }
            else{
                bioImpedance->setTempQuad(i, quadVoltage);
                qDebug() << "Channel " << i+1 << "'s Quad Voltage may be enough, Voltage is " << quadVoltage;
                bioImpedance->setLowCurrentQuad(i, true);
            }
        }
    }
}

void CommandDialog::runLowCurrentMeasurement(int i, BioImpedance *bioImpedance){
    // Set Small Current
    setCurrentType(SMALL_CURRENT);
    setBioImpedanceGain(MEDIUM_GAIN);
    qDebug() << "Running small current measurement";
    setBioImpedanceChannel(i);

    // Measurement for SMALL CURRENT INLINE
    if(bioImpedance->getLowCurrentInline(i)){
        setInlineQuad(0);
        if(NeutrinoSerial->isConnected()){
            NeutrinoSerial->writeCommand(NeutrinoCommand->constructCommand());
        }
        if(socketNeutrino->isConnected()){
            socketNeutrino->writeCommand(NeutrinoCommand->constructCommand());
        }
        delay3seconds();
        double inlineVoltage;
        if(NeutrinoSerial->isConnected()){
            inlineVoltage = (((quint8)NeutrinoSerial->getCurrentByte()/255.0)*1.2);
        }
        if(socketNeutrino->isConnected()){
            inlineVoltage = (((quint8)socketNeutrino->getCurrentByte()/255.0)*1.2);
        }
        if((inlineVoltage - bioImpedance->getResetVoltage()) < ((resolution/255.0)*1.2)){
            bioImpedance->setFinalInline(i, bioImpedance->getTempInline(i));
            qDebug() << "Channel " << i+1 << "'s Inline voltage finalised at Medium Current, Voltage: " << bioImpedance->getTempInline(i);
        }
        else{
            bioImpedance->setFinalInline(i, inlineVoltage);
            qDebug() << "Channel " << i+1 << "'s Inline voltage finalised at Small Current, Voltage: " << inlineVoltage;
        }
    }

    // Measurement for SMALL CURRENT QUAD
    if(bioImpedance->getLowCurrentQuad(i)){
        setInlineQuad(1);
        if(NeutrinoSerial->isConnected()){
            NeutrinoSerial->writeCommand(NeutrinoCommand->constructCommand());
        }
        if(socketNeutrino->isConnected()){
            socketNeutrino->writeCommand(NeutrinoCommand->constructCommand());
        }
        delay3seconds();
        double quadVoltage;
        if(NeutrinoSerial->isConnected()){
            quadVoltage = (((quint8)NeutrinoSerial->getCurrentByte()/255.0)*1.2);
        }
        if(socketNeutrino->isConnected()){
            quadVoltage = (((quint8)socketNeutrino->getCurrentByte()/255.0)*1.2);
        }
        if((bioImpedance->getResetVoltage() - quadVoltage) < ((resolution/255.0)*1.2)){
            bioImpedance->setFinalQuad(i, bioImpedance->getTempQuad(i));
            qDebug() << "Channel " << i+1 << "'s Quad voltage finalised at Medium Current, Voltage: " << bioImpedance->getTempQuad(i);
        }
        else{
            bioImpedance->setFinalQuad(i, quadVoltage);
            qDebug() << "Channel " << i+1 << "'s Quad voltage finalised at Small Current, Voltage: " << quadVoltage;
        }
    }
}

void CommandDialog::runHighCurrentMeasurement(int i, BioImpedance *bioImpedance){
    // Set Large Current
    setCurrentType(LARGE_CURRENT);
    setBioImpedanceGain(MEDIUM_GAIN);
    qDebug() << "Running large current measurement";
    setBioImpedanceChannel(i);

    // Measurement for LARGE CURRENT INLINE
    if(bioImpedance->getHighCurrentInline(i)){
        setInlineQuad(0);
        if(NeutrinoSerial->isConnected()){
            NeutrinoSerial->writeCommand(NeutrinoCommand->constructCommand());
        }
        if(socketNeutrino->isConnected()){
            socketNeutrino->writeCommand(NeutrinoCommand->constructCommand());
        }
        delay3seconds();
        double inlineVoltage;
        if(NeutrinoSerial->isConnected()){
            inlineVoltage = (((quint8)NeutrinoSerial->getCurrentByte()/255.0)*1.2);
        }
        if(socketNeutrino->isConnected()){
            inlineVoltage = (((quint8)socketNeutrino->getCurrentByte()/255.0)*1.2);
        }
        if((inlineVoltage - bioImpedance->getResetVoltage()) < ((resolution/255.0)*1.2)){
            bioImpedance->setHighCurrentHighGainInline(i, true);
            qDebug() << "Channel " << i+1 << "'s Inline Voltage at High Current is still not enough, Voltage is " << inlineVoltage;
        }
        else{
            bioImpedance->setFinalInline(i, inlineVoltage);
            qDebug() << "Channel " << i+1 << "'s Inline Voltage finalised at High Current, Voltage: " << inlineVoltage;
        }
    }


    // Measurement for LARGE CURRENT QUAD
    if(bioImpedance->getHighCurrentQuad(i)){
        setInlineQuad(1);
        if(NeutrinoSerial->isConnected()){
            NeutrinoSerial->writeCommand(NeutrinoCommand->constructCommand());
        }
        if(socketNeutrino->isConnected()){
            socketNeutrino->writeCommand(NeutrinoCommand->constructCommand());
        }
        delay3seconds();
        double quadVoltage;
        if(NeutrinoSerial->isConnected()){
            quadVoltage = (((quint8)NeutrinoSerial->getCurrentByte()/255.0)*1.2);
        }
        if(socketNeutrino->isConnected()){
            quadVoltage = (((quint8)socketNeutrino->getCurrentByte()/255.0)*1.2);
        }
        if((bioImpedance->getResetVoltage() - quadVoltage) < ((resolution/255.0)*1.2)){
            bioImpedance->setHighCurrentHighGainQuad(i, true);
            qDebug() << "Channel " << i+1 << "'s Quad Voltage at High Current is still not enough, Voltage is " << quadVoltage;
        }
        else{
            bioImpedance->setFinalQuad(i, quadVoltage);
            qDebug() << "Channel " << i+1 << "'s quad Voltage finalised at High Current, Voltage: " << quadVoltage;
        }
    }
}

void CommandDialog::runHighCurrentHighGainMeasurement(int i, BioImpedance *bioImpedance){
    // Set Large Current
    setCurrentType(LARGE_CURRENT);
    setBioImpedanceGain(SUPER_HIGH_GAIN);
    setBioImpedanceChannel(i);

    measureResetVoltage(bioImpedance);

    // Measurement for LARGE CURRENT INLINE
    if(bioImpedance->getHighCurrentHighGainInline(i)){
        setInlineQuad(0);
        if(NeutrinoSerial->isConnected()){
            NeutrinoSerial->writeCommand(NeutrinoCommand->constructCommand());
        }
        if(socketNeutrino->isConnected()){
            socketNeutrino->writeCommand(NeutrinoCommand->constructCommand());
        }
        delay3seconds();
        double inlineVoltage;
        if(NeutrinoSerial->isConnected()){
            inlineVoltage = (((quint8)NeutrinoSerial->getCurrentByte()/255.0)*1.2);
        }
        if(socketNeutrino->isConnected()){
            inlineVoltage = (((quint8)socketNeutrino->getCurrentByte()/255.0)*1.2);
        }
        bioImpedance->setFinalInline(i, inlineVoltage);
        qDebug() << "Channel " << i+1 << "'s Inline Voltage finalised at High Current High Gain, Voltage: " << inlineVoltage;
    }


    // Measurement for LARGE CURRENT QUAD
    if(bioImpedance->getHighCurrentHighGainQuad(i)){
        setInlineQuad(1);
        if(NeutrinoSerial->isConnected()){
            NeutrinoSerial->writeCommand(NeutrinoCommand->constructCommand());
        }
        if(socketNeutrino->isConnected()){
            socketNeutrino->writeCommand(NeutrinoCommand->constructCommand());
        }
        delay3seconds();
        double quadVoltage;
        if(NeutrinoSerial->isConnected()){
            quadVoltage = (((quint8)NeutrinoSerial->getCurrentByte()/255.0)*1.2);
        }
        if(socketNeutrino->isConnected()){
            quadVoltage = (((quint8)socketNeutrino->getCurrentByte()/255.0)*1.2);
        }
        bioImpedance->setFinalQuad(i, quadVoltage);
        qDebug() << "Channel " << i+1 << "'s quad Voltage finalised at High Current High Gain, Voltage: " << quadVoltage;
    }
}

void CommandDialog::delay3seconds(){
    delayThread->start();
    while (!delayThread->isFinished())
            QCoreApplication::processEvents();
    delayThread->wait();
}

void CommandDialog::on_startBioImpedanceMeasurement(){
    mboxWait->show();
    setWindowTitle(tr("Neutrino Command - Running Auto BioImpedance Measurement"));
    JTAG[10]->setChecked(true);
    NeutrinoCommand->setJTAGbit(10);    //PDN of BIOIMPEDANCE
    JTAG[82]->setChecked(true);
    NeutrinoCommand->setJTAGbit(82);    //OFFCMFB
    JTAG[89]->setChecked(true);
    NeutrinoCommand->setJTAGbit(89);    //INVBIASRESET
    JTAG[96]->setChecked(true);
    NeutrinoCommand->setJTAGbit(96);    //PDS2
    JTAG[97]->setChecked(true);
    NeutrinoCommand->setJTAGbit(97);    //PDS1
    BioImpData[0]->setChecked(false);
    BioImpData[1]->setChecked(false);
    BioImpData[2]->setChecked(false);
    BioImpData[3]->setChecked(false);
    BioImpData[4]->setChecked(true);    //ENBIOPIN
    BioImpData[5]->setChecked(false);
    on_JTAG_toggled();
    on_BioImp_toggled();
}

void CommandDialog::on_exitBioImpedanceMeasurement(){
    JTAG[10]->setChecked(false);
    NeutrinoCommand->clearJTAGbit(10);
    JTAG[14]->setChecked(false);
    NeutrinoCommand->clearJTAGbit(14);
    JTAG[15]->setChecked(false);
    NeutrinoCommand->clearJTAGbit(15);
    JTAG[16]->setChecked(false);
    NeutrinoCommand->clearJTAGbit(16);
    JTAG[82]->setChecked(false);
    NeutrinoCommand->clearJTAGbit(82);    // OFFCMFB
    JTAG[89]->setChecked(false);
    NeutrinoCommand->clearJTAGbit(89);    //INVBIASRESET
    JTAG[96]->setChecked(false);
    NeutrinoCommand->clearJTAGbit(96);    //PDS2
    JTAG[97]->setChecked(false);
    NeutrinoCommand->clearJTAGbit(97);    //PDS1
    BioImpData[0]->setChecked(false);
    BioImpData[1]->setChecked(false);
    BioImpData[2]->setChecked(false);
    BioImpData[3]->setChecked(false);
    BioImpData[4]->setChecked(false);
    BioImpData[5]->setChecked(false);
    on_chipReset_clicked();
    on_JTAG_toggled();
    on_BioImp_toggled();
    setWindowTitle(tr("Neutrino Command"));
    mboxWait->hide();
}

void CommandDialog::updateHeader(){
    QString temp;
    for(int i = 0; i < 2; i++){
        temp.append("Neutrino Setting " + QString::number(i) + " :,");
        for(int j = 0; j < 7; j++){
            temp.append(QString::number(NeutrinoCommand->getJTAG(i*7+j)) + " ,");
        }
        temp.append("\n");
    }
    dataProcessor->setHeader(temp);
}

void CommandDialog::on_chipReset_clicked(){
    if(socketNeutrino->isConnected()){
        socketNeutrino->writeCommand(NeutrinoCommand->resetCommand());
    }
    if(NeutrinoSerial->isConnected()){
        NeutrinoSerial->writeCommand(NeutrinoCommand->resetCommand());
    }
}

void CommandDialog::on_CMReset_clicked(){
    qDebug() << "CM Resetting.. ";
    JTAG[93]->setChecked(true);
    NeutrinoCommand->setJTAGbit(93);
    if(NeutrinoSerial->isConnected()){
        NeutrinoSerial->writeCommand(NeutrinoCommand->constructCommand());
    }
    if(socketNeutrino->isConnected()){
        socketNeutrino->writeCommand(NeutrinoCommand->constructCommand());
    }
    qDebug() << "CM Unresetting.. ";
    JTAG[93]->setChecked(false);
    NeutrinoCommand->clearJTAGbit(93);
    if(NeutrinoSerial->isConnected()){
        NeutrinoSerial->writeCommand(NeutrinoCommand->constructCommand());
    }
    if(socketNeutrino->isConnected()){
        socketNeutrino->writeCommand(NeutrinoCommand->constructCommand());
    }
}

void CommandDialog::on_shortRefToGND_clicked(){
    if(!JTAG[67]->isChecked()){
        qDebug() << "Setting REFPWRDWN";
        JTAG[67]->setChecked(true);
        NeutrinoCommand->setJTAGbit(67);
        if(NeutrinoSerial->isConnected()){
            NeutrinoSerial->writeCommand(NeutrinoCommand->constructCommand());
        }
        if(socketNeutrino->isConnected()){
            socketNeutrino->writeCommand(NeutrinoCommand->constructCommand());
        }
    }
    else{
        qDebug() << "Unsetting REFPWRDWN";
        JTAG[67]->setChecked(false);
        NeutrinoCommand->clearJTAGbit(67);
        if(NeutrinoSerial->isConnected()){
            NeutrinoSerial->writeCommand(NeutrinoCommand->constructCommand());
        }
        if(socketNeutrino->isConnected()){
            socketNeutrino->writeCommand(NeutrinoCommand->constructCommand());
        }
    }
}

void CommandDialog::on_JTAG_toggled(){
    for(int i=0;i<112;i++){
        if(JTAG[i]->isChecked()){
            NeutrinoCommand->setJTAGbit(i);
        }
        else{
            NeutrinoCommand->clearJTAGbit(i);
        }
    }
    if(JTAG[13]->isChecked()){          // SEL_LNA GAIN X40
        if(JTAG[12]->isChecked()){      // JTAG[12] = A0 | JTAG[11] = A1
            if(JTAG[11]->isChecked()){  // A0 = A1 = 1
                bioImpGain = 40.0 * 8.0;
            }
            else{                       // A0 = 1, A1 = 0
                bioImpGain = 40.0 * 4.0;
            }
        }
        else{
            if(JTAG[11]->isChecked()){  // A0 = 0, A1 = 1
                bioImpGain = 40.0 * 6.0;
            }
            else{                       // A0 = A1 = 0
                bioImpGain = 40.0 * 2.0;
            }
        }
    }
    else{
        if(JTAG[12]->isChecked()){      // JTAG[12] = A0 | JTAG[11] = A1
            if(JTAG[11]->isChecked()){  // A0 = A1 = 1
                bioImpGain = 20.0 * 8.0;
            }
            else{                       // A0 = 1, A1 = 0
                bioImpGain = 20.0 * 4.0;
            }
        }
        else{
            if(JTAG[11]->isChecked()){  // A0 = 0, A1 = 1
                bioImpGain = 20.0 * 6.0;
            }
            else{                       // A0 = A1 = 0
                bioImpGain = 20.0 * 2.0;
            }
        }
    }
}

void CommandDialog::on_JTAGextension_clicked(){
    if(JTAGTabWidget->isHidden()){
        JTAGreset->show();
        JTAGTabWidget->show();
        JTAGextension->setText(tr("Hide JTAG"));
    }
    else{
        JTAGreset->hide();
        JTAGTabWidget->hide();
        JTAGextension->setText(tr("Show JTAG"));
    }
}

void CommandDialog::on_JTAGreset_clicked(){
    loadDefault();
}

void CommandDialog::on_SELALL_clicked(){
    for(int i=0;i<10;i++){
        SELCHN[i]->setChecked(true);
    }
}

void CommandDialog::on_SELNONE_clicked(){
    for(int i=0;i<10;i++){
        SELCHN[i]->setChecked(false);
    }
}

void CommandDialog::on_amplifierSelectAll_clicked(){
    for(int i=0;i<10;i++){
        amplifierSelect[i]->setChecked(true);
    }
}

void CommandDialog::on_amplifierSelectNone_clicked(){
    for(int i=0;i<10;i++){
        amplifierSelect[i]->setChecked(false);
    }
}

void CommandDialog::on_SELCHN_toggled(){

}

void CommandDialog::on_amplifierSelect_toggled(){
    for(int i = 0; i < 10; i++){
        if(amplifierSelect[i]->isChecked()){
            JTAG[(66-i*5)]->setChecked(false);
        }
        else{
            JTAG[(66-i*5)]->setChecked(true);
        }
    }
}

void CommandDialog::on_Mode_Changed(int Mode){
    NeutrinoCommand->setOPMode(Mode);
    qDebug() << Mode;
    if(Mode == 5 || Mode == 6){
        JTAG[94]->setChecked(true);
        NeutrinoCommand->setJTAGbit(94);
    }
    else{
        JTAG[94]->setChecked(false);
        NeutrinoCommand->clearJTAGbit(94);
    }
    if(Mode == 7 || Mode == 8 || Mode == 10 || Mode == 11){
        JTAG[10]->setChecked(true);
        NeutrinoCommand->setJTAGbit(10);
        BioImpData[4]->setChecked(true);
        on_BioImp_toggled();
    }
    else{
        JTAG[10]->setChecked(false);
        NeutrinoCommand->clearJTAGbit(10);
        BioImpData[4]->setChecked(false);
        on_BioImp_toggled();
    }
}

void CommandDialog::on_ChipID_Changed(int IDnum){
    NeutrinoCommand->setChipID(IDnum);
}

void CommandDialog::on_BioImp_toggled(){
    for(int i=0;i<6;i++){
        if(BioImpData[i]->isChecked()){
            NeutrinoCommand->setBioImpBit(i);
        }
        else{
            NeutrinoCommand->clearBioImpBit(i);
        }
    }
}

void CommandDialog::on_DCL_toggled(){
    if(Enter->isChecked()){
        NeutrinoCommand->setDCLMode(DCL_ENTER);
    }
    else if(Exit->isChecked()){
        NeutrinoCommand->setDCLMode(DCL_EXIT);
    }
}

void CommandDialog::on_BER_textEdited(){
    for(int i=0;i<8;i++){
        NeutrinoCommand->updateBER(i, BER_byte[i]->text());
    }
}

void CommandDialog::loadlastCommand(){
    qDebug() << "Loading from last command";
    ModeComboBox->setCurrentIndex(NeutrinoCommand->getOPMode());
    CIDComboBox->setCurrentIndex(NeutrinoCommand->getChipID());
    quint8 lastBioImp = NeutrinoCommand->getBioImp();
    bool *lastSELCHN;
    lastSELCHN = NeutrinoChannel->getChannelState_Bool();
    quint8 lastJTAGarray[14];
    if(NeutrinoCommand->getDCLMode() == DCL_EXIT){
        Exit->setChecked(true);
        Enter->setChecked(false);
    }
    else{
        Exit->setChecked(false);
        Enter->setChecked(true);
    }
    for(int i=0;i<14;i++){
        lastJTAGarray[i] = NeutrinoCommand->getJTAG(i);
    }
    for(int i=0;i<112;i++){
        if(lastJTAGarray[i/8] & 1<<(i%8)){
            JTAG[i]->setChecked(true);
        }
    }
    for(int i=0;i<6;i++){
        if(lastBioImp & 1<< i){
            BioImpData[i]->setChecked(true);
        }
    }
    for(int i=0;i<10;i++){
        if(lastSELCHN[i]){
            SELCHN[i]->setChecked(true);
        }
    }
}

void CommandDialog::loadDefault(){
    ModeComboBox->setCurrentIndex(2);
    on_Mode_Changed(2);
    for(int i=0;i<112;i++){
        JTAG[i]->setChecked(false);
    }
    for(int i=84;i<87;i++){         //Default checked JTAG bits (INTRES3, INTRES2, INTRES1)
        JTAG[i]->setChecked(true);
        NeutrinoCommand->setJTAGbit(i);
    }
//    for(int i=5;i<10;i++){
//        JTAG[i]->setChecked(true);
//        NeutrinoCommand->setJTAGbit(i);
//    }
    JTAG[102]->setChecked(true);
    NeutrinoCommand->setJTAGbit(102);

    JTAG[5]->setChecked(true);
    NeutrinoCommand->setJTAGbit(6);
    JTAG[7]->setChecked(true);
    NeutrinoCommand->setJTAGbit(8);

    for(int i = 0; i < 10; i++){
        JTAG[(66-i*5)]->setChecked(true);
    }

//    JTAG[8]->setChecked(false);
//    JTAG[9]->setChecked(false);
}
