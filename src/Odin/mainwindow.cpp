#include "mainwindow.h"

MainWindow::MainWindow(){
    serialOdin = new SerialOdin(this);
    serialOdin->connectOdin();
    serialOdin->initOdin();
    commandOdin = new CommandOdin(serialOdin);

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
