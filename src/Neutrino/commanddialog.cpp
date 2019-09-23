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
    NeutrinoCommand->havelastCommand()? loadlastCommand() : loadDefault();

    mboxWait = new QMessageBox;
    mboxWait->setText("Please wait... \nMeasurement in progress...");
    mboxWait->setStandardButtons(0);

    delayThread = new DelayThread();
    QThread *thread = new QThread;
    delayThread->moveToThread(thread);

    connect(this, SIGNAL(closeWaitingMBox()), this, SLOT(on_exitBioImpedanceMeasurement()));
//    on_sendCommand_clicked();
}

CommandDialog::~CommandDialog(){
    NeutrinoCommand->setlastCommand(true);
}

void CommandDialog::createLayout(){
    ModeLabel = new QLabel;
    ModeLabel->setText("Mode");
    ModeComboBox = new QComboBox;
    for(int i = 0; i < 12; i++){
        ModeComboBox->addItem(ModeComboBoxNames[i]);
    }

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

    QHBoxLayout *bioDelayLayout = new QHBoxLayout;
    QLabel *bioImpDelay = new QLabel(tr("Delay (Milli-Seconds):"));
    bioImpDelay->setFixedWidth(100);
    bioImpDelayLineEdit = new QLineEdit;
    bioImpDelayLineEdit->setText("1000");
    bioImpDelayLineEdit->setFixedWidth(50);
    bioDelayLayout->addWidget(bioImpDelay);
    bioDelayLayout->addWidget(bioImpDelayLineEdit);
    BioImpLayout->addLayout(bioDelayLayout);

    bioImpMapper = new QSignalMapper(this);
    connect(bioImpMapper, SIGNAL(mapped(int)), this, SLOT(on_BioImp_toggled(int)));
    for(int i=0;i<6;i++){
        BioImpData[i] = new QCheckBox;
        BioImpData[i]->setText(BioImpNames[i]);
        bioImpMapper->setMapping(BioImpData[i], i);
        BioImpLayout->addWidget(BioImpData[i]);
        connect(BioImpData[i], SIGNAL(toggled(bool)), bioImpMapper, SLOT(map()));
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

    QLabel *baudLabel = new QLabel("Baud Rate:");
    baudRateComboBox = new QComboBox;
    baudRateComboBox->addItem("19200 bps");
    baudRateComboBox->addItem("38400 bps");

    connect(baudRateComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_baud_toggled()));

    CMReset = new QPushButton(tr("CM Reset"));
    connect(CMReset, SIGNAL(clicked(bool)), this, SLOT(on_CMReset_clicked()));

    shortRefToGND = new QPushButton(tr("Short Ref to GND"));
    connect(shortRefToGND, SIGNAL(clicked(bool)), this, SLOT(on_shortRefToGND_clicked()));

    ChipReset = new QPushButton(tr("Chip Reset"));
    connect(ChipReset, SIGNAL(clicked(bool)), this, SLOT(on_chipReset_clicked()));

    DCL->addWidget(DigComLoopback);
    DCL->addWidget(Exit);
    DCL->addWidget(Enter);
    DCL->addWidget(baudLabel);
    DCL->addWidget(baudRateComboBox);
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

    QVBoxLayout *AmplifierSelectLayout = new QVBoxLayout;
    QLabel *amplifierEnableLabel = new QLabel(tr("Amplifier Enable:"));
    AmplifierSelectLayout->addWidget(amplifierEnableLabel);
    amplifierSelectMapper = new QSignalMapper(this);
    connect(amplifierSelectMapper, SIGNAL(mapped(int)), this, SLOT(on_amplifierSelect_toggled(int)));
    for(int i = 0; i < 10; i++){
        amplifierSelect[i] = new QCheckBox("Amplifier " + QString::number(i+1), this);
        amplifierSelectMapper->setMapping(amplifierSelect[i], i);
        connect(amplifierSelect[i], SIGNAL(toggled(bool)), amplifierSelectMapper, SLOT(map()));
        AmplifierSelectLayout->addWidget(amplifierSelect[i]);
    }
    amplifierSelectAll = new QPushButton(tr("Select All"));
    amplifierSelectNone = new QPushButton(tr("Select None"));
    connect(amplifierSelectAll, SIGNAL(clicked(bool)), this, SLOT(on_amplifierSelectAll_clicked()));
    connect(amplifierSelectNone, SIGNAL(clicked(bool)), this, SLOT(on_amplifierSelectNone_clicked()));

    AmplifierSelectLayout->addWidget(amplifierSelectAll);
    AmplifierSelectLayout->addWidget(amplifierSelectNone);

    topLayout->addLayout(AmplifierSelectLayout);

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

    jtagMapper = new QSignalMapper(this);
    connect(jtagMapper, SIGNAL(mapped(int)), this, SLOT(on_JTAG_toggled(int)));
    for(int i=0; i<112; i++){
        JTAG[i] = new QCheckBox(JTAGNames[i], this);
        jtagMapper->setMapping(JTAG[i], i);
        connect(JTAG[i], SIGNAL(toggled(bool)), jtagMapper, SLOT(map()));
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

    QHBoxLayout *NEUAMPChannelLayout[10];
    QVBoxLayout *NEUAMPChannelMainLayout = new QVBoxLayout;
    for(int j = 0; j < 10; j++){
        NEUAMPChannelLayout[j] = new QHBoxLayout;
        for(int i=17;i<22;i++){
            NEUAMPChannelLayout[j]->addWidget(JTAG[i+j*5]);
        }
        NEUAMPChannelMainLayout->addLayout(NEUAMPChannelLayout[j]);
    }
    NEUAMPChannel->setLayout(NEUAMPChannelMainLayout);

    QHBoxLayout *NEUAMPLayout[8];
    QVBoxLayout *NEUAMPMainLayout = new QVBoxLayout;
    for(int j = 0; j < 8; j++){
        NEUAMPLayout[j] = new QHBoxLayout;
        for(int i=67;i<71;i++){
            NEUAMPLayout[j]->addWidget(JTAG[i+j*4]);
        }
        NEUAMPMainLayout->addLayout(NEUAMPLayout[j]);
    }
    NEUAMP->setLayout(NEUAMPMainLayout);

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
    int numChannels = 0;
    for(int i=0;i<10;i++){
        NeutrinoChannel->setChannelState(i, SELCHN[i]->isChecked());
        if(SELCHN[i]->isChecked()){
            numChannels++;
        }
    }
    NeutrinoChannel->setNumChannels(numChannels);
    if(ModeComboBox->currentIndex() == 10){
        runFullBioImpedanceMeasurement();
    }
    else if(ModeComboBox->currentIndex() == 11){
        runAutoRangedBioImpedanceMeasurement();
    }
    else{
        sendCommand(NeutrinoCommand->constructCommand());
    }
    if(NeutrinoCommand->constructCommand().at(6) == (char) WORDLENGTH_8){
        dataProcessor->setBitMode(WORDLENGTH_8);
        dataProcessor->setPlotEnabled(true);
        dataProcessor->setSamplingFreq((3000000.0/((14.0)*(NeutrinoChannel->getNumChannels()+2.0))));
        dataProcessor->clearallChannelData();
        NeutrinoChannel->setNumChannels(NeutrinoCommand->getNumChannels());
    }
    else if(NeutrinoCommand->constructCommand().at(6) == (char) WORDLENGTH_10){
        dataProcessor->setBitMode(WORDLENGTH_10);
        dataProcessor->setPlotEnabled(true);
        dataProcessor->setSamplingFreq((3000000.0/((14.0)*(NeutrinoChannel->getNumChannels()+2.0))));
        dataProcessor->clearallChannelData();
        NeutrinoChannel->setNumChannels(NeutrinoCommand->getNumChannels());
    }
    else {
        dataProcessor->setPlotEnabled(false);
    }
    if(ModeComboBox->currentIndex() > 4 && ModeComboBox->currentIndex() < 10 ){
        MeasurementDialog measurementDialog(NeutrinoSerial, socketNeutrino);
        measurementDialog.exec();
        on_chipReset_clicked();
    }
    updateHeader();
}

void CommandDialog::on_baud_toggled(){
    baudRateComboBox->currentIndex() == 0? NeutrinoSerial->setBaudRate(19200) : NeutrinoSerial->setBaudRate(38400);
}

void CommandDialog::runFullBioImpedanceMeasurement(){
    bioImpedanceData.clear();
    QTimer::singleShot(0, [=] {
        on_startBioImpedanceMeasurement();
        sendCommand(NeutrinoCommand->constructCommand());
    });
    QTimer::singleShot(3000, [=] {
        BioImpData[5]->setChecked(true);    // STEP 2: Check ETIRST
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
    for(int i = 0; i < 4; i++){
        BioImpData[i]->setChecked(false);
    }
    switch (channel){
        case 0:{
            break;
        }
        case 1:{
            BioImpData[0]->setChecked(true);
            break;
        }
        case 2:{
            BioImpData[1]->setChecked(true);
            break;
        }
        case 3:{
            BioImpData[0]->setChecked(true);
            BioImpData[1]->setChecked(true);
            break;
        }
        case 4:{
            BioImpData[2]->setChecked(true);
            break;
        }
        case 5:{
            BioImpData[0]->setChecked(true);
            BioImpData[2]->setChecked(true);
            break;
        }
        case 6:{
            BioImpData[1]->setChecked(true);
            BioImpData[2]->setChecked(true);
            break;
        }
        case 7:{
            BioImpData[0]->setChecked(true);
            BioImpData[1]->setChecked(true);
            BioImpData[2]->setChecked(true);
            break;
        }
        case 8:{
            BioImpData[3]->setChecked(true);
            break;
        }
        case 9:{
            BioImpData[0]->setChecked(true);
            BioImpData[3]->setChecked(true);
            break;
        }
    }
}

void CommandDialog::setInlineQuad(int input){
    setJTAGChecked((input%2 != 0), 16);
}

void CommandDialog::setCurrentType(CURRENT_TYPE type){
    switch (type){
        case SMALL_CURRENT:{
            setJTAGChecked(false, 14);
            setJTAGChecked(true, 15);
            break;
        }
        case MEDIUM_CURRENT:{
            setJTAGChecked(true, 14);
            setJTAGChecked(false, 15);
            break;
        }
        case LARGE_CURRENT:{
            setJTAGChecked(true, 14);
            setJTAGChecked(true, 15);
            break;
        }
        default:{
            setJTAGChecked(false, 14);
            setJTAGChecked(false, 15);
            BioImpData[5]->setChecked(false);    // STEP 2: Check ETIRST
            break;
        }
    }
}

void CommandDialog::setBioImpedanceGain(GAIN gain){
    switch (gain){
        case MEDIUM_GAIN:{
            setJTAGChecked(false, 11);            // A1 = 0
            setJTAGChecked(true, 12);             // A0 = 1
            setJTAGChecked(false, 13);            // SEL_LNA = 0
            qDebug() << "Setting Gain... SEL_LNA = A1 = 0 | A0 = 1";
            break;
        }
        case HIGH_GAIN:{
            setJTAGChecked(true, 11);             // A1 = 1
            setJTAGChecked(true, 12);             // A0 = 1
            setJTAGChecked(false, 13);            // SEL_LNA = 0
            qDebug() << "Setting Gain... SEL_LNA = 0 | A1 = A0 = 1";
            break;
        }
        case SUPER_HIGH_GAIN:{
            setJTAGChecked(true, 11);             // A1 = 1
            setJTAGChecked(true, 12);             // A0 = 1
            setJTAGChecked(true, 13);             // SEL_LNA = 1
            qDebug() << "Setting Gain... SEL_LNA = A1 = A0 = 1";
            break;
        }
        default:{
            setJTAGChecked(false, 11);            // A0 = 0
            setJTAGChecked(false, 12);            // A1 = 0
            setJTAGChecked(false, 13);            // SEL_LNA = 0
            qDebug() << "Setting Gain... SEL_LNA = A1 = A0 = 0";
            break;
        }
    }
}

void CommandDialog::runAutoRangedBioImpedanceMeasurement(){
    // Initialise default command with A0 = 1
    bioImpedance = new BioImpedance;

    QTime myTimer;
    myTimer.start();

    setBioImpedanceGain(MEDIUM_GAIN);
    measureResetVoltage(bioImpedance, MEDIUM_GAIN);
    setBioImpedanceGain(SUPER_HIGH_GAIN);
    measureResetVoltage(bioImpedance, SUPER_HIGH_GAIN);

    // Turn off Reset voltage command
    BioImpData[5]->setChecked(true);    // Check ETIRST true

    for(int i = 0; i < 10; i++){
        runCurrentMeasurement(i, bioImpedance, MEDIUM_GAIN, MEDIUM_CURRENT);
        runCurrentMeasurement(i, bioImpedance, MEDIUM_GAIN, SMALL_CURRENT);
        runCurrentMeasurement(i, bioImpedance, MEDIUM_GAIN, LARGE_CURRENT);
        if(bioImpedance->getHighCurrentHighGainQuad(i) || bioImpedance->getHighCurrentHighGainInline(i)){
            runCurrentMeasurement(i, bioImpedance, SUPER_HIGH_GAIN, LARGE_CURRENT);
        }
    }

    for(int i = 0; i < 10; i++){
        qDebug() << "Final Magnitude for Channel " << i + 1 << " is: " << bioImpedance->calculateMagnitude(i);
        qDebug() << "Final Phase for Channel " << i + 1 << " is: " << bioImpedance->calculatePhase(i);
        qDebug() << " ";
    }

    on_exitBioImpedanceMeasurement();

    int nMilliseconds = myTimer.elapsed();

    qDebug() << "Total time taken for BioImpedance Measurement: " << nMilliseconds << "ms";

    BioImpedanceDialog bioImpedanceDialog(bioImpedance);
    bioImpedanceDialog.exec();


}

void CommandDialog::measureResetVoltage(BioImpedance *bioImpedance, GAIN gain){
    on_startBioImpedanceMeasurement();

    // Send reset voltage command
    BioImpData[5]->setChecked(false);    // Check ETIRST false
    sendCommand(NeutrinoCommand->constructCommand());
    delaySeconds();

    // Read reset Voltage
    if(NeutrinoSerial->isConnected()){
        qDebug() << "Reset Voltage:" << (((quint8) NeutrinoSerial->getCurrentByte() / 255.0) * 1.2);
        bioImpedance->setResetVoltage(NeutrinoSerial->getCurrentByte(), gain);
    }
    if(socketNeutrino->isConnected()){
        qDebug() << "Reset Voltage:" << (((quint8) socketNeutrino->getCurrentByte() / 255.0) * 1.2);
        bioImpedance->setResetVoltage(socketNeutrino->getCurrentByte(), gain);
    }

    BioImpData[5]->setChecked(true);    // Check ETIRST true
}

void CommandDialog::bioReset(){
    BioImpData[5]->setChecked(false);   // Check ETIRST false
    sendCommand(NeutrinoCommand->constructCommand());
    delaySeconds();
    BioImpData[5]->setChecked(true);    // Check ETIRST true
}

void CommandDialog::runCurrentMeasurement(int i, BioImpedance *bioImpedance, GAIN bioGain, CURRENT_TYPE bioCurrent){
    QString tag;
    bool flagInline = false;
    bool flagQuad = false;

    setCurrentType(bioCurrent);
    setBioImpedanceGain(bioGain);
    setBioImpedanceChannel(i);

    switch (bioCurrent) {
    case SMALL_CURRENT:
        flagInline = bioImpedance->getLowCurrentInline(i);
        flagQuad = bioImpedance->getLowCurrentQuad(i);
        tag = "small current";
        break;
    case MEDIUM_CURRENT:
        flagInline = true;
        flagQuad = true;
        tag = "medium current";
        break;
    case LARGE_CURRENT:
        if(bioGain == MEDIUM_GAIN){
            flagInline = bioImpedance->getHighCurrentInline(i);
            flagQuad = bioImpedance->getHighCurrentQuad(i);
            tag = "large current";
        }
        else if(bioGain == SUPER_HIGH_GAIN){
            flagInline = bioImpedance->getHighCurrentHighGainInline(i);
            flagQuad = bioImpedance->getHighCurrentHighGainQuad(i);
            bioReset();
            tag = "large current high gain";
        }
        break;
    default:
        break;
    }
    qDebug() << "Running " << tag << " measurement";

    if(flagInline){
        setInlineQuad(0);
        sendCommand(NeutrinoCommand->constructCommand());
        delaySeconds();
        double inlineVoltage;
        if(NeutrinoSerial->isConnected()){
            inlineVoltage = (((quint8)NeutrinoSerial->getCurrentByte()/255.0)*1.2);
        }
        if(socketNeutrino->isConnected()){
            inlineVoltage = (((quint8)socketNeutrino->getCurrentByte()/255.0)*1.2);
        }
        if(bioCurrent == LARGE_CURRENT && bioGain == SUPER_HIGH_GAIN){
            bioImpedance->setFinalInline(i, inlineVoltage, SUPER_HIGH_GAIN, LARGE_CURRENT);
            qDebug() << "Channel " << i + 1 << "'s Inline Voltage finalised at High Current High Gain, Voltage: " << inlineVoltage;
            bioReset();
        }
        else{
            if((inlineVoltage - bioImpedance->getResetVoltage(bioGain)) < ((resolution/255.0)*1.2)){
                if(bioCurrent == SMALL_CURRENT){
                    bioImpedance->setFinalInline(i, bioImpedance->getTempInline(i), bioGain, bioCurrent);
                    qDebug() << "Channel " << i+1 << "'s Inline voltage finalised at Medium Current, Voltage: " << bioImpedance->getTempInline(i);
                }
                else if(bioCurrent == MEDIUM_CURRENT){
                    bioImpedance->setHighCurrentInline(i, true);
                    qDebug() << "Channel " << i+1 << "'s Inline Voltage is not enough at Medium Current, Voltage is " << inlineVoltage;
                }
                else if(bioCurrent == LARGE_CURRENT && bioGain == MEDIUM_GAIN){
                    bioImpedance->setHighCurrentHighGainInline(i, true);
                    qDebug() << "Channel " << i+1 << "'s Inline Voltage at High Current is still not enough, Voltage is " << inlineVoltage;
                }
            }
            else{
                if(bioCurrent == MEDIUM_CURRENT){
                    bioImpedance->setTempInline(i, inlineVoltage);
                    qDebug() << "Channel " << i + 1 << "'s Inline Voltage may be enough at Medium Current, Voltage is " << inlineVoltage;
                    bioImpedance->setLowCurrentInline(i, true);
                }
                else{
                    bioImpedance->setFinalInline(i, inlineVoltage, bioGain, bioCurrent);
                    qDebug() << "Channel " << i + 1 << "'s Inline voltage finalised at " << tag << ", Voltage: " << inlineVoltage;
                }
            }
        }
    }

    if(flagQuad){
        setInlineQuad(1);
        sendCommand(NeutrinoCommand->constructCommand());
        delaySeconds();
        double quadVoltage;
        if(NeutrinoSerial->isConnected()){
            quadVoltage = (((quint8)NeutrinoSerial->getCurrentByte()/255.0)*1.2);
        }
        if(socketNeutrino->isConnected()){
            quadVoltage = (((quint8)socketNeutrino->getCurrentByte()/255.0)*1.2);
        }
        if(bioCurrent == LARGE_CURRENT && bioGain == SUPER_HIGH_GAIN){
            bioImpedance->setFinalQuad(i, quadVoltage, SUPER_HIGH_GAIN, LARGE_CURRENT);
            qDebug() << "Channel " << i+1 << "'s quad Voltage finalised at High Current High Gain, Voltage: " << quadVoltage;
            bioReset();
        }
        else{
            if((bioImpedance->getResetVoltage(bioGain) - quadVoltage) < ((resolution/255.0)*1.2)){
                if(bioCurrent == SMALL_CURRENT){
                    bioImpedance->setFinalQuad(i, bioImpedance->getTempQuad(i), bioGain, bioCurrent);
                    qDebug() << "Channel " << i+1 << "'s Quad voltage finalised at Medium Current, Voltage: " << bioImpedance->getTempQuad(i);
                }
                else if(bioCurrent == MEDIUM_CURRENT){
                    bioImpedance->setHighCurrentQuad(i, true);
                    qDebug() << "Channel " << i+1 << "'s Quad Voltage is not enough at Medium Current, Voltage is " << quadVoltage;
                }
                else if(bioCurrent == LARGE_CURRENT && bioGain == MEDIUM_GAIN){
                    bioImpedance->setHighCurrentHighGainQuad(i, true);
                    qDebug() << "Channel " << i+1 << "'s Quad Voltage at High Current is still not enough, Voltage is " << quadVoltage;
                }
            }
            else{
                if(bioCurrent == MEDIUM_CURRENT){
                    bioImpedance->setTempQuad(i, quadVoltage);
                    qDebug() << "Channel " << i + 1 << "'s Quad Voltage may be enough at Medium Current, Voltage is " << quadVoltage;
                    bioImpedance->setLowCurrentQuad(i, true);
                }
                else{
                    bioImpedance->setFinalQuad(i, quadVoltage, bioGain, bioCurrent);
                    qDebug() << "Channel " << i + 1 << "'s Quad voltage finalised at " << tag << ", Voltage: " << quadVoltage;
                }
            }
        }
    }
}


void CommandDialog::delaySeconds(){
    delayThread->delay = (int) bioImpDelayLineEdit->text().toInt();
    delayThread->start();
    while (!delayThread->isFinished())
            QCoreApplication::processEvents();
    delayThread->wait();
}

void CommandDialog::on_startBioImpedanceMeasurement(){
    mboxWait->show();
    setWindowTitle(tr("Neutrino Command - Running Auto BioImpedance Measurement"));
    setJTAGChecked(true, 10);
    setJTAGChecked(true, 82);
    setJTAGChecked(true, 89);
    setJTAGChecked(true, 96);
    setJTAGChecked(true, 97);
    for(int i = 0; i < 6; i++){
        BioImpData[i]->setChecked(false);
    }
    BioImpData[4]->setChecked(true);    //ENBIOPIN
}

void CommandDialog::on_exitBioImpedanceMeasurement(){
    setJTAGChecked(false, 10);
    setJTAGChecked(false, 14);
    setJTAGChecked(false, 15);
    setJTAGChecked(false, 16);
    setJTAGChecked(false, 82);
    setJTAGChecked(false, 89);
    setJTAGChecked(false, 96);
    setJTAGChecked(false, 97);
    for(int i = 0; i < 6; i++){
        BioImpData[i]->setChecked(false);
    }
    on_chipReset_clicked();
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
    sendCommand(NeutrinoCommand->resetCommand());
}

void CommandDialog::on_CMReset_clicked(){
    setJTAGChecked(true, 93);
    sendCommand(NeutrinoCommand->constructCommand());
    setJTAGChecked(false, 93);
    sendCommand(NeutrinoCommand->constructCommand());
}

void CommandDialog::on_shortRefToGND_clicked(){
    setJTAGChecked(!JTAG[67]->isChecked(), 67);
    sendCommand(NeutrinoCommand->constructCommand());
}

void CommandDialog::on_JTAG_toggled(int index){
    JTAG[index]->isChecked()? NeutrinoCommand->setJTAGbit(index) : NeutrinoCommand->clearJTAGbit(index);
    if(index > 20 && index < 67 && (index-21)%5 == 0){      // Interested bits for Amplifier Select Enable Checkboxes: 21, 26, 31... 56, 61, 66
        amplifierSelect[9-(index-21)/5]->setChecked(!JTAG[index]->isChecked());
    }
    if(JTAG[13]->isChecked()){          // SEL_LNA GAIN X40
        if(JTAG[12]->isChecked()){      // JTAG[12] = A0 | JTAG[11] = A1
            bioImpGain = JTAG[11]->isChecked()? 40.0 * 8.0 : 40.0 * 4.0;        // A0 = A1 = 1 : A0 = 1, A1 = 0
        }
        else{
            bioImpGain = JTAG[11]->isChecked()? 40.0 * 6.0 : 40.0 * 2.0;        // A0 = 0, A1 = 1 : A0 = A1 = 0
        }
    }
    else{
        if(JTAG[12]->isChecked()){      // JTAG[12] = A0 | JTAG[11] = A1
            bioImpGain = JTAG[11]->isChecked()? 20.0 * 8.0 : 20.0 * 4.0;        // A0 = A1 = 1 : A0 = 1, A1 = 0
        }
        else{
            bioImpGain = JTAG[11]->isChecked()? 20.0 * 6.0 : 20.0 * 2.0;        // A0 = 0, A1 = 1 : A0 = A1 = 0
        }
    }
    if(JTAG[79]->isChecked() || JTAG[77]->isChecked() || JTAG[76]->isChecked()){
        dataProcessor->setGain(JTAG[79]->isChecked(), JTAG[77]->isChecked(), JTAG[76]->isChecked());
    }
}

void CommandDialog::on_JTAGextension_clicked(){
    JTAGTabWidget->isHidden()? JTAGextension->setText(tr("Hide JTAG")) : JTAGextension->setText(tr("Show JTAG"));
    JTAGreset->setHidden(!JTAGTabWidget->isHidden());
    JTAGTabWidget->setHidden(!JTAGTabWidget->isHidden());
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
        setJTAGChecked(false, 66-i*5);
    }
}

void CommandDialog::on_amplifierSelectNone_clicked(){
    for(int i=0;i<10;i++){
        amplifierSelect[i]->setChecked(false);
        setJTAGChecked(true, 66-i*5);
    }
}

void CommandDialog::on_amplifierSelect_toggled(int index){
    setJTAGChecked(!amplifierSelect[index]->isChecked(), 66-index*5);
}

void CommandDialog::on_Mode_Changed(int Mode){
    NeutrinoCommand->setOPMode(Mode);
    on_chipReset_clicked();
    setJTAGChecked((Mode == 5 || Mode == 6), 94);
    setJTAGChecked((Mode > 6 && Mode < 12), 10);
    BioImpData[4]->setChecked((Mode > 6 && Mode < 12));
}

void CommandDialog::on_ChipID_Changed(int IDnum){
    NeutrinoCommand->setChipID(IDnum);
}

void CommandDialog::on_BioImp_toggled(int index){
    BioImpData[index]->isChecked()? NeutrinoCommand->setBioImpBit(index) : NeutrinoCommand->clearBioImpBit(index);
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
    ModeComboBox->setCurrentIndex(NeutrinoCommand->getOPMode());
    CIDComboBox->setCurrentIndex(NeutrinoCommand->getChipID());
    bool *lastSELCHN = NeutrinoChannel->getChannelState_Bool();
    quint8 lastJTAGarray[14];
    Exit->setChecked((NeutrinoCommand->getDCLMode() == DCL_EXIT));
    Enter->setChecked((NeutrinoCommand->getDCLMode() == DCL_ENTER));
    for(int i=0;i<14;i++){
        lastJTAGarray[i] = NeutrinoCommand->getJTAG(i);
    }
    for(int i=0;i<112;i++){
        setJTAGChecked((lastJTAGarray[i/8] & 1<<(i%8)), i);
    }
    for(int i = 21; i < 67; i += 5){
        amplifierSelect[9-(i-21)/5]->setChecked(!(lastJTAGarray[i/8] & 1<<(i%8)));
    }
    for(int i=0;i<6;i++){
        BioImpData[i]->setChecked((NeutrinoCommand->getBioImp() & 1 << i));
    }
    for(int i=0;i<10;i++){
        SELCHN[i]->setChecked(lastSELCHN[i]);
    }
}

void CommandDialog::loadDefault(){
    ModeComboBox->setCurrentIndex(3);
    on_Mode_Changed(3);
    for(int i=0;i<112;i++){
        JTAG[i]->setChecked(false);
    }
    for(int i=84;i<87;i++){         //Default checked JTAG bits (INTRES3, INTRES2, INTRES1)
        setJTAGChecked(true, i);
    }
    setJTAGChecked(true, 102);
    setJTAGChecked(true, 5);
//    setJTAGChecked(true, 6);
    setJTAGChecked(true, 7);
    setJTAGChecked(true, 9);

    Exit->setChecked(true);
    on_SELALL_clicked();
    on_amplifierSelectAll_clicked();
}

void CommandDialog::sendCommand(QByteArray command){
    if(NeutrinoSerial->isConnected()){
        NeutrinoSerial->writeCommand(command);
    }
    if(socketNeutrino->isConnected()){
        socketNeutrino->writeCommand(command);
    }
}

void CommandDialog::setJTAGChecked(bool checked, int bitNum){
    JTAG[bitNum]->setChecked(checked);
    checked? NeutrinoCommand->setJTAGbit(bitNum) : NeutrinoCommand->clearJTAGbit(bitNum);
}
