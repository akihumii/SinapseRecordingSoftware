#ifndef CATWINDOW_H
#define CATWINDOW_H

#include <QMainWindow>
#include "../common/qtincludes.h"
#include <QtMath>

namespace Cat {

class CatWindow : public QMainWindow
{
    Q_OBJECT

public:
    CatWindow();
    ~CatWindow();

    QLabel *statusBarLabel;
private:
    QStatusBar *statusBarMainWindow;

    QGroupBox *createMethodsGroup();
    QGroupBox *createThreasholdingGroup();
    QGroupBox *createSettingsGroup();
    QGroupBox *createTrainingGroup();
    QGroupBox *createParametersGroup();

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

    QPushButton *trainingStart;
    QPushButton *trainingRedo;
    QPushButton *trainingNext;
    QPushButton *trainingCancel;
    QPushButton *trainingSave;

    void createLayout();
    void createStatusBar();

};

}

#endif // CATWINDOW_H

