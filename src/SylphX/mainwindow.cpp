#include "mainwindow.h"

namespace SylphX {

MainWindow::MainWindow(){
    x = new Odin::OdinWindow();
    x->setFixedSize(x->sizeHint());
//    x->show();
    QString version(APP_VERSION);
    timer.start();
    setWindowTitle(tr("SINAPSE Sylph X Recording Software V") + version);
    dataStream = new DataStream(this);
    dataProcessor = new DataProcessor(dataStream);
    serialChannel = new SerialChannel(this, dataProcessor);
    socketSylph = new SocketSylph(dataProcessor);
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(updateData()));

    connect(x, SIGNAL(commandSent(char*)), dataProcessor, SLOT(setLastSentBytes(char*)));
    connect(x, SIGNAL(amplitudeChanged(double*)), dataProcessor, SLOT(setLastSentAmplitudes(double*)));
    connect(x, SIGNAL(debounceEditted(int)), dataProcessor, SLOT(setDebounce(int)));
    connect(x, SIGNAL(upperThresholdEditted(double)), dataProcessor, SLOT(setUpperThreshold(double)));
    connect(x, SIGNAL(lowerThresholdEditted(double)), dataProcessor, SLOT(setLowerThreshold(double)));
//    connect(dataProcessor, SIGNAL(channelACrossed()), x, SLOT(on_channelAThreshold_crossed()));
//    connect(dataProcessor, SIGNAL(channelBCrossed()), x, SLOT(on_channelBThreshold_crossed()));

    dataTimer.start(50);     //tick timer every XXX msec
    createStatusBar();
    createLayout();
    createActions();
    createMenus();
    connectSylph();
    on_timeFrame_changed(DEFAULT_XAXIS);
    on_voltage_changed(DEFAULT_YAXIS);
    qDebug() << "Starting SYLPH..";
}

void MainWindow::createLayout(){
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

    audioSelectMapper = new QSignalMapper(this);
    connect(audioSelectMapper, SIGNAL(mapped(int)), this, SLOT(on_graph_clicked(int)));

    for(int i = 0; i < 10; i ++){
        audioSelectMapper->setMapping(channelGraph[i], i);
        connect(channelGraph[i], SIGNAL(mousePress(QMouseEvent*)), audioSelectMapper, SLOT(map()));
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
    channelGraph[11]->yAxis->setLabel("Frame Marker");
    channelGraph[11]->yAxis->setLabelPadding(35);
    channelGraph[11]->yAxis->setLabelFont(QFont(font().family(), 8));
    channelGraph[11]->setFixedHeight(100);

    channelGraph[0]->graph()->setPen(QPen(Qt::red));
    channelGraph[10]->graph()->setPen(QPen(Qt::darkGreen));

    channelGraph[10]->yAxis->setRange(-5, 260, Qt::AlignLeft);
    channelGraph[10]->yAxis->setTickStep(50);
    channelGraph[11]->yAxis->setRange(0, 65535, Qt::AlignLeft);
    channelGraph[11]->yAxis->setTickStep(13000);
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

void MainWindow::createActions(){
    for(int i = 0; i < 10; i++){
        audio[i] = new QAction("Channel " + QString::number(i+1, 10)+ " Audio Output");
    }
//    audio[10] = new QAction(tr("Sync Pulse Audio Output"));

//    connect(audio[0], SIGNAL(triggered(bool)), this, SLOT(on_graph1_clicked()));
//    connect(audio[1], SIGNAL(triggered(bool)), this, SLOT(on_graph2_clicked()));
//    connect(audio[2], SIGNAL(triggered(bool)), this, SLOT(on_graph3_clicked()));
//    connect(audio[3], SIGNAL(triggered(bool)), this, SLOT(on_graph4_clicked()));
//    connect(audio[4], SIGNAL(triggered(bool)), this, SLOT(on_graph5_clicked()));
//    connect(audio[5], SIGNAL(triggered(bool)), this, SLOT(on_graph6_clicked()));
//    connect(audio[6], SIGNAL(triggered(bool)), this, SLOT(on_graph7_clicked()));
//    connect(audio[7], SIGNAL(triggered(bool)), this, SLOT(on_graph8_clicked()));
//    connect(audio[8], SIGNAL(triggered(bool)), this, SLOT(on_graph9_clicked()));
//    connect(audio[9], SIGNAL(triggered(bool)), this, SLOT(on_graph10_clicked()));
//    connect(audio[10], SIGNAL(triggered(bool)), this, SLOT(on_graph11_clicked()));

    aboutAction = new QAction(tr("About SINAPSE Recording Software"));
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(about()));

    odinAction = new QAction(tr("Odin Control Panel"));
    connect(odinAction, SIGNAL(triggered(bool)), this, SLOT(on_odin_triggered()));

    disableStream = new QAction(tr("&Disable stream"));
    disableStream->setShortcut(tr("Ctrl+D"));
    connect(disableStream, SIGNAL(triggered(bool)), this, SLOT(on_disableStream_triggered()));

    filterAction = new QAction(tr("Filter Configuration"), this);
    filterAction->setShortcut(tr("Ctrl+F"));
    connect(filterAction, SIGNAL(triggered(bool)), this, SLOT(on_filterConfig_trigger()));

    resetDefaultX = new QAction(tr("Default Time Scale"), this);
    resetDefaultX->setShortcut(tr("Ctrl+X"));
    connect(resetDefaultX, SIGNAL(triggered()), this, SLOT(on_resetX_triggered()));

    chooseDirectoryAction = new QAction(tr("&Save as.."), this);
    chooseDirectoryAction->setShortcut(tr("Ctrl+S"));
    connect(chooseDirectoryAction, SIGNAL(triggered()), this, SLOT(on_chooseDirectory_triggered()));

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

    isSmart = new QAction(tr("Smart Data Processor"), this);
    connect(isSmart, SIGNAL(triggered(bool)), this, SLOT(on_smartDataProcessor_triggered()));
    isDumb = new QAction(tr("Dumb Data Processor"), this);
    connect(isDumb, SIGNAL(triggered(bool)), this, SLOT(on_dumbDataProcessor_triggered()));

    restartAction = new QAction(tr("Resync data"), this);
    restartAction->setShortcut(tr("Ctrl+A"));
    connect(restartAction, SIGNAL(triggered(bool)), this, SLOT(on_restart_triggered()));

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
    for(int i = 0; i < 8; i++){
        voltageAction[i] = new QAction(voltageActionNames[i]);
        voltageMapper->setMapping(voltageAction[i], i);
        connect(voltageAction[i], SIGNAL(triggered(bool)), voltageMapper, SLOT(map()));
    }

}

void MainWindow::createMenus(){
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(odinAction);
    fileMenu->addSeparator();
    fileMenu->addAction(disableStream);
    fileMenu->addSeparator();
    fileMenu->addAction(filterAction);
    fileMenu->addSeparator();
    fileMenu->addAction(dataAnalyzerAction);
    fileMenu->addSeparator();
    fileMenu->addAction(pauseAction);
    fileMenu->addSeparator();
    fileMenu->addAction(recordAction);
    fileMenu->addAction(chooseDirectoryAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

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
    for(int i = 0; i < 8; i++){
        voltageMenu->addAction(voltageAction[i]);
        voltageAction[i]->setCheckable(true);
        voltageGroup->addAction(voltageAction[i]);
    }
    voltageAction[DEFAULT_YAXIS]->setChecked(true);

    voltageMenu->addSeparator();
    voltageMenu->addAction(resetDefaultY);

//    audioOutputMenu = menuBar()->addMenu(tr("Audio Output"));
//    audioGroup = new QActionGroup(this);
//    for(int i = 0; i < 11; i++){
//        audioOutputMenu->addAction(audio[i]);
//        audio[i]->setCheckable(true);
//        audioGroup->addAction(audio[i]);
//    }
//    audio[0]->setChecked(true);

    processorMenu = menuBar()->addMenu(tr("Data Processor Options"));
    processorMenu->addAction(isSmart);
    isSmart->setCheckable(true);
    processorMenu->addAction(isDumb);
    isDumb->setCheckable(true);

    smartOrDumbGroup = new QActionGroup(this);
    smartOrDumbGroup->addAction(isSmart);
    smartOrDumbGroup->addAction(isDumb);
    isSmart->setChecked(true);

    processorMenu->addSeparator();
    processorMenu->addAction(restartAction);

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
    QString temp;
    if(portInfo.size()>1){
        serialChannel->connectSylph();
        serialChannel->isImplantConnected() ? temp.append("Connected to Implant Port |") : temp.append("Connection to Implant Port failed |");
        serialChannel->isADCConnected()? temp.append(" Connected to ADC Port") : temp.append(" Connection to ADC Port failed");
        updateStatusBar(0, temp);
    }
    if(!serialChannel->isADCConnected() && !serialChannel->isImplantConnected()){
        int i = 1;
        do{
            i++;
            socketSylph->doConnect("192.168.4."+QString::number(i), 8888);
        } while(!socketSylph->isConnected() && i < 4);
        if(socketSylph->isConnected()){
            updateStatusBar(0, "Connected to Sylph WiFi Module at 192.168.4." + QString::number(i) + "/8888");
        }
        else{
            updateStatusBar(0, "Failed to connect...");
            QMessageBox::information(this, "Failed to connect!", "No Sylph device detected.. \n"
                                                                 "Check your connections and run the program again..");
        }
    }
}

MainWindow::~MainWindow(){
    if(socketSylph->isConnected()){
        socketSylph->sendDisconnectSignal();
        socketSylph->doDisconnect();
    }
    delete x;
}

void MainWindow::updateData(){
//    if(socketSylph->isConnected()){
    updateStatusBar(1, "Data Rate: " + QString::number(socketSylph->isConnected()? socketSylph->getRate() : serialChannel->getRate()) + " kbps");
    updateStatusBar(3, "Resync Count: " + QString::number(dataProcessor->getResyncCounter()));
//    }
//    else if (serialChannel->isImplantConnected()){
//        updateStatusBar(1, "Data Rate: " + QString::number(serialChannel->getRate()) + " kbps");
//    }
    for(int i=0; i<12; i++){
            channelGraph[i]->graph()->setData(dataProcessor->retrieveXAxis(), (dataProcessor->isFilterEnabled() && i < 10)? dataProcessor->filterData(dataProcessor->retrieveData(i), i): dataProcessor->retrieveData(i));
            if(i < 10 && dataStream->getStreamConnected(i)){
                dataStream->streamData(i);
            }
            if(!pause){
                channelGraph[i]->replot();
            }
    }
}

void MainWindow::on_disableStream_triggered(){
    for(int i = 0; i < 10; i++){
        dataStream->disableStream(i);
    }
}

void MainWindow::on_timeFrame_changed(int timeFrameIndex){
    dataProcessor->setNumDataPoints((TimeFrames) timeFrameIndex, samplingRate);
    for(int i=0;i<12;i++){
        channelGraph[i]->xAxis->setRange(0, dataProcessor->getNumDataPoints()*period, Qt::AlignLeft);
        channelGraph[i]->xAxis->setTickStep(timeFrameSteps[timeFrameIndex]);
        channelGraph[i]->replot();
    }
}

void MainWindow::on_resetX_triggered(){
    on_timeFrame_changed(DEFAULT_XAXIS);
    timeFrameAction[DEFAULT_XAXIS]->setChecked(true);
}

void MainWindow::on_voltage_changed(int voltageIndex){
    voltageIndex < 4? dataProcessor->setScale(1000000) : dataProcessor->setScale(1000);
    for(int i = 0; i < 10; i++){
        channelGraph[i]->yAxis->setRange(voltageMin[voltageIndex], voltageRange[voltageIndex], Qt::AlignLeft);
        channelGraph[i]->yAxis->setTickStep(voltageStep[voltageIndex]);
        channelGraph[i]->yAxis->setLabel("Channel "+ QString::number(i+1, 10) + (dataProcessor->getScale()==1000? " (mV)": " (uV)"));
        channelGraph[i]->replot();
    }
}

void MainWindow::on_resetY_triggered(){
    on_voltage_changed(DEFAULT_YAXIS);
    voltageAction[DEFAULT_YAXIS]->setChecked(true);
}

void MainWindow::on_record_triggered(){
    if(!dataProcessor->isRecordEnabled()){
        dataProcessor->setRecordEnabled(true);
//        dataProcessor->setADCRecordEnabled(true);
        updateStatusBar(2, "<b><FONT COLOR='#ff0000' FONT SIZE = 4> Recording...</b>");
        recordAction->setText("Stop &Recording");
    }
    else if(dataProcessor->isRecordEnabled()){
        dataProcessor->setRecordEnabled(false);
//        dataProcessor->setADCRecordEnabled(false);
        updateStatusBar(2, "<b><FONT COLOR='#ff0000' FONT SIZE = 4> Recording stopped!!! File saved to " + dataProcessor->getFileName() + "</b>");
        recordAction->setText("Start &Recording");
    }
}

void MainWindow::on_playPause_triggered(){
    pause? pauseAction->setText("Pause graph") : pauseAction->setText("Resume graph");
    pause = !pause;
}

void MainWindow::on_chooseDirectory_triggered(){
    updateStatusBar(2, "Set your preferred save file directory");
    dataProcessor->setDirectory(QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 QDir::homePath() + "/Desktop/",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks));

    updateStatusBar(2, "Save directory set to: " + dataProcessor->getDirectory());
}

void MainWindow::on_odin_triggered(){
    x->show();
}

void MainWindow::on_filterConfig_trigger(){
    FilterDialog filterDialog(dataProcessor);
    filterDialog.exec();
}

void MainWindow::on_dataAnalyzer_triggered(){
//    QProcess *process = new QProcess(this);
//    QString file = QDir::currentPath() + QDir::separator() + "SylphAnalyzerX.exe";
//    process->start(file);
}

void MainWindow::on_restart_triggered(){
//    if(serialChannel->isADCConnected()){
//        serialChannel->closeADCPort();
//    }
//    if(serialChannel->isImplantConnected()){
//        serialChannel->closeImplantPort();
//    }
//    connectSylph();
    socketSylph->setChecked(false);
    serialChannel->setChecked(false);
}

// Display "About" message box.
void MainWindow::about(){
    QMessageBox::about(this, tr("About SINAPSE Recording Software"),
            tr("<h2>Singapore Institute for Neurotechnology</h2>"
               "<p>Version 1.0"
               "<p>Copyright &copy; 2016-2018 SINAPSE"));
}

void MainWindow::on_graph_clicked(int index){
    setDefaultGraph();
    activateChannelGraph(index);
}

void MainWindow::setDefaultGraph(){
    for(int i = 0; i < 11; i++){
        channelGraph[i]->graph()->setPen(QPen(Qt::black));
//        audio[i]->setChecked(false);
    }
}

void MainWindow::activateChannelGraph(int index){
    channelGraph[index]->graph()->setPen(QPen(Qt::red));
//    dataProcessor->setAudioChannel(index);
//    audio[index]->setChecked(true);
}

void MainWindow::on_smartDataProcessor_triggered(){
    dataProcessor->setSmartDataProcessor(true);
}

void MainWindow::on_dumbDataProcessor_triggered(){
    dataProcessor->setSmartDataProcessor(false);
}

void MainWindow::updateStatusBar(int index, QString message){
    statusBarText[index].clear();
    statusBarText[index].append(message);
    statusBarLabel->setText(statusBarText[0] + " | " +  statusBarText[1] + " | " + statusBarText[2] + " | " + statusBarText[3]);
}

}
