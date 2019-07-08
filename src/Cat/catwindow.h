#ifndef CATWINDOW_H
#define CATWINDOW_H

#include <QMainWindow>
#include "../common/qtincludes.h"
#include "commandcat.h"
#include "socketfilename.h"
#include "filenamedialog.h"
#include <QtMath>

namespace Cat {

class CatWindow : public QMainWindow
{
    Q_OBJECT

public:
    CatWindow();
    ~CatWindow();

    QLabel *statusBarLabel;
public slots:
    void setRpiCommand(char *data);

private:
    CommandCat *commandCat;
    SocketFilename *filenameSocket;
    FilenameDialog *filenameDialog;

    QStatusBar *statusBarMainWindow;
    QMenu *fileMenu;
    QAction *odinAction;
    QAction *sylphAction;

    QGroupBox *createMethodsGroup();
    QGroupBox *createThreasholdingGroup();
    QGroupBox *createStimPatternGroup();
    QGroupBox *createSettingsGroup();
    QGroupBox *createTrainingGroup();
    QGroupBox *createParametersGroup();
    QGroupBox *createMethodsClassifyGroup();
    QHBoxLayout *createStartButton();
    QGroupBox *createRecordingGroup();

    QGroupBox *parametersGroup;
    QGroupBox *groupSettings;
    QGroupBox *groupThreasholding;
    QGroupBox *groupStimPattern;

    QLabel *trainingSaveDir;
    QHBoxLayout *settingsLabelSubLayout[2];
    QVBoxLayout *settingsInputLayout;
    QHBoxLayout *settingsInputSubLayout[30];
    QVBoxLayout *settingsOutputLayout;
    QHBoxLayout *settingsOutputSubLayout[30];
    QHBoxLayout *IOLayout;
    QGroupBox *settingsInputGroup;
    QGroupBox *settingsOutputGroup;

    QRadioButton *methodsSMChannelBox[2];
    QRadioButton *methodsClassifyBox[2];
    QRadioButton *methodsStimulationPatternBox[2];
    QRadioButton *onoffStimBoxCh1[2];
    QRadioButton *onoffStimBoxCh2[2];
    QRadioButton *onoffStimBoxCh3[2];
    QRadioButton *onoffStimBoxCh4[2];

    QSpinBox *thresholdingSpinBox[4];
    QSpinBox *thresholdingPowerSpinBox[4];
    QSpinBox *numClassSpinBox;
    QSpinBox *windowDecodingSpinBox;
    QSpinBox *windowOverlapSpinBox;
    QSpinBox *windowSamplingFrequencySpinBox;
    QSpinBox *highpassSpinBox;
    QSpinBox *lowpassSpinBox;
    QSpinBox *notchSpinBox;
    QSpinBox *extendStimSpinBox;

    QCheckBox *stimTargetCheckBox[4];
    QCheckBox *highpassCheckBox;
    QCheckBox *lowpassCheckBox;
    QCheckBox *notchCheckBox;
    QCheckBox *inputBoxCh1[30];
    QCheckBox *inputBoxCh2[30];
    QCheckBox *inputBoxCh3[30];
    QCheckBox *inputBoxCh4[30];
    QCheckBox *outputBoxCh1[30];
    QCheckBox *outputBoxCh2[30];
    QCheckBox *outputBoxCh3[30];
    QCheckBox *outputBoxCh4[30];

    QPushButton *trainingStart;
    QPushButton *trainingRedo;
    QPushButton *trainingNext;
    QPushButton *trainingCancel;
    QPushButton *trainingSave;
    QPushButton *startButton;
    QPushButton *recordingButton;
    QPushButton *recordingTransferButton;
    QPushButton *addSettingsButton;
    QPushButton *removeSettingsButton[30];

    QVector<double> *highpassValueSets = new QVector<double>;
    QVector<double> *lowpassValueSets = new QVector<double>;
    QVector<double> *notchValueSets = new QVector<double>;

    QProcess receivingSavedFiles;
    QString transferStatus;
    QStringList commandArg;
    QString commandStdout;

    QString filename;
    QString filenameDiscard = "DISCARDFILE!!!";

    int startDelay = 3000;
    int indexThreshold = 2;
    QVector<int> *inputCheckBoxValue;
    QVector<int> *outputCheckBoxValue;
    int boxWidth = 18;
    int boxHeight = 18;

    void createLayout();
    void createStatusBar();
    void createActions();
    void createChannelLabel();
    void createSettingsLayout(int index);

    void sendParameters();
    void sendHighpassParameters(int value);
    void sendLowpassParameters(int value);
    void sendNotchParameters(int value);
    void sendFilteringParameters();

    void sendHighpassSignal(double highpassValue);
    void sendLowpassSignal(double lowpassValue);
    void sendNotchSignal(double notchValue);

    void emitCommandSent();
    void receiveSavedFiles();
    void controlInput(bool flag);

    char *lastSentCommand = new char[3];
    bool startStimulationFlag = false;
    bool recordingFlag = false;

private slots:
    void on_sm_channel_changed();
    void on_threshold_changed();
    void on_threshold_power_changed();
    void on_decoding_window_size_changed();
    void on_overlap_window_size_changed();
    void on_sampling_freq_changed();
    void on_extend_stimulation_changed();
    void on_highpass_cutoff_freq_changed();
    void on_highpass_cutoff_freq_checkbox_changed();
    void on_lowpass_cutoff_freq_changed();
    void on_lowpass_cutoff_freq_checkbox_changed();
    void on_notch_cutoff_freq_changed();
    void on_notch_cutoff_freq_checkbox_changed();
    void on_odin_triggered();
    void on_sylph_triggered();
    void on_start_changed();
    void on_recording_changed();
    void on_recording_transfer_changed();
    void on_classify_methods_changed();
    void on_stimulation_pattern_changed();
    void on_stimulation_on_off_changed();
    void on_stimulation_target_changed();
    void on_filename_changed();
    void on_filename_discard();
    void readOutput();

signals:
    void commandSent(char *bytes);
    void highpassSent(QVector<double> *values);
    void lowpassSent(QVector<double> *values);
    void notchSent(QVector<double> *values);
    void showOdinSignal();
    void showSylphSignal();
};

}

#endif // CATWINDOW_H

