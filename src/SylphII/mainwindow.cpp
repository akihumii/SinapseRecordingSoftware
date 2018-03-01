#include "mainwindow.h"

MainWindow::MainWindow(){
    QString version(APP_VERSION);
    timer.start();
    setWindowTitle(tr("SINAPSE Sylph II Recording Software V") + version);
    data = new DataProcessor(samplingRate);
    serialChannel = new SerialChannel(this, data);
    socketSylph = new SocketSylph(data);
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

    channelGraph[2]->yAxis->setRange(-5,260, Qt::AlignLeft);
    channelGraph[2]->yAxis->setTickStep(50);
    channelGraph[3]->yAxis->setRange(0, 65535, Qt::AlignLeft);
    channelGraph[3]->yAxis->setTickStep(13000);
    channelGraph[3]->axisRect()->setMargins(QMargins(75,10,0,15));

    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}

void MainWindow::createActions(){
    audio1 = new QAction(tr("Channel 1 Audio Output"));
    audio2 = new QAction(tr("Channel 2 Audio Output"));
    audio3 = new QAction(tr("Sync Pulse Audio Output"));

    connect(audio1, SIGNAL(triggered(bool)), this, SLOT(on_graph1_clicked()));
    connect(audio2, SIGNAL(triggered(bool)), this, SLOT(on_graph2_clicked()));
    connect(audio3, SIGNAL(triggered(bool)), this, SLOT(on_graph3_clicked()));

    aboutAction = new QAction(tr("About SINAPSE Recording Software"));
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(about()));

    filterAction = new QAction(tr("Filter Configuration"), this);
    filterAction->setShortcut(tr("Ctrl+F"));
    connect(filterAction, SIGNAL(triggered(bool)), this, SLOT(on_filterConfig_trigger()));

    restartAction = new QAction(tr("Restart serial port"));
    restartAction->setShortcut(tr("Ctrl+A"));
    connect(restartAction, SIGNAL(triggered(bool)), this, SLOT(on_restart_triggered()));

    dataAnalyzerAction = new QAction(tr("Data Analy&zer"), this);
    dataAnalyzerAction->setShortcut(tr("Ctrl+Z"));
    connect(dataAnalyzerAction, SIGNAL(triggered()), this, SLOT(on_dataAnalyzer_triggered()));

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

    voltage50u = new QAction(tr("+/- 50uV"));
    voltage100u = new QAction(tr("+/- 100uV"));
    voltage200u = new QAction(tr("+/- 200uV"));
    voltage500u = new QAction(tr("+/- 500uV"));
    voltage1000u = new QAction(tr("+/- 1000uV"));
    voltage2000u = new QAction(tr("+/- 2000uV"));
    voltage5000u = new QAction(tr("+/- 5000uV"));
    voltage10000u = new QAction(tr("+/- 10000uV"));

    connect(voltage50u, SIGNAL(triggered(bool)), this, SLOT(on_voltage50u_triggered()));
    connect(voltage100u, SIGNAL(triggered(bool)), this, SLOT(on_voltage100u_triggered()));
    connect(voltage200u, SIGNAL(triggered(bool)), this, SLOT(on_voltage200u_triggered()));
    connect(voltage500u, SIGNAL(triggered(bool)), this, SLOT(on_voltage500u_triggered()));
    connect(voltage1000u, SIGNAL(triggered(bool)), this, SLOT(on_voltage1000u_triggered()));
    connect(voltage2000u, SIGNAL(triggered(bool)), this, SLOT(on_voltage2000u_triggered()));
    connect(voltage5000u, SIGNAL(triggered(bool)), this, SLOT(on_voltage5000u_triggered()));
    connect(voltage10000u, SIGNAL(triggered(bool)), this, SLOT(on_voltage10000u_triggered()));
}

void MainWindow::createMenus(){
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(filterAction);
    fileMenu->addSeparator();
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
    voltageMenu->addAction(voltage10000u);
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
    voltageGroup->addAction(voltage10000u);

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
        connectionStatus.clear();
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
            connectionStatus.clear();
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
    serialChannel->closeADCPort();
    serialChannel->closeImplantPort();
}

void MainWindow::updateData(){
    if(restartCount < 15 && serialChannel->isConnected()){
        on_restart_triggered();
        restartCount++;
    }
    QVector<double> X_axis = data->retrieveXAxis();
    if(X_axis.size() >= data->getNumDataPoints()){
        for(int i=0; i<4; i++){
            if(!data->isEmpty(i)){
                channelGraph[i]->graph()->setData(X_axis, data->retrieveData(i));
                channelGraph[i]->xAxis->setRange(X_axis.at(0), data->getNumDataPoints()*period, Qt::AlignLeft);
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
    setTimeFrameTickStep(TimeFrames10ms, 0.001);
}

void MainWindow::on_timeFrame20_triggered(){
    setTimeFrameTickStep(TimeFrames20ms, 0.002);
}

void MainWindow::on_timeFrame50_triggered(){
    setTimeFrameTickStep(TimeFrames50ms, 0.005);
}

void MainWindow::on_timeFrame100_triggered(){
    setTimeFrameTickStep(TimeFrames100ms, 0.01);
}

void MainWindow::on_timeFrame200_triggered(){
    setTimeFrameTickStep(TimeFrames200ms, 0.02);
}

void MainWindow::on_timeFrame500_triggered(){
    setTimeFrameTickStep(TimeFrames500ms, 0.05);
}

void MainWindow::on_timeFrame1000_triggered(){
    setTimeFrameTickStep(TimeFrames1000ms, 0.1);
}

void MainWindow::on_timeFrame2000_triggered(){
    setTimeFrameTickStep(TimeFrames2000ms, 0.2);
}

void MainWindow::on_timeFrame5000_triggered(){
    setTimeFrameTickStep(TimeFrames5000ms, 0.5);
}

void MainWindow::setTimeFrameTickStep(TimeFrames timeframe, double step){
    data->setNumDataPoints(timeframe, samplingRate);
    data->clearallChannelData();
    for(int i=0;i<4;i++){
        channelGraph[i]->xAxis->setTickStep(step);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_voltage50u_triggered(){
    setVoltageTickStep(0.000050, 0.0001, 0.00001);
}

void MainWindow::on_voltage100u_triggered(){
    setVoltageTickStep(0.0001, 0.0002, 0.00002);
}

void MainWindow::on_voltage200u_triggered(){
    setVoltageTickStep(0.0002, 0.0004, 0.00004);
}

void MainWindow::on_voltage500u_triggered(){
    setVoltageTickStep(0.00050, 0.001, 0.0001);
}

void MainWindow::on_voltage1000u_triggered(){
    setVoltageTickStep(0.001, 0.002, 0.0002);
}

void MainWindow::on_voltage2000u_triggered(){
    setVoltageTickStep(0.002, 0.004, 0.0004);
}

void MainWindow::on_voltage5000u_triggered(){
    setVoltageTickStep(0.005, 0.01, 0.001);
}

void MainWindow::on_voltage10000u_triggered(){
    setVoltageTickStep(0.01, 0.02, 0.002);
}

void MainWindow::setVoltageTickStep(double position, double size, double step){
    for(int i=0;i<2;i++){
        channelGraph[i]->yAxis->setRange(-position, size, Qt::AlignLeft);
        channelGraph[i]->yAxis->setTickStep(step);
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
    data->setNumDataPoints(TimeFrames100ms, 20864.0);
    data->clearallChannelData();
    timeFrame100ms->setChecked(true);
}


void MainWindow::on_filterConfig_trigger(){
    FilterDialog filterDialog(data);
    filterDialog.exec();
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

void MainWindow::on_dataAnalyzer_triggered(){
    QProcess *process = new QProcess(this);
    QString file = QDir::currentPath() + QDir::separator() + "SylphAnalyzerII.exe";
    process->start(file);
}

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
    if(socketSylph->isConnected()){
        socketSylph->discardData();
    }
}

// Display "About" message box.
void MainWindow::about()
{
    QMessageBox::about(this, tr("About SINAPSE Recording Software"),
            tr("<h2>Singapore Institute for Neurotechnology</h2>"
               "<p>Version 1.0"
               "<p>Copyright &copy; 2016-2018 SINAPSE"));
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
