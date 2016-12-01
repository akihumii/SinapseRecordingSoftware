#include "mainwindow_ma.h"

MainWindow_MA::MainWindow_MA(){

    QString version(APP_VERSION);

    defaultRange = new QCPRange(-0.00000016, 0.00000032);

    setWindowTitle(tr("Marshal Wirless Recording V") + version);
    timer.start();
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(updateData()));
    data = new DataProcessor_MA;
    serialChannel = new SerialChannel(this, data);
    dataTimer.start(10);     //tick timer every XXX msec
    createStatusBar();
    createLayout();
    createActions();
    createMenus();

    statusBarLabel->setText("Scanning Serial Devices");

    portInfo = QSerialPortInfo::availablePorts();
    if(portInfo.size()>1){
        QProcess::execute("sudo chmod a+rw /dev/" + portInfo.at(0).portName());
        QProcess::execute("sudo chmod a+rw /dev/" + portInfo.at(1).portName());
        if(serialChannel->enableImplantPort(portInfo.at(0).portName())){
            connectionStatus.append("Connected to Implant Port |");
        }
        else{
            connectionStatus.append("Connection to Implant Port failed |");
        }
        if(serialChannel->enableADCPort(portInfo.at(1).portName())){
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
}

void MainWindow_MA::createLayout(){
    QVBoxLayout *mainLayout = new QVBoxLayout;
    for(int i=0;i<3;i++){
        channelGraph[i] = new QCustomPlot;
        mainLayout->addWidget(channelGraph[i]);
        channelGraph[i]->xAxis->setVisible(true);
        channelGraph[i]->axisRect()->setAutoMargins(QCP::msNone);
        channelGraph[i]->axisRect()->setMargins(QMargins(70,0,0,0));
        channelGraph[i]->yAxis->setRange(-0.00000016, 0.00000032, Qt::AlignLeft);
        channelGraph[i]->addGraph();
        channelGraph[i]->xAxis2->setTickStep(0.1);//0.000048);
        connect(channelGraph[i]->yAxis, SIGNAL(rangeChanged(QCPRange)), channelGraph[i]->yAxis2, SLOT(setRange(QCPRange)));
        connect(channelGraph[i]->xAxis, SIGNAL(rangeChanged(QCPRange)), channelGraph[i]->xAxis2, SLOT(setRange(QCPRange)));
        channelGraph[i]->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);
        channelGraph[i]->axisRect()->setRangeZoom(Qt::Vertical);
        channelGraph[i]->axisRect()->setRangeDrag(Qt::Vertical);
    }

    channelGraph[0]->graph()->setPen(QPen(Qt::black));
    channelGraph[1]->graph()->setPen(QPen(Qt::darkRed));
    channelGraph[2]->graph()->setPen(QPen(Qt::darkGreen));

    channelGraph[2]->axisRect()->setMargins(QMargins(70,0,0,15));
    channelGraph[2]->yAxis->setRange(0, 250, Qt::AlignLeft);

    QWidget *mainWidget = new QWidget;
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}

void MainWindow_MA::createActions(){
    serialPortAction = new QAction(tr("S&erial Port Configuration"), this);
    serialPortAction->setShortcut(tr("Ctrl+E"));
    connect(serialPortAction, SIGNAL(triggered()), this, SLOT(on_serialConfig_triggered()));


    recordFileNameAction = new QAction(tr("&Specify File Name"), this);
    recordFileNameAction->setShortcut(tr("Ctrl+S"));
    connect(recordFileNameAction, SIGNAL(triggered()), this, SLOT(on_recordFileName_triggered()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+X"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    recordAction = new QAction(tr("&Record"), this);
    recordAction->setShortcut(tr("Ctrl+R"));
    connect(recordAction, SIGNAL(triggered()), this, SLOT(on_record_triggered()));

    resetDefaultRange = new QAction(tr("Default Range"), this);
    resetDefaultRange->setShortcut(tr("Ctrl+Space"));
    connect(resetDefaultRange, SIGNAL(triggered()), this, SLOT(on_resetRange_triggered()));

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

void MainWindow_MA::createMenus(){
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(serialPortAction);
    fileMenu->addAction(resetDefaultRange);
    fileMenu->addSeparator();
    fileMenu->addAction(recordAction);
    fileMenu->addAction(recordFileNameAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    timeFrameMenu = menuBar()->addMenu(tr("&Time Frames"));
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
}

void MainWindow_MA::createStatusBar(){
    statusBarLabel = new QLabel;
    statusBarMainWindow = statusBar();
    statusBarMainWindow->addPermanentWidget(statusBarLabel, 1);
    statusBarMainWindow->setSizeGripEnabled(false);  // fixed window size
}

MainWindow_MA::~MainWindow_MA(){

}

void MainWindow_MA::updateData(){
    QVector<double> X_axis = data->retrieveXAxis();
    if(X_axis.size() > data->getNumDataPoints()){
        for(int i=0; i<3; i++){
            if(!data->isEmpty(i)){
                channelGraph[i]->graph()->setData(X_axis, data->retrieveData(i));
                channelGraph[i]->xAxis->setRange(X_axis.at(0), data->getNumDataPoints()*0.000048, Qt::AlignLeft);
                channelGraph[i]->replot();
                data->clearChannelData(i);
            }
        }
        data->removeXAxis();
    }
}

void MainWindow_MA::on_serialConfig_triggered(){
    SerialPortDialog serialPortDialog(serialChannel);
    serialPortDialog.exec();
    portInfo = QSerialPortInfo::availablePorts();
    qDebug() << "portInfo.size() = " << portInfo.size();
    if(portInfo.size()>1){
        if(serialChannel->enableImplantPort(portInfo.at(0).portName())){
            QMessageBox::information(this, "Connected!", "Implant Port");
            statusBarLabel->setText("Connected Implant Port");
        }
        if(serialChannel->enableADCPort(portInfo.at(1).portName())){
            QMessageBox::information(this, "Connected!", "ADC Port");
            statusBarLabel->setText("Connected ADC Port");
        }
    }
}

void MainWindow_MA::on_record_triggered(){
    if(!data->isRecordEnabled()){
        data->setRecordEnabled(true);
        statusBarLabel->setText("Recording...");
    }
    else if(data->isRecordEnabled()){
        data->setRecordEnabled(false);
        statusBarLabel->setText("Recording stopped!!! File saved to " + data->getFileName());
    }
}

void MainWindow_MA::on_recordFileName_triggered(){
    statusBarLabel->setText("Set your preferred file name");
    FileNameDialog fileNameDialog(data);
    fileNameDialog.exec();
    statusBarLabel->setText("File Name set to: " + data->getFileName());
}

void MainWindow_MA::on_resetRange_triggered(){
    for(int i=0;i<2;i++){
        channelGraph[i]->yAxis->setRange(-0.00000016, 0.00000032, Qt::AlignLeft);
        channelGraph[i]->replot();
    }
    channelGraph[2]->yAxis->setRange(0, 255, Qt::AlignLeft);
    channelGraph[2]->replot();
}

void MainWindow_MA::resetGraph1Range(){
    channelGraph[0]->yAxis->setRange(-0.00000016, 0.00000032, Qt::AlignLeft);
    channelGraph[0]->replot();
}

void MainWindow_MA::resetGraph2Range(){
    channelGraph[1]->yAxis->setRange(-0.00000016, 0.00000032, Qt::AlignLeft);
    channelGraph[1]->replot();
}

void MainWindow_MA::resetGraph3Range(){
    channelGraph[2]->yAxis->setRange(0, 250, Qt::AlignLeft);
    channelGraph[2]->replot();
}

void MainWindow_MA::on_timeFrame10_triggered(){
    data->setNumDataPoints(TimeFrames10ms);
    data->clearallChannelData();
}

void MainWindow_MA::on_timeFrame20_triggered(){
    data->setNumDataPoints(TimeFrames20ms);
    data->clearallChannelData();
}

void MainWindow_MA::on_timeFrame50_triggered(){
    data->setNumDataPoints(TimeFrames50ms);
    data->clearallChannelData();
}

void MainWindow_MA::on_timeFrame100_triggered(){
    data->setNumDataPoints(TimeFrames100ms);
    data->clearallChannelData();
}

void MainWindow_MA::on_timeFrame200_triggered(){
    data->setNumDataPoints(TimeFrames200ms);
    data->clearallChannelData();
}

void MainWindow_MA::on_timeFrame500_triggered(){
    data->setNumDataPoints(TimeFrames500ms);
    data->clearallChannelData();
}

void MainWindow_MA::on_timeFrame1000_triggered(){
    data->setNumDataPoints(TimeFrames1000ms);
    data->clearallChannelData();
}

void MainWindow_MA::on_timeFrame2000_triggered(){
    data->setNumDataPoints(TimeFrames2000ms);
    data->clearallChannelData();
}

void MainWindow_MA::on_timeFrame5000_triggered(){
    data->setNumDataPoints(TimeFrames5000ms);
    data->clearallChannelData();
}
