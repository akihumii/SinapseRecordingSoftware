#include "catwindow.h"

namespace Cat {

CatWindow::CatWindow(){
    QString version(APP_VERSION);
    setWindowTitle(tr("Cat Software V") + version);
    commandCat = new CommandCat;
    filenameSocket = new SocketFilename;
    filenameDialog = new FilenameDialog;
    connect(filenameDialog, SIGNAL(filenameChanged()), this, SLOT(on_filename_changed()));
    connect(filenameDialog, SIGNAL(filenameDiscard()), this, SLOT(on_filename_discard()));
//    filenameDialog->setFixedSize(filenameDialog->sizeHint());
//    filenameDialog->show();
    configurationFile = new ConfigurationFile;
    configurationFile->setFilenameSettingsMostRecent(filenameMostRecent);
    connect(configurationFile, SIGNAL(writeSettingsSignal()), this, SLOT(on_write_settings_changed()));
    connect(configurationFile, SIGNAL(readSettingsSignal()), this, SLOT(on_read_settings_changed()));
    highpassValueSets = new QVector<double>;
    lowpassValueSets = new QVector<double>;
    notchValueSets = new QVector<double>;
    createStatusBar();
    configurationFile->readMostRecentSettings();
    createActions();
    filenameSettingsAllMapper = new QSignalMapper(this);
    connect(filenameSettingsAllMapper, SIGNAL(mapped(int)), this, SLOT(on_read_settings_selected_changed(int)));
    createLayout();
    sendParameters();
    startDelay = 0;  //no need to wait for other connection anymore
//    highpassCheckBox->setChecked(true);
//    notchCheckBox->setChecked(true);
    on_update_numClass_changed();
}

void CatWindow::createLayout(){
    mainWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(createMethodsGroup());
    mainLayout->addWidget(createSettingsGroup());
//    mainLayout->addWidget(createTrainingGroup());
    mainLayout->addWidget(createParametersGroup());
    mainLayout->addWidget(createRecordingGroup());
    mainLayout->addLayout(createStartButton());
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
    if(methodsClassifyBox[0]->isChecked()) check_input_boxes();
//    mainLayout->setSizeConstraint( QLayout::SetFixedSize );
    this->resize(this->sizeHint());
}

QGroupBox *CatWindow::createMethodsGroup(){
    QGroupBox *methodsGroup = new QGroupBox(tr("Activation"));

    QVBoxLayout *methodsParameterLayout = new QVBoxLayout;
    methodsParameterLayout->addWidget(createMethodsClassifyGroup());

    methodsGroup->setLayout(methodsParameterLayout);

    return methodsGroup;
}

QGroupBox *CatWindow::createSettingsGroup(){
    groupSettings= new QGroupBox(tr("Settings"));
    methodsClassifyBox[0]->isChecked() ? groupSettings->setLayout(createThresholdLayouot()) : groupSettings->setLayout(createFeatureLayout());

    return groupSettings;
}

QHBoxLayout *CatWindow::createFeatureLayout(){
    QHBoxLayout *featureLayout = new QHBoxLayout;

    updateNumClassButton = new QPushButton("Update Movement");
    updateNumClassButton->setFixedSize(100,20);
    QFont updateNumClassFont = updateNumClassButton->font();
    updateNumClassFont.setPointSize(8);
    updateNumClassButton->setFont(updateNumClassFont);
    connect(updateNumClassButton, SIGNAL(clicked(bool)), this, SLOT(on_update_numClass_changed()));

    QHBoxLayout *settingsFeatureOutputLabelLayout = new QHBoxLayout;
    QLabel *channelFeatureLabel[4];
    for(int i = 0; i < 4; i++){
        channelFeatureLabel[i] = new QLabel("Channel " + QString::number(i+1));
        settingsFeatureOutputLabelLayout->addWidget(channelFeatureLabel[i]);
    }

    //Layout
    settingsFeatureInputLayout = new QVBoxLayout;
    settingsFeatureInputLayout->addWidget(updateNumClassButton);

    settingsFeatureOutputLayout = new QVBoxLayout;
    settingsFeatureOutputLayout->addLayout(settingsFeatureOutputLabelLayout);

    //Mapper
    featureInputMapper = new QSignalMapper(this);
    featureOutputMapperCh1 = new QSignalMapper(this);
    featureOutputMapperCh2 = new QSignalMapper(this);
    featureOutputMapperCh3 = new QSignalMapper(this);
    featureOutputMapperCh4 = new QSignalMapper(this);
    connect(featureInputMapper, SIGNAL(mapped(int)), this, SLOT(on_input_ch1_changed(int)));
    connect(featureOutputMapperCh1, SIGNAL(mapped(int)), this, SLOT(on_output_ch1_changed(int)));
    connect(featureOutputMapperCh2, SIGNAL(mapped(int)), this, SLOT(on_output_ch2_changed(int)));
    connect(featureOutputMapperCh3, SIGNAL(mapped(int)), this, SLOT(on_output_ch3_changed(int)));
    connect(featureOutputMapperCh4, SIGNAL(mapped(int)), this, SLOT(on_output_ch4_changed(int)));

    //row of checkboxes
    for(int i = 0; i < indexFeature; i++){
        createFeatureSettingsLayout(i);  //create input and output boxes
    }
    QLabel *emptyFeatureInputRow = new QLabel;
    QLabel *emptyFeatureOutputRow = new QLabel;
    settingsFeatureInputLayout->addWidget(emptyFeatureInputRow);
    settingsFeatureOutputLayout->addWidget(emptyFeatureOutputRow);

    QLabel *emptyFeatureButton = new QLabel;
    emptyFeatureButton->setFixedSize(boxWidth, boxHeight);

    //Grouping
    settingsFeatureInputGroup = new QGroupBox(tr("Input"));
    settingsFeatureInputGroup->setLayout(settingsFeatureInputLayout);

    settingsFeatureOutputGroup = new QGroupBox(tr("Output"));
    settingsFeatureOutputGroup->setLayout(settingsFeatureOutputLayout);

    featureLayout->addWidget(settingsFeatureInputGroup);
    featureLayout->addWidget(settingsFeatureOutputGroup);

    return featureLayout;

}

void CatWindow::createFeatureSettingsLayout(int index){
    //Boxes
    featureInputBox[index] = new QCheckBox;
    featureOutputBoxCh1[index] = new QCheckBox;
    featureOutputBoxCh2[index] = new QCheckBox;
    featureOutputBoxCh3[index] = new QCheckBox;
    featureOutputBoxCh4[index] = new QCheckBox;

    //Size
    featureInputBox[index]->setMinimumSize(boxWidth, boxHeight);
    featureOutputBoxCh1[index]->setMinimumSize(boxWidth, boxHeight);
    featureOutputBoxCh2[index]->setMinimumSize(boxWidth, boxHeight);
    featureOutputBoxCh3[index]->setMinimumSize(boxWidth, boxHeight);
    featureOutputBoxCh4[index]->setMinimumSize(boxWidth, boxHeight);

    //Checks
    featureInputBox[index]->setChecked(featureInputCheckBoxValue[index]);

    featureOutputBoxCh1[index]->setChecked(featureOutputCheckBoxValue[index] & (1 << 0));
    featureOutputBoxCh2[index]->setChecked(featureOutputCheckBoxValue[index] & (1 << 1));
    featureOutputBoxCh3[index]->setChecked(featureOutputCheckBoxValue[index] & (1 << 2));
    featureOutputBoxCh4[index]->setChecked(featureOutputCheckBoxValue[index] & (1 << 3));

    //Connect
    connect(featureInputBox[index], SIGNAL(toggled(bool)), featureInputMapper, SLOT(map()));
    connect(featureOutputBoxCh1[index], SIGNAL(toggled(bool)), featureOutputMapperCh1, SLOT(map()));
    connect(featureOutputBoxCh2[index], SIGNAL(toggled(bool)), featureOutputMapperCh2, SLOT(map()));
    connect(featureOutputBoxCh3[index], SIGNAL(toggled(bool)), featureOutputMapperCh3, SLOT(map()));
    connect(featureOutputBoxCh4[index], SIGNAL(toggled(bool)), featureOutputMapperCh4, SLOT(map()));
    featureInputMapper->setMapping(featureInputBox[index], index);
    featureOutputMapperCh1->setMapping(featureOutputBoxCh1[index], index);
    featureOutputMapperCh2->setMapping(featureOutputBoxCh2[index], index);
    featureOutputMapperCh3->setMapping(featureOutputBoxCh3[index], index);
    featureOutputMapperCh4->setMapping(featureOutputBoxCh4[index], index);

    //Labels
    QLabel *movementLabel = new QLabel("Movement " + QString::number(index + 1) + "              ");

    //Layout
    settingsFeatureInputSubLayout[index] = new QHBoxLayout;
    settingsFeatureOutputSubLayout[index] = new QHBoxLayout;

    settingsFeatureInputSubLayout[index]->addWidget(featureInputBox[index]);
    settingsFeatureInputSubLayout[index]->addWidget(movementLabel);

    settingsFeatureOutputSubLayout[index]->addWidget(featureOutputBoxCh1[index]);
    settingsFeatureOutputSubLayout[index]->addWidget(featureOutputBoxCh2[index]);
    settingsFeatureOutputSubLayout[index]->addWidget(featureOutputBoxCh3[index]);
    settingsFeatureOutputSubLayout[index]->addWidget(featureOutputBoxCh4[index]);

    settingsFeatureInputLayout->addLayout(settingsFeatureInputSubLayout[index]);
    settingsFeatureOutputLayout->addLayout(settingsFeatureOutputSubLayout[index]);
}

QVBoxLayout *CatWindow::createThresholdLayouot(){
    QVBoxLayout *thresholdLayout = new QVBoxLayout;

    //channel labels
    QLabel *channelLabel[8];
    QHBoxLayout *settingsInputLabelLayout = new QHBoxLayout;
    for(int i = 0; i < 4; i++){
        channelLabel[i] = new QLabel("Ch " + QString::number(i+1));
        settingsInputLabelLayout->addWidget(channelLabel[i]);
    }

    QHBoxLayout *settingsOutputLabelLayout = new QHBoxLayout;
    for(int i = 4; i < 8; i++){
        channelLabel[i] = new QLabel("Ch " + QString::number(i-3));
        settingsOutputLabelLayout->addWidget(channelLabel[i]);
    }

    //Layout
    settingsInputLayout = new QVBoxLayout;
    settingsInputLayout->addLayout(settingsInputLabelLayout);

    settingsOutputLayout = new QVBoxLayout;
    settingsOutputLayout->addLayout(settingsOutputLabelLayout);

    //Mapper
    inputMapperCh1 = new QSignalMapper(this);
    inputMapperCh2 = new QSignalMapper(this);
    inputMapperCh3 = new QSignalMapper(this);
    inputMapperCh4 = new QSignalMapper(this);
    outputMapperCh1 = new QSignalMapper(this);
    outputMapperCh2 = new QSignalMapper(this);
    outputMapperCh3 = new QSignalMapper(this);
    outputMapperCh4 = new QSignalMapper(this);
    connect(inputMapperCh1, SIGNAL(mapped(int)), this, SLOT(on_input_ch1_changed(int)));
    connect(inputMapperCh2, SIGNAL(mapped(int)), this, SLOT(on_input_ch2_changed(int)));
    connect(inputMapperCh3, SIGNAL(mapped(int)), this, SLOT(on_input_ch3_changed(int)));
    connect(inputMapperCh4, SIGNAL(mapped(int)), this, SLOT(on_input_ch4_changed(int)));
    connect(outputMapperCh1, SIGNAL(mapped(int)), this, SLOT(on_output_ch1_changed(int)));
    connect(outputMapperCh2, SIGNAL(mapped(int)), this, SLOT(on_output_ch2_changed(int)));
    connect(outputMapperCh3, SIGNAL(mapped(int)), this, SLOT(on_output_ch3_changed(int)));
    connect(outputMapperCh4, SIGNAL(mapped(int)), this, SLOT(on_output_ch4_changed(int)));

    removeMapper = new QSignalMapper(this);
    connect(removeMapper, SIGNAL(mapped(int)), this, SLOT(on_remove_checkbox_clicked(int)));

    //Row of checkboxes
    QVBoxLayout *settingsButtonLayout = new QVBoxLayout();
    addSettingsButton = new QPushButton("+");
    addSettingsButton->setFixedSize(35, 30);
    connect(addSettingsButton, SIGNAL(clicked(bool)), this, SLOT(on_add_checkbox_clicked()));
    settingsButtonLayout->addWidget(addSettingsButton);
    if(indexThreshold >= 30){
        addSettingsButton->setEnabled(false);
//        settingsButtonLayout->setAlignment(addSettingsButton, Qt::AlignRight);
    }

    QHBoxLayout *removeSubLayout[indexThreshold];
    QLabel *removeLabel[indexThreshold];
    for(int i = 0; i < indexThreshold; i++){
        createSettingsLayout(i);  // create input and output boxes
        removeSettingsButton[i] = new QPushButton("-");
        removeSettingsButton[i]->setFixedSize(boxWidth, boxHeight);
        connect(removeSettingsButton[i], SIGNAL(clicked(bool)), removeMapper, SLOT(map()));
        removeMapper->setMapping(removeSettingsButton[i], i);

        removeSubLayout[i] = new QHBoxLayout;
        removeLabel[i] = new QLabel(QString::number(i+1));
        removeLabel[i]->setFixedSize(15, boxHeight);
        removeSubLayout[i]->addWidget(removeLabel[i]);
        removeSubLayout[i]->addWidget(removeSettingsButton[i]);
        settingsButtonLayout->addLayout(removeSubLayout[i]);
//        settingsButtonLayout->setAlignment(removeSettingsButton[i], Qt::AlignRight);
    }

    //Grouping
    settingsInputGroup = new QGroupBox(tr("Input"));
    settingsInputGroup->setLayout(settingsInputLayout);

    settingsOutputGroup = new QGroupBox(tr("Output"));
    settingsOutputGroup->setLayout(settingsOutputLayout);

    //Major Grouping
    QHBoxLayout *settingsThresholdGroupLayout = new QHBoxLayout;
    settingsThresholdGroupLayout->addLayout(settingsButtonLayout);
    settingsThresholdGroupLayout->addWidget(settingsInputGroup);
    settingsThresholdGroupLayout->addWidget(settingsOutputGroup);

    QGroupBox *settingsThresholdGroup = new QGroupBox;
    settingsThresholdGroup->setLayout(settingsThresholdGroupLayout);

    thresholdLayout->addWidget(createThreasholdingGroup());
    thresholdLayout->addWidget(settingsThresholdGroup);

    return thresholdLayout;
}

void CatWindow::sendStimulationPattern(){
    if(methodsClassifyBox[0]->isChecked()){  //threshold
        for(int i = 0; i < indexThreshold; i++){
            commandCat->setStimulationPattern(inputCheckBoxValue[i] | outputCheckBoxValue[i] << 4);
            commandCat->sendStimulationPattern();
            emitCommandSent();
        }
    }
    else{
        for(int i = 0; i < indexFeature; i++){
            if(featureInputCheckBoxValue[i]){
                commandCat->setStimulationPattern(i | featureOutputCheckBoxValue[i] << 4);
                commandCat->sendStimulationPattern();
                emitCommandSent();
            }
        }
    }

}

void CatWindow::updateStimulationPattern(){
    commandCat->sendStimulationPatternFlag();
    emitCommandSent();
    sendStimulationPattern();
}

void CatWindow::check_input_boxes(){
    if(methodsClassifyBox[0]->isChecked()){  //threshold
        bool seen[255] = {false};
        int loc[255] = {0};
        for(int i = 0; i < indexThreshold; i++){
            if(seen[inputCheckBoxValue[i]]){
                repeatedLocs[0] = loc[inputCheckBoxValue[i]];
                repeatedLocs[1] = i+1;
                statusBarLabel->setText(tr("<b><FONT COLOR='#ff0000' FONT SIZE = 4> Repeated input sets: ") + QString::number(repeatedLocs[0]) + tr(", ") + QString::number(repeatedLocs[1]) + tr(" ..."));
                startButton->setEnabled(false);
                addSettingsButton->setEnabled(false);
                return;
            }
            else{
                seen[inputCheckBoxValue[i]] = true;
                loc[inputCheckBoxValue[i]] = i+1;
            }
        }
    }
    startButton->setEnabled(true);
    addSettingsButton->setEnabled(true);
    updateStimulationPattern();
    statusBarLabel->setText(tr("Ready..."));
}

void CatWindow::on_add_checkbox_clicked(){
    indexThreshold++;
    createLayout();
}

void CatWindow::on_remove_checkbox_clicked(int index){
    for(int i = index; i < 29; i++){
        inputCheckBoxValue[i] = inputCheckBoxValue[i+1];
        outputCheckBoxValue[i] = outputCheckBoxValue[i+1];
    }
    inputCheckBoxValue[29] = 0;
    outputCheckBoxValue[29] = 0;

    indexThreshold --;
    createLayout();
}

void CatWindow::createSettingsLayout(int index){
    //Boxes
    inputBoxCh1[index] = new QCheckBox;
    inputBoxCh2[index] = new QCheckBox;
    inputBoxCh3[index] = new QCheckBox;
    inputBoxCh4[index] = new QCheckBox;
    outputBoxCh1[index] = new QCheckBox;
    outputBoxCh2[index] = new QCheckBox;
    outputBoxCh3[index] = new QCheckBox;
    outputBoxCh4[index] = new QCheckBox;

    //Size
    inputBoxCh1[index]->setMinimumSize(boxWidth, boxHeight);
    inputBoxCh2[index]->setMinimumSize(boxWidth, boxHeight);
    inputBoxCh3[index]->setMinimumSize(boxWidth, boxHeight);
    inputBoxCh4[index]->setMinimumSize(boxWidth, boxHeight);
    outputBoxCh1[index]->setMinimumSize(boxWidth, boxHeight);
    outputBoxCh2[index]->setMinimumSize(boxWidth, boxHeight);
    outputBoxCh3[index]->setMinimumSize(boxWidth, boxHeight);
    outputBoxCh4[index]->setMinimumSize(boxWidth, boxHeight);

    //Checks
    inputBoxCh1[index]->setChecked(inputCheckBoxValue[index] & (1 << 0));
    inputBoxCh2[index]->setChecked(inputCheckBoxValue[index] & (1 << 1));
    inputBoxCh3[index]->setChecked(inputCheckBoxValue[index] & (1 << 2));
    inputBoxCh4[index]->setChecked(inputCheckBoxValue[index] & (1 << 3));

    outputBoxCh1[index]->setChecked(outputCheckBoxValue[index] & (1 << 0));
    outputBoxCh2[index]->setChecked(outputCheckBoxValue[index] & (1 << 1));
    outputBoxCh3[index]->setChecked(outputCheckBoxValue[index] & (1 << 2));
    outputBoxCh4[index]->setChecked(outputCheckBoxValue[index] & (1 << 3));

    //Connect
    connect(inputBoxCh1[index], SIGNAL(toggled(bool)), inputMapperCh1, SLOT(map()));
    connect(inputBoxCh2[index], SIGNAL(toggled(bool)), inputMapperCh2, SLOT(map()));
    connect(inputBoxCh3[index], SIGNAL(toggled(bool)), inputMapperCh3, SLOT(map()));
    connect(inputBoxCh4[index], SIGNAL(toggled(bool)), inputMapperCh4, SLOT(map()));
    connect(outputBoxCh1[index], SIGNAL(toggled(bool)), outputMapperCh1, SLOT(map()));
    connect(outputBoxCh2[index], SIGNAL(toggled(bool)), outputMapperCh2, SLOT(map()));
    connect(outputBoxCh3[index], SIGNAL(toggled(bool)), outputMapperCh3, SLOT(map()));
    connect(outputBoxCh4[index], SIGNAL(toggled(bool)), outputMapperCh4, SLOT(map()));
    inputMapperCh1->setMapping(inputBoxCh1[index], index);
    inputMapperCh2->setMapping(inputBoxCh2[index], index);
    inputMapperCh3->setMapping(inputBoxCh3[index], index);
    inputMapperCh4->setMapping(inputBoxCh4[index], index);
    outputMapperCh1->setMapping(outputBoxCh1[index], index);
    outputMapperCh2->setMapping(outputBoxCh2[index], index);
    outputMapperCh3->setMapping(outputBoxCh3[index], index);
    outputMapperCh4->setMapping(outputBoxCh4[index], index);

    //Layout
    settingsInputSubLayout[index] = new QHBoxLayout;
    settingsOutputSubLayout[index] = new QHBoxLayout;

    settingsInputSubLayout[index]->addWidget(inputBoxCh1[index]);
    settingsInputSubLayout[index]->addWidget(inputBoxCh2[index]);
    settingsInputSubLayout[index]->addWidget(inputBoxCh3[index]);
    settingsInputSubLayout[index]->addWidget(inputBoxCh4[index]);

    settingsOutputSubLayout[index]->addWidget(outputBoxCh1[index]);
    settingsOutputSubLayout[index]->addWidget(outputBoxCh2[index]);
    settingsOutputSubLayout[index]->addWidget(outputBoxCh3[index]);
    settingsOutputSubLayout[index]->addWidget(outputBoxCh4[index]);

    settingsInputLayout->addLayout(settingsInputSubLayout[index]);
    settingsOutputLayout->addLayout(settingsOutputSubLayout[index]);
}

void CatWindow::on_input_ch1_changed(int index){
    if(methodsClassifyBox[0]->isChecked()){  //threshold
        inputBoxCh1[index]->isChecked() ? (inputCheckBoxValue[index] |= (1 << 0)) : (inputCheckBoxValue[index] &= ~(1 << 0));
        check_input_boxes();
    }
    else{
        featureInputCheckBoxValue[index] = featureInputBox[index]->isChecked();
        updateStimulationPattern();
    }
}

void CatWindow::on_input_ch2_changed(int index){
    inputBoxCh2[index]->isChecked() ? (inputCheckBoxValue[index] |= (1 << 1)) : (inputCheckBoxValue[index] &= ~(1 << 1));
    check_input_boxes();
}

void CatWindow::on_input_ch3_changed(int index){
    inputBoxCh3[index]->isChecked() ? (inputCheckBoxValue[index] |= (1 << 2)) : (inputCheckBoxValue[index] &= ~(1 << 2));
    check_input_boxes();
}

void CatWindow::on_input_ch4_changed(int index){
    inputBoxCh4[index]->isChecked() ? (inputCheckBoxValue[index] |= (1 << 3)) : (inputCheckBoxValue[index] &= ~(1 << 3));
    check_input_boxes();
}

void CatWindow::on_output_ch1_changed(int index){
    if(methodsClassifyBox[0]->isChecked()){  //threshold
        outputBoxCh1[index]->isChecked() ? (outputCheckBoxValue[index] |= (1 << 0)) : (outputCheckBoxValue[index] &= ~(1 << 0));
        check_input_boxes();
    }
    else{
        featureOutputBoxCh1[index]->isChecked() ? (featureOutputCheckBoxValue[index] |= (1 << 0)) : (featureOutputCheckBoxValue[index] &= ~(1 << 0));
        updateStimulationPattern();
    }
}

void CatWindow::on_output_ch2_changed(int index){
    if(methodsClassifyBox[0]->isChecked()){  //threshold
        outputBoxCh2[index]->isChecked() ? (outputCheckBoxValue[index] |= (1 << 1)) : (outputCheckBoxValue[index] &= ~(1 << 1));
        check_input_boxes();
    }
    else{
        featureOutputBoxCh2[index]->isChecked() ? (featureOutputCheckBoxValue[index] |= (1 << 1)) : (featureOutputCheckBoxValue[index] &= ~(1 << 1));
        updateStimulationPattern();
    }
}

void CatWindow::on_output_ch3_changed(int index){
    if(methodsClassifyBox[0]->isChecked()){  //threshold
        outputBoxCh3[index]->isChecked() ? (outputCheckBoxValue[index] |= (1 << 2)) : (outputCheckBoxValue[index] &= ~(1 << 2));
        check_input_boxes();
    }
    else{
        featureOutputBoxCh3[index]->isChecked() ? (featureOutputCheckBoxValue[index] |= (1 << 2)) : (featureOutputCheckBoxValue[index] &= ~(1 << 2));
        updateStimulationPattern();
    }
}

void CatWindow::on_output_ch4_changed(int index){
    if(methodsClassifyBox[0]->isChecked()){  //threshold
        outputBoxCh4[index]->isChecked() ? (outputCheckBoxValue[index] |= (1 << 3)) : (outputCheckBoxValue[index] &= ~(1 << 3));
        check_input_boxes();
    }
    else{
        featureOutputBoxCh4[index]->isChecked() ? (featureOutputCheckBoxValue[index] |= (1 << 3)) : (featureOutputCheckBoxValue[index] &= ~(1 << 3));
        updateStimulationPattern();
    }
}

QGroupBox *CatWindow::createMethodsClassifyGroup(){
    methodsClassifyBox[0] = new QRadioButton(tr("&Treshold"));
    methodsClassifyBox[1] = new QRadioButton(tr("&Feature"));
    methodsClassifyBox[!((commandCat->getClassifyMethods() - 520) & (1 << 0))]->setChecked(true);
    QHBoxLayout *methodsClassifyLayout = new QHBoxLayout;
    for(int i = 0; i < 2; i++){
        methodsClassifyBox[i]->setMinimumWidth(150);
        methodsClassifyLayout->addWidget(methodsClassifyBox[i]);
        connect(methodsClassifyBox[i], SIGNAL(clicked(bool)), this, SLOT(on_classify_methods_changed()));
    }
    groupMethodsClassify = new QGroupBox();
    groupMethodsClassify->setLayout(methodsClassifyLayout);

    return groupMethodsClassify;
}

QGroupBox *CatWindow::createTrainingGroup(){
    QGroupBox *trainingGroup = new QGroupBox(tr("Training"));

    //number of class
    QLabel *numClassLabel = new QLabel(tr("No. of class"));
    numClassLabel->setMaximumWidth(100);
    numClassSpinBox = new QSpinBox;
    QHBoxLayout *numClassLayout = new QHBoxLayout;
    numClassLayout->addWidget(numClassLabel);
    numClassLayout->addWidget(numClassSpinBox);

    //control buttons
    trainingStart = new QPushButton(tr("Start"));
    trainingRedo = new QPushButton(tr("Redo Class"));
    trainingNext = new QPushButton(tr("Next Class"));
    trainingCancel = new QPushButton(tr("Cancel"));
    QHBoxLayout *trainingButtonsLayout = new QHBoxLayout;
    trainingButtonsLayout->addWidget(trainingStart);
    trainingButtonsLayout->addWidget(trainingRedo);
    trainingButtonsLayout->addWidget(trainingNext);
    trainingButtonsLayout->addWidget(trainingCancel);

    //save buttons
    trainingSave = new QPushButton(tr("Save File"));
    trainingSave->setMaximumWidth(100);
    trainingSaveDir = new QLabel("[display chosen saving location...]");
    QHBoxLayout *trainingSaveLayout = new QHBoxLayout;
    trainingSaveLayout->addWidget(trainingSave);
    trainingSaveLayout->addWidget(trainingSaveDir);

    //Layout
    QVBoxLayout *trainingLayout = new QVBoxLayout;
    trainingLayout->addLayout(numClassLayout);
    trainingLayout->addLayout(trainingButtonsLayout);
    trainingLayout->addLayout(trainingSaveLayout);

    trainingGroup->setLayout(trainingLayout);

    return trainingGroup;
}

QGroupBox *CatWindow::createParametersGroup(){
    parametersGroup = new QGroupBox(tr("Parameters"));

    //window
    int windowWidth = 60;
    QHBoxLayout *windowSubLayout[4];
    QLabel* windowDecodingLabel = new QLabel(tr("Decoding window size (ms): "));
    windowDecodingSpinBox = new QSpinBox;
    windowDecodingSpinBox->setFixedWidth(windowWidth);
    windowDecodingSpinBox->setMinimum(0);
    windowDecodingSpinBox->setMaximum(9999);
    windowDecodingSpinBox->setValue(commandCat->getDecodingWindowSize());
    windowSubLayout[0] = new QHBoxLayout;
    windowSubLayout[0]->addWidget(windowDecodingLabel);
    windowSubLayout[0]->addWidget(windowDecodingSpinBox);
    connect(windowDecodingSpinBox, SIGNAL(editingFinished()), this, SLOT(on_decoding_window_size_changed()));

    QLabel *windowOverlapLabel = new QLabel(tr("Overlap window size (ms): "));
    windowOverlapSpinBox = new QSpinBox;
    windowOverlapSpinBox->setFixedWidth(windowWidth);
    windowOverlapSpinBox->setMinimum(0);
    windowOverlapSpinBox->setMaximum(9999);
    windowOverlapSpinBox->setValue(commandCat->getOverlapWindowSize());
    windowSubLayout[1] = new QHBoxLayout;
    windowSubLayout[1]->addWidget(windowOverlapLabel);
    windowSubLayout[1]->addWidget(windowOverlapSpinBox);
    connect(windowOverlapSpinBox, SIGNAL(editingFinished()), this, SLOT(on_overlap_window_size_changed()));

    QLabel *windowSamplingFrequencyLabel = new QLabel(tr("Sampling frequency (Hz): "));
    windowSamplingFrequencySpinBox = new QSpinBox;
    windowSamplingFrequencySpinBox->setFixedWidth(windowWidth);
    windowSamplingFrequencySpinBox->setMinimum(0);
    windowSamplingFrequencySpinBox->setMaximum(30000);
    windowSamplingFrequencySpinBox->setValue(commandCat->getSamplingFreq());
    windowSamplingFrequencySpinBox->setMaximumWidth(windowWidth);
    windowSubLayout[2] = new QHBoxLayout;
    windowSubLayout[2]->addWidget(windowSamplingFrequencyLabel);
    windowSubLayout[2]->addWidget(windowSamplingFrequencySpinBox);
    connect(windowSamplingFrequencySpinBox, SIGNAL(editingFinished()), this, SLOT(on_sampling_freq_changed()));

    QLabel *extendStimLabel = new QLabel(tr("Extend Stimulation (ms): "));
    extendStimSpinBox = new QSpinBox;
    extendStimSpinBox->setFixedWidth(windowWidth);
    extendStimSpinBox->setMinimum(0);
    extendStimSpinBox->setMaximum(9999);
    extendStimSpinBox->setValue(commandCat->getExtendStimulation());
    windowSubLayout[3] = new QHBoxLayout;
    windowSubLayout[3]->addWidget(extendStimLabel);
    windowSubLayout[3]->addWidget(extendStimSpinBox);
    connect(extendStimSpinBox, SIGNAL(editingFinished()), this, SLOT(on_extend_stimulation_changed()));


    //layout window
    QVBoxLayout *windowLayout = new QVBoxLayout();
    for(int i = 0; i < 4; i++){
        windowLayout->addLayout(windowSubLayout[i]);
    }

    //filtering
    QHBoxLayout *filteringSubLayout[3];
    QLabel *highpassLabel = new QLabel(tr("Highpass cutoff freq.: "));
    highpassCheckBox = new QCheckBox;
    highpassCheckBox->setChecked(highpassFlag);
    highpassSpinBox = new QSpinBox;
    highpassSpinBox->setFixedWidth(windowWidth);
    highpassSpinBox->setMinimum(0);
    highpassSpinBox->setMaximum(windowSamplingFrequencySpinBox->text().toInt()/2 - 1);
    highpassSpinBox->setValue(commandCat->getHighpassCutoffFreq());
    highpassSpinBox->setEnabled(highpassFlag);
    filteringSubLayout[0] = new QHBoxLayout;
    filteringSubLayout[0]->addWidget(highpassCheckBox);
    filteringSubLayout[0]->addWidget(highpassLabel);
    filteringSubLayout[0]->addWidget(highpassSpinBox);
    connect(highpassSpinBox, SIGNAL(editingFinished()), this, SLOT(on_highpass_cutoff_freq_changed()));
    connect(highpassCheckBox, SIGNAL(toggled(bool)), this, SLOT(on_highpass_cutoff_freq_checkbox_changed()));

    QLabel *lowpassLabel = new QLabel(tr("Lowpass cutoff freq.: "));
    lowpassCheckBox = new QCheckBox;
    lowpassCheckBox->setChecked(lowpassFlag);
    lowpassSpinBox = new QSpinBox;
    lowpassSpinBox->setFixedWidth(windowWidth);
    lowpassSpinBox->setMinimum(highpassSpinBox->text().toInt() + 1);
    lowpassSpinBox->setMaximum(windowSamplingFrequencySpinBox->text().toInt()/2 - 1);
    commandCat->getLowpassCutoffFreq() ? lowpassSpinBox->setValue(commandCat->getLowpassCutoffFreq()) :
                                         lowpassSpinBox->setValue(windowSamplingFrequencySpinBox->text().toInt()/2 - 1);
    lowpassSpinBox->setEnabled(lowpassFlag);
    filteringSubLayout[1] = new QHBoxLayout;
    filteringSubLayout[1]->addWidget(lowpassCheckBox);
    filteringSubLayout[1]->addWidget(lowpassLabel);
    filteringSubLayout[1]->addWidget(lowpassSpinBox);
    connect(lowpassSpinBox, SIGNAL(editingFinished()), this, SLOT(on_lowpass_cutoff_freq_changed()));
    connect(lowpassCheckBox, SIGNAL(toggled(bool)), this, SLOT(on_lowpass_cutoff_freq_checkbox_changed()));

    QLabel *notchLabel = new QLabel(tr("Notch cutoff freq.: "));
    notchCheckBox = new QCheckBox;
    notchCheckBox->setChecked(notchFlag);
    notchSpinBox = new QSpinBox;
    notchSpinBox->setFixedWidth(windowWidth);
    notchSpinBox->setMinimum(0);
    notchSpinBox->setMaximum(windowSamplingFrequencySpinBox->text().toInt()-1);
    notchSpinBox->setValue(commandCat->getNotchCutoffFreq());
    notchSpinBox->setEnabled(notchFlag);
    filteringSubLayout[2] = new QHBoxLayout;
    filteringSubLayout[2]->addWidget(notchCheckBox);
    filteringSubLayout[2]->addWidget(notchLabel);
    filteringSubLayout[2]->addWidget(notchSpinBox);
    connect(notchSpinBox, SIGNAL(editingFinished()), this, SLOT(on_notch_cutoff_freq_changed()));
    connect(notchCheckBox, SIGNAL(toggled(bool)), this, SLOT(on_notch_cutoff_freq_checkbox_changed()));

    //layout filtering
    QVBoxLayout *filteringLayout = new QVBoxLayout();
    for(int i = 0; i < 3; i++){
        filteringLayout->addLayout(filteringSubLayout[i]);
    }

    //layout
    QHBoxLayout *parametersLayout = new QHBoxLayout();
    parametersLayout->addLayout(windowLayout);
    parametersLayout->addLayout(filteringLayout);

    parametersGroup->setLayout(parametersLayout);

    return parametersGroup;
}

QGroupBox *CatWindow::createRecordingGroup(){
    QGroupBox *groupRecording = new QGroupBox(tr("Recording"));

    //buttons
    recordingButton = new QPushButton;
    commandCat->getRecording() == 520 ? recordingButton->setText("Start Recor&ding") : recordingButton->setText("Stop Recor&ding");
    connect(recordingButton, SIGNAL(clicked(bool)), this, SLOT(on_recording_changed()));

    recordingTransferButton = new QPushButton(tr("Transfer Recordings"));
    connect(recordingTransferButton, SIGNAL(clicked(bool)), this, SLOT(on_recording_transfer_changed()));

    //connection
    receiveSavedFiles();

    //Layout
    QHBoxLayout *recordingLayout = new QHBoxLayout;
    recordingLayout->addWidget(recordingButton);
    recordingLayout->addWidget(recordingTransferButton);

    groupRecording->setLayout(recordingLayout);

    return groupRecording;
}

QHBoxLayout *CatWindow::createStartButton(){
    QHBoxLayout *startLayout = new QHBoxLayout();

    //start button
    startButton = new QPushButton(tr("Start Integration"));
    connect(startButton, SIGNAL(clicked(bool)), this, SLOT(on_start_changed()));
    startLayout->addWidget(startButton);

    return startLayout;
}

QGroupBox *CatWindow::createThreasholdingGroup(){
    groupThreasholding = new QGroupBox(tr("Thresholding Parameters"));

    QVBoxLayout *thresholdingSubLayout[4];
    QLabel *channelLabel[4];
    for(int i = 0; i < 4; i++){
        thresholdingSubLayout[i] = new QVBoxLayout;

        //Labels
        channelLabel[i] = new QLabel("Channel " + QString::number(i+1));
        thresholdingSubLayout[i]->addWidget(channelLabel[i]);

        //Digits
        QHBoxLayout *thresholdingSpinBoxLayout = new QHBoxLayout;
        thresholdingSpinBox[i] = new QSpinBox;
        thresholdingSpinBox[i]->setMinimum(0);
        thresholdingSpinBox[i]->setMaximum(99);
        thresholdingSpinBox[i]->setValue(commandCat->getThreshold(i));
        thresholdingSpinBoxLayout->addWidget(thresholdingSpinBox[i]);
        connect(thresholdingSpinBox[i], SIGNAL(editingFinished()), this, SLOT(on_threshold_changed()));

        //E
        QLabel *powerLetter = new QLabel("E");
        thresholdingSpinBoxLayout->addWidget(powerLetter);

        //Powers
        thresholdingPowerSpinBox[i] = new QSpinBox;
        thresholdingPowerSpinBox[i]->setMinimum(-20);
        thresholdingPowerSpinBox[i]->setMaximum(10);
        thresholdingPowerSpinBox[i]->setValue(commandCat->getThresholdPower(i));
        thresholdingSpinBoxLayout->addWidget(thresholdingPowerSpinBox[i]);
        connect(thresholdingPowerSpinBox[i], SIGNAL(editingFinished()), this, SLOT(on_threshold_power_changed()));

        thresholdingSubLayout[i]->addLayout(thresholdingSpinBoxLayout);
    }

    //Layout
    QHBoxLayout *thresholdingLayout = new QHBoxLayout;
    for(int i = 0; i < 4; i++){
        thresholdingLayout->addLayout(thresholdingSubLayout[i]);
    }

    groupThreasholding->setLayout(thresholdingLayout);

    return groupThreasholding;
}

void CatWindow::createStatusBar(){
    statusBarLabel = new QLabel;
    statusBarMainWindow = statusBar();
    statusBarMainWindow->addPermanentWidget(statusBarLabel, 1);
    statusBarMainWindow->setSizeGripEnabled(false);  //fixed window size
}

void CatWindow::createActions(){
    //FileMenu
    openSettingsAction = new QAction(tr("&Open"));
    openSettingsAction->setShortcut(tr("Ctrl+O"));
    connect(openSettingsAction, SIGNAL(triggered(bool)), configurationFile, SLOT(on_read_settings_changed()));

//    openSettingsRecentAction = new QAction(tr("&Open Recent"));
//    openSettingsRecentAction->setShortcut(tr("Ctrl+P"));
//    connect(openSettingsRecentAction, SIGNAL(triggered(bool)), this, SLOT(on_open_recent_changed()));

    saveSettingsAction = new QAction(tr("&Save"));
    saveSettingsAction->setShortcut(tr("Ctrl+S"));
    connect(saveSettingsAction, SIGNAL(triggered(bool)), this, SLOT(on_write_settings_changed()));

    saveSettingsAsAction = new QAction(tr("S&ave As"));
    saveSettingsAsAction->setShortcut(tr("Ctrl+A"));
    connect(saveSettingsAsAction, SIGNAL(triggered(bool)), configurationFile, SLOT(on_write_settings_changed()));

    //GUIMenu
    odinAction = new QAction(tr("Od&in Control Panel"));
    odinAction->setShortcut(tr("Ctrl+I"));
    connect(odinAction, SIGNAL(triggered(bool)), this, SLOT(on_odin_triggered()));

    sylphAction = new QAction(tr("Sylph Control Panel"));
    sylphAction->setShortcut(tr("Ctrl+R"));
    connect(sylphAction, SIGNAL(triggered(bool)), this, SLOT(on_sylph_triggered()));

    //Add to menu
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openSettingsAction);
//    fileMenu->addAction(openSettingsRecentAction);
    openSettingsRecentAction = fileMenu->addMenu(tr("O&pen Recent"));
//    filenameSettingsAll = configurationFile->getFilenameSettingsAll();
//    int size = filenameSettingsAll.size();
//    QMenu *openSettingsRecenctSubMenu[size];
//    for(int i = 0; i < size; i++){

//    }
    fileMenu->addAction(saveSettingsAction);
    fileMenu->addAction(saveSettingsAsAction);

    GUIMenu = menuBar()->addMenu(tr("G&UI"));
    GUIMenu->addAction(odinAction);
    GUIMenu->addAction(sylphAction);
}

void CatWindow::sendParameters(){
    int delay = 0;
    int delayInterval = 60;
    for(int i = 0; i < 4; i++){  // send threhsold digits
        QTimer::singleShot((startDelay+delay+i*delayInterval), [=] {
            commandCat->sendThreshold(i);
            emitCommandSent();
        });
    }
    delay += 4*delayInterval;
    for(int i = 0; i < 4; i++){  // send threshold power
        QTimer::singleShot((startDelay+delay+i*delayInterval), [=] {
            commandCat->sendThresholdPower(i);
            emitCommandSent();
        });
    }
    delay += 4*delayInterval;
    QTimer::singleShot((startDelay+delay), [=] {  // send single or multi channel classification
        commandCat->sendSMChannel();
        emitCommandSent();
    });
    delay += delayInterval;
    QTimer::singleShot((startDelay+delay), [=] {  // send classify methods
        commandCat->sendClassifyMethods();
        emitCommandSent();
    });
    delay += delayInterval;
    QTimer::singleShot((startDelay+delay), [=] {  // send start stimulation status
        commandCat->sendStartStimulation();
        emitCommandSent();
    });
    delay += delayInterval;
    QTimer::singleShot((startDelay+delay), [=] {  // send decoding window size
        commandCat->sendDecodingWindowSize();
        emitCommandSent();
        });
    delay += delayInterval;
    QTimer::singleShot((startDelay+delay), [=] {  // send overlap window size
        commandCat->sendOverlapWindowSize();
        emitCommandSent();
        });
    delay += delayInterval;
    QTimer::singleShot((startDelay+delay), [=] {  // send sampling freq
        commandCat->sendSamplingFreq();
        emitCommandSent();
        });
    delay += delayInterval;
    QTimer::singleShot((startDelay+delay), [=] {  // send extend stimulation
        commandCat->sendExtendStimulation();
        emitCommandSent();
        });
    delay += delayInterval;
    QTimer::singleShot((startDelay+delay), [=] {  // send highpass freq
        commandCat->sendHighpassCutoffFreq();
        emitCommandSent();
        });
    delay += delayInterval;
    QTimer::singleShot((startDelay+delay), [=] {  // send lowpass freq
        commandCat->sendLowpassCutoffFreq();
        emitCommandSent();
        });
    delay += delayInterval;
    QTimer::singleShot((startDelay+delay), [=] {  //send notch freq
        commandCat->sendNotchCutoffFreq();
        emitCommandSent();
        });
    delay += delayInterval;
    QTimer::singleShot((startDelay+delay), [=] {
        QMessageBox::information(this, "Done!", "Classification parameters have been sent...");
        mainWidget->setEnabled(true);
        });
    delay += delayInterval;
}

void CatWindow::on_classify_methods_changed(){
    int temp = commandCat->getClassifyMethods();
    for(int i = 0; i < 2; i++){
        commandCat->setClassifyMethods(i, methodsClassifyBox[i]->isChecked());
        commandCat->setSMChannel(i, methodsClassifyBox[i]->isChecked());
    }

    if(temp != commandCat->getClassifyMethods()){
        qDebug() << "Sent classify methods to: " << commandCat->getClassifyMethods();
        commandCat->sendClassifyMethods();
        emitCommandSent();

        qDebug() << "Sent SM channel to: " << commandCat->getSMChannel();
        commandCat->sendSMChannel();
        emitCommandSent();

        createLayout();
    }
}

void CatWindow::on_update_numClass_changed(){
    statusBarLabel->setText("Updating feature classes...");
    mainWidget->setEnabled(false);
    filenameSocket->doConnect("192.168.4.3", 7777);
    commandCat->sendFilename(commandNumClass);
    filenameSocket->write(commandCat->getFilenameCommand());
    QTimer::singleShot(3000, [=] {
        numClassValue = filenameSocket->read();
        qDebug() << "Received num class value: " << numClassValue.toInt();
        indexFeature = numClassValue.toInt();
        filenameSocket->doDisconnect();
        createLayout();
//        statusBarLabel->setText("Ready...");
    });
}

void CatWindow::on_sm_channel_changed(){
    int temp = commandCat->getSMChannel();
    for(int i = 0; i < 2; i++){
        commandCat->setSMChannel(i, methodsSMChannelBox[i]->isChecked());
    }
    if(temp != commandCat->getSMChannel()){
        qDebug() << "Sent SM Channel to: " << commandCat->getSMChannel();
        commandCat->sendSMChannel();
        emitCommandSent();
    }
}

void CatWindow::on_threshold_changed(){
    for(int i = 0; i < 4; i++){
        if(thresholdingSpinBox[i]->text().toInt() != commandCat->getThreshold(i)){
            commandCat->setThreshold(i, thresholdingSpinBox[i]->text().toInt());
            qDebug() << "Sent channel " << i << "threshold power to : " << thresholdingSpinBox[i]->text().toInt();
            commandCat->sendThreshold(i);
            emitCommandSent();
        }
    }
}

void CatWindow::on_threshold_power_changed(){
    for(int i = 0; i < 4; i++){
        if(thresholdingPowerSpinBox[i]->text().toInt() != commandCat->getThresholdPower(i)){
            commandCat->setThresholdPower(i, thresholdingPowerSpinBox[i]->text().toInt());
            qDebug() << "Sent channel " << i << "threshold power to : " << thresholdingPowerSpinBox[i]->text().toInt();
            commandCat->sendThresholdPower(i);
            emitCommandSent();
        }
    }
}

void CatWindow::on_decoding_window_size_changed(){
    if(windowDecodingSpinBox->text().toInt() != commandCat->getDecodingWindowSize()){
        commandCat->setDecodingWindowSize(windowDecodingSpinBox->text().toInt());
        qDebug() << "Sent decoding window size to : " << windowDecodingSpinBox->text().toInt();
        commandCat->sendDecodingWindowSize();
        emitCommandSent();
    }
}

void CatWindow::on_overlap_window_size_changed(){
    if(windowOverlapSpinBox->text().toInt() != commandCat->getOverlapWindowSize()){
        commandCat->setOverlapWindowSize(windowOverlapSpinBox->text().toInt());
        qDebug() << "Sent overlap window size to : " << windowOverlapSpinBox->text().toInt();
        commandCat->sendOverlapWindowSize();
        emitCommandSent();
    }
}

void CatWindow::on_sampling_freq_changed(){
    if(windowSamplingFrequencySpinBox->text().toInt() != commandCat->getSamplingFreq()){
        highpassSpinBox->setMaximum(lowpassSpinBox->text().toInt() - 1);
        lowpassSpinBox->setMinimum(highpassSpinBox->text().toInt() + 1);
        lowpassSpinBox->setMaximum(windowSamplingFrequencySpinBox->text().toInt()/2 - 1);
        notchSpinBox->setMaximum(windowSamplingFrequencySpinBox->text().toInt()-1);

        sendFilteringParameters();

        QTimer::singleShot(150, [=] {
            commandCat->setSamplingFreq(windowSamplingFrequencySpinBox->text().toInt());
            qDebug() << "Sent decoding window size to : " << windowSamplingFrequencySpinBox->text().toInt();
            commandCat->sendSamplingFreq();
            emitCommandSent();
        });

    }
}

void CatWindow::on_extend_stimulation_changed(){
    if(extendStimSpinBox->text().toInt() != commandCat->getExtendStimulation()){
        commandCat->setExtendStimulation(extendStimSpinBox->text().toInt());
        qDebug() << "Sent extend stimulation to : " << extendStimSpinBox->text().toInt();
        commandCat->sendExtendStimulation();
        emitCommandSent();
    }
}

void CatWindow::sendHighpassParameters(int value){
    commandCat->setHighpassCutoffFreq(value);
    qDebug() << "Sent highpass cutoff freq to : " << value;
    commandCat->sendHighpassCutoffFreq();
    emitCommandSent();
}

void CatWindow::on_highpass_cutoff_freq_changed(){
    if(highpassSpinBox->text().toInt() != commandCat->getHighpassCutoffFreq()){
        sendHighpassParameters(highpassSpinBox->text().toInt());
        sendHighpassSignal(highpassSpinBox->text().toInt());
    }
}

void CatWindow::on_highpass_cutoff_freq_checkbox_changed(){
    if(highpassCheckBox->isChecked()){
        highpassSpinBox->setEnabled(true);
        sendFilteringParameters();
        highpassFlag = true;
    }
    else{
        highpassSpinBox->setEnabled(false);
        sendHighpassParameters(0);
        sendHighpassSignal(0);
        highpassFlag = false;
    }
}

void CatWindow::sendLowpassParameters(int value){
    commandCat->setLowpassCutoffFreq(value);
    qDebug() << "Sent lowpass cutoff freq to : " << value;
    commandCat->sendLowpassCutoffFreq();
    emitCommandSent();
}

void CatWindow::on_lowpass_cutoff_freq_changed(){
    if(lowpassSpinBox->text().toInt() != commandCat->getLowpassCutoffFreq()){
        sendLowpassParameters(lowpassSpinBox->text().toInt());
        sendLowpassSignal(lowpassSpinBox->text().toInt());
    }
}

void CatWindow::on_lowpass_cutoff_freq_checkbox_changed(){
    if(lowpassCheckBox->isChecked()){
        lowpassSpinBox->setEnabled(true);
        sendFilteringParameters();
        lowpassFlag = true;
    }
    else{
        lowpassSpinBox->setEnabled(false);
        sendLowpassParameters(0);
        sendLowpassSignal(0);
        lowpassFlag = false;
    }
}

void CatWindow::sendNotchParameters(int value){
    commandCat->setNotchCutoffFreq(value);
    qDebug() << "Sent notch cutoff freq to : " << value;
    commandCat->sendNotchCutoffFreq();
    emitCommandSent();

}

void CatWindow::on_notch_cutoff_freq_changed(){
    if(notchSpinBox->text().toInt() != commandCat->getNotchCutoffFreq()){
        sendNotchParameters(notchSpinBox->text().toInt());
        sendNotchSignal(notchSpinBox->text().toInt());
    }
}

void CatWindow::on_notch_cutoff_freq_checkbox_changed(){
    if(notchCheckBox->isChecked()){
        notchSpinBox->setEnabled(true);
        sendFilteringParameters();
        notchFlag = true;
    }
    else{
        notchSpinBox->setEnabled(false);
        sendNotchParameters(0);
        sendNotchSignal(0);
        notchFlag = false;
    }
}

void CatWindow::on_recording_changed(){
    if(!recordingFlag){
        recordingFlag = true;
        commandCat->setRecording(recordingFlag);
        recordingButton->setText("Stop Recor&ding");
        statusBarLabel->setText(tr("<b><FONT COLOR='#ff0000' FONT SIZE = 4> Recording...</b>"));
//        controlInput(false);

        filenameDialog->setFilename(QDateTime::currentDateTime().toString("'data_'yyyyMMdd_HHmmss'"));
    }
    else{
        recordingFlag = false;
        commandCat->setRecording(recordingFlag);
        recordingButton->setText("Start Recor&ding");
        statusBarLabel->setText(tr("<b><FONT COLOR='#ff0000'> Recording stopped!!!"));
//        controlInput(true);

        filenameDialog->show();
    }
    commandCat->sendRecording();
    emitCommandSent();
    qDebug() << "Sent recording to : " << commandCat->getRecording();
}

//void CatWindow::on_tab_changed(int value){
//    currentTab = value;
//}

void CatWindow::on_filename_changed(){
    filenameSocket->doConnect("192.168.4.3", 7777);
    commandCat->sendFilename(filenameDialog->getFilename());
    filenameSocket->write(commandCat->getFilenameCommand());
    filenameSocket->doDisconnect();
    filenameDialog->hide();

    statusBarLabel->setText(tr("<b><FONT COLOR='#ff0000'> Recording stopped!!! Filename: ") + filenameDialog->getFilename());
}

void CatWindow::on_recording_transfer_changed(){
    recordingTransferButton->setEnabled(false);
    QString savingDirStr = "C:/Data";
    QDir savingDir(savingDirStr);
    if(!savingDir.exists()){  // check if saving folder exists; if not, create it
        savingDir.mkpath(".");
    }
    QString command = "pscp";
    commandArg.clear();
    commandArg.append("-pw");
    commandArg.append("raspberry");
    commandArg.append("-scp");
    commandArg.append("-unsafe");
    commandArg.append("pi@192.168.4.3:/home/pi/Data/*.csv");
    commandArg.append(savingDirStr);
    receivingSavedFiles.start(command, commandArg, QIODevice::ReadWrite);
}

void CatWindow::on_start_changed(){
    if(!startStimulationFlag){  // start stimulation
        startStimulationFlag = true;
        commandCat->setStartStimulation(startStimulationFlag);
        startButton->setText("Stop Integration");
        controlInput(false);
    }
    else{
        startStimulationFlag = false;
        commandCat->setStartStimulation(startStimulationFlag);
        startButton->setText("Start Integration");
        controlInput(true);
    }
    commandCat->sendStartStimulation();
    emitCommandSent();
    qDebug() << "Sent start stimulation to : " << commandCat->getStartStimulation();
}

void CatWindow::sendFilteringParameters(){
    highpassSpinBox->setMaximum(lowpassSpinBox->text().toInt() - 1);
    lowpassSpinBox->setMinimum(highpassSpinBox->text().toInt() + 1);
    lowpassSpinBox->setMaximum(windowSamplingFrequencySpinBox->text().toInt()/2 - 1);
    notchSpinBox->setMaximum(windowSamplingFrequencySpinBox->text().toInt()-1);

    QTimer::singleShot(0, [=] {
        int highpassValue;
        highpassCheckBox->isChecked()? highpassValue = highpassSpinBox->text().toInt() : highpassValue = 0;
        sendHighpassParameters(highpassValue);
        sendHighpassSignal(highpassValue);
    });
    QTimer::singleShot(150, [=] {
        int lowpassValue;
        lowpassCheckBox->isChecked()? lowpassValue = lowpassSpinBox->text().toInt() : lowpassValue = 0;
        sendLowpassParameters(lowpassValue);
        sendLowpassSignal(lowpassValue);
    });
    QTimer::singleShot(300, [=] {
        int notchValue;
        notchCheckBox->isChecked()? notchValue = notchSpinBox->text().toInt() : notchValue = 0;
        sendNotchParameters(notchValue);
        sendNotchSignal(notchValue);
    });
}

void CatWindow::sendHighpassSignal(double highpassValue){
    highpassValueSets->clear();
    highpassValueSets->append((double) highpassValue);
    highpassValueSets->append((double) windowSamplingFrequencySpinBox->text().toDouble());
    highpassValueSets->append((double) highpassCheckBox->isChecked());
    emit highpassSent(highpassValueSets);
}

void CatWindow::sendLowpassSignal(double lowpassValue){
    lowpassValueSets->clear();
    lowpassValueSets->append((double) lowpassValue);
    lowpassValueSets->append((double) windowSamplingFrequencySpinBox->text().toDouble());
    lowpassValueSets->append((double) lowpassCheckBox->isChecked());
    emit lowpassSent(lowpassValueSets);
}

void CatWindow::sendNotchSignal(double notchValue){
    notchValueSets->clear();
    notchValueSets->append((double) notchValue);
    notchValueSets->append((double) windowSamplingFrequencySpinBox->text().toDouble());
    notchValueSets->append((double) notchCheckBox->isChecked());
    emit notchSent(notchValueSets);
}

void CatWindow::receiveSavedFiles(){
    connect(&receivingSavedFiles, SIGNAL(readyReadStandardError()), this, SLOT(readOutput()));
    connect(&receivingSavedFiles, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));
    connect(&receivingSavedFiles, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [=] (int /*exitCode*/, QProcess::ExitStatus exitStatus){
        if(!exitStatus){
            statusBarLabel->setText(tr("Finished transferring..."));
            qDebug() << "Finished transferring...";
            commandCat->sendRecordingTransfer();
            emitCommandSent();
            qDebug() << "Sent recording transfer...";
        }
        else{
            statusBarLabel->setText(tr("Transferring failed..."));
            qDebug() << "Transferring failed...";
        }
        recordingTransferButton->setEnabled(true);
    });
}

void CatWindow::readOutput(){
    commandStdout.clear();
    commandStdout.append(receivingSavedFiles.readAllStandardOutput());
    transferStatus.clear();
    transferStatus.append(commandStdout);
    if(!transferStatus.isEmpty()){
        statusBarLabel->setText(transferStatus);
        qDebug() << commandStdout;
    }
}

void CatWindow::setRpiCommand(char *data){
    commandCat->updateRpiCommand(data);
    emitCommandSent();
}

void CatWindow::emitCommandSent(){
    strcpy(lastSentCommand, commandCat->getlastRpiCommand().data());
    emit commandSent(lastSentCommand);
}

void CatWindow::on_odin_triggered(){
    emit showOdinSignal();
}

void CatWindow::on_sylph_triggered(){
    emit showSylphSignal();
}

void CatWindow::controlInput(bool flag){
    groupSettings->setEnabled(flag);
    parametersGroup->setEnabled(flag);
    groupMethodsClassify->setEnabled(flag);
}

void CatWindow::on_filename_discard(){
    filenameSocket->doConnect("192.168.4.3", 7777);
    commandCat->sendFilename(filenameDiscard);
    filenameSocket->write(commandCat->getFilenameCommand());
    filenameSocket->doDisconnect();

    filenameDialog->hide();
    statusBarLabel->setText(tr("<b><FONT COLOR='#ff0000'> Recording stopped!!! File DISCARDED!!!"));
}

void CatWindow::updateFilenameSettingsAll(){
    if(!filenameSettings.contains(filenameMostRecent)){  //exclude the most recent filename
        if(!filenameSettingsAll.contains(filenameSettings)){  //check if it's contained in the recent filename list
            if(indexRecentFilenameSettings < 10){
                recentFilenameSettings[indexRecentFilenameSettings] = new QAction;  //add action
                connect(recentFilenameSettings[indexRecentFilenameSettings], SIGNAL(triggered(bool)), filenameSettingsAllMapper, SLOT(map()));
                filenameSettingsAllMapper->setMapping(recentFilenameSettings[indexRecentFilenameSettings], indexRecentFilenameSettings);
                openSettingsRecentAction->addAction(recentFilenameSettings[indexRecentFilenameSettings]);  //add into menu
                indexRecentFilenameSettings ++;  //add the indexFilenameSettings
            }
            else{
                filenameSettingsAll.removeLast();
            }
            filenameSettingsAll.prepend(filenameSettings);  //add into filenameSettingsAll
        }
        else{  //move that
            int index = filenameSettingsAll.indexOf(QRegExp("*"+filenameSettings+"*", Qt::CaseInsensitive, QRegExp::Wildcard));
            filenameSettingsAll.move(index, 0);
        }
        updateOpenSettingsRecent();
    }
}

void CatWindow::updateOpenSettingsRecent(){
    for(int i = 0; i < indexRecentFilenameSettings; i++){
        recentFilenameSettings[i]->setText(filenameSettingsAll[i]);
    }
}

void CatWindow::on_write_settings_changed(){
    QSettings settings(configurationFile->getFilenameSettings(), QSettings::IniFormat);

    settings.setValue("methodsClassifyBox", methodsClassifyBox[0]->isChecked());  //classify methods
    settings.beginWriteArray("threshold");  //threshold
    for(int i = 0; i < 4; i++){
        settings.setArrayIndex(i);
        settings.setValue("digit", commandCat->getThreshold(i));
        settings.setValue("power", commandCat->getThresholdPower(i));
    }
    settings.endArray();
    settings.beginWriteArray("thresholdStimulation");  //threshold stimulation pattern
    for(int i = 0; i < indexThreshold; i++){
        settings.setArrayIndex(i);
        settings.setValue("input", inputCheckBoxValue[i]);
        settings.setValue("output", outputCheckBoxValue[i]);
    }
    settings.endArray();
    settings.beginWriteArray("featureStimulation");  //feature stimulation pattern
    for(int i = 0; i < indexFeature; i++){
        settings.setArrayIndex(i);
        settings.setValue("input", featureInputCheckBoxValue[i]);
        settings.setValue("output", featureOutputCheckBoxValue[i]);
    }
    settings.endArray();
    settings.beginGroup("parameters");  //parameters
    settings.setValue("decoding", windowDecodingSpinBox->text());
    settings.setValue("overlap", windowOverlapSpinBox->text());
    settings.setValue("samplingFrequency", windowSamplingFrequencySpinBox->text());
    settings.setValue("extendStimulation", extendStimSpinBox->text());
    settings.setValue("highpass", highpassSpinBox->text());
    settings.setValue("highpassFlag", highpassFlag);
    settings.setValue("lowpass", lowpassSpinBox->text());
    settings.setValue("lowpassFlag", lowpassFlag);
    settings.setValue("notch", notchSpinBox->text());
    settings.setValue("notchFlag", notchFlag);
    settings.endGroup();

    statusBarLabel->setText(tr("Save settings as: ") + configurationFile->getFilenameSettings());
    qDebug() << "all saved keys: " << settings.allKeys();
}

void CatWindow::on_read_settings_changed(){
    filenameSettingsTemp = configurationFile->getFilenameSettings();
    if(indexTemp == -1 || !filenameSettingsTemp.contains(filenameSettings)){
        filenameSettings = filenameSettingsTemp;
        readSettings();
    }
}

void CatWindow::on_read_settings_selected_changed(int index){
    filenameSettingsTemp = filenameSettingsAll[index];
    QFile file(filenameSettingsTemp);
    qDebug() << "status of the file is: " << file.exists();
    if(file.exists()){
        if(index != 0){
            filenameSettings = filenameSettingsAll[index];
            readSettings();
        }
    }
    else{
        qDebug() << "file doesn't exist...!!!";
        indexTemp = index;
        QMessageBox removeFilenameBox;
        removeFilenameBox.setText(filenameSettingsTemp + " does not exist...\nRemove from list?");
        removeFilenameBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        removeFilenameBox.setDefaultButton(QMessageBox::Cancel);
        int ret = removeFilenameBox.exec();

        switch(ret){
            case QMessageBox::Ok:
                removeFilename();
                break;
            case QMessageBox::Cancel:
                break;
            default:
                break;
        }
    }
}

void CatWindow::removeFilename(){
    filenameSettingsAll.removeAt(indexTemp);  //remove filename in filenameSettingsAll
    openSettingsRecentAction->removeAction(recentFilenameSettings[indexTemp]);  //remove filename in recent file list
    indexRecentFilenameSettings --;  //decrease the number of filenameSettingsAll
    updateOpenSettingsRecent();
}

void CatWindow::readSettings(){
    QSettings settings(filenameSettings, QSettings::IniFormat);

    commandCat->setClassifyMethods(0, settings.value("methodsClassifyBox").toBool());  //classify methods
    commandCat->setSMChannel(0, settings.value("methodsClassifyBox").toBool());
    commandCat->setClassifyMethods(1, !settings.value("methodsClassifyBox").toBool());  //classify methods
    commandCat->setSMChannel(1, !settings.value("methodsClassifyBox").toBool());
    settings.beginReadArray("threshold");  //threshold
    for(int i = 0; i < 4; i++){
        settings.setArrayIndex(i);
        commandCat->setThreshold(i, settings.value("digit").toInt());
        commandCat->setThresholdPower(i, settings.value("power").toInt());
    }
    settings.endArray();
    indexThreshold = settings.beginReadArray("thresholdStimulation");  //threshold stimulation pattern
    for(int i = 0; i < indexThreshold; i++){
        settings.setArrayIndex(i);
        inputCheckBoxValue[i] = settings.value("input").toInt();
        outputCheckBoxValue[i] = settings.value("output").toInt();
    }
    settings.endArray();
    indexFeature = settings.beginReadArray("featureStimulation");  //feature stimulation pattern
    for(int i = 0; i < indexFeature; i++){
        settings.setArrayIndex(i);
        featureInputCheckBoxValue[i] = settings.value("input").toInt();
        featureOutputCheckBoxValue[i] = settings.value("output").toInt();
    }
    settings.endArray();
    settings.beginGroup("parameters");  //parameters
    commandCat->setDecodingWindowSize(settings.value("decoding").toInt());
    commandCat->setOverlapWindowSize(settings.value("overlap").toInt());
    commandCat->setSamplingFreq(settings.value("samplingFrequency").toInt());
    commandCat->setExtendStimulation(settings.value("extendStimulation").toInt());
    commandCat->setHighpassCutoffFreq(settings.value("highpass").toInt());
    highpassFlag = settings.value("highpassFlag").toBool();
    commandCat->setLowpassCutoffFreq(settings.value("lowpass").toInt());
    lowpassFlag = settings.value("lowpassFlag").toBool();
    commandCat->setNotchCutoffFreq(settings.value("notch").toInt());
    notchFlag = settings.value("notchFlag").toBool();
    settings.endGroup();

//    statusBarLabel->setText(tr("Read settings: ") + configurationFile->getFilenameSettings());
    createLayout();

    mainWidget->setEnabled(false);
    updateFilenameSettingsAll();
    sendParameters();
}

void CatWindow::closeEvent(QCloseEvent *event){
    configurationFile->writeMostRecentSettings();
    event->accept();
}

CatWindow::~CatWindow(){
}
}
