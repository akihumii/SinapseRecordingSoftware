#include "mainwindow.h"

MainWindow::MainWindow(QString filename){
    while(!filename.endsWith(".csv")){
        if(filename.isNull())
            break;
        QMessageBox::information(this, "Wrong file selected!",
                                 "Please load .csv file. \n");
        filename =  QFileDialog::getOpenFileName(Q_NULLPTR,
                                                "Open Document",
                                                QDir::currentPath(),
                                                "All files (*.*)");
    }
    if(!filename.isNull()){
        QFile rawData(filename);
        if(rawData.open(QIODevice::ReadOnly)){
            QTextStream in(&rawData);
            QString firstLine = in.readLine();
            QProgressDialog progress("Loading data, please wait...", "Cancel", 0, rawData.size()/firstLine.size(), this);
            progress.setWindowModality(Qt::WindowModal);
            for(int i = 0; i < firstLine.size(); i++){
                if(firstLine.at(i) == ','){
                    numChannels++;
                }
            }
            while(!in.atEnd()){
                QString line = in.readLine();
                for(int i = 0; i < numChannels; i++){
                    channelData[i].append(line.split(" ,")[i].toInt()*1.2/256);
                }
                total_data_points++;
                progress.setValue(total_data_points);
                if (progress.wasCanceled())
                    break;
            }
            progress.setValue(rawData.size()/firstLine.size());
            setWindowTitle(tr("Data Analyzer"));
            createLayout();
            GraphDialog graphDialog(firstLoad, channelData, total_data_points, this);
            graphDialog.setMinimumSize(1366,768);
            graphDialog.showMaximized();
            graphDialog.exec();
        }
    }
    else{
        exit(1);
    }
}

MainWindow::~MainWindow(){
}

void MainWindow::createLayout(){
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QWidget *mainWidget = new QWidget;
    createTabs();
    mainLayout->addWidget(modeTabWidget);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}

void MainWindow::createTabs(){
    modeTabWidget = new QTabWidget;
    singleEndMode = new QWidget;
    differentialMode = new QWidget;
    thresholdMode = new QWidget;

    modeTabWidget->addTab(singleEndMode, tr("Single End Mode"));
    modeTabWidget->addTab(differentialMode, tr("Differential Mode"));
    modeTabWidget->addTab(thresholdMode, tr("Threshold Mode"));

    QVBoxLayout *singleEndLayout = new QVBoxLayout;
    for(int i = 0; i < numChannels; i++){
        dataEnable[i] = new QCheckBox("Channel " + QString::number(i+1), this);
        singleEndLayout->addWidget(dataEnable[i]);
        connect(dataEnable[i], SIGNAL(toggled(bool)), this, SLOT(on_dataSelected()));
    }
    processSingleButton = new QPushButton(tr("Process"));
    connect(processSingleButton, SIGNAL(clicked(bool)), this, SLOT(on_startSingleEndProcessing()));
    singleEndLayout->addWidget(processSingleButton);

    singleEndMode->setLayout(singleEndLayout);

    QVBoxLayout *differentialLayout = new QVBoxLayout;
    for(int i = 0; i < 5; i++){
        minusLayout[i] = new QHBoxLayout;
        diffEnable[i] = new QCheckBox;
        connect(diffEnable[i], SIGNAL(toggled(bool)), this, SLOT(on_diffEnable_changed()));
        A[i] = new QComboBox;
        B[i] = new QComboBox;
        minus[i] = new QLabel("minus");
        A[i]->setEnabled(false);
        minus[i]->setEnabled(false);
        B[i]->setEnabled(false);
        for(int j = 0; j < 10; j++){
            A[i]->addItem("Channel " + QString::number(j+1, 10));
            B[i]->addItem("Channel " + QString::number(j+1, 10));
        }
        minusLayout[i]->addWidget(diffEnable[i]);
        minusLayout[i]->addWidget(A[i]);
        minusLayout[i]->addWidget(minus[i]);
        minusLayout[i]->addWidget(B[i]);
        differentialLayout->addLayout(minusLayout[i]);
    }
    diffEnable[0]->setChecked(true);
    A[0]->setEnabled(true);
    minus[0]->setEnabled(true);
    B[0]->setEnabled(true);
    processDiffButton = new QPushButton(tr("Process"));
    connect(processDiffButton, SIGNAL(clicked(bool)), this, SLOT(on_startDiffProcessing()));
    differentialLayout->addWidget(processDiffButton);
    differentialMode->setLayout(differentialLayout);

    QVBoxLayout *thresholdLayout = new QVBoxLayout;

    channelSelect = new QComboBox;
    for(int i = 0; i < 10; i++){
        channelSelect->addItem("Channel " + QString::number(i+1, 10));
    }

    QHBoxLayout *thresholdLimitLayout = new QHBoxLayout;
    QLabel *thresholdLimitLabel = new QLabel("Threshold: ");
    QLabel *uV = new QLabel("uV");
    lessmore = new QComboBox;
    lessmore->addItem(">");
    lessmore->addItem("<");
    thresholdLineEdit = new QLineEdit(tr("0.8"));
    QValidator *validator = new QDoubleValidator(-1.2, 1.2, 6);
    thresholdLineEdit->setValidator(validator);

    thresholdLimitLayout->addWidget(thresholdLimitLabel);
    thresholdLimitLayout->addWidget(lessmore);
    thresholdLimitLayout->addWidget(thresholdLineEdit);
    thresholdLimitLayout->addWidget(uV);

    QHBoxLayout *beforeLayout = new QHBoxLayout;
    QLabel *before = new QLabel("Pre-threshold timeframe:  ");
    QLabel *ms1 = new QLabel("ms");
    beforeLineEdit = new QLineEdit(tr("-10"));
    QValidator *beforevalidator = new QIntValidator(-9999, 0);
    beforeLineEdit->setValidator(beforevalidator);
    beforeLayout->addWidget(before);
    beforeLayout->addWidget(beforeLineEdit);
    beforeLayout->addWidget(ms1);

    QHBoxLayout *afterLayout = new QHBoxLayout;
    QLabel *after = new QLabel("Post-threshold timeframe: ");
    QLabel *ms2 = new QLabel("ms");
    afterLineEdit = new QLineEdit(tr("10"));
    QValidator *aftervalidator = new QIntValidator(0, 9999);
    afterLineEdit->setValidator(aftervalidator);
    afterLayout->addWidget(after);
    afterLayout->addWidget(afterLineEdit);
    afterLayout->addWidget(ms2);

    QHBoxLayout *maxSpikesLayout = new QHBoxLayout;
    QLabel *maxSpikesLabel = new QLabel("Maximum plots:");
    maxSpikesCombo = new QComboBox;
    for(int i = 0; i < 5; i++){
        maxSpikesCombo->addItem(QString::number((i+1)*10, 10) + "plots");
    }
    maxSpikesLayout->addWidget(maxSpikesLabel);
    maxSpikesLayout->addWidget(maxSpikesCombo);

    processThresholdButton = new QPushButton(tr("Process"));
    connect(processThresholdButton, SIGNAL(clicked(bool)), this, SLOT(on_startThresholdProcessing()));

    thresholdLayout->addWidget(channelSelect);
    thresholdLayout->addLayout(thresholdLimitLayout);
    thresholdLayout->addLayout(beforeLayout);
    thresholdLayout->addLayout(afterLayout);
    thresholdLayout->addLayout(maxSpikesLayout);
    thresholdLayout->addWidget(processThresholdButton);

    thresholdMode->setLayout(thresholdLayout);
}

void MainWindow::on_dataSelected(){
    for(int i = 0; i < numChannels; i++){
        if(dataEnable[i]->isChecked()){
            dataSelected[i] = true;
        }
        else{
            dataSelected[i] = false;
        }
    }
}

void MainWindow::on_diffEnable_changed(){
    for(int i = 0; i < 5; i++){
        if(diffEnable[i]->isChecked()){
            A[i]->setEnabled(true);
            minus[i]->setEnabled(true);
            B[i]->setEnabled(true);
        }
        else{
            A[i]->setEnabled(false);
            minus[i]->setEnabled(false);
            B[i]->setEnabled(false);
        }
    }
}

void MainWindow::on_startSingleEndProcessing(){
    GraphDialog graphDialog(dataSelected, channelData, total_data_points, this);
    graphDialog.setMinimumSize(1366,768);
    graphDialog.showMaximized();
    graphDialog.exec();
}

void MainWindow::on_startDiffProcessing(){
    GraphDialog graphDialog(&A[0], &B[0], &diffEnable[0], channelData, total_data_points, this);
    graphDialog.setMinimumSize(1366,768);
    graphDialog.showMaximized();
    graphDialog.exec();
}

void MainWindow::on_startThresholdProcessing(){
    int preThreshold = beforeLineEdit->text().toInt();
    int postThreshold = afterLineEdit->text().toInt();
    double threshold = thresholdLineEdit->text().toDouble();
    int channelSelected = channelSelect->currentIndex();
    int greaterlesser = lessmore->currentIndex();
    int maxSpikes = (maxSpikesCombo->currentIndex()+1)*10;
    GraphDialog graphDialog(preThreshold, postThreshold, greaterlesser, maxSpikes, threshold, channelData, channelSelected, this);
    graphDialog.setMinimumSize(1366,768);
    graphDialog.showMaximized();
    graphDialog.exec();
}
