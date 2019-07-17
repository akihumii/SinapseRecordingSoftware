#include "recordingdialog.h"

namespace SylphX {
RecordingDialog::RecordingDialog(){
    QString version(APP_VERSION);
    setWindowTitle(tr("SylphX Parameters GUI V") + version);
    createLayout();
}

void RecordingDialog::createLayout(){
    QWidget *mainWidget = new QWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(createParametersGroup());
    mainLayout->addWidget(createRecordingGroup());
//    mainLayout->setSizeConstraint( QLayout::SetFixedSize );
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
}

QGroupBox *RecordingDialog::createParametersGroup(){
    QGroupBox *parametersGroup = new QGroupBox(tr("Parameters"));
    QHBoxLayout *parametersSubLayout[4];
        int windowWidth = 100;

        //sampling frequency
        QLabel *windowSamplingFrequencyLabel = new QLabel(tr("Sampling frequency (Hz): "));
        windowSamplingFrequencySpinBox = new QSpinBox;
        windowSamplingFrequencySpinBox->setMinimumWidth(windowWidth);
        windowSamplingFrequencySpinBox->setMinimum(0);
        windowSamplingFrequencySpinBox->setMaximum(30000);
//        windowSamplingFrequencySpinBox->setValue(commandCat->getSamplingFreq());
        windowSamplingFrequencySpinBox->setMinimumWidth(windowWidth);
        parametersSubLayout[0] = new QHBoxLayout;
        parametersSubLayout[0]->addWidget(windowSamplingFrequencyLabel);
        parametersSubLayout[0]->addWidget(windowSamplingFrequencySpinBox);
    //    connect(windowSamplingFrequencySpinBox, SIGNAL(editingFinished()), this, SLOT(on_sampling_freq_changed()));

    //    //filtering
    //    QHBoxLayout *parametersSubLayout[3];
        QLabel *highpassLabel = new QLabel(tr("Highpass cutoff freq.: "));
        highpassCheckBox = new QCheckBox;
        highpassCheckBox->setChecked(highpassFlag);
        highpassSpinBox = new QSpinBox;
        highpassSpinBox->setMinimumWidth(windowWidth);
        highpassSpinBox->setMinimum(0);
        highpassSpinBox->setMaximum(windowSamplingFrequencySpinBox->text().toInt()/2 - 1);
//        highpassSpinBox->setValue(commandCat->getHighpassCutoffFreq());
        highpassSpinBox->setEnabled(highpassFlag);
        parametersSubLayout[1] = new QHBoxLayout;
        parametersSubLayout[1]->addWidget(highpassCheckBox);
        parametersSubLayout[1]->addWidget(highpassLabel);
        parametersSubLayout[1]->addWidget(highpassSpinBox);
    //    connect(highpassSpinBox, SIGNAL(editingFinished()), this, SLOT(on_highpass_cutoff_freq_changed()));
    //    connect(highpassCheckBox, SIGNAL(toggled(bool)), this, SLOT(on_highpass_cutoff_freq_checkbox_changed()));

        QLabel *lowpassLabel = new QLabel(tr("Lowpass cutoff freq.: "));
        lowpassCheckBox = new QCheckBox;
        lowpassCheckBox->setChecked(lowpassFlag);
        lowpassSpinBox = new QSpinBox;
        lowpassSpinBox->setMinimumWidth(windowWidth);
        lowpassSpinBox->setMinimum(highpassSpinBox->text().toInt() + 1);
        lowpassSpinBox->setMaximum(windowSamplingFrequencySpinBox->text().toInt()/2 - 1);
//        commandCat->getLowpassCutoffFreq() ? lowpassSpinBox->setValue(commandCat->getLowpassCutoffFreq()) :
//                                             lowpassSpinBox->setValue(windowSamplingFrequencySpinBox->text().toInt()/2 - 1);
        lowpassSpinBox->setEnabled(lowpassFlag);
        parametersSubLayout[2] = new QHBoxLayout;
        parametersSubLayout[2]->addWidget(lowpassCheckBox);
        parametersSubLayout[2]->addWidget(lowpassLabel);
        parametersSubLayout[2]->addWidget(lowpassSpinBox);
    //    connect(lowpassSpinBox, SIGNAL(editingFinished()), this, SLOT(on_lowpass_cutoff_freq_changed()));
    //    connect(lowpassCheckBox, SIGNAL(toggled(bool)), this, SLOT(on_lowpass_cutoff_freq_checkbox_changed()));

        QLabel *notchLabel = new QLabel(tr("Notch cutoff freq.: "));
        notchCheckBox = new QCheckBox;
        notchCheckBox->setChecked(notchFlag);
        notchSpinBox = new QSpinBox;
        notchSpinBox->setMinimumWidth(windowWidth);
        notchSpinBox->setMinimum(0);
        notchSpinBox->setMaximum(windowSamplingFrequencySpinBox->text().toInt()-1);
//        notchSpinBox->setValue(commandCat->getNotchCutoffFreq());
        notchSpinBox->setEnabled(notchFlag);
        parametersSubLayout[3] = new QHBoxLayout;
        parametersSubLayout[3]->addWidget(notchCheckBox);
        parametersSubLayout[3]->addWidget(notchLabel);
        parametersSubLayout[3]->addWidget(notchSpinBox);
    //    connect(notchSpinBox, SIGNAL(editingFinished()), this, SLOT(on_notch_cutoff_freq_changed()));
    //    connect(notchCheckBox, SIGNAL(toggled(bool)), this, SLOT(on_notch_cutoff_freq_checkbox_changed()));

        //layout
        QVBoxLayout *parametersLayout = new QVBoxLayout();
        for(int i = 0; i < 4; i++){
            parametersLayout->addLayout(parametersSubLayout[i]);
        }
        parametersGroup->setLayout(parametersLayout);

    return parametersGroup;
}

QGroupBox *RecordingDialog::createRecordingGroup(){
    QGroupBox *groupRecording = new QGroupBox(tr("Recording"));

    //buttons
    recordingButton = new QPushButton(tr("Start Recording"));
//    connect(recordingButton, SIGNAL(clicked(bool)), this, SLOT(on_recording_changed()));

    recordingTransferButton = new QPushButton(tr("Transfer Recordings"));
//    connect(recordingTransferButton, SIGNAL(clicked(bool)), this, SLOT(on_recording_transfer_changed()));

    //connection
//    receiveSavedFiles();

    //Layout
    QHBoxLayout *recordingLayout = new QHBoxLayout;
    recordingLayout->addWidget(recordingButton);
    recordingLayout->addWidget(recordingTransferButton);

    groupRecording->setLayout(recordingLayout);

    return groupRecording;
}

//void CatWindow::on_recording_changed(){
//    if(!recordingFlag){
//        recordingFlag = true;
//        commandCat->setRecording(recordingFlag);
//        recordingButton->setText("Stop Recor&ding");
//        statusBarLabel->setText(tr("<b><FONT COLOR='#ff0000' FONT SIZE = 4> Recording...</b>"));
////        controlInput(false);

//        filenameDialog->setFilename(QDateTime::currentDateTime().toString("'data_'yyyyMMdd_HHmmss'"));
//    }
//    else{
//        recordingFlag = false;
//        commandCat->setRecording(recordingFlag);
//        recordingButton->setText("Start Recor&ding");
//        statusBarLabel->setText(tr("<b><FONT COLOR='#ff0000'> Recording stopped!!!"));
////        controlInput(true);

//        filenameDialog->show();
//    }
//    commandCat->sendRecording();
//    emitCommandSent();
//    qDebug() << "Sent recording to : " << commandCat->getRecording();
//}

//void CatWindow::on_recording_transfer_changed(){
//    recordingTransferButton->setEnabled(false);
//    QString savingDirStr = "C:/Data";
//    QDir savingDir(savingDirStr);
//    if(!savingDir.exists()){  // check if saving folder exists; if not, create it
//        savingDir.mkpath(".");
//    }
//    QString command = "pscp";
//    commandArg.clear();
//    commandArg.append("-pw");
//    commandArg.append("raspberry");
//    commandArg.append("-scp");
//    commandArg.append("-unsafe");
//    commandArg.append("pi@192.168.4.3:/home/pi/Data/*.csv");
//    commandArg.append(savingDirStr);
//    receivingSavedFiles.start(command, commandArg, QIODevice::ReadWrite);
//}

//void CatWindow::receiveSavedFiles(){
//    connect(&receivingSavedFiles, SIGNAL(readyReadStandardError()), this, SLOT(readOutput()));
//    connect(&receivingSavedFiles, SIGNAL(readyReadStandardOutput()), this, SLOT(readOutput()));
//    connect(&receivingSavedFiles, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
//            [=] (int /*exitCode*/, QProcess::ExitStatus exitStatus){
//        if(!exitStatus){
//            statusBarLabel->setText(tr("Finished transferring..."));
//            qDebug() << "Finished transferring...";
//            commandCat->sendRecordingTransfer();
//            emitCommandSent();
//            qDebug() << "Sent recording transfer...";
//        }
//        else{
//            statusBarLabel->setText(tr("Transferring failed..."));
//            qDebug() << "Transferring failed...";
//        }
//        recordingTransferButton->setEnabled(true);
//    });
//}

//void CatWindow::readOutput(){
//    commandStdout.clear();
//    commandStdout.append(receivingSavedFiles.readAllStandardOutput());
//    transferStatus.clear();
//    transferStatus.append(commandStdout);
//    statusBarLabel->setText(transferStatus);
//    qDebug() << commandStdout;
//}

//void RecordingDialog::on_sampling_freq_changed(){
//    if(windowSamplingFrequencySpinBox->text().toInt() != commandCat->getSamplingFreq()){
//        highpassSpinBox->setMaximum(lowpassSpinBox->text().toInt() - 1);
//        lowpassSpinBox->setMinimum(highpassSpinBox->text().toInt() + 1);
//        lowpassSpinBox->setMaximum(windowSamplingFrequencySpinBox->text().toInt()/2 - 1);
//        notchSpinBox->setMaximum(windowSamplingFrequencySpinBox->text().toInt()-1);

//        sendFilteringParameters();

//        QTimer::singleShot(150, [=] {
//            commandCat->setSamplingFreq(windowSamplingFrequencySpinBox->text().toInt());
//            qDebug() << "Sent decoding window size to : " << windowSamplingFrequencySpinBox->text().toInt();
//            commandCat->sendSamplingFreq();
//            emitCommandSent();
//        });

//    }
//}

//void RecordingDialog::on_highpass_cutoff_freq_changed(){
//    if(highpassSpinBox->text().toInt() != commandCat->getHighpassCutoffFreq()){
//        sendHighpassParameters(highpassSpinBox->text().toInt());
//        sendHighpassSignal(highpassSpinBox->text().toInt());
//    }
//}

//void RecordingDialog::on_highpass_cutoff_freq_checkbox_changed(){
//    if(highpassCheckBox->isChecked()){
//        highpassSpinBox->setEnabled(true);
//        sendFilteringParameters();
//        highpassFlag = true;
//    }
//    else{
//        highpassSpinBox->setEnabled(false);
//        sendHighpassParameters(0);
//        sendHighpassSignal(0);
//        highpassFlag = false;
//    }
//}

//void RecordingDialog::on_lowpass_cutoff_freq_changed(){
//    if(lowpassSpinBox->text().toInt() != commandCat->getLowpassCutoffFreq()){
//        sendLowpassParameters(lowpassSpinBox->text().toInt());
//        sendLowpassSignal(lowpassSpinBox->text().toInt());
//    }
//}

//void RecordingDialog::on_lowpass_cutoff_freq_checkbox_changed(){
//    if(lowpassCheckBox->isChecked()){
//        lowpassSpinBox->setEnabled(true);
//        sendFilteringParameters();
//        lowpassFlag = true;
//    }
//    else{
//        lowpassSpinBox->setEnabled(false);
//        sendLowpassParameters(0);
//        sendLowpassSignal(0);
//        lowpassFlag = false;
//    }
//}

//void RecordingDialog::on_notch_cutoff_freq_changed(){
//    if(notchSpinBox->text().toInt() != commandCat->getNotchCutoffFreq()){
//        sendNotchParameters(notchSpinBox->text().toInt());
//        sendNotchSignal(notchSpinBox->text().toInt());
//    }
//}

//void RecordingDialog::on_notch_cutoff_freq_checkbox_changed(){
//    if(notchCheckBox->isChecked()){
//        notchSpinBox->setEnabled(true);
//        sendFilteringParameters();
//        notchFlag = true;
//    }
//    else{
//        notchSpinBox->setEnabled(false);
//        sendNotchParameters(0);
//        sendNotchSignal(0);
//        notchFlag = false;
//    }
//}

//void RecordingDialog::sendHighpassParameters(int value){
//    commandCat->setHighpassCutoffFreq(value);
//    qDebug() << "Sent highpass cutoff freq to : " << value;
//    commandCat->sendHighpassCutoffFreq();
//    emitCommandSent();
//}

//void RecordingDialog::sendLowpassParameters(int value){
//    commandCat->setLowpassCutoffFreq(value);
//    qDebug() << "Sent lowpass cutoff freq to : " << value;
//    commandCat->sendLowpassCutoffFreq();
//    emitCommandSent();
//}

//void RecordingDialog::sendNotchParameters(int value){
//    commandCat->setNotchCutoffFreq(value);
//    qDebug() << "Sent notch cutoff freq to : " << value;
//    commandCat->sendNotchCutoffFreq();
//    emitCommandSent();

//}

//void RecordingDialog::sendFilteringParameters(){
//    highpassSpinBox->setMaximum(lowpassSpinBox->text().toInt() - 1);
//    lowpassSpinBox->setMinimum(highpassSpinBox->text().toInt() + 1);
//    lowpassSpinBox->setMaximum(windowSamplingFrequencySpinBox->text().toInt()/2 - 1);
//    notchSpinBox->setMaximum(windowSamplingFrequencySpinBox->text().toInt()-1);

//    QTimer::singleShot(0, [=] {
//        int highpassValue;
//        highpassCheckBox->isChecked()? highpassValue = highpassSpinBox->text().toInt() : highpassValue = 0;
//        sendHighpassParameters(highpassValue);
//        sendHighpassSignal(highpassValue);
//    });
//    QTimer::singleShot(150, [=] {
//        int lowpassValue;
//        lowpassCheckBox->isChecked()? lowpassValue = lowpassSpinBox->text().toInt() : lowpassValue = 0;
//        sendLowpassParameters(lowpassValue);
//        sendLowpassSignal(lowpassValue);
//    });
//    QTimer::singleShot(300, [=] {
//        int notchValue;
//        notchCheckBox->isChecked()? notchValue = notchSpinBox->text().toInt() : notchValue = 0;
//        sendNotchParameters(notchValue);
//        sendNotchSignal(notchValue);
//    });
//}

//void CatWindow::sendHighpassSignal(double highpassValue){
//    highpassValueSets->clear();
//    highpassValueSets->append((double) highpassValue);
//    highpassValueSets->append((double) windowSamplingFrequencySpinBox->text().toDouble());
//    highpassValueSets->append((double) highpassCheckBox->isChecked());
//    emit highpassSent(highpassValueSets);
//}

//void CatWindow::sendLowpassSignal(double lowpassValue){
//    lowpassValueSets->clear();
//    lowpassValueSets->append((double) lowpassValue);
//    lowpassValueSets->append((double) windowSamplingFrequencySpinBox->text().toDouble());
//    lowpassValueSets->append((double) lowpassCheckBox->isChecked());
//    emit lowpassSent(lowpassValueSets);
//}

//void CatWindow::sendNotchSignal(double notchValue){
//    notchValueSets->clear();
//    notchValueSets->append((double) notchValue);
//    notchValueSets->append((double) windowSamplingFrequencySpinBox->text().toDouble());
//    notchValueSets->append((double) notchCheckBox->isChecked());
//    emit notchSent(notchValueSets);
//}


RecordingDialog::~RecordingDialog(){
}

}
