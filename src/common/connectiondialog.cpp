#include "connectiondialog.h"

ConnectionDialog::ConnectionDialog(SocketAbstract *socketAbstract_)
{
    socketAbstract = socketAbstract_;
    setWindowTitle(tr("Enter IP and Port Number"));
    createLayout();
}

ConnectionDialog::~ConnectionDialog()
{

}

void ConnectionDialog::createLayout()
{
    IPAddress = new QLineEdit(tr("10.10.10.1"));
    PortNumber = new QLineEdit(tr("30000"));

    IPAddress->setInputMask("999.999.999.999");
    IPAddress->setCursorPosition(10);
    QValidator *validator = new QIntValidator(0, 99999, this);
    PortNumber->setValidator(validator);

    IPLabel = new QLabel;
    PortLabel = new QLabel;

    QVBoxLayout *LabelLayout = new QVBoxLayout;
    LabelLayout->addWidget(IPLabel);
    IPLabel->setText("IP Address");
    LabelLayout->addWidget(PortLabel);
    PortLabel->setText("Port Number");

    QVBoxLayout *InputLayout = new QVBoxLayout;
    InputLayout->addWidget(IPAddress);
    InputLayout->addWidget(PortNumber);

    Connect = new QPushButton(tr("Connect"));
    Cancel = new QPushButton(tr("Cancel"));

    if (socketAbstract->isConnected()){
        Cancel->setText("Disconnect");
    }

    connect(Connect, SIGNAL(clicked()), this, SLOT(on_ConnectButton_clicked()));
    connect(Cancel, SIGNAL(clicked()), this, SLOT(on_CancelButton_clicked()));

    QVBoxLayout *ButtonLayout = new QVBoxLayout;
    ButtonLayout->addWidget(Connect);
    ButtonLayout->addWidget(Cancel);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(LabelLayout);
    mainLayout->addLayout(InputLayout);
    mainLayout->addLayout(ButtonLayout);
    setLayout(mainLayout);
    mainLayout->setSizeConstraint( QLayout::SetFixedSize );
}

void ConnectionDialog::on_ConnectButton_clicked(){
    socketAbstract->doConnect(IPAddress->text(), PortNumber->text().toInt());
    if (socketAbstract->isConnected()){
        setWindowTitle(tr("Connected!!"));
        QThread::msleep(1000);
        ConnectionDialog::close();
    }
    else{
        setWindowTitle(socketAbstract->getError());
    }
}

void ConnectionDialog::on_CancelButton_clicked(){
    if (socketAbstract->isConnected()){
        socketAbstract->doDisconnect();
        setWindowTitle(tr("Disconnected!!"));
        QThread::msleep(1000);
        ConnectionDialog::close();
    }
    else{
        ConnectionDialog::close();
    }
}
