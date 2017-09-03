#include "import/amwflash_mainwindow.h"
#include "import/settingdialog.h"
#include "import/console.h"

amwFlash_mainWindow::amwFlash_mainWindow(QWidget *parent) : QMainWindow(parent)
{
    createMenuBar();
    createFlashMode();

    mainConsole = new console;
    mainConsole -> setEnabled(true);
    mainConsole -> setLocalEchoEnabled(true);

    serialPort = new QSerialPort(this);
    settingsInit = new settingsDialog;

    status = new QLabel;
    status -> setText(tr("amw flash mode start"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *consoleFlashLayout = new QHBoxLayout;

    consoleFlashLayout -> addWidget(mainConsole);
    consoleFlashLayout -> addLayout(flashMode);

    mainLayout -> setMenuBar(menuBar);
    mainLayout -> addLayout(consoleFlashLayout);
    mainLayout -> addWidget(status);

    connect(connectAction, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(disconnectAction, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(settingAction, SIGNAL(triggered()), settingsInit, SLOT(show()));

    connect(serialPort, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(mainConsole, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));

    setCentralWidget(new QWidget);
    setWindowTitle(tr("amw flash Mode"));
    centralWidget() -> setLayout(mainLayout);
}

void amwFlash_mainWindow::createMenuBar(){
    menuBar = new QMenuBar();
    fileMenu = new QMenu(tr("&File"), this);
    settingMenu = new QMenu(tr("&Setting"), this);

    connectAction = fileMenu -> addAction(tr("&Connect"));
    connectAction -> setShortcut(tr("Ctrl+Shift+C"));
    disconnectAction = fileMenu -> addAction(tr("&Disconnect"));
    disconnectAction -> setShortcut(tr("Ctrl+Shift+D"));
    fileMenu -> addSeparator();
    exitAction = fileMenu -> addAction(tr("E&xit"));
    exitAction -> setShortcut(tr("Ctrl+Shift+X"));

    settingAction = settingMenu -> addAction(tr("&Setting"));
    settingAction -> setShortcut(tr("Ctrl+Shift+S"));
    clearSettingAction = settingMenu -> addAction(tr("Cl&ear"));
    clearSettingAction -> setShortcut(tr("Ctrl+Shift+E"));

    connectAction -> setEnabled(true);
    disconnectAction -> setEnabled(false);

    menuBar -> addMenu(fileMenu);
    menuBar -> addMenu(settingMenu);

    connect(exitAction, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));
}

void amwFlash_mainWindow::createFlashMode(){
    flashMode = new QGridLayout;

    fileNameEntry = new QLineEdit(tr("file name"));
    searchButton = new QPushButton(tr("sea&rch file"));
    fileDataEntry = new QPlainTextEdit(tr("data"));
    flashButton = new QPushButton(tr("&flash"));
    saveRebootButton = new QPushButton(tr("save &and reboot"));

    searchButton -> setShortcut(tr("Ctrl+Shift+R"));
    flashButton -> setShortcut(tr("Ctrl+Shift+F"));
    saveRebootButton -> setShortcut(tr("Ctrl+Shift+A"));

    connect(searchButton, SIGNAL(clicked()), this, SLOT(on_search_clicked()));
    connect(flashButton, SIGNAL(clicked()), this, SLOT(on_flash_clicked()));
    connect(saveRebootButton, SIGNAL(clicked()), this, SLOT(on_saveReboot_clicked()));

    flashMode -> addWidget(fileNameEntry, 0, 0);
    flashMode -> addWidget(searchButton, 0, 1);
    flashMode -> addWidget(fileDataEntry, 1, 0, 1, 2);
    flashMode -> addWidget(flashButton, 2, 0, 1, 2);
    flashMode -> addWidget(saveRebootButton, 3, 0, 1, 2);

    flashButton -> setEnabled(false);
    saveRebootButton -> setEnabled(true);
}

void amwFlash_mainWindow::openSerialPort(){
    settingsDialog::SettingsStruct p = settingsInit -> settings();
    serialPort -> setPortName(p.name);
    serialPort -> setBaudRate(p.baudRate);
    serialPort -> setDataBits(p.dataBits);
    serialPort -> setStopBits(p.stopBits);
    serialPort -> setFlowControl(p.flowControl);
    if(serialPort -> open(QIODevice::ReadWrite)){
        connectAction -> setEnabled(false);
        flashButton-> setEnabled(true);
        disconnectAction -> setEnabled(true);
        settingAction -> setEnabled(false);

        mainConsole -> setLocalEchoEnabled(p.localEchoEnabled);
        status -> setText(tr("Connected to %1 : %2, %3, %4, %5, %6")
                          .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                          .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
  }
}

void amwFlash_mainWindow::closeSerialPort(){
    if(serialPort -> isOpen())
        serialPort -> close();

    connectAction -> setEnabled(true);
    disconnectAction -> setEnabled(false);
    flashButton -> setEnabled(false);
    settingAction -> setEnabled(true);

    status -> setText(tr("Disconnect !!"));
}

void amwFlash_mainWindow::writeData(const QByteArray &data){
    serialPort -> write(data);
}

void amwFlash_mainWindow::readData(){
    QByteArray data = serialPort -> readAll();
    mainConsole -> putData(data);
}

void amwFlash_mainWindow::readFeedback(){
    do{
        char buf[1024];
        qint64 lineLength = serialPort -> readLine(buf, sizeof(buf));
        if(lineLength != -1)
            mainConsole -> putData(buf);
    }while(serialPort -> bytesAvailable() > 0);
}

void amwFlash_mainWindow::on_search_clicked(){
    fileName = QFileDialog::getOpenFileName(this, tr("Open Setting file"), QDir::homePath() + "/Desktop/", tr(".txt file (*.txt)"));

    file = new QFile;
    file -> setFileName(fileName);
    fileNameEntry -> setText(fileName);
    if(file -> open(QIODevice::ReadOnly|QIODevice::Text)){
        status -> setText("File opened");
        fileDataEntry -> clear();
        QTextStream in(file);
        QString line = in.readLine();
        while(!line.isNull()){
            fileDataEntry -> appendPlainText(line);
            line = in.readLine();
        }
        flashButton -> setEnabled(true);
        openFileFlag = true;
    }
    else
        status -> setText("File failed to open");
}

void amwFlash_mainWindow::on_flash_clicked(){
    if(connectAction -> isEnabled() == false && openFileFlag == true){
        status -> setText("flash AMW");
        file -> reset();
        connect(serialPort, SIGNAL(readyRead()), this, SLOT(readFeedback()));
        QTextStream in(file);
        QString line = in.readLine();
        while(!line.isNull()){
            line.append("\r\n");
            serialPort -> write(line.toUtf8().constData(), line.size());
            serialPort -> waitForBytesWritten(50);
            qDebug() << line;
            QThread::msleep(50);
            line = in.readLine();
        }
        status -> setText("flash finish !!");
    }
    else{
        status -> setText("didn't open serial port or open file");
    }
}

void amwFlash_mainWindow::on_saveReboot_clicked(){
    saveRebootFlag = true;
    flashButton -> setEnabled(false);
    status -> setText("save and reboot");
}

