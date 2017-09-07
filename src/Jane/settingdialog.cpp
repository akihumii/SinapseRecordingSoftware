#include "settingdialog.h"

settingsDialog::settingsDialog(QWidget *parent) : QDialog(parent)
{
    createLayout();
    on_scan_triggered();
    updateSettings(0);
}

settingsDialog::SettingsStruct settingsDialog::settings() const{
    return currentSettings;
}

void settingsDialog::createLayout(){
    createSelectportSetting();
    createSerialportSetting();

    QPushButton *okButton = new QPushButton(tr("&ok"));
    QPushButton *scanButton = new QPushButton(tr("&scan"));

    connect(scanButton, SIGNAL(clicked()), this, SLOT(on_scan_triggered()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(on_ok_triggered()));

    okButton -> setShortcut(tr("Ctrl+Shift+O"));
    scanButton -> setShortcut(tr("Ctrl+Shift+S"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(serialPortGroupBox);
    mainLayout -> addWidget(selectPortGroupBox);
    mainLayout -> addWidget(scanButton);
    mainLayout -> addWidget(okButton);

    setLayout(mainLayout);
}

void settingsDialog::createSelectportSetting(){
    int row = 0;
    int column = 0;
    selectPortGroupBox = new QGroupBox(tr("serial port info"));
    QGridLayout *semiLayout = new QGridLayout;
    signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(on_selected_triggered(int)));
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(on_apply_triggered(int)));

    for(int i = 0; i <4; i++){
        port[i] = new QGroupBox("serial port number : " + QString::number(i +1));
        microLayout[i] = new QVBoxLayout;

        selectedPort[i] = new QCheckBox(tr("Select this port"));
        signalMapper -> setMapping(selectedPort[i], i);
        connect(selectedPort[i], SIGNAL(clicked()), signalMapper, SLOT(map()));

        portNameLabel[i] = new QLabel("name:                                           ");
        portDescLabel[i] = new QLabel("Description:                                    ");
        portManuLabel[i] = new QLabel("Manufacturer:                                   ");
        portSeriLabel[i] = new QLabel("Serial number:                                  ");

        selectedPort[i] -> setEnabled(false);
        portNameLabel[i] -> setEnabled(false);
        portDescLabel[i] -> setEnabled(false);
        portManuLabel[i] -> setEnabled(false);
        portSeriLabel[i] -> setEnabled(false);

        microLayout[i] -> addWidget(selectedPort[i]);
        microLayout[i] -> addWidget(portNameLabel[i]);
        microLayout[i] -> addWidget(portDescLabel[i]);
        microLayout[i] -> addWidget(portManuLabel[i]);
        microLayout[i] -> addWidget(portSeriLabel[i]);

        port[i] -> setLayout(microLayout[i]);
        semiLayout -> addWidget(port[i], row, column);
        if(row%2 == 0)
            column = 0;
        if(i%2 == 0){
           column += 1;
        }
        else
            row += 1;
    }
    selectPortGroupBox -> setLayout(semiLayout);
}

void settingsDialog::createSerialportSetting(){
    serialPortGroupBox = new QGroupBox(tr("serial port setting"));
    QFormLayout *semiLayout = new QFormLayout;

    baudRateComboBox = new QComboBox;
    dataBitsComboBox = new QComboBox;
    parityComboBox = new QComboBox;
    stopBitsComboBox = new QComboBox;
    flowControlComboBox = new QComboBox;

    semiLayout -> addRow(new QLabel(tr("&baudrate")), baudRateComboBox);
    semiLayout -> addRow(new QLabel(tr("&data bits")), dataBitsComboBox);
    semiLayout -> addRow(new QLabel(tr("&parity")), parityComboBox);
    semiLayout -> addRow(new QLabel(tr("s&top bits")), stopBitsComboBox);
    semiLayout -> addRow(new QLabel(tr("&flow control")), flowControlComboBox);

    baudRateComboBox -> addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    baudRateComboBox -> addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    baudRateComboBox -> addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    baudRateComboBox -> addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    baudRateComboBox -> addItem(tr("custom"));
    baudRateComboBox -> setCurrentIndex(3);

    dataBitsComboBox -> addItem(QStringLiteral("5"), QSerialPort::Data5);
    dataBitsComboBox -> addItem(QStringLiteral("6"), QSerialPort::Data6);
    dataBitsComboBox -> addItem(QStringLiteral("7"), QSerialPort::Data7);
    dataBitsComboBox -> addItem(QStringLiteral("8"), QSerialPort::Data8);
    dataBitsComboBox -> setCurrentIndex(3);

    parityComboBox -> addItem(QStringLiteral("None"), QSerialPort::NoParity);
    parityComboBox -> addItem(QStringLiteral("Even"), QSerialPort::EvenParity);
    parityComboBox -> addItem(QStringLiteral("Odd"), QSerialPort::OddParity);
    parityComboBox -> addItem(QStringLiteral("Mark"), QSerialPort::MarkParity);
    parityComboBox -> addItem(QStringLiteral("Space"), QSerialPort::SpaceParity);

    stopBitsComboBox -> addItem(QStringLiteral("1"), QSerialPort::OneStop);
    stopBitsComboBox -> addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    flowControlComboBox -> addItem(QStringLiteral("None"), QSerialPort::NoFlowControl);
    flowControlComboBox -> addItem(QStringLiteral("RTS/CTS"), QSerialPort::HardwareControl);
    flowControlComboBox -> addItem(QStringLiteral("XON/XOFF"), QSerialPort::SoftwareControl);;

    serialPortGroupBox -> setLayout(semiLayout);
}

void settingsDialog::updateSettings(int selectedPort){
    QList<QSerialPortInfo> portInfo = QSerialPortInfo::availablePorts();
    if(portInfo.size()>0){


        for(int i = 0; i < 4; i++){
            if(i == selectedPort){
                currentSettings.name = portInfo.at(i).portName();

                currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(baudRateComboBox -> itemData(baudRateComboBox -> currentIndex()).toInt());
                currentSettings.stringBaudRate = QString::number(currentSettings.baudRate);

                currentSettings.dataBits = static_cast<QSerialPort::DataBits>(dataBitsComboBox -> itemData(dataBitsComboBox -> currentIndex()).toInt());
                currentSettings.stringDataBits = QString::number(currentSettings.dataBits);

                currentSettings.parity = static_cast<QSerialPort::Parity>(parityComboBox -> itemData(parityComboBox -> currentIndex()).toInt());
                currentSettings.stringParity = QString::number(currentSettings.parity);

                currentSettings.stopBits = static_cast<QSerialPort::StopBits>(stopBitsComboBox -> itemData(stopBitsComboBox -> currentIndex()).toInt());
                currentSettings.stringStopBits = QString::number(currentSettings.stopBits);

                currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(flowControlComboBox -> itemData(flowControlComboBox -> currentIndex()).toInt());
                currentSettings.stringFlowControl = QString::number(currentSettings.flowControl);

                currentSettings.localEchoEnabled = true;
            }
        }
    }
}

void settingsDialog::on_scan_triggered(){
    int portCount = 0;
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        if(info.hasProductIdentifier()){
            portNameLabel[portCount] -> setText("name : " + info.portName());
            portDescLabel[portCount] -> setText("description : " + info.description());
            portManuLabel[portCount] -> setText("Manufacturer : " + info.manufacturer());
            portSeriLabel[portCount] -> setText("Serial number : " + info.serialNumber());
            selectedPort[portCount] -> setEnabled(true);
        }
        else{
            portNameLabel[portCount] -> setText("name:                                           ");
            portDescLabel[portCount] -> setText("Description:                                    ");
            portManuLabel[portCount] -> setText("Manufacturer:                                   ");
            portSeriLabel[portCount] -> setText("Serial number:                                  ");

            portNameLabel[portCount] -> setEnabled(false);
            portDescLabel[portCount] -> setEnabled(false);
            portManuLabel[portCount] -> setEnabled(false);
            portSeriLabel[portCount] -> setEnabled(false);
            selectedPort[portCount] -> setEnabled(false);
        }
        portCount++;
        if(portCount ==4)
            break;
    }

    for(int i = portCount; i < 4; i++){
        portNameLabel[portCount] -> setText("name:                                           ");
        portDescLabel[portCount] -> setText("Description:                                    ");
        portManuLabel[portCount] -> setText("Manufacturer:                                   ");
        portSeriLabel[portCount] -> setText("Serial number:                                  ");

        portNameLabel[portCount] -> setEnabled(false);
        portDescLabel[portCount] -> setEnabled(false);
        portManuLabel[portCount] -> setEnabled(false);
        portSeriLabel[portCount] -> setEnabled(false);
        selectedPort[portCount] -> setEnabled(false);
    }
}

void settingsDialog::on_selected_triggered(int stateCheck){
    for(int i = 0; i < 4; i++){
        if(i == stateCheck){
            selectedPort[stateCheck] -> setChecked(true);
            portNameLabel[stateCheck] -> setEnabled(true);
            portDescLabel[stateCheck] -> setEnabled(true);
            portManuLabel[stateCheck] -> setEnabled(true);
            portSeriLabel[stateCheck] -> setEnabled(true);
        }
        else{
            selectedPort[i] -> setChecked(false);
            portNameLabel[i] -> setEnabled(false);
            portDescLabel[i] -> setEnabled(false);
            portManuLabel[i] -> setEnabled(false);
            portSeriLabel[i] -> setEnabled(false);
        }
    }
}

void settingsDialog::on_apply_triggered(int stateCheck){
    stateCurrentPort = stateCheck;
}

void settingsDialog::on_ok_triggered(){
    updateSettings(stateCurrentPort);
    hide();
}
