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
    highpassValueSets = new QVector<double>;
    lowpassValueSets = new QVector<double>;
    notchValueSets = new QVector<double>;
    createLayout();
    sendParameters();
    createActions();
//    highpassCheckBox->setChecked(true);
//    notchCheckBox->setChecked(true);
    createStatusBar();
}

void CatWindow::createLayout(){
    QWidget *mainWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(createMethodsGroup());
    mainLayout->addWidget(createSettingsGroup());
//    mainLayout->addWidget(createTrainingGroup());
    mainLayout->addWidget(createParametersGroup());
    mainLayout->addWidget(createRecordingGroup());
    mainLayout->addLayout(createStartButton());
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
    mainLayout->setSizeConstraint( QLayout::SetFixedSize );
    this->resize(this->sizeHint());
}

QGroupBox *CatWindow::createMethodsGroup(){
    QGroupBox *methodsGroup = new QGroupBox(tr("Activation"));

    QVBoxLayout *methodsParameterLayout = new QVBoxLayout;
    methodsParameterLayout->addWidget(createMethodsClassifyGroup());
    methodsParameterLayout->addWidget(createThreasholdingGroup());
//    methodsParameterLayout->addWidget(createStimPatternGroup());

    methodsGroup->setLayout(methodsParameterLayout);

    return methodsGroup;
}

QGroupBox *CatWindow::createSettingsGroup(){
    groupSettings= new QGroupBox(tr("Settings"));

    //add button
    doneSettingsButton = new QPushButton;
    doneSettingsFlag ? doneSettingsButton->setText("Edit") : doneSettingsButton->setText("Done");
    doneSettingsButton->setFixedSize(35, 30);
    connect(doneSettingsButton, SIGNAL(clicked(bool)), this, SLOT(on_done_settings_changed()));

    QVBoxLayout *settingsButtonLayout = new QVBoxLayout();
    settingsButtonLayout->addWidget(doneSettingsButton);

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
        removeSubLayout[i]->addWidget(removeLabel[i]);
        removeSubLayout[i]->addWidget(removeSettingsButton[i]);
        settingsButtonLayout->addLayout(removeSubLayout[i]);
//        settingsButtonLayout->setAlignment(removeSettingsButton[i], Qt::AlignRight);
    }
    QLabel *emptyInputRow = new QLabel;
    QLabel *emptyOutputRow = new QLabel;

    settingsInputLayout->addWidget(emptyInputRow);
    settingsOutputLayout->addWidget(emptyOutputRow);

    QLabel *emptyButton = new QLabel;
    emptyButton->setFixedSize(boxWidth, boxHeight);

    addSettingsButton = new QPushButton("+");
    addSettingsButton->setFixedSize(35, boxHeight);
    connect(addSettingsButton, SIGNAL(clicked(bool)), this, SLOT(on_add_checkbox_clicked()));
    if(indexThreshold < 30){
        settingsButtonLayout->addWidget(addSettingsButton);
//        settingsButtonLayout->setAlignment(addSettingsButton, Qt::AlignRight);
    }
    else{
        settingsButtonLayout->addWidget(emptyButton);
    }

    //Grouping
    settingsInputGroup = new QGroupBox(tr("Input"));
    settingsInputGroup->setLayout(settingsInputLayout);

    settingsOutputGroup = new QGroupBox(tr("Output"));
    settingsOutputGroup->setLayout(settingsOutputLayout);

    IOLayout = new QHBoxLayout;
    IOLayout->addLayout(settingsButtonLayout);
    IOLayout->addWidget(settingsInputGroup);
    IOLayout->addWidget(settingsOutputGroup);
    groupSettings->setLayout(IOLayout);

    return groupSettings;
}

void CatWindow::on_done_settings_changed(){
    doneSettingsFlag = !doneSettingsFlag;

    if(doneSettingsFlag){
        if(!check_input_boxes()){  //no repeated input
            doneSettingsButton->setText("Edit");
            statusBarLabel->setText(tr("Ready..."));
        }
        else{
            doneSettingsFlag = !doneSettingsFlag;
            statusBarLabel->setText(tr("<b><FONT COLOR='#ff0000' FONT SIZE = 4> Repeated input sets: ") +
                                       QString::number(repeatedLocs[0]) + tr(", ") + QString::number(repeatedLocs[1]) + tr(" ..."));
        }
    }
    else{
        doneSettingsButton->setText("Done");
        statusBarLabel->setText(tr("Editting threshold input output..."));
    }

    // set enable
    settingsInputGroup->setEnabled(!doneSettingsFlag);
    settingsOutputGroup->setEnabled(!doneSettingsFlag);
    for(int i = 0; i < indexThreshold; i++){
        removeSettingsButton[i]->setEnabled(!doneSettingsFlag);
    }
    addSettingsButton->setEnabled(!doneSettingsFlag);
}

bool CatWindow::check_input_boxes(){
    bool seen[255] = {false};
    int loc[255] = {0};
    for(int i = 0; i < indexThreshold; i++){
        if(seen[inputCheckBoxValue[i]]){
            repeatedLocs[0] = loc[inputCheckBoxValue[i]];
            repeatedLocs[1] = i+1;
            return true;
        }
        else{
            seen[inputCheckBoxValue[i]] = true;
            loc[inputCheckBoxValue[i]] = i+1;
        }
    }
    return false;
}

void CatWindow::on_add_checkbox_clicked(){
    indexThreshold ++;
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
    inputCheckBoxValue[index] & (1 << 0) ? inputBoxCh1[index]->setChecked(true) : inputBoxCh1[index]->setChecked(false);
    inputCheckBoxValue[index] & (1 << 1) ? inputBoxCh2[index]->setChecked(true) : inputBoxCh2[index]->setChecked(false);
    inputCheckBoxValue[index] & (1 << 2) ? inputBoxCh3[index]->setChecked(true) : inputBoxCh3[index]->setChecked(false);
    inputCheckBoxValue[index] & (1 << 3) ? inputBoxCh4[index]->setChecked(true) : inputBoxCh4[index]->setChecked(false);

    outputCheckBoxValue[index] & (1 << 0) ? outputBoxCh1[index]->setChecked(true) : outputBoxCh1[index]->setChecked(false);
    outputCheckBoxValue[index] & (1 << 1) ? outputBoxCh2[index]->setChecked(true) : outputBoxCh2[index]->setChecked(false);
    outputCheckBoxValue[index] & (1 << 2) ? outputBoxCh3[index]->setChecked(true) : outputBoxCh3[index]->setChecked(false);
    outputCheckBoxValue[index] & (1 << 3) ? outputBoxCh4[index]->setChecked(true) : outputBoxCh4[index]->setChecked(false);


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
    inputBoxCh1[index]->isChecked() ? (inputCheckBoxValue[index] |= (1 << 0)) : (inputCheckBoxValue[index] &= ~(1 << 0));
}

void CatWindow::on_input_ch2_changed(int index){
    inputBoxCh2[index]->isChecked() ? (inputCheckBoxValue[index] |= (1 << 1)) : (inputCheckBoxValue[index] &= ~(1 << 1));
}

void CatWindow::on_input_ch3_changed(int index){
    inputBoxCh3[index]->isChecked() ? (inputCheckBoxValue[index] |= (1 << 2)) : (inputCheckBoxValue[index] &= ~(1 << 2));
}

void CatWindow::on_input_ch4_changed(int index){
    inputBoxCh4[index]->isChecked() ? (inputCheckBoxValue[index] |= (1 << 3)) : (inputCheckBoxValue[index] &= ~(1 << 3));
}

void CatWindow::on_output_ch1_changed(int index){
    outputBoxCh1[index]->isChecked() ? (outputCheckBoxValue[index] |= (1 << 0)) : (outputCheckBoxValue[index] &= ~(1 << 0));
}

void CatWindow::on_output_ch2_changed(int index){
    outputBoxCh2[index]->isChecked() ? (outputCheckBoxValue[index] |= (1 << 1)) : (outputCheckBoxValue[index] &= ~(1 << 1));
}

void CatWindow::on_output_ch3_changed(int index){
    outputBoxCh3[index]->isChecked() ? (outputCheckBoxValue[index] |= (1 << 2)) : (outputCheckBoxValue[index] &= ~(1 << 2));
}

void CatWindow::on_output_ch4_changed(int index){
    outputBoxCh4[index]->isChecked() ? (outputCheckBoxValue[index] |= (1 << 3)) : (outputCheckBoxValue[index] &= ~(1 << 3));
}

QGroupBox *CatWindow::createMethodsClassifyGroup(){
    methodsClassifyBox[0] = new QRadioButton(tr("&Treshold"));
    methodsClassifyBox[1] = new QRadioButton(tr("&Feature"));
    (commandCat->getClassifyMethods() - 520) & (1 << 0) ? methodsClassifyBox[0]->setChecked(true) : methodsClassifyBox[1]->setChecked(true);
    QHBoxLayout *methodsClassifyLayout = new QHBoxLayout;
    for(int i = 0; i < 2; i++){
        methodsClassifyBox[i]->setMinimumWidth(150);
        methodsClassifyLayout->addWidget(methodsClassifyBox[i]);
        connect(methodsClassifyBox[i], SIGNAL(clicked(bool)), this, SLOT(on_classify_methods_changed()));
    }
    QGroupBox *groupMethodsClassify = new QGroupBox();
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
    windowDecodingSpinBox->setMaximumWidth(windowWidth);
    windowDecodingSpinBox->setMinimum(0);
    windowDecodingSpinBox->setMaximum(9999);
    windowDecodingSpinBox->setValue(commandCat->getDecodingWindowSize());
    windowSubLayout[0] = new QHBoxLayout;
    windowSubLayout[0]->addWidget(windowDecodingLabel);
    windowSubLayout[0]->addWidget(windowDecodingSpinBox);
    connect(windowDecodingSpinBox, SIGNAL(editingFinished()), this, SLOT(on_decoding_window_size_changed()));

    QLabel *windowOverlapLabel = new QLabel(tr("Overlap window size (ms): "));
    windowOverlapSpinBox = new QSpinBox;
    windowOverlapSpinBox->setMaximumWidth(windowWidth);
    windowOverlapSpinBox->setMinimum(0);
    windowOverlapSpinBox->setMaximum(9999);
    windowOverlapSpinBox->setValue(commandCat->getOverlapWindowSize());
    windowSubLayout[1] = new QHBoxLayout;
    windowSubLayout[1]->addWidget(windowOverlapLabel);
    windowSubLayout[1]->addWidget(windowOverlapSpinBox);
    connect(windowOverlapSpinBox, SIGNAL(editingFinished()), this, SLOT(on_overlap_window_size_changed()));

    QLabel *windowSamplingFrequencyLabel = new QLabel(tr("Sampling frequency (Hz): "));
    windowSamplingFrequencySpinBox = new QSpinBox;
    windowSamplingFrequencySpinBox->setMaximumWidth(windowWidth);
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
    extendStimSpinBox->setMaximumWidth(windowWidth);
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
    highpassSpinBox->setMaximumWidth(windowWidth);
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
    lowpassSpinBox->setMaximumWidth(windowWidth);
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
    notchSpinBox->setMaximumWidth(windowWidth);
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
    recordingButton = new QPushButton(tr("Start Recording"));
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

QGroupBox *CatWindow::createStimPatternGroup(){
    groupStimPattern = new QGroupBox(tr("Stimulation Pattern"));

    QHBoxLayout *stimPatternSubLayout[3];

    int labelWidth = 200;

    // Label:
    stimPatternSubLayout[0] = new QHBoxLayout;
    QLabel *stimEmptyLabel = new QLabel(tr(" "));
    stimEmptyLabel->setMaximumWidth(labelWidth);
    stimPatternSubLayout[0]->addWidget(stimEmptyLabel);
    for(int i = 0; i < 4; i++){
        QLabel *channelLabels = new QLabel(tr("Channel ") + QString::number(i+1));
        stimPatternSubLayout[0]->addWidget(channelLabels);
    }

    // stim on and off:
    QVBoxLayout *onoffStimSubLayout[5];
    QLabel *stimOnLabel = new QLabel(tr("On: "));
    QLabel *stimOffLabel = new QLabel(tr("Off: "));
    for(int i = 0; i < 5; i++){
        onoffStimSubLayout[i] = new QVBoxLayout;
    }
    onoffStimSubLayout[0]->addWidget(stimOnLabel);
    onoffStimSubLayout[0]->addWidget(stimOffLabel);

    for(int i = 0; i < 2; i++){
        onoffStimBoxCh1[i] = new QRadioButton;
        onoffStimBoxCh2[i] = new QRadioButton;
        onoffStimBoxCh3[i] = new QRadioButton;
        onoffStimBoxCh4[i] = new QRadioButton;

        connect(onoffStimBoxCh1[i], SIGNAL(clicked(bool)), this, SLOT(on_stimulation_on_off_changed()));
        connect(onoffStimBoxCh2[i], SIGNAL(clicked(bool)), this, SLOT(on_stimulation_on_off_changed()));
        connect(onoffStimBoxCh3[i], SIGNAL(clicked(bool)), this, SLOT(on_stimulation_on_off_changed()));
        connect(onoffStimBoxCh4[i], SIGNAL(clicked(bool)), this, SLOT(on_stimulation_on_off_changed()));

        onoffStimSubLayout[1]->addWidget(onoffStimBoxCh1[i]);
        onoffStimSubLayout[2]->addWidget(onoffStimBoxCh2[i]);
        onoffStimSubLayout[3]->addWidget(onoffStimBoxCh3[i]);
        onoffStimSubLayout[4]->addWidget(onoffStimBoxCh4[i]);
    }

    onoffStimBoxCh1[0]->setChecked(true);
    onoffStimBoxCh2[0]->setChecked(true);
    onoffStimBoxCh3[1]->setChecked(true);
    onoffStimBoxCh4[1]->setChecked(true);

    QGroupBox *groupOnOffStimSubLayout[4];
    for(int i = 0; i < 4; i++){
        groupOnOffStimSubLayout[i] = new QGroupBox;
    }
    groupOnOffStimSubLayout[0]->setLayout(onoffStimSubLayout[1]);
    groupOnOffStimSubLayout[1]->setLayout(onoffStimSubLayout[2]);
    groupOnOffStimSubLayout[2]->setLayout(onoffStimSubLayout[3]);
    groupOnOffStimSubLayout[3]->setLayout(onoffStimSubLayout[4]);


    stimPatternSubLayout[1] = new QHBoxLayout;
    stimPatternSubLayout[1]->addLayout(onoffStimSubLayout[0]);
    for(int i = 0; i < 4; i++){
        stimPatternSubLayout[1]->addWidget(groupOnOffStimSubLayout[i]);
    }

    // stim target:
    stimPatternSubLayout[2] = new QHBoxLayout;
    QLabel *stimTargetLabel = new QLabel(tr("Stimulation: "));
    stimTargetLabel->setMaximumWidth(labelWidth);
    stimPatternSubLayout[2]->addWidget(stimTargetLabel);
    for(int i = 0; i < 4; i++){
        stimTargetCheckBox[i] = new QCheckBox;
        stimPatternSubLayout[2]->addWidget(stimTargetCheckBox[i]);
        connect(stimTargetCheckBox[i], SIGNAL(toggled(bool)), this, SLOT(on_stimulation_target_changed()));
    }

    //Layout
    QVBoxLayout *stimPatternLayout = new QVBoxLayout;
    for(int i = 0; i < 3; i++){
        stimPatternLayout->addLayout(stimPatternSubLayout[i]);
    }

    groupStimPattern->setLayout(stimPatternLayout);
    groupStimPattern->setEnabled(false);

    return groupStimPattern;
}

void CatWindow::createStatusBar(){
    statusBarLabel = new QLabel;
    statusBarMainWindow = statusBar();
    statusBarMainWindow->addPermanentWidget(statusBarLabel, 1);
    statusBarMainWindow->setSizeGripEnabled(false);  //fixed window size
}

void CatWindow::createActions(){
    odinAction = new QAction(tr("Od&in Control Panel"));
    odinAction->setShortcut(tr("Ctrl+I"));
    connect(odinAction, SIGNAL(triggered(bool)), this, SLOT(on_odin_triggered()));

    sylphAction = new QAction(tr("Sylph Control Panel"));
    sylphAction->setShortcut(tr("Ctrl+R"));
    connect(sylphAction, SIGNAL(triggered(bool)), this, SLOT(on_sylph_triggered()));

    fileMenu = menuBar()->addMenu(tr("G&UI"));
    fileMenu->addAction(odinAction);
    fileMenu->addAction(sylphAction);
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
        });
    delay += delayInterval;
}

void CatWindow::on_classify_methods_changed(){
    int temp = commandCat->getClassifyMethods();
    for(int i = 0; i < 2; i++){
        commandCat->setClassifyMethods(i, methodsClassifyBox[i]->isChecked());
    }

    if(methodsClassifyBox[1]->isChecked()){  // if feature is selected as classify method
        groupThreasholding->setEnabled(false);
    }
    else{
        groupThreasholding->setEnabled(true);
    }

    if(temp != commandCat->getClassifyMethods()){
        qDebug() << "Sent classify methods to: " << commandCat->getClassifyMethods();
        commandCat->sendClassifyMethods();
        emitCommandSent();
    }
}

void CatWindow::on_stimulation_pattern_changed(){
    int temp = commandCat->getStimulationPattern();
    for(int i = 0; i < 2; i++){
        commandCat->setStimulationPattern(i, methodsStimulationPatternBox[i]->isChecked());
    }

    if(methodsStimulationPatternBox[1]->isChecked()){  // if Target stimulation pattern is selected
        groupStimPattern->setEnabled(true);
    }
    else{
        groupStimPattern->setEnabled(false);
    }

    if(temp != commandCat->getStimulationPattern()){
        qDebug() << "Sent stimulation pattern to: " << commandCat->getStimulationPattern();
        commandCat->sendStimulationPattern();
        emitCommandSent();
    }
}

void CatWindow::on_stimulation_on_off_changed(){
    int temp = commandCat->getStimulationPatternOnOff();
    commandCat->setStimulationPatternOnOff(0, onoffStimBoxCh1[0]->isChecked());
    commandCat->setStimulationPatternOnOff(1, onoffStimBoxCh2[0]->isChecked());
    commandCat->setStimulationPatternOnOff(2, onoffStimBoxCh3[0]->isChecked());
    commandCat->setStimulationPatternOnOff(3, onoffStimBoxCh4[0]->isChecked());

    if(temp != commandCat->getStimulationPatternOnOff()){
        qDebug() << "Sent stimulation pattern on off to: " << commandCat->getStimulationPatternOnOff();
        commandCat->sendStimulationPatternOnOff();
        emitCommandSent();
    }
}

void CatWindow::on_stimulation_target_changed(){
    for(int i = 0; i < 4; i++){
        commandCat->setStimulationPatternTarget(i, stimTargetCheckBox[i]->isChecked());
    }
    qDebug() << "Sent stimulation pattern target to: " << commandCat->getStimulationPatternTarget();
    commandCat->sendStimulationPatternTarget();
    emitCommandSent();
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
        controlInput(false);

        filenameDialog->setFilename(QDateTime::currentDateTime().toString("'data_'yyyyMMdd_HHmmss'"));
    }
    else{
        recordingFlag = false;
        commandCat->setRecording(recordingFlag);
        recordingButton->setText("Start Recor&ding");
        statusBarLabel->setText(tr("<b><FONT COLOR='#ff0000'> Recording stopped!!!"));
        controlInput(true);

        filenameDialog->show();
    }
    commandCat->sendRecording();
    emitCommandSent();
    qDebug() << "Sent recording to : " << commandCat->getRecording();
}

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
    if(!startStimulationFlag){
        startStimulationFlag = true;
        commandCat->setStartStimulation(startStimulationFlag);
        startButton->setText("Stop Integration");
    }
    else{
        startStimulationFlag = false;
        commandCat->setStartStimulation(startStimulationFlag);
        startButton->setText("Start Integration");
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
    statusBarLabel->setText(transferStatus);
    qDebug() << commandStdout;
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
}

void CatWindow::on_filename_discard(){
    filenameSocket->doConnect("192.168.4.3", 7777);
    commandCat->sendFilename(filenameDiscard);
    filenameSocket->write(commandCat->getFilenameCommand());
    filenameSocket->doDisconnect();

    filenameDialog->hide();
    statusBarLabel->setText(tr("<b><FONT COLOR='#ff0000'> Recording stopped!!! File DISCARDED!!!"));
}

CatWindow::~CatWindow(){
//    filenameSocket->doDisconnect();
}
}
