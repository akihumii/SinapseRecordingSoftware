#include "serialportdialog.h"

SerialPortDialog::SerialPortDialog(SerialChannel *serialChannel_)
{
    serialChannel = serialChannel_;
    setWindowTitle(tr("Serial Port Configurations"));
    createLayout();
    on_scanPorts_triggered();
}

void SerialPortDialog::createLayout(){

    QVBoxLayout *mainLayout = new QVBoxLayout;
    scanPorts = new QPushButton(tr("Scan Ports"));
    connect(scanPorts, SIGNAL(clicked()), this, SLOT(on_scanPorts_triggered()));

    implantGroup = new QButtonGroup;
    ADCGroup = new QButtonGroup;

    for(int i=0;i<4;i++){
        port[i] = new QGroupBox("Serial Port " + QString::number(i+1));

        portLayout[i] = new QVBoxLayout;
        portTypeLayout[i] = new QHBoxLayout;

        portNameLabel[i] = new QLabel("Name :                                                   ");
        portDescLabel[i] = new QLabel("Description :                                            ");
        portManuLabel[i] = new QLabel("Manufacturer:                                            ");

        implantButton[i] = new QRadioButton("Implant Channel");
        implantGroup->addButton(implantButton[i]);

        ADCButton[i] = new QRadioButton("ADC Channel");
        ADCGroup->addButton(ADCButton[i]);

        portTypeLayout[i]->addWidget(implantButton[i]);
        portTypeLayout[i]->addWidget(ADCButton[i]);

        portLayout[i]->addLayout(portTypeLayout[i]);
        portLayout[i]->addWidget(portNameLabel[i]);
        portLayout[i]->addWidget(portDescLabel[i]);
        portLayout[i]->addWidget(portManuLabel[i]);
        port[i]->setLayout(portLayout[i]);
        mainLayout->addWidget(port[i]);
    }

    implantButton[0]->setChecked(true);
    ADCButton[1]->setChecked(true);

    implantGroup->setExclusive(true);
    ADCGroup->setExclusive(true);

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
            implantButton[portCount]->setEnabled(true);
            ADCButton[portCount]->setEnabled(true);
        }
        else{
            portNameLabel[portCount]->setText("Name :                                                   ");
            portDescLabel[portCount]->setText("Description :                                            ");
            portManuLabel[portCount]->setText("Manufacturer:                                            ");
            implantButton[portCount]->setEnabled(false);
            ADCButton[portCount]->setEnabled(false);
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
        implantButton[i]->setEnabled(false);
        ADCButton[i]->setEnabled(false);
        portNameLabel[i]->setEnabled(false);
        portDescLabel[i]->setEnabled(false);
        portManuLabel[i]->setEnabled(false);
    }
}
