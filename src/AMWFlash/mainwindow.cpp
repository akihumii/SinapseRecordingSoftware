#include "mainwindow.h"

MainWindow::MainWindow(){
    createLayout();
}

MainWindow::~MainWindow()
{

}

void MainWindow::createLayout(){
    QLabel *instructions = new QLabel(tr("Please choose the AMW setup file. \n"
                                         "Instructions to set up AMW can be found at: \n"
                                         "https://docs.zentri.com/zentrios/wz/3.3/cmd/variables/\n"
                                         "Edit the default configuration at your own risk!"));

    QLineEdit *fileDir = new QLineEdit;

    QPushButton *browse = new QPushButton(tr("Browse"));

    connect(browse, SIGNAL(clicked(bool)), this, SLOT(on_browse_clicked()));
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(instructions);
    mainLayout->addWidget(fileDir);
    mainLayout->addWidget(browse);

    QWidget *mainWidget = new QWidget;

    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
    mainLayout->setSizeConstraint( QLayout::SetFixedSize );
}

void MainWindow::readFeedback(){
    do{
        char buf[1024];
        qint64 lineLength = amwPort->readLine(buf, sizeof(buf));
        if(lineLength != -1)
            qDebug() << buf;
    }while(amwPort->bytesAvailable() > 0);
}

void MainWindow::on_browse_clicked(){
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Setting file"), QDir::homePath() + "/Desktop/", tr(".txt file (*.txt)"));

    amwPort = new QSerialPort(this);

    portInfo = QSerialPortInfo::availablePorts();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
    }
    for(int i = 0; i < portInfo.size(); i++){
        if(portInfo.at(i).manufacturer().contains("FTDI", Qt::CaseInsensitive)){
            amwPort->setPortName(portInfo.at(i).portName());
            qDebug() << "Connected to port " << portInfo.at(i).portName();
            amwPort->setBaudRate(115200);
            amwPort->setDataBits(QSerialPort::Data8);
            amwPort->setParity(QSerialPort::NoParity);
            amwPort->setStopBits(QSerialPort::OneStop);
            amwPort->setFlowControl(QSerialPort::NoFlowControl);
        }
    }
    if(amwPort->open(QIODevice::ReadWrite)){
        qDebug() << "Connected to amw serial";
    }

    connect(amwPort, SIGNAL(readyRead()), this, SLOT(readFeedback()));

    QFile *File = new QFile;

    File->setFileName(fileName);

    if(File->open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug()<< "File opened";
    }
    else{
        qDebug() << "File failed to open";
    }

    QTextStream in(File);

    QString line = in.readLine();
    while (!line.isNull()) {
        line.append("\r\n");
        qDebug() << line;
        amwPort->write(line.toLocal8Bit());
//        QThread::msleep(50);
        line = in.readLine();
    }
}
