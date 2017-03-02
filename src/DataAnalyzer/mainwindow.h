#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qtincludes.h"
#include "graphdialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString filename);
    ~MainWindow();

private:
    void createLayout();
    void createTabs();

    QLabel *syncPulseLabel[3];
    QLineEdit *syncPulsePara[2];
    QComboBox *syncPulseResolution;
    QPushButton *addFileButton;

    QCheckBox *dataEnable[12];
    QPushButton *processSingleButton;
    QPushButton *processDiffButton;
    QPushButton *processThresholdButton;

    QHBoxLayout *minusLayout[6];
    QCheckBox *diffEnable[6];
    QComboBox *A[6];
    QComboBox *B[6];
    QLabel *minus[6];

    QComboBox *channelSelect;
    QComboBox *lessmore;
    QLineEdit *thresholdLineEdit;

    QLineEdit *beforeLineEdit;
    QLineEdit *afterLineEdit;
    QComboBox *maxSpikesCombo;

    QVector<double> channelData[12];

    QTabWidget *modeTabWidget;
    QWidget *addFileMode;
    QWidget *singleEndMode;
    QWidget *differentialMode;
    QWidget *thresholdMode;

    bool dataSelected[12] = {false, false, false, false, false, false, false, false, false, false, false, false};
    bool firstLoad[12] = {true, true, true, true, true, true, true, true, true, true, true, true};

    int ADCnumChannels = 0;
    int numChannels = 0;
    quint64 total_data_points = 0;
    quint64 total_ADC_points = 0;
    double ADC_ySteps;
    double ADC_xSteps;
    double vpp;

private slots:
    void on_dataSelected();
    void on_startSingleEndProcessing();
    void on_startDiffProcessing();
    void on_startThresholdProcessing();
    void on_diffEnable_changed();
    void on_openSyncPulse();
};

#endif // MAINWINDOW_H
