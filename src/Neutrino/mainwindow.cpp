#include "mainwindow.h"

MainWindow::MainWindow(){
    x = new Odin::OdinWindow();
    x->setFixedSize(x->sizeHint());
//    x->show();
    QString version(APP_VERSION);
    timer.start();
    NeutrinoChannel = new Channel;
    NeutrinoCommand = new Command(NeutrinoChannel);
    dataStream = new DataStream(this);
    data = new DataProcessor(NeutrinoChannel, dataStream);
    serialNeutrino = new SerialChannel(this, NeutrinoCommand, data, NeutrinoChannel);
    socketOdin = new Odin::SocketOdin;
    socketNeutrino = new SocketNeutrino(NeutrinoCommand, data, NeutrinoChannel);
    setWindowTitle(tr("SINAPSE Neutrino II Recording Software V") + version);
    createStatusBar();
    create5x2Layout();
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(updateData()));
    dataTimer.start(1);
    createActions();
    createMenus();
    on_voltage_changed(DEFAULT_YAXIS);
    connectNeutrino();
//    on_CommandMenu_triggered();
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

    inputReferred = new QAction(tr("Input referred"));
    inputReferred->setCheckable(true);
    connect(inputReferred, SIGNAL(toggled(bool)), this, SLOT(on_inputRefer_triggered()));
}

void MainWindow::create10x1Layout(){
    QVBoxLayout *mainLayout = new QVBoxLayout;
    for(int i=0;i<10;i++){
        channelGraph[i] = new QCustomPlot;
        mainLayout->addWidget(channelGraph[i]);
        channelGraph[i]->xAxis->setVisible(true);
        channelGraph[i]->axisRect()->setAutoMargins(QCP::msNone);
        channelGraph[i]->axisRect()->setMargins(QMargins(75,0,0,0));
        channelGraph[i]->addGraph();
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
//    QVBoxLayout *leftLayout = new QVBoxLayout;
//    for(int i=0;i<4;i++){
//        channelGraph[i] = new QCustomPlot;
//        leftLayout->addWidget(channelGraph[i]);
//        channelGraph[i]->axisRect()->setMargins(QMargins(75,0,0,0));
//    }
//    channelGraph[4] = new QCustomPlot;
//    leftLayout->addWidget(channelGraph[4]);
//    channelGraph[4]->axisRect()->setMargins(QMargins(75,0,0,15));

//    QVBoxLayout *rightLayout = new QVBoxLayout;
//    for(int i=5;i<9;i++){
//        channelGraph[i] = new QCustomPlot;
//        rightLayout->addWidget(channelGraph[i]);
//        channelGraph[i]->axisRect()->setMargins(QMargins(75,0,0,0));
//    }
//    channelGraph[9] = new QCustomPlot;
//    rightLayout->addWidget(channelGraph[9]);
//    channelGraph[9]->axisRect()->setMargins(QMargins(75,0,0,15));

//    for(int i=0;i<12;i++){
//        channelGraph[i]->addGraph();
//        channelGraph[i]->yAxis->setLabel("Channel "+QString::number(i+1,10)+" (V)");
//        channelGraph[i]->yAxis->setLabelFont(QFont(font().family(), 10));
//        channelGraph[i]->axisRect()->setAutoMargins(QCP::msNone);
//        channelGraph[i]->xAxis->setVisible(true);
//        channelGraph[i]->yAxis->setAutoTickStep(false);
//        channelGraph[i]->setInteractions(QCP::iRangeDrag);
//        channelGraph[i]->axisRect()->setRangeDrag(Qt::Vertical);
//        channelGraph[i]->graph()->setPen(colors[i]);
//    }

//    QHBoxLayout *mainLayout = new QHBoxLayout;
//    QWidget *mainWidget = new QWidget;
//    mainLayout->addLayout(leftLayout);
//    mainLayout->addLayout(rightLayout);
//    mainWidget->setLayout(mainLayout);
//    setCentralWidget(mainWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout;
        for(int i=0;i<12;i++){
            channelGraph[i] = new QCustomPlot;
            channelGraph[i]->xAxis->setVisible(false);
            channelGraph[i]->axisRect()->setAutoMargins(QCP::msNone);
            channelGraph[i]->axisRect()->setMargins(QMargins(65,10,0,15));
            channelGraph[i]->yAxis->setRange(-500, 500);
            channelGraph[i]->addGraph();
            channelGraph[i]->yAxis->setAutoTickStep(false);
            channelGraph[i]->xAxis->setAutoTickStep(false);
            channelGraph[i]->xAxis->setTickStep(0.01);
            channelGraph[i]->yAxis->setTickStep(100);
        }


        for(int i = 0; i < 10; i ++){
//            audioSelectMapper->setMapping(channelGraph[i], i);
//            connect(channelGraph[i], SIGNAL(mousePress(QMouseEvent*)), audioSelectMapper, SLOT(map()));
            channelGraph[i]->yAxis->setLabel("Channel "+ QString::number(i+1, 10) + " (uV)");
            channelGraph[i]->yAxis->setLabelFont(QFont(font().family(), 8));
            channelGraph[i]->graph()->setPen(QPen(Qt::black));
            channelGraph[i]->setInteractions(QCP::iRangeDrag);
            channelGraph[i]->axisRect()->setRangeDragAxes(0, channelGraph[i]->yAxis);
        }

        channelGraph[10]->yAxis->setLabel("Sync Pulse (V)");
        channelGraph[10]->yAxis->setLabelPadding(35);
        channelGraph[10]->yAxis->setLabelFont(QFont(font().family(), 8));
        channelGraph[10]->setFixedHeight(100);
        channelGraph[11]->yAxis->setLabel("Frame Marker ('000)");
        channelGraph[11]->yAxis->setLabelPadding(35);
        channelGraph[11]->yAxis->setLabelFont(QFont(font().family(), 8));
        channelGraph[11]->setFixedHeight(100);

        channelGraph[0]->graph()->setPen(QPen(Qt::red));
        channelGraph[10]->graph()->setPen(QPen(Qt::darkGreen));

        channelGraph[10]->yAxis->setRange(-5, 260, Qt::AlignLeft);
        channelGraph[10]->yAxis->setTickStep(50);
        channelGraph[11]->yAxis->setRange(0, 2000, Qt::AlignLeft);
        channelGraph[11]->yAxis->setTickStep(400);
        channelGraph[11]->axisRect()->setMargins(QMargins(85,10,0,15));

        QVBoxLayout *leftLayout = new QVBoxLayout;
        for(int i = 0; i < 5; i++){
            leftLayout->addWidget(channelGraph[i]);
        }

        QVBoxLayout *rightLayout = new QVBoxLayout;
        for(int i = 5; i < 10; i++){
            rightLayout->addWidget(channelGraph[i]);
        }

        QHBoxLayout *topLayout = new QHBoxLayout;
        topLayout->addLayout(leftLayout);
        topLayout->addLayout(rightLayout);

        QVBoxLayout *bottomLayout = new QVBoxLayout;
        for(int i = 10; i < 12; i++){
            bottomLayout->addWidget(channelGraph[i]);
        }

        mainLayout->addLayout(topLayout);
        mainLayout->addLayout(bottomLayout);

        QWidget *mainWidget = new QWidget;
        mainWidget->setLayout(mainLayout);
        setCentralWidget(mainWidget);
}

void MainWindow::createMenus(){
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

    layoutMenu = menuBar()-> addMenu(tr("&Layout"));
    layoutMenu->addAction(tenby1Action);
    layoutMenu->addAction(fiveby2Action);

    timeFrameMenu = menuBar()->addMenu(tr("&Time Scales"));
    timeFrameGroup = new QActionGroup(this);
    for(int i = 0; i < 9; i++){
        timeFrameMenu->addAction(timeFrameAction[i]);
        timeFrameAction[i]->setCheckable(true);
        timeFrameGroup->addAction(timeFrameAction[i]);
    }
    timeFrameAction[DEFAULT_XAXIS]->setChecked(true);

    timeFrameMenu->addSeparator();
    timeFrameMenu->addAction(resetDefaultX);

    voltageMenu = menuBar()->addMenu(tr("&Voltage Scales"));
    voltageGroup = new QActionGroup(this);
    for(int i = 0; i < 7; i++){
        voltageMenu->addAction(voltageAction[i]);
        voltageAction[i]->setCheckable(true);
        voltageGroup->addAction(voltageAction[i]);
    }
    voltageAction[DEFAULT_YAXIS]->setChecked(true);

    voltageMenu->addSeparator();
    voltageMenu->addAction(resetDefaultY);
    voltageMenu->addSeparator();
    voltageMenu->addAction(inputReferred);
}

void MainWindow::createStatusBar(){
    statusBarLabel = new QLabel;
    statusBarMainWindow = statusBar();
    statusBarMainWindow->addPermanentWidget(statusBarLabel, 1);
    statusBarMainWindow->setSizeGripEnabled(false);
}

void MainWindow::connectNeutrino(){
    portInfo = QSerialPortInfo::availablePorts();
    connectionStatus.clear();
    if(portInfo.size() > 0){
        serialNeutrino->doConnect()? connectionStatus.append("Connected to Neutrino!!") : connectionStatus.append("Connection to Neutrino failed");
    }
    if(!serialNeutrino->isConnected()){
        connectionStatus.clear();
//        if(socketNeutrino->doConnect(ipAddress, portNumber)){
//            connectionStatus.append("Connected to Neutrino WiFi Module at " + ipAddress + "/" + QString::number(portNumber));
//        }
//        else{
//            connectionStatus.append("Connection to Neutrino failed! Restart this program after connecting Neutrino.");
//            QMessageBox::information(this, "Failed to connect!", "No Neutrino device detected.. \n"
//                                                                 "Check your connections and run the program again..");
//        }
        int i = 1;
        do{
            i++;
            socketNeutrino->doConnect("192.168.4."+QString::number(i), 8888);
        } while(!socketNeutrino->isConnected() && i < 4);
        if(socketNeutrino->isConnected()){
            connectionStatus.append("Connected to Neutrino WiFi Module at " + ipAddress + "/" + QString::number(portNumber));
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
//        socketOdin->sendDisconnectSignal();
    }
    delete x;
    serialNeutrino->closePort();
}

void MainWindow::updateData(){
    QVector<double> X_axis = data->retrieveXAxis();
    if(data->isPlotEnabled() && X_axis.size() >= (data->getNumDataPoints())){
        for(int i=0; i<12; i++){
            if(!data->isEmpty(i)){
                channelGraph[i]->graph()->setData(X_axis, (i < 10 && data->isFilterEnabled())? data->filterData(data->retrieveData(i), i): data->retrieveData(i));
                channelGraph[i]->xAxis->setRange(X_axis.at(0),
                                                 (data->getNumDataPoints()*data->getPeriod()),
                                                 Qt::AlignLeft);
                if(i < 10 && dataStream->getStreamConnected(i)){
                    dataStream->streamData(i);
                }
                else if(i < 10 && dataStream->getChannelSize(i) > 40960){
                    dataStream->clearChannelData(i);
                }
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
    data->setNumDataPoints((TimeFrames) timeFrameIndex, data->getSamplingFreq());
    data->clearallChannelData();
}

void MainWindow::on_voltage_changed(int voltageIndex){
    double yMin = (data->isFilterEnabled() || data->getInputReferred())? voltageMin[voltageIndex][0] : voltageMin[voltageIndex][1];
    double yRange = voltageRange[voltageIndex];
    double step = voltageStep[voltageIndex];
    for(int i = 0; i < 10; i++){
        channelGraph[i]->yAxis->setRange(yMin, yRange, Qt::AlignLeft);
        channelGraph[i]->yAxis->setTickStep(step);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_inputRefer_triggered(){
    data->setInputReferred(inputReferred->isChecked());
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
    on_timeFrame_changed(DEFAULT_XAXIS);
    timeFrameAction[DEFAULT_XAXIS]->setChecked(true);
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
    on_voltage_changed(DEFAULT_YAXIS);
    voltageAction[DEFAULT_YAXIS]->setChecked(true);
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
    on_resetX_triggered();
    on_resetY_triggered();
}

void MainWindow::on_fiveby2_triggered(){
    create5x2Layout();
    on_resetX_triggered();
    on_resetY_triggered();
}
