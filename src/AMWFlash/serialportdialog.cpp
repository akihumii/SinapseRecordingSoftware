#include "serialportdialog.h"

SerialPortDialog::SerialPortDialog(QSerialPort *port)
{
    serialPort = port;
    setWindowTitle(tr("Serial Port Configurations"));
    createLayout();
    on_scanPorts_triggered();
}

void SerialPortDialog::createLayout(){

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QLabel *label = new QLabel(tr("Select the port that AMW004 is connected to:"));
    mainLayout->addWidget(label);
    scanPorts = new QPushButton(tr("Scan Ports"));
    connect(scanPorts, SIGNAL(clicked()), this, SLOT(on_scanPorts_triggered()));

    for(int i=0;i<4;i++){
        port[i] = new QGroupBox("Serial Port " + QString::number(i+1));

        portLayout[i] = new QVBoxLayout;
        portTypeLayout[i] = new QHBoxLayout;

        selected[i] = new QCheckBox(tr("Select"));
        connect(selected[i], SIGNAL(clicked(bool)), this, SLOT(on_portSelected()));

        portNameLabel[i] = new QLabel("Name :                                                   ");
        portDescLabel[i] = new QLabel("Description :                                            ");
        portManuLabel[i] = new QLabel("Manufacturer:                                            ");

        portLayout[i]->addWidget(selected[i]);
        portLayout[i]->addLayout(portTypeLayout[i]);
        portLayout[i]->addWidget(portNameLabel[i]);
        portLayout[i]->addWidget(portDescLabel[i]);
        portLayout[i]->addWidget(portManuLabel[i]);
        port[i]->setLayout(portLayout[i]);
        mainLayout->addWidget(port[i]);
    }

    mainLayout->addWidget(scanPorts);
    setLayout(mainLayout);
}

SerialPortDialog::~SerialPortDialog()
{

}

void SerialPortDialog::on_scanPorts_triggered(){
    portCount = 0;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if(info.hasProductIdentifier()){
            portNameLabel[portCount]->setText("Name : " + info.portName());
            portDescLabel[portCount]->setText("Description : " + info.description());
            portManuLabel[portCount]->setText("Manufacturer: " + info.manufacturer());
            portNameLabel[portCount]->setEnabled(true);
            portDescLabel[portCount]->setEnabled(true);
            portManuLabel[portCount]->setEnabled(true);
        }
        else{
            portNameLabel[portCount]->setText("Name :                                                   ");
            portDescLabel[portCount]->setText("Description :                                            ");
            portManuLabel[portCount]->setText("Manufacturer:                                            ");
            portNameLabel[portCount]->setEnabled(false);
            portDescLabel[portCount]->setEnabled(false);
            portManuLabel[portCount]->setEnabled(false);
        }
        portCount++;
        if(portCount==4){                           // Scan a maximum of 4 Serial Devices
            break;
        }
    }

    for(int i = portCount; i<4; i++){
        portNameLabel[i]->setText("Name :                                                   ");
        portDescLabel[i]->setText("Description :                                            ");
        portManuLabel[i]->setText("Manufacturer:                                            ");
        portNameLabel[i]->setEnabled(false);
        portDescLabel[i]->setEnabled(false);
        portManuLabel[i]->setEnabled(false);
    }
}

void SerialPortDialog::on_portSelected(){
    QList<QSerialPortInfo> portInfo = QSerialPortInfo::availablePorts();
    for(int i = 0; i < 4; i++){
        if(selected[i]->isChecked()){
            serialPort->setPortName(portInfo.at(i).portName());
            qDebug() << "Connected to port " << portInfo.at(i).portName();
            serialPort->setBaudRate(115200);
            serialPort->setDataBits(QSerialPort::Data8);
            serialPort->setParity(QSerialPort::NoParity);
            serialPort->setStopBits(QSerialPort::OneStop);
            serialPort->setFlowControl(QSerialPort::NoFlowControl);
        }
    }
    if(serialPort->open(QIODevice::ReadWrite)){
        qDebug() << "Connected to amw serial";
    }
    this->close();
}
