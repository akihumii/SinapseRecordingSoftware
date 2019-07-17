#ifndef RECORDINGDIALOG_H
#define RECORDINGDIALOG_H

#include <QMainWindow>
#include "../common/qtincludes.h"

namespace SylphX{

class RecordingDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit RecordingDialog();
    ~RecordingDialog();

private:
    QGroupBox *createParametersGroup();
    QGroupBox *createRecordingGroup();

    QPushButton *recordingButton;
    QPushButton *recordingTransferButton;
    QProcess receivingSavedFiles;
    QString commandStdout;
    QString transferStatus;
    QStringList commandArg;

    QSpinBox *windowSamplingFrequencySpinBox;
    QSpinBox *highpassSpinBox;
    QSpinBox *lowpassSpinBox;
    QSpinBox *notchSpinBox;

    QCheckBox *highpassCheckBox;
    QCheckBox *lowpassCheckBox;
    QCheckBox *notchCheckBox;
    bool highpassFlag = false;
    bool lowpassFlag = false;
    bool notchFlag = false;

    void createLayout();
//    void receiveSavedFiles();

private slots:
//    void on_recording_changed();
//    void on_recording_transfer_changed();
//    void on_sampling_freq_changed();
//    void on_extend_stimulation_changed();
//    void on_highpass_cutoff_freq_changed();
//    void on_highpass_cutoff_freq_checkbox_changed();
//    void on_lowpass_cutoff_freq_changed();
//    void on_lowpass_cutoff_freq_checkbox_changed();
//    void on_notch_cutoff_freq_changed();
//    void on_notch_cutoff_freq_checkbox_changed();
//    void sendHighpassParameters();
//    void sendLowpassParameters();
//    void sendNotchParameters();
//    void sendFilteringParameters();
//    void sendHighpassSignal(double highpassValue);
//    void sendLowpassSignal(double lowpassValue);
//    void sendNotchSignal(double notchValue);
//    void readOutput();


signals:
    void highpassSent(QVector<double> *values);
    void lowpassSent(QVector<double> *values);
    void notchSent(QVector<double> *values);
};

}

#endif // RECORDINGDIALOG_H
