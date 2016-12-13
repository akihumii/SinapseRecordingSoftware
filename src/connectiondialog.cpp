#include "connectiondialog.h"

bool ok;

ConnectionDialog::ConnectionDialog(SocketEdison *socketEdison_)
{
    socketEdison = socketEdison_;
    setWindowTitle(tr("Enter IP and Port Number"));
    createLayout();
}

ConnectionDialog::~ConnectionDialog()
{

}

void ConnectionDialog::createLayout()
{
    IPAddress = new QLineEdit(tr("192.168.0.100"));
    PortNumber = new QLineEdit(tr("8888"));

    IPAddress->setInputMask("999.999.9.999");
    IPAddress->setCursorPosition(10);
    QValidator *validator = new QIntValidator(0, 9999, this);
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

    if (socketEdison->isConnected()){
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
    socketEdison->doConnect(IPAddress->text(), PortNumber->text().toInt());
    if (socketEdison->isConnected()){
        setWindowTitle(tr("Connected!!"));
        QThread::msleep(1000);
        ConnectionDialog::close();
    }
    else{
        setWindowTitle(socketEdison->getError());
    }
}

void ConnectionDialog::on_CancelButton_clicked(){
    if (socketEdison->isConnected()){
        socketEdison->writeCommand(QByteArray::number(255, 10));
        socketEdison->doDisconnect();
        setWindowTitle(tr("Disconnected!!"));
        QThread::msleep(1000);
        ConnectionDialog::close();
    }
    else{
        ConnectionDialog::close();
    }
}
