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

    timeFrameMapper = new QSignalMapper(this);
    connect(timeFrameMapper, SIGNAL(mapped(int)), this, SLOT(on_timeFrame_changed(int)));
    for(int i = 0; i < 9; i++){
        timeFrameAction[i] = new QAction(timeFrameActionNames[i]);
        timeFrameMapper->setMapping(timeFrameAction[i], i);
        connect(timeFrameAction[i], SIGNAL(triggered(bool)), timeFrameMapper, SLOT(map()));
    }

    resetDefaultY = new QAction(tr("Default Voltage Scale"), this);
    resetDefaultY->setShortcut(tr("Ctrl+Y"));
    connect(resetDefaultY, SIGNAL(triggered()), this, SLOT(on_resetY_triggered()));

    voltageMapper = new QSignalMapper(this);
    connect(voltageMapper, SIGNAL(mapped(int)), this, SLOT(on_voltage_changed(int)));
    for(int i = 0; i < 7; i++){
        voltageAction[i] = new QAction(voltageActionNames[i]);
        voltageMapper->setMapping(voltageAction[i], i);
        connect(voltageAction[i], SIGNAL(triggered(bool)), voltageMapper, SLOT(map()));
    }
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
        channelGraph[i]->graph()->setPen(colors[i]);
    }

    channelGraph[9]->axisRect()->setMargins(QMargins(75,0,0,15));

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
        channelGraph[i]->axisRect()->setMargins(QMargins(75,0,0,0));
        channelGraph[i]->yAxis->setRange(-0.21, 1.42, Qt::AlignLeft);
    }
    channelGraph[4] = new QCustomPlot;
    leftLayout->addWidget(channelGraph[4]);
    channelGraph[4]->xAxis->setVisible(true);
    channelGraph[4]->axisRect()->setAutoMargins(QCP::msNone);
    channelGraph[4]->axisRect()->setMargins(QMargins(75,0,0,15));
    channelGraph[4]->yAxis->setRange(-0.21, 1.42, Qt::AlignLeft);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    for(int i=5;i<9;i++){
        channelGraph[i] = new QCustomPlot;
        rightLayout->addWidget(channelGraph[i]);
        channelGraph[i]->xAxis->setVisible(true);
        channelGraph[i]->axisRect()->setAutoMargins(QCP::msNone);
        channelGraph[i]->axisRect()->setMargins(QMargins(75,0,0,0));
        channelGraph[i]->yAxis->setRange(-0.21, 1.42, Qt::AlignLeft);
    }
    channelGraph[9] = new QCustomPlot;
    rightLayout->addWidget(channelGraph[9]);
    channelGraph[9]->xAxis->setVisible(true);
    channelGraph[9]->axisRect()->setAutoMargins(QCP::msNone);
    channelGraph[9]->axisRect()->setMargins(QMargins(75,0,0,15));
    channelGraph[9]->yAxis->setRange(-0.21, 1.42, Qt::AlignLeft);

    for(int i=0;i<10;i++){
        channelGraph[i]->addGraph();
        channelGraph[i]->xAxis->setTickStep((double) (3000000.0/((14.0)*(NeutrinoChannel->getNumChannels()+2.0))));
        channelGraph[i]->yAxis->setLabel("Channel "+QString::number(i+1,10)+" (V)");
        channelGraph[i]->yAxis->setLabelFont(QFont(font().family(), 10));
        channelGraph[i]->yAxis->setAutoTickStep(false);
        channelGraph[i]->yAxis->setTickStep(0.25);
        channelGraph[i]->setInteractions(QCP::iRangeDrag);
        channelGraph[i]->axisRect()->setRangeDrag(Qt::Vertical);
        channelGraph[i]->graph()->setPen(colors[i]);
    }

    QHBoxLayout *mainLayout = new QHBoxLayout;
    QWidget *mainWidget = new QWidget;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}

void MainWindow::createMenus(){
//--------------------------- FILE MENU -----------------------------//
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(filterAction);
    fileMenu->addSeparator();
    fileMenu->addAction(dataAnalyzerAction);
    fileMenu->addSeparator();
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
    for(int i = 0; i < 9; i++){
        timeFrameMenu->addAction(timeFrameAction[i]);
        timeFrameAction[i]->setCheckable(true);
        timeFrameGroup->addAction(timeFrameAction[i]);
    }
    timeFrameAction[4]->setChecked(true);

    timeFrameMenu->addSeparator();
    timeFrameMenu->addAction(resetDefaultX);

//------------------------- TIMEFRAME MENU --------------------------//
    voltageMenu = menuBar()->addMenu(tr("&Voltage Scales"));
    voltageGroup = new QActionGroup(this);
    for(int i = 0; i < 7; i++){
        voltageMenu->addAction(voltageAction[i]);
        voltageAction[i]->setCheckable(true);
        voltageGroup->addAction(voltageAction[i]);
    }
    voltageAction[6]->setChecked(true);

    voltageMenu->addSeparator();
    voltageMenu->addAction(resetDefaultY);
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
    if(portInfo.size() > 0){
        serialNeutrino->doConnect()? connectionStatus.append("Connected to Neutrino!!") : connectionStatus.append("Connection to Neutrino failed");
    }
    if(!serialNeutrino->isConnected()){
        connectionStatus.clear();
        if(socketNeutrino->doConnect("192.168.42.1", 8888)){
            connectionStatus.append("Connected to Neutrino WiFi Module at 192.168.42.1/8888");
        }
        else{
            connectionStatus.append("Connection to Neutrino failed! Restart this program after connecting Neutrino.");
            QMessageBox::information(this, "Failed to connect!", "No Neutrino device detected.. \n"
                                                                 "Check your connections and run the program again..");
        }
    }
    statusBarLabel->setText(connectionStatus);
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

void MainWindow::on_timeFrame_changed(int timeFrameIndex){
    switch(timeFrameIndex){
    case 0:
        data->setNumDataPoints(TimeFrames10ms, (double) (3000000.0/((14.0)*(NeutrinoChannel->getNumChannels()+2.0))));
        break;
    case 1:
        data->setNumDataPoints(TimeFrames20ms, (double) (3000000.0/((14.0)*(NeutrinoChannel->getNumChannels()+2.0))));
        break;
    case 2:
        data->setNumDataPoints(TimeFrames50ms, (double) (3000000.0/((14.0)*(NeutrinoChannel->getNumChannels()+2.0))));
        break;
    case 3:
        data->setNumDataPoints(TimeFrames100ms, (double) (3000000.0/((14.0)*(NeutrinoChannel->getNumChannels()+2.0))));
        break;
    case 4:
        data->setNumDataPoints(TimeFrames200ms, (double) (3000000.0/((14.0)*(NeutrinoChannel->getNumChannels()+2.0))));
        break;
    case 5:
        data->setNumDataPoints(TimeFrames500ms, (double) (3000000.0/((14.0)*(NeutrinoChannel->getNumChannels()+2.0))));
        break;
    case 6:
        data->setNumDataPoints(TimeFrames1000ms, (double) (3000000.0/((14.0)*(NeutrinoChannel->getNumChannels()+2.0))));
        break;
    case 7:
        data->setNumDataPoints(TimeFrames2000ms, (double) (3000000.0/((14.0)*(NeutrinoChannel->getNumChannels()+2.0))));
        break;
    case 8:
        data->setNumDataPoints(TimeFrames5000ms, (double) (3000000.0/((14.0)*(NeutrinoChannel->getNumChannels()+2.0))));
        break;
    }
    data->clearallChannelData();
}

void MainWindow::on_voltage_changed(int voltageIndex){
    for(int i = 0; i < 10; i++){
        switch(voltageIndex){
        case 0:
            data->isFilterEnabled()? channelGraph[i]->yAxis->setRange(-0.0105, 0.021, Qt::AlignLeft) : channelGraph[i]->yAxis->setRange(0.4895, 0.021, Qt::AlignLeft);
            channelGraph[i]->yAxis->setTickStep(0.0025);
            break;
        case 1:
            data->isFilterEnabled()? channelGraph[i]->yAxis->setRange(-0.021, 0.042, Qt::AlignLeft) : channelGraph[i]->yAxis->setRange(0.479, 0.042, Qt::AlignLeft);
            channelGraph[i]->yAxis->setTickStep(0.005);
            break;
        case 2:
            data->isFilterEnabled()? channelGraph[i]->yAxis->setRange(-0.051, 0.102, Qt::AlignLeft) : channelGraph[i]->yAxis->setRange(0.449, 0.102, Qt::AlignLeft);
            channelGraph[i]->yAxis->setTickStep(0.01);
            break;
        case 3:
            data->isFilterEnabled()? channelGraph[i]->yAxis->setRange(-0.101, 0.202, Qt::AlignLeft) : channelGraph[i]->yAxis->setRange(0.399, 0.202, Qt::AlignLeft);
            channelGraph[i]->yAxis->setTickStep(0.02);
            break;
        case 4:
            data->isFilterEnabled()? channelGraph[i]->yAxis->setRange(-0.21, 0.42, Qt::AlignLeft) : channelGraph[i]->yAxis->setRange(0.29, 0.42, Qt::AlignLeft);
            channelGraph[i]->yAxis->setTickStep(0.05);
            break;
        case 5:
            data->isFilterEnabled()? channelGraph[i]->yAxis->setRange(-0.51, 1.02, Qt::AlignLeft) : channelGraph[i]->yAxis->setRange(-0.01, 1.02, Qt::AlignLeft);
            channelGraph[i]->yAxis->setTickStep(0.1);
            break;
        case 6:
            data->isFilterEnabled()? channelGraph[i]->yAxis->setRange(-0.71, 1.42, Qt::AlignLeft) : channelGraph[i]->yAxis->setRange(-0.21, 1.42, Qt::AlignLeft);
            channelGraph[i]->yAxis->setTickStep(0.25);
            break;
        }
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
    pause? pauseAction->setText("Pause graph") : pauseAction->setText("Resume graph");
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
    on_timeFrame_changed(4);
    timeFrameAction[4]->setChecked(true);
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
    on_voltage_changed(6);
    voltageAction[6]->setChecked(true);
}

void MainWindow::on_dataAnalyzer_triggered(){
    QProcess *process = new QProcess(this);
    QString file = QDir::currentPath() + "/DataAnalyzer.exe";
    statusBarLabel->setText("Opening: " + file);
    process->start(file);
}

void MainWindow::on_CommandMenu_triggered(){
    statusBarLabel->setText("Command Dialog Opened");
    CommandDialog commandDialog(socketNeutrino, NeutrinoCommand, NeutrinoChannel, serialNeutrino, data);
    commandDialog.exec();

}

void MainWindow::on_tenby1_triggered(){
    create10x1Layout();
}

void MainWindow::on_fiveby2_triggered(){
    create5x2Layout();
}
