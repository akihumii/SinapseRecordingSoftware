#include "mainwindow_ka.h"

MainWindow_KA::MainWindow_KA(){
    QString version(APP_VERSION);
    NeutrinoChannel = new Channel;
    NeutrinoCommand = new Command(NeutrinoChannel);
    NeutrinoData = new DataProcessor_KA(NeutrinoChannel);

    ////serialNeutrino = new //serialNeutrino(this, NeutrinoCommand, NeutrinoData, NeutrinoChannel);
    socketEdison = new SocketEdison(this, NeutrinoCommand, NeutrinoData, NeutrinoChannel);//, //serialNeutrino);
    setWindowTitle(tr("Neutrino Edison V") + version);
    timer.start();
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(updateData()));
    dataTimer.start(10);     //tick timer every XXX msec
    createStatusBar();
    create5x2Layout();
    createActions();
    createMenus();

}

void MainWindow_KA::create10x1Layout(){
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

void MainWindow_KA::create5x2Layout(){
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

void MainWindow_KA::createActions(){
    connectAction = new QAction(tr("&Connect"), this);
    connectAction->setShortcut(tr("Ctrl+C"));
    connect(connectAction, SIGNAL(triggered()), this, SLOT(on_ConnectMenu_triggered()));

    disconnectAction = new QAction(tr("&Disconnect"), this);
    disconnectAction->setShortcut(tr("Ctrl+D"));
    connect(disconnectAction, SIGNAL(triggered()), this, SLOT(on_DisconnectMenu_triggered()));

    commandAction = new QAction(tr("Comm&and"), this);
    commandAction->setShortcut(tr("Ctrl+A"));
    connect(commandAction, SIGNAL(triggered()), this, SLOT(on_CommandMenu_triggered()));

    recordFileNameAction = new QAction(tr("&Specify File Name"), this);
    recordFileNameAction->setShortcut(tr("Ctrl+S"));
    connect(recordFileNameAction, SIGNAL(triggered()), this, SLOT(on_recordFileName_triggered()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+X"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    tenby1Action = new QAction(tr("10x&1 Layout"), this);
    tenby1Action->setShortcut(tr("Ctrl+1"));
    connect(tenby1Action, SIGNAL(triggered()),this,SLOT(on_tenby1_triggered()));

    fiveby2Action = new QAction(tr("5x&2 Layout"), this);
    fiveby2Action->setShortcut(tr("Ctrl+2"));
    connect(fiveby2Action, SIGNAL(triggered()),this,SLOT(on_fiveby2_triggered()));

    recordAction = new QAction(tr("&Record"), this);
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

    wifiMode = new QAction(tr("Wireless Mode"));
    wiredMode = new QAction(tr("Wired Mode"));

    connect(wifiMode, SIGNAL(triggered(bool)), this, SLOT(on_wifi_triggered()));
    connect(wiredMode, SIGNAL(triggered(bool)), this, SLOT(on_wired_triggered()));
}

void MainWindow_KA::createMenus(){
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(connectAction);
    fileMenu->addAction(disconnectAction);
    fileMenu->addAction(commandAction);
    fileMenu->addSeparator();
    fileMenu->addAction(recordAction);
    fileMenu->addAction(recordFileNameAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    layoutMenu = menuBar()-> addMenu(tr("&Layout"));
    layoutMenu->addAction(tenby1Action);
    layoutMenu->addAction(fiveby2Action);

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

    connectivityMenu = menuBar()->addMenu(tr("Connection Mode"));
    connectivityGroup = new QActionGroup(this);
    connectivityMenu->addAction(wifiMode);
    connectivityMenu->addAction(wiredMode);
    wifiMode->setCheckable(true);
    wifiMode->setChecked(true);
    wiredMode->setCheckable(true);

    connectivityGroup->addAction(wifiMode);
    connectivityGroup->addAction(wiredMode);
}

void MainWindow_KA::createStatusBar(){
    statusBarLabel = new QLabel;
    statusBarLabel->setText("Hello world");
    statusBarMainWindow = statusBar();
    statusBarMainWindow->addPermanentWidget(statusBarLabel, 1);
    statusBarMainWindow->setSizeGripEnabled(false);  // fixed window size
}

MainWindow_KA::~MainWindow_KA(){
    if(socketEdison->isConnected()){
        socketEdison->writeCommand(QByteArray::number(255, 10));
        socketEdison->doDisconnect();
    }
    //serialNeutrino->closePort();
}

void MainWindow_KA::on_ConnectMenu_triggered(){
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

void MainWindow_KA::on_DisconnectMenu_triggered(){
    if(socketEdison->isConnected()){
        qDebug() << "Disconnecting, please wait...";
        socketEdison->writeCommand(QByteArray::number(255, 10));
        socketEdison->doDisconnect();
    }
    if(!socketEdison->isConnected()){
        statusBarLabel->setText("Disconnected!!");
    }
}

void MainWindow_KA::on_CommandMenu_triggered(){
    statusBarLabel->setText("Command Dialog Opened");
    CommandDialog commandDialog(socketEdison, NeutrinoCommand, NeutrinoChannel);
    commandDialog.exec();
}

void MainWindow_KA::on_tenby1_triggered(){
    create10x1Layout();
}

void MainWindow_KA::on_fiveby2_triggered(){
    create5x2Layout();
}

void MainWindow_KA::updateData(){
    QVector<double> X_axis = NeutrinoData->retrieveXAxis();
    if(NeutrinoData->isPlotEnabled() && X_axis.size() > (NeutrinoData->getNumDataPoints())){
        for(int i=0; i<10; i++){
            if(!NeutrinoData->isEmpty(i)){
                channelGraph[i]->graph()->setData(X_axis, NeutrinoData->retrieveData(i));
                channelGraph[i]->xAxis->setRange(X_axis.at(0), (NeutrinoData->getNumDataPoints())*0.000056, Qt::AlignLeft);
                channelGraph[i]->replot();
                NeutrinoData->clearChannelData(i);
            }
        }
        NeutrinoData->removeXAxis();
    }
}

void MainWindow_KA::on_record_triggered(){
    if(!NeutrinoData->isRecordEnabled()){
        NeutrinoData->setRecordEnabled(true);
        statusBarLabel->setText("Recording...");
    }
    else if(NeutrinoData->isRecordEnabled()){
        NeutrinoData->setRecordEnabled(false);
        statusBarLabel->setText("Recording stopped!!! File saved to ");
    }
}

void MainWindow_KA::on_recordFileName_triggered(){
    statusBarLabel->setText("Set your preferred file name");
    FileNameDialog fileNameDialog(NeutrinoData);
    fileNameDialog.exec();
    statusBarLabel->setText("File Name set to: " + NeutrinoData->getFileName());
}

void MainWindow_KA::on_timeFrame10_triggered(){
    NeutrinoData->setNumDataPoints(TimeFrames10ms);
    NeutrinoData->clearallChannelData();
}

void MainWindow_KA::on_timeFrame20_triggered(){
    NeutrinoData->setNumDataPoints(TimeFrames20ms);
    NeutrinoData->clearallChannelData();
}

void MainWindow_KA::on_timeFrame50_triggered(){
    NeutrinoData->setNumDataPoints(TimeFrames50ms);
    NeutrinoData->clearallChannelData();
}

void MainWindow_KA::on_timeFrame100_triggered(){
    NeutrinoData->setNumDataPoints(TimeFrames100ms);
    NeutrinoData->clearallChannelData();
}

void MainWindow_KA::on_timeFrame200_triggered(){
    NeutrinoData->setNumDataPoints(TimeFrames200ms);
    NeutrinoData->clearallChannelData();
}

void MainWindow_KA::on_timeFrame500_triggered(){
    NeutrinoData->setNumDataPoints(TimeFrames500ms);
    NeutrinoData->clearallChannelData();
}

void MainWindow_KA::on_timeFrame1000_triggered(){
    NeutrinoData->setNumDataPoints(TimeFrames1000ms);
    NeutrinoData->clearallChannelData();
}

void MainWindow_KA::on_timeFrame2000_triggered(){
    NeutrinoData->setNumDataPoints(TimeFrames2000ms);
    NeutrinoData->clearallChannelData();
}

void MainWindow_KA::on_timeFrame5000_triggered(){
    NeutrinoData->setNumDataPoints(TimeFrames5000ms);
    NeutrinoData->clearallChannelData();
}

void MainWindow_KA::on_wifi_triggered(){
    qDebug() << "ready via Wifi";
    //serialNeutrino->closePort();
    //serialNeutrino->//serialenabled = false;
    socketEdison->wifiEnabled = true;
}

void MainWindow_KA::on_wired_triggered(){
    qDebug() << "ready via USB";
    //serialNeutrino->//serialenabled = true;
    //serialNeutrino->doConnect();
    socketEdison->wifiEnabled = false;
}

