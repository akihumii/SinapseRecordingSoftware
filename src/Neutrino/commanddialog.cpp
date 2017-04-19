#include "commanddialog.h"

CommandDialog::CommandDialog(SocketNeutrino *socketNeutrino_, Command *NeutrinoCommand_, Channel *NeutrinoChannel_, SerialChannel *NeutrinoSerial_){
    socketNeutrino = socketNeutrino_;
    NeutrinoChannel = NeutrinoChannel_;
    NeutrinoCommand = NeutrinoCommand_;
    NeutrinoSerial = NeutrinoSerial_;
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

    ModeComboBox->setCurrentIndex(2);
    on_Mode_Changed(2);

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

    for(int i=0;i<6;i++){
        BioImpData[i] = new QCheckBox;
        BioImpData[i]->setText(BioImpNames[i]);
        BioImpLayout->addWidget(BioImpData[i]);
        connect(BioImpData[i], SIGNAL(toggled(bool)), this, SLOT(on_BioImp_toggled()));
    }

    SendCommand = new QPushButton(tr("Send Command"));
    connect(SendCommand, SIGNAL(clicked(bool)), this, SLOT(on_sendCommand_clicked()));

    BioImpLayout->addWidget(BioImpLabel);
    BioImpLayout->addWidget(SendCommand);


    QVBoxLayout *DCL = new QVBoxLayout;
    DigComLoopback = new QLabel;
    DigComLoopback->setText("Digital Command Loopback");

    Exit = new QRadioButton("Exit", this);
    Enter = new QRadioButton("Enter", this);

    connect(Exit, SIGNAL(toggled(bool)), this, SLOT(on_DCL_toggled()));
    connect(Enter, SIGNAL(toggled(bool)), this, SLOT(on_DCL_toggled()));

    ChipReset = new QPushButton(tr("Chip Reset"));
    connect(ChipReset, SIGNAL(clicked(bool)), this, SLOT(on_chipReset_clicked()));

    DCL->addWidget(DigComLoopback);
    DCL->addWidget(Exit);
    DCL->addWidget(Enter);
    DCL->addSpacing(100);
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
    if(NeutrinoSerial->isConnected()){
        NeutrinoSerial->writeCommand(NeutrinoCommand->constructCommand());
    }
    if(socketNeutrino->isConnected()){
        socketNeutrino->writeCommand(NeutrinoCommand->constructCommand());
    }
}

void CommandDialog::on_chipReset_clicked(){
    if(socketNeutrino->isConnected()){
        socketNeutrino->writeCommand(NeutrinoCommand->resetCommand());
    }
    if(NeutrinoSerial->isConnected()){
        NeutrinoSerial->writeCommand(NeutrinoCommand->resetCommand());
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

void CommandDialog::on_SELCHN_toggled(){

}

void CommandDialog::on_Mode_Changed(int Mode){
    NeutrinoCommand->setOPMode(Mode);
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
    for(int i=0;i<112;i++){
        JTAG[i]->setChecked(false);
    }
    for(int i=84;i<87;i++){         //Default checked JTAG bits (INTRES3, INTRES2, INTRES1)
        JTAG[i]->setChecked(true);
        NeutrinoCommand->setJTAGbit(i);
    }
    for(int i=5;i<10;i++){
        JTAG[i]->setChecked(true);
        NeutrinoCommand->setJTAGbit(i);
    }
//    JTAG[8]->setChecked(false);
//    JTAG[9]->setChecked(false);
}
