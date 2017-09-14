#include "mainwindow.h"

MainWindow::MainWindow(){
    QString version(APP_VERSION);
    timer.start();
    NeutrinoChannel = new Channel;
    NeutrinoCommand = new Command(NeutrinoChannel);
    data = new DataProcessor(NeutrinoChannel);
    serialNeutrino = new SerialChannel(this, NeutrinoCommand, data, NeutrinoChannel);
    socketNeutrino = new SocketNeutrino(NeutrinoCommand, data, NeutrinoChannel);
    setWindowTitle(tr("SINAPSE Neutrino II Recording Software V") + version);
    createStatusBar();
    create5x2Layout();
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(updateData()));
    dataTimer.start(1);     //tick timer every XXX msec
    createActions();
    createMenus();
    connectNeutrino();
    qDebug() << "Starting NEUTRINO II..";
}

void MainWindow::createActions(){
//    connectAction = new QAction(tr("&Connect"), this);
//    connectAction->setShortcut(tr("Ctrl+C"));
//    connect(connectAction, SIGNAL(triggered()), this, SLOT(on_ConnectMenu_triggered()));

//    disconnectAction = new QAction(tr("&Disconnect"), this);
//    disconnectAction->setShortcut(tr("Ctrl+D"));
//    connect(disconnectAction, SIGNAL(triggered()), this, SLOT(on_DisconnectMenu_triggered()));

    commandAction = new QAction(tr("Comm&and"), this);
    commandAction->setShortcut(tr("Ctrl+A"));
    connect(commandAction, SIGNAL(triggered()), this, SLOT(on_CommandMenu_triggered()));

    tenby1Action = new QAction(tr("10x&1 Layout"), this);
    tenby1Action->setShortcut(tr("Ctrl+1"));
    connect(tenby1Action, SIGNAL(triggered()),this,SLOT(on_tenby1_triggered()));

    fiveby2Action = new QAction(tr("5x&2 Layout"), this);
    fiveby2Action->setShortcut(tr("Ctrl+2"));
    connect(fiveby2Action, SIGNAL(triggered()),this,SLOT(on_fiveby2_triggered()));

    dataAnalyzerAction = new QAction(tr("Data Analy&zer"), this);
    dataAnalyzerAction->setShortcut(tr("Ctrl+Z"));
    connect(dataAnalyzerAction, SIGNAL(triggered()), this, SLOT(on_dataAnalyzer_triggered()));

//    wifiMode = new QAction(tr("Wireless Mode"));
//    wiredMode = new QAction(tr("Wired Mode"));

//    connect(wifiMode, SIGNAL(triggered(bool)), this, SLOT(on_wifi_triggered()));
//    connect(wiredMode, SIGNAL(triggered(bool)), this, SLOT(on_wired_triggered()));

    filterAction = new QAction(tr("Filter Configuration"), this);
    filterAction->setShortcut(tr("Ctrl+F"));
    connect(filterAction, SIGNAL(triggered(bool)), this, SLOT(on_filterConfig_trigger()));

    swapAction = new QAction(tr("Swap &Port"), this);
    swapAction->setShortcut(tr("Ctrl+P"));
    connect(swapAction, SIGNAL(triggered()), this, SLOT(on_swap_triggered()));

    resetDefaultX = new QAction(tr("Default Time Scale"), this);
    resetDefaultX->setShortcut(tr("Ctrl+X"));
    connect(resetDefaultX, SIGNAL(triggered()), this, SLOT(on_resetX_triggered()));

    chooseDirectoryAction = new QAction(tr("&Save as.."), this);
    chooseDirectoryAction->setShortcut(tr("Ctrl+S"));
    connect(chooseDirectoryAction, SIGNAL(triggered()), this, SLOT(on_chooseDirectory_triggered()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+X"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    pauseAction = new QAction(tr("Pause graph"), this);
    pauseAction->setShortcut(tr("Ctrl+Space"));
    connect(pauseAction, SIGNAL(triggered()), this, SLOT(on_playPause_triggered()));

    recordAction = new QAction(tr("Start &Recording"), this);
    recordAction->setShortcut(tr("Ctrl+R"));
    connect(recordAction, SIGNAL(triggered()), this, SLOT(on_record_triggered()));

    timeFrame10ms = new QAction(tr("10 milliseconds"), this);
    timeFrame20ms = new QAction(tr("20 milliseconds"), this);
    timeFrame50ms = new QAction(tr("50 milliseconds"), this);
    timeFrame100ms = new QAction(tr("100 milliseconds"), this);
    timeFrame200ms = new QAction(tr("200 milliseconds"), this);
    timeFrame500ms = new QAction(tr("500 milliseconds"), this);
    timeFrame1000ms = new QAction(tr("1 second"), this);
    timeFrame2000ms = new QAction(tr("2 seconds"), this);
    timeFrame5000ms = new QAction(tr("5 seconds"), this);

    connect(timeFrame10ms, SIGNAL(triggered(bool)), this, SLOT(on_timeFrame10_triggered()));
    connect(timeFrame20ms, SIGNAL(triggered(bool)), this, SLOT(on_timeFrame20_triggered()));
    connect(timeFrame50ms, SIGNAL(triggered(bool)), this, SLOT(on_timeFrame50_triggered()));
    connect(timeFrame100ms, SIGNAL(triggered(bool)), this, SLOT(on_timeFrame100_triggered()));
    connect(timeFrame200ms, SIGNAL(triggered(bool)), this, SLOT(on_timeFrame200_triggered()));
    connect(timeFrame500ms, SIGNAL(triggered(bool)), this, SLOT(on_timeFrame500_triggered()));
    connect(timeFrame1000ms, SIGNAL(triggered(bool)), this, SLOT(on_timeFrame1000_triggered()));
    connect(timeFrame2000ms, SIGNAL(triggered(bool)), this, SLOT(on_timeFrame2000_triggered()));
    connect(timeFrame5000ms, SIGNAL(triggered(bool)), this, SLOT(on_timeFrame5000_triggered()));

    resetDefaultY = new QAction(tr("Default Voltage Scale"), this);
    resetDefaultY->setShortcut(tr("Ctrl+Y"));
    connect(resetDefaultY, SIGNAL(triggered()), this, SLOT(on_resetY_triggered()));

    voltage50u = new QAction(tr("+/- 10mV"));
    voltage100u = new QAction(tr("+/- 20mV"));
    voltage200u = new QAction(tr("+/- 50mV"));
    voltage500u = new QAction(tr("+/- 100mV"));
    voltage1000u = new QAction(tr("+/- 200mV"));
    voltage2000u = new QAction(tr("+/- 500mV"));
    voltage5000u = new QAction(tr("+/- 700mV"));

    connect(voltage50u, SIGNAL(triggered(bool)), this, SLOT(on_voltage50u_triggered()));
    connect(voltage100u, SIGNAL(triggered(bool)), this, SLOT(on_voltage100u_triggered()));
    connect(voltage200u, SIGNAL(triggered(bool)), this, SLOT(on_voltage200u_triggered()));
    connect(voltage500u, SIGNAL(triggered(bool)), this, SLOT(on_voltage500u_triggered()));
    connect(voltage1000u, SIGNAL(triggered(bool)), this, SLOT(on_voltage1000u_triggered()));
    connect(voltage2000u, SIGNAL(triggered(bool)), this, SLOT(on_voltage2000u_triggered()));
    connect(voltage5000u, SIGNAL(triggered(bool)), this, SLOT(on_voltage5000u_triggered()));
}

void MainWindow::create10x1Layout(){
    QVBoxLayout *mainLayout = new QVBoxLayout;
    for(int i=0;i<10;i++){
        channelGraph[i] = new QCustomPlot;
        mainLayout->addWidget(channelGraph[i]);
        channelGraph[i]->xAxis->setVisible(true);
        channelGraph[i]->axisRect()->setAutoMargins(QCP::msNone);
        channelGraph[i]->axisRect()->setMargins(QMargins(75,0,0,0));
        channelGraph[i]->yAxis->setRange(-0.21, 1.42, Qt::AlignLeft);
        channelGraph[i]->addGraph();
        channelGraph[i]->xAxis->setTickStep((double) (3000000.0/((14.0)*(NeutrinoChannel->getNumChannels()+2.0))));
        channelGraph[i]->yAxis->setLabel("Channel "+QString::number(i+1,10)+" (V)");
        channelGraph[i]->yAxis->setLabelFont(QFont(font().family(), 10));
        channelGraph[i]->graph()->setPen(QPen(colorList[i]));
    }

//    channelGraph[0]->graph()->setPen(QPen(Qt::black));
//    channelGraph[1]->graph()->setPen(QPen(Qt::darkRed));
//    channelGraph[2]->graph()->setPen(QPen(Qt::darkGreen));
//    channelGraph[3]->graph()->setPen(QPen(Qt::darkCyan));
//    channelGraph[4]->graph()->setPen(QPen(Qt::blue));
//    channelGraph[5]->graph()->setPen(QPen(Qt::darkBlue));
//    channelGraph[6]->graph()->setPen(QPen(Qt::darkMagenta));
//    channelGraph[7]->graph()->setPen(QPen(Qt::darkYellow));
//    channelGraph[8]->graph()->setPen(QPen(Qt::darkGray));
//    channelGraph[9]->graph()->setPen(QPen(Qt::red));

    channelGraph[9]->axisRect()->setMargins(QMargins(75,0,0,15));

    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}

void MainWindow::create5x2Layout(){
    QVBoxLayout *leftLayout = new QVBoxLayout;
    QVBoxLayout *rightLayout = new QVBoxLayout;
    QVBoxLayout *lowerLayout = new QVBoxLayout;

    for (int i=0;i<11;i++){
        channelGraph[i] = new QCustomPlot;
        channelGraph[i]->xAxis->setVisible(true);
        channelGraph[i]->axisRect()->setAutoMargins(QCP::msNone);
        channelGraph[i]->axisRect()->setMargins(QMargins(75,0,0,0));
        channelGraph[i]->yAxis->setRange(-0.21, 1.42, Qt::AlignLeft);
    }
//    channelGraph[4]->axisRect()->setMargins(QMargins(75,0,0,15));
//    channelGraph[9]->axisRect()->setMargins(QMargins(75,0,0,15));
//    channelGraph[10]->axisRect()->setMargins(QMargins(75,0,0,30));

    for(int i=0;i<5;i++){
        leftLayout->addWidget(channelGraph[i]);
    }

    for(int i=5;i<10;i++){
        rightLayout->addWidget(channelGraph[i]);
    }
    channelGraph[10]->setMaximumHeight(250);
    lowerLayout->addWidget(channelGraph[10]);



    for(int i=0;i<11;i++){
        channelGraph[i]->addGraph();
        channelGraph[i]->xAxis->setTickStep((double) (3000000.0/((14.0)*(NeutrinoChannel->getNumChannels()+2.0))));
        channelGraph[i]->yAxis->setLabel("Channel "+QString::number(i+1,10)+" (V)");
        channelGraph[i]->yAxis->setLabelFont(QFont(font().family(), 10));
        channelGraph[i]->yAxis->setAutoTickStep(false);
        channelGraph[i]->yAxis->setTickStep(0.25);
        channelGraph[i]->setInteractions(QCP::iRangeDrag);
        channelGraph[i]->axisRect()->setRangeDrag(Qt::Vertical);
        channelGraph[i]->graph()->setPen(QPen(colorList[i]));
    }

    QHBoxLayout *uppperLayout = new QHBoxLayout;
    QVBoxLayout *mainLayout = new QVBoxLayout;

    QWidget *mainWidget = new QWidget;
    uppperLayout->addLayout(leftLayout);
    uppperLayout->addLayout(rightLayout);
    mainLayout->addLayout(uppperLayout);
    mainLayout->addLayout(lowerLayout);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}

void MainWindow::createMenus(){
//--------------------------- FILE MENU -----------------------------//
    fileMenu = menuBar()->addMenu(tr("&File"));
//    fileMenu->addAction(swapAction);
    fileMenu->addAction(filterAction);
    fileMenu->addSeparator();
    fileMenu->addAction(dataAnalyzerAction);
    fileMenu->addSeparator();
//    fileMenu->addAction(connectAction);
//    fileMenu->addAction(disconnectAction);
    fileMenu->addAction(commandAction);
    fileMenu->addAction(pauseAction);
    fileMenu->addSeparator();

    fileMenu->addAction(recordAction);
    fileMenu->addAction(chooseDirectoryAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
//--------------------------- FILE MENU -----------------------------//

//-------------------------- LAYOUT MENU ----------------------------//
    layoutMenu = menuBar()-> addMenu(tr("&Layout"));
    layoutMenu->addAction(tenby1Action);
    layoutMenu->addAction(fiveby2Action);
//-------------------------- LAYOUT MENU ----------------------------//

//------------------------- TIMEFRAME MENU --------------------------//
    timeFrameMenu = menuBar()->addMenu(tr("&Time Scales"));
    timeFrameGroup = new QActionGroup(this);
    timeFrameMenu->addAction(timeFrame10ms);
    timeFrame10ms->setCheckable(true);
    timeFrameMenu->addAction(timeFrame20ms);
    timeFrame20ms->setCheckable(true);
    timeFrameMenu->addAction(timeFrame50ms);
    timeFrame50ms->setCheckable(true);
    timeFrameMenu->addAction(timeFrame100ms);
    timeFrame100ms->setCheckable(true);
    timeFrameMenu->addAction(timeFrame200ms);
    timeFrame200ms->setCheckable(true);
    timeFrameMenu->addAction(timeFrame500ms);
    timeFrame500ms->setCheckable(true);
    timeFrameMenu->addAction(timeFrame1000ms);
    timeFrame1000ms->setCheckable(true);
    timeFrameMenu->addAction(timeFrame2000ms);
    timeFrame2000ms->setCheckable(true);
    timeFrameMenu->addAction(timeFrame5000ms);
    timeFrame5000ms->setCheckable(true);

    timeFrameGroup = new QActionGroup(this);
    timeFrameGroup->addAction(timeFrame10ms);
    timeFrameGroup->addAction(timeFrame20ms);
    timeFrameGroup->addAction(timeFrame50ms);
    timeFrameGroup->addAction(timeFrame100ms);
    timeFrame100ms->setChecked(true);
    timeFrameGroup->addAction(timeFrame200ms);
    timeFrameGroup->addAction(timeFrame500ms);
    timeFrameGroup->addAction(timeFrame1000ms);
    timeFrameGroup->addAction(timeFrame2000ms);
    timeFrameGroup->addAction(timeFrame5000ms);
//------------------------- TIMEFRAME MENU --------------------------//
  
    timeFrameMenu->addSeparator();
    timeFrameMenu->addAction(resetDefaultX);

//------------------------- TIMEFRAME MENU --------------------------//
    voltageMenu = menuBar()->addMenu(tr("&Voltage Scales"));
    voltageMenu->addAction(voltage50u);
    voltage50u->setCheckable(true);
    voltageMenu->addAction(voltage100u);
    voltage100u->setCheckable(true);
    voltageMenu->addAction(voltage200u);
    voltage200u->setCheckable(true);
    voltageMenu->addAction(voltage500u);
    voltage500u->setCheckable(true);
    voltageMenu->addAction(voltage1000u);
    voltage1000u->setCheckable(true);
    voltageMenu->addAction(voltage2000u);
    voltage2000u->setCheckable(true);
    voltageMenu->addAction(voltage5000u);
    voltage5000u->setCheckable(true);

    voltageGroup = new QActionGroup(this);
    voltageGroup->addAction(voltage50u);
    voltageGroup->addAction(voltage100u);
    voltageGroup->addAction(voltage200u);
    voltageGroup->addAction(voltage500u);
    voltageGroup->addAction(voltage1000u);
    voltageGroup->addAction(voltage2000u);
    voltageGroup->addAction(voltage5000u);
    voltage5000u->setChecked(true);

    voltageMenu->addSeparator();
    voltageMenu->addAction(resetDefaultY);
//------------------------- TIMEFRAME MENU --------------------------//

//------------------------ CONNECTIVITY MENU ------------------------//
//    connectivityMenu = menuBar()->addMenu(tr("Connection Mode"));
//    connectivityGroup = new QActionGroup(this);
//    connectivityMenu->addAction(wifiMode);
//    connectivityMenu->addAction(wiredMode);
//    wifiMode->setCheckable(true);
//    wifiMode->setChecked(true);
//    wiredMode->setCheckable(true);

//    connectivityGroup->addAction(wifiMode);
//    connectivityGroup->addAction(wiredMode);
//------------------------ CONNECTIVITY MENU ------------------------//
}

void MainWindow::createStatusBar(){
    statusBarLabel = new QLabel;
    statusBarMainWindow = statusBar();
    statusBarMainWindow->addPermanentWidget(statusBarLabel, 1);
    statusBarMainWindow->setSizeGripEnabled(false);  // fixed window size
}

void MainWindow::connectNeutrino(){
    portInfo = QSerialPortInfo::availablePorts();
    connectionStatus.clear();
    if(portInfo.size()>0){
        serialNeutrino->doConnect();
        connectionStatus.clear();
        if(serialNeutrino->isConnected()){
            connectionStatus.append("Connected to Neutrino!!");
        }
        else{
            connectionStatus.append("Connection to Neutrino failed");
        }
        statusBarLabel->setText(connectionStatus);
    }
    if(!serialNeutrino->isConnected()){
        socketNeutrino->doConnect("192.168.4.1", 8888);
        connectionStatus.clear();
        if(socketNeutrino->isConnected()){
            connectionStatus.append("Connected to Neutrino WiFi Module at 192.168.42.1/8888");
        }
        else{
            connectionStatus.append("Connection to Neutrino failed! Restart this program after connecting Neutrino.");
            QMessageBox::information(this, "Failed to connect!", "No Neutrino device detected.. \n"
                                                                 "Check your connections and run the program again..");
        }
        statusBarLabel->setText(connectionStatus);
    }
}

MainWindow::~MainWindow(){
    if(socketNeutrino->isConnected()){
        socketNeutrino->writeCommand(QByteArray::number(255, 10));
        socketNeutrino->doDisconnect();
    }
    serialNeutrino->closePort();
}

void MainWindow::updateData(){
    QVector<double> X_axis = data->retrieveXAxis();
    if(data->isPlotEnabled() && X_axis.size() >= (data->getNumDataPoints())){
        for(int i=0; i<10; i++){
            if(!data->isEmpty(i)){
                channelGraph[i]->graph()->setData(X_axis, data->retrieveData(i));
                channelGraph[i]->xAxis->setRange(X_axis.at(0),
                                                 (data->getNumDataPoints())*(14.0*(NeutrinoChannel->getNumChannels()+2.0)/3000000.0),
                                                 Qt::AlignLeft);
                if(!pause){
                    channelGraph[i]->replot();
                }
                data->clearChannelData(i);
            }
        }
        data->removeXAxis();
    }
}

void MainWindow::on_timeFrame10_triggered(){
    data->setNumDataPoints(TimeFrames10ms, (double) (3000000.0/((14.0)*(NeutrinoChannel->getNumChannels()+2.0))));
    data->clearallChannelData();
}

void MainWindow::on_timeFrame20_triggered(){
    data->setNumDataPoints(TimeFrames20ms, (double) (3000000.0/((14.0)*(NeutrinoChannel->getNumChannels()+2.0))));
    data->clearallChannelData();
}

void MainWindow::on_timeFrame50_triggered(){
    data->setNumDataPoints(TimeFrames50ms, (double) (3000000.0/((14.0)*(NeutrinoChannel->getNumChannels()+2.0))));
    data->clearallChannelData();
}

void MainWindow::on_timeFrame100_triggered(){
    data->setNumDataPoints(TimeFrames100ms, (double) (3000000.0/((14.0)*(NeutrinoChannel->getNumChannels()+2.0))));
    data->clearallChannelData();
}

void MainWindow::on_timeFrame200_triggered(){
    data->setNumDataPoints(TimeFrames200ms, (double) (3000000.0/((14.0)*(NeutrinoChannel->getNumChannels()+2.0))));
    data->clearallChannelData();
}

void MainWindow::on_timeFrame500_triggered(){
    data->setNumDataPoints(TimeFrames500ms, (double) (3000000.0/((14.0)*(NeutrinoChannel->getNumChannels()+2.0))));
    data->clearallChannelData();
}

void MainWindow::on_timeFrame1000_triggered(){
    data->setNumDataPoints(TimeFrames1000ms, (double) (3000000.0/((14.0)*(NeutrinoChannel->getNumChannels()+2.0))));
    data->clearallChannelData();
}

void MainWindow::on_timeFrame2000_triggered(){
    data->setNumDataPoints(TimeFrames2000ms, (double) (3000000.0/((14.0)*(NeutrinoChannel->getNumChannels()+2.0))));
    data->clearallChannelData();
}

void MainWindow::on_timeFrame5000_triggered(){
    data->setNumDataPoints(TimeFrames5000ms, (double) (3000000.0/((14.0)*(NeutrinoChannel->getNumChannels()+2.0))));
    data->clearallChannelData();
}

void MainWindow::on_voltage50u_triggered(){
    for(int i = 0; i < 10; i++){
        if(!data->isFilterEnabled()){
            channelGraph[i]->yAxis->setRange(0.4895, 0.021, Qt::AlignLeft);
        }
        else{
            channelGraph[i]->yAxis->setRange(-0.0105, 0.021, Qt::AlignLeft);
        }
        channelGraph[i]->yAxis->setTickStep(0.0025);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_voltage100u_triggered(){
    for(int i = 0; i < 11; i++){
        if(!data->isFilterEnabled()){
            channelGraph[i]->yAxis->setRange(0.479, 0.042, Qt::AlignLeft);
        }
        else{
            channelGraph[i]->yAxis->setRange(-0.021, 0.042, Qt::AlignLeft);
        }
        channelGraph[i]->yAxis->setTickStep(0.005);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_voltage200u_triggered(){
    for(int i = 0; i < 10; i++){
        if(!data->isFilterEnabled()){
            channelGraph[i]->yAxis->setRange(0.449, 0.102, Qt::AlignLeft);
        }
        else{
            channelGraph[i]->yAxis->setRange(-0.051, 0.102, Qt::AlignLeft);
        }
        channelGraph[i]->yAxis->setTickStep(0.01);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_voltage500u_triggered(){
    for(int i = 0; i < 10; i++){
        if(!data->isFilterEnabled()){
            channelGraph[i]->yAxis->setRange(0.399, 0.202, Qt::AlignLeft);
        }
        else{
            channelGraph[i]->yAxis->setRange(-0.101, 0.202, Qt::AlignLeft);
        }
        channelGraph[i]->yAxis->setTickStep(0.02);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_voltage1000u_triggered(){
    for(int i = 0; i < 10; i++){
        if(!data->isFilterEnabled()){
            channelGraph[i]->yAxis->setRange(0.29, 0.42, Qt::AlignLeft);
        }
        else{
            channelGraph[i]->yAxis->setRange(-0.21, 0.42, Qt::AlignLeft);
        }
        channelGraph[i]->yAxis->setTickStep(0.05);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_voltage2000u_triggered(){
    for(int i = 0; i < 10; i++){
        if(!data->isFilterEnabled()){
            channelGraph[i]->yAxis->setRange(-0.01, 1.02, Qt::AlignLeft);
        }
        else{
            channelGraph[i]->yAxis->setRange(-0.51, 1.02, Qt::AlignLeft);
        }
        channelGraph[i]->yAxis->setTickStep(0.1);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_voltage5000u_triggered(){
    for(int i = 0; i < 11; i++){
        if(!data->isFilterEnabled()){
            channelGraph[i]->yAxis->setRange(-0.21, 1.42, Qt::AlignLeft);
        }
        else{
            channelGraph[i]->yAxis->setRange(-0.71, 1.42, Qt::AlignLeft);
        }
        channelGraph[i]->yAxis->setTickStep(0.25);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_record_triggered(){
    if(!data->isRecordEnabled()){
        data->setRecordEnabled(true);
        statusBarLabel->setText("<b><FONT COLOR='#ff0000' FONT SIZE = 4> Recording...</b>");
        recordAction->setText("Stop &Recording");
    }
    else if(data->isRecordEnabled()){
        data->setRecordEnabled(false);
        statusBarLabel->setText("<b><FONT COLOR='#ff0000' FONT SIZE = 4> Recording stopped!!! File saved to " + data->getFileName() + "</b>");
        recordAction->setText("Start &Recording");
    }
}

void MainWindow::on_playPause_triggered(){
    if(pause){
        pauseAction->setText("Pause graph");
    }
    else{
        pauseAction->setText("Resume graph");
    }
    pause = !pause;
}

void MainWindow::on_chooseDirectory_triggered(){
    statusBarLabel->setText("Set your preferred save file directory");
    data->setDirectory(QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 QDir::homePath() + "/Desktop/",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks));

    statusBarLabel->setText("Save directory set to: " + data->getDirectory());
}

void MainWindow::on_resetX_triggered(){
    data->setNumDataPoints(TimeFrames100ms, (double) (3000000.0/((14.0)*(NeutrinoChannel->getNumChannels()+2.0))));
    data->clearallChannelData();
    timeFrame100ms->setChecked(true);
}

void MainWindow::on_swap_triggered(){
    serialNeutrino->swapPort();
    statusBarLabel->setText("Port swapped");
}

void MainWindow::on_filterConfig_trigger(){
    FilterDialog filterDialog(data);
    filterDialog.exec();
}

void MainWindow::on_resetY_triggered(){
    for(int i=0;i<10;i++){
        if(!data->isFilterEnabled()){
            channelGraph[i]->yAxis->setRange(-0.21, 1.42, Qt::AlignLeft);
        }
        else{
            channelGraph[i]->yAxis->setRange(-0.71, 1.42, Qt::AlignLeft);
        }
        channelGraph[i]->yAxis->setTickStep(0.25);
        channelGraph[i]->replot();
    }
    voltage5000u->setChecked(true);
}

//void MainWindow::on_ConnectMenu_triggered(){
//    statusBarLabel->setText("Connection Dialog Opened");
//    ConnectionDialog connectionDialog(socketNeutrino);
//    connectionDialog.exec();
//    if(socketNeutrino->isConnected()){
//        statusBarLabel->setText("Connected!!");
//    }
//    else{
//        statusBarLabel->setText("Disconnected!!");
//    }
//}

//void MainWindow::on_DisconnectMenu_triggered(){
//    if(socketNeutrino->isConnected()){
//        qDebug() << "Disconnecting, please wait...";
//        socketNeutrino->writeCommand(QByteArray::number(255, 10));
//        socketNeutrino->doDisconnect();
//    }
//    if(!socketNeutrino->isConnected()){
//        statusBarLabel->setText("Disconnected!!");
//    }
//}

void MainWindow::on_dataAnalyzer_triggered(){
    QProcess *process = new QProcess(this);
    QString file = QDir::currentPath() + "/DataAnalyzer.exe";
    statusBarLabel->setText("Opening: " + file);
    process->start(file);
}

void MainWindow::on_CommandMenu_triggered(){
    statusBarLabel->setText("Command Dialog Opened");
    CommandDialog commandDialog(socketNeutrino, NeutrinoCommand, NeutrinoChannel, serialNeutrino);
    commandDialog.exec();

}

void MainWindow::on_tenby1_triggered(){
    create10x1Layout();
}

void MainWindow::on_fiveby2_triggered(){
    create5x2Layout();
}

//void MainWindow::on_wifi_triggered(){
//    qDebug() << "ready via Wifi";
//    serialNeutrino->closePort();
//    serialNeutrino->serialenabled = false;
//    socketNeutrino->wifiEnabled = true;
//}

//void MainWindow::on_wired_triggered(){
//    serialNeutrino->serialenabled = true;
//    if(serialNeutrino->doConnect()){
//        qDebug() << "ready via USB";
//    }
//    socketNeutrino->wifiEnabled = false;
//}
