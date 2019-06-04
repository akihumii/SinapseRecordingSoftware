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

private:
    QGroupBox *createMethodsGroup();
    QVBoxLayout *createMethodsBoxLayout();
    QGroupBox *createThreasholdingGroup();
    QGroupBox *createSettingsGroup();
    QRadioButton *methodsSMChannelBox[2];
    QRadioButton *methodsClassifyBox[2];
    QSpinBox *thresholdingSpinBox[4];
    QSpinBox *thresholdingPowerSpinBox[4];

    void createLayout();

};

}

#endif // CATWINDOW_H

