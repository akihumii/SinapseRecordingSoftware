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

    browse = new QPushButton(tr("Browse"));
    connect(browse, SIGNAL(clicked(bool)), this, SLOT(on_browse_clicked()));
    flash = new QPushButton(tr("Flash AMW!"));
    connect(flash, SIGNAL(clicked(bool)), this, SLOT(on_flash_clicked()));
    flash->setDisabled(true);

    console = new Console;
    console->setEnabled(true);
    console->setLocalEchoEnabled(false);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(instructions);
    mainLayout->addWidget(console);
    mainLayout->addWidget(browse);
    mainLayout->addWidget(flash);

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
            console->putData(buf);
    }while(amwPort->bytesAvailable() > 0);
}

void MainWindow::on_browse_clicked(){
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Setting file"), QDir::homePath() + "/Desktop/", tr(".txt file (*.txt)"));

    File = new QFile;

    File->setFileName(fileName);
    browse->setText(fileName);

    if(File->open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug()<< "File opened";
        browse->setDisabled(true);
        flash->setEnabled(true);
    }
    else{
        qDebug() << "File failed to open";
    }
}

void MainWindow::on_flash_clicked(){
    if(!savereboot){
        amwPort = new QSerialPort(this);
        connect(amwPort, SIGNAL(readyRead()), this, SLOT(readFeedback()));
        SerialPortDialog serialPortDialog(amwPort);
        serialPortDialog.exec();
        QTextStream in(File);
        QString line = in.readLine();
        while (!line.isNull()) {
            line.append("\r\n");
            qDebug() << line;
            amwPort->write(line.toUtf8().constData(), line.size());
            amwPort->waitForBytesWritten(50);
            QThread::msleep(50);
            line = in.readLine();
        }
        savereboot = true;
        flash->setText(tr("Save and reboot"));
    }
    else{
        amwPort->write("save\r\n");
        amwPort->write("reboot\r\n");
    }
}
