#include "mainwindow.h"

MainWindow::MainWindow(){
    QString version(APP_VERSION);
    timer.start();
//    createStatusBar();
#ifdef NEUTRINO_II
    NeutrinoChannel = new Channel;
    NeutrinoCommand = new Command(NeutrinoChannel);
    data = new DataProcessor(NeutrinoChannel);
    serialNeutrino = new SerialChannel(this, NeutrinoCommand, data, NeutrinoChannel);
    portInfo = QSerialPortInfo::availablePorts();
    if(portInfo.size()>0){
        qDebug() << portInfo.at(0).portName();
        QProcess::execute("sudo chmod a+rw /dev/" + portInfo.at(0).portName());
    }
    socketEdison = new SocketEdison(this, NeutrinoCommand, data, NeutrinoChannel);
    setWindowTitle(tr("Sinapse NeutrinoII Recording Software V") + version);
    create5x2Layout();
#endif //NEUTRINO_II MAINWINDOW

#ifdef SYLPH
    defaultRange = new QCPRange(-0.00050, 0.00100);
    setWindowTitle(tr("SINAPSE Sylph Recording Software V") + version + " Beta");
    data = new DataProcessor;
    serialChannel = new SerialChannel(this, data);
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(updateData()));
    dataTimer.start(1);     //tick timer every XXX msec
    createStatusBar();
    createLayout();
    createActions();
    createMenus();
    statusBarLabel->setText("Insufficient Serial Devices...");
    portInfo = QSerialPortInfo::availablePorts();
    if(portInfo.size()>1){
        if(serialChannel->enableImplantPort(portInfo.at(1).portName())){
            connectionStatus.append("Connected to Implant Port |");
        }
        else{
            connectionStatus.append("Connection to Implant Port failed |");
        }
        if(serialChannel->enableADCPort(portInfo.at(0).portName())){
            connectionStatus.append(" Connected to ADC Port");
        }
        else{
            connectionStatus.append(" Connection to ADC Port failed");
        }
        statusBarLabel->setText(connectionStatus);
    }
    else{
        QMessageBox::information(this, "Insufficient Serial Devices",
                                 "Please check Serial Devices connection and try again. \n"
                                 "Press Ctrl+E to open up Serial Port Configurations");
    }
#endif //SYLPH MAINWAINDOW
//    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(updateData()));
//    dataTimer.start(1);     //tick timer every XXX msec
//    createActions();
//    createMenus();
}

#ifdef SYLPH
void MainWindow::createLayout(){
    QVBoxLayout *mainLayout = new QVBoxLayout;
    for(int i=0;i<4;i++){
        channelGraph[i] = new QCustomPlot;
        mainLayout->addWidget(channelGraph[i]);
        channelGraph[i]->xAxis->setVisible(true);
        channelGraph[i]->axisRect()->setAutoMargins(QCP::msNone);
        channelGraph[i]->axisRect()->setMargins(QMargins(75,10,0,15));
        channelGraph[i]->yAxis->setRange(-0.00050, 0.00100, Qt::AlignLeft);
        channelGraph[i]->addGraph();
        channelGraph[i]->yAxis->setAutoTickStep(false);
        channelGraph[i]->xAxis->setAutoTickStep(false);
        channelGraph[i]->xAxis->setTickStep(0.01);
        channelGraph[i]->yAxis->setTickStep(0.0001);
    }

    connect(channelGraph[0], SIGNAL(mousePress(QMouseEvent*)), this, SLOT(on_graph1_clicked()));
    connect(channelGraph[1], SIGNAL(mousePress(QMouseEvent*)), this, SLOT(on_graph2_clicked()));
    connect(channelGraph[2], SIGNAL(mousePress(QMouseEvent*)), this, SLOT(on_graph3_clicked()));

    channelGraph[0]->yAxis->setLabel("Channel 1 (V)");
    channelGraph[0]->yAxis->setLabelFont(QFont(font().family(), 11));
    channelGraph[1]->yAxis->setLabel("Channel 2 (V)");
    channelGraph[1]->yAxis->setLabelFont(QFont(font().family(), 11));
    channelGraph[2]->yAxis->setLabel("Sync Pulse (V)");
    channelGraph[2]->yAxis->setLabelPadding(35);
    channelGraph[2]->yAxis->setLabelFont(QFont(font().family(), 11));
    channelGraph[3]->yAxis->setLabel("Frame Marker");
    channelGraph[3]->yAxis->setLabelPadding(35);
    channelGraph[3]->yAxis->setLabelFont(QFont(font().family(), 11));

    channelGraph[0]->graph()->setPen(QPen(Qt::red));
    channelGraph[1]->graph()->setPen(QPen(Qt::black));
    channelGraph[2]->graph()->setPen(QPen(Qt::black));
    channelGraph[3]->graph()->setPen(QPen(Qt::darkGreen));

    channelGraph[2]->yAxis->setRange(0, 2.5, Qt::AlignLeft);
    channelGraph[2]->yAxis->setTickStep(0.5);
    channelGraph[3]->yAxis->setRange(0, 250, Qt::AlignLeft);
    channelGraph[3]->yAxis->setTickStep(50);
    channelGraph[3]->axisRect()->setMargins(QMargins(75,10,0,15));

    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}

#endif //SYLPH CREATELAYOUT

void MainWindow::createActions(){
#ifdef NEUTRINO_II
    connectAction = new QAction(tr("&Connect"), this);
    connectAction->setShortcut(tr("Ctrl+C"));
    connect(connectAction, SIGNAL(triggered()), this, SLOT(on_ConnectMenu_triggered()));

    disconnectAction = new QAction(tr("&Disconnect"), this);
    disconnectAction->setShortcut(tr("Ctrl+D"));
    connect(disconnectAction, SIGNAL(triggered()), this, SLOT(on_DisconnectMenu_triggered()));

    commandAction = new QAction(tr("Comm&and"), this);
    commandAction->setShortcut(tr("Ctrl+A"));
    connect(commandAction, SIGNAL(triggered()), this, SLOT(on_CommandMenu_triggered()));

    tenby1Action = new QAction(tr("10x&1 Layout"), this);
    tenby1Action->setShortcut(tr("Ctrl+1"));
    connect(tenby1Action, SIGNAL(triggered()),this,SLOT(on_tenby1_triggered()));

    fiveby2Action = new QAction(tr("5x&2 Layout"), this);
    fiveby2Action->setShortcut(tr("Ctrl+2"));
    connect(fiveby2Action, SIGNAL(triggered()),this,SLOT(on_fiveby2_triggered()));

    wifiMode = new QAction(tr("Wireless Mode"));
    wiredMode = new QAction(tr("Wired Mode"));

    connect(wifiMode, SIGNAL(triggered(bool)), this, SLOT(on_wifi_triggered()));
    connect(wiredMode, SIGNAL(triggered(bool)), this, SLOT(on_wired_triggered()));
#endif //NEUTRINO_II CREATEACTIONS

#ifdef SYLPH
    //    serialPortAction = new QAction(tr("S&erial Port Configuration"), this);
    //    serialPortAction->setShortcut(tr("Ctrl+E"));
    //    connect(serialPortAction, SIGNAL(triggered()), this, SLOT(on_serialConfig_triggered()));

        filterAction = new QAction(tr("Filter Configuration"), this);
        filterAction->setShortcut(tr("Ctrl+F"));
        connect(filterAction, SIGNAL(triggered(bool)), this, SLOT(on_filterConfig_trigger()));

        resetDefaultX = new QAction(tr("Default Time Scale"), this);
        resetDefaultX->setShortcut(tr("Ctrl+X"));
        connect(resetDefaultX, SIGNAL(triggered()), this, SLOT(on_resetX_triggered()));

        resetDefaultY = new QAction(tr("Default Voltage Scale"), this);
        resetDefaultY->setShortcut(tr("Ctrl+Y"));
        connect(resetDefaultY, SIGNAL(triggered()), this, SLOT(on_resetY_triggered()));

        voltage50u = new QAction(tr("+/- 50uV"));
        voltage100u = new QAction(tr("+/- 100uV"));
        voltage200u = new QAction(tr("+/- 200uV"));
        voltage500u = new QAction(tr("+/- 500uV"));
        voltage1000u = new QAction(tr("+/- 1000uV"));
        voltage2000u = new QAction(tr("+/- 2000uV"));
        voltage5000u = new QAction(tr("+/- 5000uV"));

        connect(voltage50u, SIGNAL(triggered(bool)), this, SLOT(on_voltage50u_triggered()));
        connect(voltage100u, SIGNAL(triggered(bool)), this, SLOT(on_voltage100u_triggered()));
        connect(voltage200u, SIGNAL(triggered(bool)), this, SLOT(on_voltage200u_triggered()));
        connect(voltage500u, SIGNAL(triggered(bool)), this, SLOT(on_voltage500u_triggered()));
        connect(voltage1000u, SIGNAL(triggered(bool)), this, SLOT(on_voltage1000u_triggered()));
        connect(voltage2000u, SIGNAL(triggered(bool)), this, SLOT(on_voltage2000u_triggered()));
        connect(voltage5000u, SIGNAL(triggered(bool)), this, SLOT(on_voltage5000u_triggered()));

        audio1 = new QAction(tr("Channel 1 Audio Output"));
        audio2 = new QAction(tr("Channel 2 Audio Output"));
        audio3 = new QAction(tr("Sync Pulse Audio Output"));

        connect(audio1, SIGNAL(triggered(bool)), this, SLOT(on_graph1_clicked()));
        connect(audio2, SIGNAL(triggered(bool)), this, SLOT(on_graph2_clicked()));
        connect(audio3, SIGNAL(triggered(bool)), this, SLOT(on_graph3_clicked()));

        aboutAction = new QAction(tr("About SINAPSE Recording Software"));
        connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(about()));
#endif //SYLPH CREATEACTIONS

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
}

#ifdef NEUTRINO_II
void MainWindow::create10x1Layout(){
    QVBoxLayout *mainLayout = new QVBoxLayout;
    for(int i=0;i<10;i++){
        channelGraph[i] = new QCustomPlot;
        mainLayout->addWidget(channelGraph[i]);
        channelGraph[i]->xAxis->setVisible(true);
        channelGraph[i]->axisRect()->setAutoMargins(QCP::msNone);
        channelGraph[i]->axisRect()->setMargins(QMargins(35,0,0,0));
        channelGraph[i]->yAxis->setRange(-0.01, 1.21, Qt::AlignLeft);
        channelGraph[i]->addGraph();
        channelGraph[i]->xAxis->setTickStep(0.000056);
        connect(channelGraph[i]->yAxis, SIGNAL(rangeChanged(QCPRange)), channelGraph[i]->yAxis2, SLOT(setRange(QCPRange)));
        connect(channelGraph[i]->xAxis, SIGNAL(rangeChanged(QCPRange)), channelGraph[i]->xAxis2, SLOT(setRange(QCPRange)));
    }

    channelGraph[0]->graph()->setPen(QPen(Qt::black));
    channelGraph[1]->graph()->setPen(QPen(Qt::darkRed));
    channelGraph[2]->graph()->setPen(QPen(Qt::darkGreen));
    channelGraph[3]->graph()->setPen(QPen(Qt::darkCyan));
    channelGraph[4]->graph()->setPen(QPen(Qt::blue));
    channelGraph[5]->graph()->setPen(QPen(Qt::darkBlue));
    channelGraph[6]->graph()->setPen(QPen(Qt::darkMagenta));
    channelGraph[7]->graph()->setPen(QPen(Qt::darkYellow));
    channelGraph[8]->graph()->setPen(QPen(Qt::darkGray));
    channelGraph[9]->graph()->setPen(QPen(Qt::red));

    channelGraph[9]->axisRect()->setMargins(QMargins(35,0,0,15));

    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}

void MainWindow::create5x2Layout(){
    QVBoxLayout *leftLayout = new QVBoxLayout;
    for(int i=0;i<4;i++){
        channelGraph[i] = new QCustomPlot;
        leftLayout->addWidget(channelGraph[i]);
        channelGraph[i]->xAxis->setVisible(true);
        channelGraph[i]->axisRect()->setAutoMargins(QCP::msNone);
        channelGraph[i]->axisRect()->setMargins(QMargins(35,0,0,0));
        channelGraph[i]->yAxis->setRange(-0.01, 1.21, Qt::AlignLeft);
    }
    channelGraph[4] = new QCustomPlot;
    leftLayout->addWidget(channelGraph[4]);
    channelGraph[4]->xAxis->setVisible(true);
    channelGraph[4]->axisRect()->setAutoMargins(QCP::msNone);
    channelGraph[4]->axisRect()->setMargins(QMargins(35,0,0,15));
    channelGraph[4]->yAxis->setRange(-0.01, 1.21, Qt::AlignLeft);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    for(int i=5;i<9;i++){
        channelGraph[i] = new QCustomPlot;
        rightLayout->addWidget(channelGraph[i]);
        channelGraph[i]->xAxis->setVisible(true);
        channelGraph[i]->axisRect()->setAutoMargins(QCP::msNone);
        channelGraph[i]->axisRect()->setMargins(QMargins(35,0,0,0));
        channelGraph[i]->yAxis->setRange(-0.01, 1.21, Qt::AlignLeft);
    }
    channelGraph[9] = new QCustomPlot;
    rightLayout->addWidget(channelGraph[9]);
    channelGraph[9]->xAxis->setVisible(true);
    channelGraph[9]->axisRect()->setAutoMargins(QCP::msNone);
    channelGraph[9]->axisRect()->setMargins(QMargins(35,0,0,15));
    channelGraph[9]->yAxis->setRange(-0.01, 1.21, Qt::AlignLeft);

    for(int i=0;i<10;i++){
        channelGraph[i]->addGraph();
        channelGraph[i]->xAxis->setTickStep(0.000056);
        connect(channelGraph[i]->yAxis, SIGNAL(rangeChanged(QCPRange)), channelGraph[i]->yAxis2, SLOT(setRange(QCPRange)));
        connect(channelGraph[i]->xAxis, SIGNAL(rangeChanged(QCPRange)), channelGraph[i]->xAxis2, SLOT(setRange(QCPRange)));
    }

    channelGraph[0]->graph()->setPen(QPen(Qt::black));
    channelGraph[1]->graph()->setPen(QPen(Qt::darkRed));
    channelGraph[2]->graph()->setPen(QPen(Qt::darkGreen));
    channelGraph[3]->graph()->setPen(QPen(Qt::darkCyan));
    channelGraph[4]->graph()->setPen(QPen(Qt::blue));
    channelGraph[5]->graph()->setPen(QPen(Qt::darkBlue));
    channelGraph[6]->graph()->setPen(QPen(Qt::darkMagenta));
    channelGraph[7]->graph()->setPen(QPen(Qt::darkYellow));
    channelGraph[8]->graph()->setPen(QPen(Qt::darkGray));
    channelGraph[9]->graph()->setPen(QPen(Qt::red));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    QWidget *mainWidget = new QWidget;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}
#endif //NEUTRINO_II CREATE10X1LAYOUT CREATE5X2LAYOUT

void MainWindow::createMenus(){
    fileMenu = menuBar()->addMenu(tr("&File"));

#ifdef NEUTRINO_II
    fileMenu->addAction(connectAction);
    fileMenu->addAction(disconnectAction);
    fileMenu->addAction(commandAction);
    fileMenu->addSeparator();
#endif //NEUTRINO_II CREATEMENU FILEMENU

#ifdef SYLPH
//    fileMenu->addAction(serialPortAction);
    fileMenu->addAction(filterAction);
//    fileMenu->addAction(resetDefaultRange);
    fileMenu->addSeparator();

    fileMenu->addAction(pauseAction);
    fileMenu->addSeparator();
#endif //SYLPH CREATEMENU FILEMENU

    fileMenu->addAction(pauseAction);
    fileMenu->addSeparator();

    fileMenu->addAction(recordAction);
    fileMenu->addAction(chooseDirectoryAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

#ifdef NEUTRINO_II
    layoutMenu = menuBar()-> addMenu(tr("&Layout"));
    layoutMenu->addAction(tenby1Action);
    layoutMenu->addAction(fiveby2Action);
#endif //NEUTRINO_II CREATEMENUA LAYOUTMENU

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

    timeFrameMenu->addSeparator();
    timeFrameMenu->addAction(resetDefaultX);

#ifdef NEUTRINO_II
    connectivityMenu = menuBar()->addMenu(tr("Connection Mode"));
    connectivityGroup = new QActionGroup(this);
    connectivityMenu->addAction(wifiMode);
    connectivityMenu->addAction(wiredMode);
    wifiMode->setCheckable(true);
    wifiMode->setChecked(true);
    wiredMode->setCheckable(true);

    connectivityGroup->addAction(wifiMode);
    connectivityGroup->addAction(wiredMode);
#endif //NEUTRINO_II CREATEMENU CONNECTIVITYMENU

#ifdef SYLPH
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
    voltage500u->setChecked(true);
    voltageGroup->addAction(voltage1000u);
    voltageGroup->addAction(voltage2000u);
    voltageGroup->addAction(voltage5000u);

    voltageMenu->addSeparator();
    voltageMenu->addAction(resetDefaultY);

    audioOutputMenu = menuBar()->addMenu(tr("Audio Output"));
    audioOutputMenu->addAction(audio1);
    audio1->setCheckable(true);
    audioOutputMenu->addAction(audio2);
    audio2->setCheckable(true);
    audioOutputMenu->addAction(audio3);
    audio3->setCheckable(true);

    audioGroup = new QActionGroup(this);
    audioGroup->addAction(audio1);
    audio1->setChecked(true);
    audioGroup->addAction(audio2);
    audioGroup->addAction(audio3);

    helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(aboutAction);
#endif //SYLPH CREATEMENU VOLTAGEMENU AUDIOOUTPUTMENU HELPMENU
}

void MainWindow::createStatusBar(){
    statusBarLabel = new QLabel;
    statusBarMainWindow = statusBar();
    statusBarMainWindow->addPermanentWidget(statusBarLabel, 1);
    statusBarMainWindow->setSizeGripEnabled(false);  // fixed window size
}

MainWindow::~MainWindow(){
#ifdef NEUTRINO_II
    if(socketEdison->isConnected()){
        socketEdison->writeCommand(QByteArray::number(255, 10));
        socketEdison->doDisconnect();
    }
    serialNeutrino->closePort();
#endif //NEUTRINO_II ~MAINWINDOW

#ifdef SYLPH
    serialChannel->closeADCPort();
    serialChannel->closeImplantPort();
#endif //SYLPH ~MAINWINDOW
}

void MainWindow::updateData(){
#ifdef NEUTRINO_II
    QVector<double> X_axis = data->retrieveXAxis();
    if(data->isPlotEnabled() && X_axis.size() > (data->getNumDataPoints())){
        for(int i=0; i<10; i++){
            if(!data->isEmpty(i)){
                channelGraph[i]->graph()->setData(X_axis, data->retrieveData(i));
                channelGraph[i]->xAxis->setRange(X_axis.at(0), (data->getNumDataPoints())*0.000056, Qt::AlignLeft);
                channelGraph[i]->replot();
                data->clearChannelData(i);
            }
        }
        data->removeXAxis();
    }
#endif

#ifdef SYLPH
    QVector<double> X_axis = data->retrieveXAxis();
    if(X_axis.size() >= data->getNumDataPoints()){
        for(int i=0; i<4; i++){
            if(!data->isEmpty(i)){
                channelGraph[i]->graph()->setData(X_axis, data->retrieveData(i));
                channelGraph[i]->xAxis->setRange(X_axis.at(0), data->getNumDataPoints()*0.000048, Qt::AlignLeft);
                if(!pause){
                    channelGraph[i]->replot();
                }
                data->clearChannelData(i);
            }
        }
        data->removeXAxis();
    }
#endif
}

void MainWindow::on_timeFrame10_triggered(){
    data->setNumDataPoints(TimeFrames10ms);
    data->clearallChannelData();
#ifdef SYLPH
    for(int i=0;i<4;i++){
        channelGraph[i]->xAxis->setTickStep(0.001);
        channelGraph[i]->replot();
    }
#endif
}

void MainWindow::on_timeFrame20_triggered(){
    data->setNumDataPoints(TimeFrames20ms);
    data->clearallChannelData();
#ifdef SYLPH
    for(int i=0;i<4;i++){
        channelGraph[i]->xAxis->setTickStep(0.002);
        channelGraph[i]->replot();
    }
#endif
}

void MainWindow::on_timeFrame50_triggered(){
    data->setNumDataPoints(TimeFrames50ms);
    data->clearallChannelData();
#ifdef SYLPH
    for(int i=0;i<4;i++){
        channelGraph[i]->xAxis->setTickStep(0.005);
        channelGraph[i]->replot();
    }
#endif
}

void MainWindow::on_timeFrame100_triggered(){
    data->setNumDataPoints(TimeFrames100ms);
    data->clearallChannelData();
#ifdef SYLPH
    for(int i=0;i<4;i++){
        channelGraph[i]->xAxis->setTickStep(0.01);
        channelGraph[i]->replot();
    }
#endif
}

void MainWindow::on_timeFrame200_triggered(){
    data->setNumDataPoints(TimeFrames200ms);
    data->clearallChannelData();
#ifdef SYLPH
    for(int i=0;i<4;i++){
        channelGraph[i]->xAxis->setTickStep(0.02);
        channelGraph[i]->replot();
    }
#endif
}

void MainWindow::on_timeFrame500_triggered(){
    data->setNumDataPoints(TimeFrames500ms);
    data->clearallChannelData();
#ifdef SYLPH
    for(int i=0;i<4;i++){
        channelGraph[i]->xAxis->setTickStep(0.05);
        channelGraph[i]->replot();
    }
#endif
}

void MainWindow::on_timeFrame1000_triggered(){
    data->setNumDataPoints(TimeFrames1000ms);
    data->clearallChannelData();
#ifdef SYLPH
    for(int i=0;i<4;i++){
        channelGraph[i]->xAxis->setTickStep(0.1);
        channelGraph[i]->replot();
    }
#endif
}

void MainWindow::on_timeFrame2000_triggered(){
    data->setNumDataPoints(TimeFrames2000ms);
    data->clearallChannelData();
#ifdef SYLPH
    for(int i=0;i<4;i++){
        channelGraph[i]->xAxis->setTickStep(0.2);
        channelGraph[i]->replot();
    }
#endif
}

void MainWindow::on_timeFrame5000_triggered(){
    data->setNumDataPoints(TimeFrames5000ms);
    data->clearallChannelData();
#ifdef SYLPH
    for(int i=0;i<4;i++){
        channelGraph[i]->xAxis->setTickStep(0.5);
        channelGraph[i]->replot();
    }
#endif
}

void MainWindow::on_record_triggered(){
    if(!data->isRecordEnabled()){
        data->setRecordEnabled(true);
        statusBarLabel->setText("Recording...");
        recordAction->setText("Stop &Recording");
    }
    else if(data->isRecordEnabled()){
        data->setRecordEnabled(false);
        statusBarLabel->setText("Recording stopped!!! File saved to " + data->getFileName());
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

#ifdef NEUTRINO_II
void MainWindow::on_ConnectMenu_triggered(){
    statusBarLabel->setText("Connection Dialog Opened");
    ConnectionDialog connectionDialog(socketEdison);
    connectionDialog.exec();
    if(socketEdison->isConnected()){
        statusBarLabel->setText("Connected!!");
    }
    else{
        statusBarLabel->setText("Disconnected!!");
    }
}

void MainWindow::on_DisconnectMenu_triggered(){
    if(socketEdison->isConnected()){
        qDebug() << "Disconnecting, please wait...";
        socketEdison->writeCommand(QByteArray::number(255, 10));
        socketEdison->doDisconnect();
    }
    if(!socketEdison->isConnected()){
        statusBarLabel->setText("Disconnected!!");
    }
}

void MainWindow::on_CommandMenu_triggered(){
    statusBarLabel->setText("Command Dialog Opened");
    CommandDialog commandDialog(socketEdison, NeutrinoCommand, NeutrinoChannel);
    commandDialog.exec();
}

void MainWindow::on_tenby1_triggered(){
    create10x1Layout();
}

void MainWindow::on_fiveby2_triggered(){
    create5x2Layout();
}

void MainWindow::on_wifi_triggered(){
    qDebug() << "ready via Wifi";
    serialNeutrino->closePort();
    serialNeutrino->serialenabled = false;
    socketEdison->wifiEnabled = true;
}

void MainWindow::on_wired_triggered(){
    serialNeutrino->serialenabled = true;
    if(serialNeutrino->doConnect()){
        qDebug() << "ready via USB";
    }
    socketEdison->wifiEnabled = false;
}

#endif //NEUTRINO_II

#ifdef SYLPH

//void MainWindow::on_serialConfig_triggered(){
//    SerialPortDialog serialPortDialog(serialChannel);
//    serialPortDialog.exec();
//    portInfo = QSerialPortInfo::availablePorts();
//    qDebug() << "portInfo.size() = " << portInfo.size();
//    if(portInfo.size()>1){
//        if(serialChannel->enableImplantPort(portInfo.at(0).portName())){
//            QMessageBox::information(this, "Connected!", "Implant Port");
//            statusBarLabel->setText("Connected Implant Port");
//        }
//        if(serialChannel->enableADCPort(portInfo.at(1).portName())){
//            QMessageBox::information(this, "Connected!", "ADC Port");
//            statusBarLabel->setText("Connected ADC Port");
//        }
//    }
//}

void MainWindow::on_filterConfig_trigger(){
    FilterDialog filterDialog(data);
    filterDialog.exec();
}

void MainWindow::on_resetX_triggered(){
    data->setNumDataPoints(TimeFrames100ms);
    data->clearallChannelData();
    timeFrame100ms->setChecked(true);
}

void MainWindow::on_resetY_triggered(){
    for(int i=0;i<2;i++){
        channelGraph[i]->yAxis->setRange(-0.00050, 0.00100, Qt::AlignLeft);
        channelGraph[i]->replot();
    }
    channelGraph[2]->yAxis->setRange(0, 2.5, Qt::AlignLeft);
    channelGraph[3]->yAxis->setRange(0, 250, Qt::AlignLeft);
    channelGraph[3]->replot();
    voltage500u->setChecked(true);
}

void MainWindow::resetGraph1Range(){
    channelGraph[0]->yAxis->setRange(-0.00050, 0.00100, Qt::AlignLeft);
    channelGraph[0]->replot();
}

void MainWindow::resetGraph2Range(){
    channelGraph[1]->yAxis->setRange(-0.00050, 0.00100, Qt::AlignLeft);
    channelGraph[1]->replot();
}

void MainWindow::resetGraph3Range(){
    channelGraph[2]->yAxis->setRange(0, 2.5, Qt::AlignLeft);
    channelGraph[2]->replot();
}

void MainWindow::resetGraph4Range(){
    channelGraph[3]->yAxis->setRange(0, 250, Qt::AlignLeft);
    channelGraph[3]->replot();
}

void MainWindow::on_voltage50u_triggered(){
    for(int i = 0; i < 2; i++){
        channelGraph[i]->yAxis->setRange(-0.000050, 0.0001, Qt::AlignLeft);
        channelGraph[i]->yAxis->setTickStep(0.00001);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_voltage100u_triggered(){
    for(int i = 0; i < 2; i++){
        channelGraph[i]->yAxis->setRange(-0.0001, 0.0002, Qt::AlignLeft);
        channelGraph[i]->yAxis->setTickStep(0.00002);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_voltage200u_triggered(){
    for(int i = 0; i < 2; i++){
        channelGraph[i]->yAxis->setRange(-0.0002, 0.0004, Qt::AlignLeft);
        channelGraph[i]->yAxis->setTickStep(0.00004);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_voltage500u_triggered(){
    for(int i = 0; i < 2; i++){
        channelGraph[i]->yAxis->setRange(-0.00050, 0.001, Qt::AlignLeft);
        channelGraph[i]->yAxis->setTickStep(0.0001);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_voltage1000u_triggered(){
    for(int i = 0; i < 2; i++){
        channelGraph[i]->yAxis->setRange(-0.001, 0.002, Qt::AlignLeft);
        channelGraph[i]->yAxis->setTickStep(0.0002);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_voltage2000u_triggered(){
    for(int i = 0; i < 2; i++){
        channelGraph[i]->yAxis->setRange(-0.002, 0.004, Qt::AlignLeft);
        channelGraph[i]->yAxis->setTickStep(0.0004);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_voltage5000u_triggered(){
    for(int i = 0; i < 2; i++){
        channelGraph[i]->yAxis->setRange(-0.005, 0.01, Qt::AlignLeft);
        channelGraph[i]->yAxis->setTickStep(0.001);
        channelGraph[i]->replot();
    }
}

// Display "About" message box.
void MainWindow::about()
{
    QMessageBox::about(this, tr("About SINAPSE Recording Software"),
            tr("<h2>Singapore Institute for Neurotechnology</h2>"
               "<p>Version 1.0"
               "<p>Copyright &copy; 2016-2018 SINAPSE"
               "<p>This biopotential recording application controls the RHD2000 "
               "USB Interface Board from Intan Technologies.  The C++/Qt source code "
               "for this application is freely available from Intan Technologies. "
               "For more information visit <i>http://www.intantech.com</i>."
               "<p>This program is free software: you can redistribute it and/or modify "
               "it under the terms of the GNU Lesser General Public License as published "
               "by the Free Software Foundation, either version 3 of the License, or "
               "(at your option) any later version."
               "<p>This program is distributed in the hope that it will be useful, "
               "but WITHOUT ANY WARRANTY; without even the implied warranty of "
               "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
               "GNU Lesser General Public License for more details."
               "<p>You should have received a copy of the GNU Lesser General Public License "
               "along with this program.  If not, see <i>http://www.gnu.org/licenses/</i>."));
}

void MainWindow::on_graph1_clicked(){
    channelGraph[0]->graph()->setPen(QPen(Qt::red));
    channelGraph[1]->graph()->setPen(QPen(Qt::black));
    channelGraph[2]->graph()->setPen(QPen(Qt::black));
    data->setAudioChannel(0);
    audio1->setChecked(true);
    audio2->setChecked(false);
    audio3->setChecked(false);
}

void MainWindow::on_graph2_clicked(){
    channelGraph[1]->graph()->setPen(QPen(Qt::red));
    channelGraph[0]->graph()->setPen(QPen(Qt::black));
    channelGraph[2]->graph()->setPen(QPen(Qt::black));
    data->setAudioChannel(1);
    audio1->setChecked(false);
    audio2->setChecked(true);
    audio3->setChecked(false);
}
void MainWindow::on_graph3_clicked(){
    channelGraph[2]->graph()->setPen(QPen(Qt::red));
    channelGraph[0]->graph()->setPen(QPen(Qt::black));
    channelGraph[1]->graph()->setPen(QPen(Qt::black));
    data->setAudioChannel(2);
    audio1->setChecked(false);
    audio2->setChecked(false);
    audio3->setChecked(true);
}

#endif //SYLPH
