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

    QCheckBox *dataEnable[10];
    QPushButton *processSingleButton;
    QPushButton *processDiffButton;
    QPushButton *processThresholdButton;

    QHBoxLayout *minusLayout[5];
    QCheckBox *diffEnable[5];
    QComboBox *A[5];
    QComboBox *B[5];
    QLabel *minus[5];

    QComboBox *channelSelect;
    QComboBox *lessmore;
    QLineEdit *thresholdLineEdit;

    QLineEdit *beforeLineEdit;
    QLineEdit *afterLineEdit;
    QComboBox *maxSpikesCombo;

    QVector<double> channelData[10];

    QTabWidget *modeTabWidget;
    QWidget *singleEndMode;
    QWidget *differentialMode;
    QWidget *thresholdMode;

    bool dataSelected[10];
    bool firstLoad[10] = {true, true, true, true, true, true, true, true, true, true};

    int numChannels = 0;
    int total_data_points = 0;

private slots:
    void on_dataSelected();
    void on_startSingleEndProcessing();
    void on_startDiffProcessing();
    void on_startThresholdProcessing();
    void on_diffEnable_changed();
};

#endif // MAINWINDOW_H
