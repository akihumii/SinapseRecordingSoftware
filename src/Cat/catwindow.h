#ifndef CATWINDOW_H
#define CATWINDOW_H

#include <QMainWindow>
#include "../common/qtincludes.h"
#include "commandcat.h"
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

    QStatusBar *statusBarMainWindow;
    QMenu *fileMenu;
    QAction *odinAction;
    QAction *sylphAction;

    QGroupBox *createMethodsGroup();
    QGroupBox *createThreasholdingGroup();
    QGroupBox *createSettingsGroup();
    QGroupBox *createTrainingGroup();
    QGroupBox *createParametersGroup();
    QHBoxLayout *createStartButton();

    QLabel *trainingSaveDir;

    QRadioButton *methodsSMChannelBox[2];
    QRadioButton *methodsClassifyBox[2];

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

    QCheckBox *highpassCheckBox;
    QCheckBox *lowpassCheckBox;
    QCheckBox *notchCheckBox;

    QPushButton *trainingStart;
    QPushButton *trainingRedo;
    QPushButton *trainingNext;
    QPushButton *trainingCancel;
    QPushButton *trainingSave;
    QPushButton *startButton;

    QVector<double> *highpassValueSets = new QVector<double>;
    QVector<double> *lowpassValueSets = new QVector<double>;
    QVector<double> *notchValueSets = new QVector<double>;

    int startDelay = 3000;

    void createLayout();
    void createStatusBar();
    void createActions();

    void sendParameters();
    void sendHighpassParameters(int value);
    void sendLowpassParameters(int value);
    void sendNotchParameters(int value);
    void sendFilteringParameters();

    void sendHighpassSignal(double highpassValue);
    void sendLowpassSignal(double lowpassValue);
    void sendNotchSignal(double notchValue);

    char *lastSentCommand = new char[3];
    bool startStimulationFlag = false;

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

