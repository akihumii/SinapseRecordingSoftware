#include "mainwindow.h"

MainWindow::MainWindow(){
    serialOdin = new SerialOdin(this);
//    serialOdin->connectOdin();
//    serialOdin->initOdin();
    socketOdin = new SocketOdin(this);
    socketOdin->doConnect("10.10.10.1", 30000);
    socketOdin->initOdin();
    commandOdin = new CommandOdin(serialOdin, socketOdin);

    testButton = new QPushButton(tr("Send!"));
    connect(testButton, SIGNAL(clicked(bool)), this, SLOT(sendCommand()));
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(testButton);
    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}

void MainWindow::sendCommand(){
    commandOdin->sendTestCommand();
}

MainWindow::~MainWindow(){
}
