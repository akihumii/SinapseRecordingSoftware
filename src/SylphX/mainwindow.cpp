#include "mainwindow.h"

MainWindow::MainWindow(){
    QString version(APP_VERSION);
    timer.start();
    defaultRange = new QCPRange(-0.00050, 0.00100);
    setWindowTitle(tr("SINAPSE Sylph X Recording Software V") + version);
    data = new DataProcessor;
    serialChannel = new SerialChannel(this, data);
    socketSylph = new SocketSylph(this, data);
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(updateData()));
    dataTimer.start(1);     //tick timer every XXX msec
    createStatusBar();
    createLayout();
    createActions();
    createMenus();
    connectSylph();
    qDebug() << "Starting SYLPH..";
}

void MainWindow::createLayout(){
    QVBoxLayout *mainLayout = new QVBoxLayout;
    for(int i=0;i<12;i++){
        channelGraph[i] = new QCustomPlot;
        channelGraph[i]->xAxis->setVisible(true);
        channelGraph[i]->axisRect()->setAutoMargins(QCP::msNone);
        channelGraph[i]->axisRect()->setMargins(QMargins(85,10,0,15));
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
    connect(channelGraph[3], SIGNAL(mousePress(QMouseEvent*)), this, SLOT(on_graph4_clicked()));
    connect(channelGraph[4], SIGNAL(mousePress(QMouseEvent*)), this, SLOT(on_graph5_clicked()));
    connect(channelGraph[5], SIGNAL(mousePress(QMouseEvent*)), this, SLOT(on_graph6_clicked()));
    connect(channelGraph[6], SIGNAL(mousePress(QMouseEvent*)), this, SLOT(on_graph7_clicked()));
    connect(channelGraph[7], SIGNAL(mousePress(QMouseEvent*)), this, SLOT(on_graph8_clicked()));
    connect(channelGraph[8], SIGNAL(mousePress(QMouseEvent*)), this, SLOT(on_graph9_clicked()));
    connect(channelGraph[9], SIGNAL(mousePress(QMouseEvent*)), this, SLOT(on_graph10_clicked()));
    connect(channelGraph[10], SIGNAL(mousePress(QMouseEvent*)), this, SLOT(on_graph11_clicked()));


    for(int i = 0; i < 10; i ++){
        channelGraph[i]->yAxis->setLabel("Channel "+ QString::number(i+1, 10) + " (V)");
        channelGraph[i]->yAxis->setLabelFont(QFont(font().family(), 10));
        channelGraph[i]->graph()->setPen(QPen(Qt::black));
    }
    channelGraph[10]->yAxis->setLabel("Sync Pulse (V)");
    channelGraph[10]->yAxis->setLabelPadding(35);
    channelGraph[10]->yAxis->setLabelFont(QFont(font().family(), 10));
    channelGraph[10]->setFixedHeight(100);
    channelGraph[11]->yAxis->setLabel("Frame Marker");
    channelGraph[11]->yAxis->setLabelPadding(35);
    channelGraph[11]->yAxis->setLabelFont(QFont(font().family(), 10));
    channelGraph[11]->setFixedHeight(100);

    channelGraph[0]->graph()->setPen(QPen(Qt::red));
    channelGraph[10]->graph()->setPen(QPen(Qt::darkGreen));

    channelGraph[10]->yAxis->setRange(0, 2.5, Qt::AlignLeft);
    channelGraph[10]->yAxis->setTickStep(0.5);
    channelGraph[11]->yAxis->setRange(0, 250, Qt::AlignLeft);
    channelGraph[11]->yAxis->setTickStep(50);
    channelGraph[11]->axisRect()->setMargins(QMargins(75,10,0,15));

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
    on_timeFrame100_triggered();
}

void MainWindow::createActions(){
    //    serialPortAction = new QAction(tr("S&erial Port Configuration"), this);
    //    serialPortAction->setShortcut(tr("Ctrl+E"));
    //    connect(serialPortAction, SIGNAL(triggered()), this, SLOT(on_serialConfig_triggered()));

    for(int i = 0; i < 10; i++){
        audio[i] = new QAction("Channel " + QString::number(i+1, 10)+ " Audio Output");
    }
    audio[10] = new QAction(tr("Sync Pulse Audio Output"));

    connect(audio[0], SIGNAL(triggered(bool)), this, SLOT(on_graph1_clicked()));
    connect(audio[1], SIGNAL(triggered(bool)), this, SLOT(on_graph2_clicked()));
    connect(audio[2], SIGNAL(triggered(bool)), this, SLOT(on_graph3_clicked()));
    connect(audio[3], SIGNAL(triggered(bool)), this, SLOT(on_graph4_clicked()));
    connect(audio[4], SIGNAL(triggered(bool)), this, SLOT(on_graph5_clicked()));
    connect(audio[5], SIGNAL(triggered(bool)), this, SLOT(on_graph6_clicked()));
    connect(audio[6], SIGNAL(triggered(bool)), this, SLOT(on_graph7_clicked()));
    connect(audio[7], SIGNAL(triggered(bool)), this, SLOT(on_graph8_clicked()));
    connect(audio[8], SIGNAL(triggered(bool)), this, SLOT(on_graph9_clicked()));
    connect(audio[9], SIGNAL(triggered(bool)), this, SLOT(on_graph10_clicked()));
    connect(audio[10], SIGNAL(triggered(bool)), this, SLOT(on_graph11_clicked()));


    aboutAction = new QAction(tr("About SINAPSE Recording Software"));
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(about()));

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

    restartAction = new QAction(tr("Restart serial port"), this);
    restartAction->setShortcut(tr("Ctrl+A"));
    connect(restartAction, SIGNAL(triggered(bool)), this, SLOT(on_restart_triggered()));

    dataAnalyzerAction = new QAction(tr("Data Analy&zer"), this);
    dataAnalyzerAction->setShortcut(tr("Ctrl+Z"));
    connect(dataAnalyzerAction, SIGNAL(triggered()), this, SLOT(on_dataAnalyzer_triggered()));

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
}

void MainWindow::createMenus(){
    fileMenu = menuBar()->addMenu(tr("&File"));
//    fileMenu->addAction(swapAction);
    fileMenu->addAction(filterAction);
    fileMenu->addSeparator();

//    fileMenu->addAction(serialPortAction);
//    fileMenu->addAction(resetDefaultRange);

    fileMenu->addAction(dataAnalyzerAction);
    fileMenu->addSeparator();

    fileMenu->addAction(pauseAction);
    fileMenu->addSeparator();

    fileMenu->addAction(recordAction);
    fileMenu->addAction(chooseDirectoryAction);
    fileMenu->addSeparator();
    fileMenu->addAction(restartAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

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
    audioGroup = new QActionGroup(this);

    for(int i = 0; i < 11; i++){
        audioOutputMenu->addAction(audio[i]);
        audio[i]->setCheckable(true);
        audioGroup->addAction(audio[i]);
    }
    audio[0]->setChecked(true);

    helpMenu = menuBar()->addMenu(tr("Help"));
    helpMenu->addAction(aboutAction);
}

void MainWindow::createStatusBar(){
    statusBarLabel = new QLabel;
    statusBarMainWindow = statusBar();
    statusBarMainWindow->addPermanentWidget(statusBarLabel, 1);
    statusBarMainWindow->setSizeGripEnabled(false);  // fixed window size
}

void MainWindow::connectSylph(){
    portInfo = QSerialPortInfo::availablePorts();
    connectionStatus.clear();
    if(portInfo.size()>1){
        serialChannel->connectSylph();
        if(serialChannel->isImplantConnected()){
            connectionStatus.append("Connected to Implant Port |");
        }
        else{
            connectionStatus.append("Connection to Implant Port failed |");
        }
        if(serialChannel->isADCConnected()){
            connectionStatus.append(" Connected to ADC Port");
        }
        else{
            connectionStatus.append(" Connection to ADC Port failed");
        }
        statusBarLabel->setText(connectionStatus);
    }
    if(!serialChannel->isADCConnected() && !serialChannel->isImplantConnected()){
        socketSylph->doConnect("10.10.10.1", 30000);
        if(socketSylph->isConnected()){
            connectionStatus.append("Connected to Sylph WiFi Module at 10.10.10.1/30000");
        }
        else{
            connectionStatus.append("Failed to connect...");
            QMessageBox::information(this, "Failed to connect!", "No Sylph device detected.. \n"
                                                                 "Check your connections and run the program again..");
        }
        statusBarLabel->setText(connectionStatus);
    }
}

MainWindow::~MainWindow(){
    socketSylph->doDisconnect();
}

void MainWindow::updateData(){
    if(restartCount < 2 && serialChannel->isConnected()){
        on_restart_triggered();
        restartCount++;
//        qDebug() << restartCount;
    }
    QVector<double> X_axis = data->retrieveXAxis();
    if(X_axis.size() >= data->getNumDataPoints()){
        for(int i=0; i<12; i++){
            if(!data->isEmpty(i)){
                channelGraph[i]->graph()->setData(X_axis, data->retrieveData(i));
                channelGraph[i]->xAxis->setRange(X_axis.at(0), data->getNumDataPoints()*0.000202, Qt::AlignLeft);
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
    data->setNumDataPoints(TimeFrames10ms, 4960.0);
    data->clearallChannelData();
    for(int i=0;i<12;i++){
        channelGraph[i]->xAxis->setTickStep(0.001);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_timeFrame20_triggered(){
    data->setNumDataPoints(TimeFrames20ms, 4960.0);
    data->clearallChannelData();
    for(int i=0;i<12;i++){
        channelGraph[i]->xAxis->setTickStep(0.002);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_timeFrame50_triggered(){
    data->setNumDataPoints(TimeFrames50ms, 4960.0);
    data->clearallChannelData();
    for(int i=0;i<12;i++){
        channelGraph[i]->xAxis->setTickStep(0.005);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_timeFrame100_triggered(){
    data->setNumDataPoints(TimeFrames100ms, 4960.0);
    data->clearallChannelData();
    for(int i=0;i<12;i++){
        channelGraph[i]->xAxis->setTickStep(0.01);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_timeFrame200_triggered(){
    data->setNumDataPoints(TimeFrames200ms, 4960.0);
    data->clearallChannelData();
    for(int i=0;i<12;i++){
        channelGraph[i]->xAxis->setTickStep(0.02);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_timeFrame500_triggered(){
    data->setNumDataPoints(TimeFrames500ms, 4960.0);
    data->clearallChannelData();
    for(int i=0;i<12;i++){
        channelGraph[i]->xAxis->setTickStep(0.05);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_timeFrame1000_triggered(){
    data->setNumDataPoints(TimeFrames1000ms, 4960.0);
    data->clearallChannelData();
    for(int i=0;i<12;i++){
        channelGraph[i]->xAxis->setTickStep(0.1);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_timeFrame2000_triggered(){
    data->setNumDataPoints(TimeFrames2000ms, 4960.0);
    data->clearallChannelData();
    for(int i=0;i<12;i++){
        channelGraph[i]->xAxis->setTickStep(0.2);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_timeFrame5000_triggered(){
    data->setNumDataPoints(TimeFrames5000ms, 4960.0);
    data->clearallChannelData();
    for(int i=0;i<12;i++){
        channelGraph[i]->xAxis->setTickStep(0.5);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_voltage50u_triggered(){
    for(int i=0;i<10;i++){
        channelGraph[i]->yAxis->setRange(-0.000050, 0.0001, Qt::AlignLeft);
        channelGraph[i]->yAxis->setTickStep(0.00001);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_voltage100u_triggered(){
    for(int i=0;i<10;i++){
        channelGraph[i]->yAxis->setRange(-0.0001, 0.0002, Qt::AlignLeft);
        channelGraph[i]->yAxis->setTickStep(0.00002);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_voltage200u_triggered(){
    for(int i=0;i<10;i++){
        channelGraph[i]->yAxis->setRange(-0.0002, 0.0004, Qt::AlignLeft);
        channelGraph[i]->yAxis->setTickStep(0.00004);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_voltage500u_triggered(){
    for(int i=0;i<10;i++){
        channelGraph[i]->yAxis->setRange(-0.00050, 0.001, Qt::AlignLeft);
        channelGraph[i]->yAxis->setTickStep(0.0001);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_voltage1000u_triggered(){
    for(int i=0;i<10;i++){
        channelGraph[i]->yAxis->setRange(-0.001, 0.002, Qt::AlignLeft);
        channelGraph[i]->yAxis->setTickStep(0.0002);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_voltage2000u_triggered(){
    for(int i=0;i<10;i++){
        channelGraph[i]->yAxis->setRange(-0.002, 0.004, Qt::AlignLeft);
        channelGraph[i]->yAxis->setTickStep(0.0004);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_voltage5000u_triggered(){
    for(int i=0;i<10;i++){
        channelGraph[i]->yAxis->setRange(-0.005, 0.01, Qt::AlignLeft);
        channelGraph[i]->yAxis->setTickStep(0.001);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_record_triggered(){
    if(!data->isRecordEnabled()){
        data->setRecordEnabled(true);
        data->setADCRecordEnabled(true);
        statusBarLabel->setText("<b><FONT COLOR='#ff0000' FONT SIZE = 4> Recording...</b>");
        recordAction->setText("Stop &Recording");
    }
    else if(data->isRecordEnabled()){
        data->setRecordEnabled(false);
        data->setADCRecordEnabled(false);
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
    data->setNumDataPoints(TimeFrames100ms, 4960.0);
    data->clearallChannelData();
    timeFrame100ms->setChecked(true);
}

void MainWindow::on_swap_triggered(){
//    serialChannel->swapPort();
//    statusBarLabel->setText("Port swapped");
}

void MainWindow::on_filterConfig_trigger(){
    FilterDialog filterDialog(data);
    filterDialog.exec();
}

void MainWindow::on_resetY_triggered(){
    on_voltage500u_triggered();
    channelGraph[10]->yAxis->setRange(0, 2.5, Qt::AlignLeft);
    channelGraph[11]->yAxis->setRange(0, 250, Qt::AlignLeft);
    channelGraph[11]->replot();
    voltage500u->setChecked(true);
}

void MainWindow::on_dataAnalyzer_triggered(){
    QProcess *process = new QProcess(this);
    QString file = QDir::currentPath() + QDir::separator() + "SylphAnalyzer.exe";
    process->start(file);
}

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

void MainWindow::on_restart_triggered(){
    if(data->isADCRecordEnabled()){
        data->setADCRecordEnabled(false);
        data->setADCRecordEnabled(true);
    }
    if(data->isRecordEnabled()){
        data->setRecordEnabled(false);
        data->setRecordEnabled(true);
    }
    if(serialChannel->isADCConnected()){
        serialChannel->closeADCPort();
    }
    if(serialChannel->isImplantConnected()){
        serialChannel->closeImplantPort();
    }
    data->clearallChannelData();
    connectSylph();
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
    for(int i = 0; i < 11; i++){
        channelGraph[i]->graph()->setPen(QPen(Qt::black));
        audio[i]->setChecked(false);
    }
    channelGraph[0]->graph()->setPen(QPen(Qt::red));
    data->setAudioChannel(0);
    audio[0]->setChecked(true);
}

void MainWindow::on_graph2_clicked(){
    for(int i = 0; i < 11; i++){
        channelGraph[i]->graph()->setPen(QPen(Qt::black));
        audio[i]->setChecked(false);
    }
    channelGraph[1]->graph()->setPen(QPen(Qt::red));
    data->setAudioChannel(1);
    audio[1]->setChecked(true);
}
void MainWindow::on_graph3_clicked(){
    for(int i = 0; i < 11; i++){
        channelGraph[i]->graph()->setPen(QPen(Qt::black));
        audio[i]->setChecked(false);
    }
    channelGraph[2]->graph()->setPen(QPen(Qt::red));
    data->setAudioChannel(2);
    audio[2]->setChecked(true);
}

void MainWindow::on_graph4_clicked(){
    for(int i = 0; i < 11; i++){
        channelGraph[i]->graph()->setPen(QPen(Qt::black));
        audio[i]->setChecked(false);
    }
    channelGraph[3]->graph()->setPen(QPen(Qt::red));
    data->setAudioChannel(3);
    audio[3]->setChecked(true);
}

void MainWindow::on_graph5_clicked(){
    for(int i = 0; i < 11; i++){
        channelGraph[i]->graph()->setPen(QPen(Qt::black));
        audio[i]->setChecked(false);
    }
    channelGraph[4]->graph()->setPen(QPen(Qt::red));
    data->setAudioChannel(4);
    audio[4]->setChecked(true);
}

void MainWindow::on_graph6_clicked(){
    for(int i = 0; i < 11; i++){
        channelGraph[i]->graph()->setPen(QPen(Qt::black));
        audio[i]->setChecked(false);
    }
    channelGraph[5]->graph()->setPen(QPen(Qt::red));
    data->setAudioChannel(5);
    audio[5]->setChecked(true);
}

void MainWindow::on_graph7_clicked(){
    for(int i = 0; i < 11; i++){
        channelGraph[i]->graph()->setPen(QPen(Qt::black));
        audio[i]->setChecked(false);
    }
    channelGraph[6]->graph()->setPen(QPen(Qt::red));
    data->setAudioChannel(6);
    audio[6]->setChecked(true);
}

void MainWindow::on_graph8_clicked(){
    for(int i = 0; i < 11; i++){
        channelGraph[i]->graph()->setPen(QPen(Qt::black));
        audio[i]->setChecked(false);
    }
    channelGraph[7]->graph()->setPen(QPen(Qt::red));
    data->setAudioChannel(7);
    audio[7]->setChecked(true);
}

void MainWindow::on_graph9_clicked(){
    for(int i = 0; i < 11; i++){
        channelGraph[i]->graph()->setPen(QPen(Qt::black));
        audio[i]->setChecked(false);
    }
    channelGraph[8]->graph()->setPen(QPen(Qt::red));
    data->setAudioChannel(8);
    audio[8]->setChecked(true);
}

void MainWindow::on_graph10_clicked(){
    for(int i = 0; i < 11; i++){
        channelGraph[i]->graph()->setPen(QPen(Qt::black));
        audio[i]->setChecked(false);
    }
    channelGraph[9]->graph()->setPen(QPen(Qt::red));
    data->setAudioChannel(9);
    audio[9]->setChecked(true);
}

void MainWindow::on_graph11_clicked(){
    for(int i = 0; i < 11; i++){
        channelGraph[i]->graph()->setPen(QPen(Qt::black));
        audio[i]->setChecked(false);
    }
    channelGraph[10]->graph()->setPen(QPen(Qt::red));
    data->setAudioChannel(10);
    audio[10]->setChecked(true);
}
