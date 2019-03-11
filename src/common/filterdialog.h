#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include "qtincludes.h"
#include "data.h"

namespace Ui {
class FilterDialog;
}

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    FilterDialog(Data *_dataProcessor);
    ~FilterDialog();

private:
    void createLayout();
    void loadPrevSetting();

    Data *dataProcessor;

    QGroupBox *channel;

    QVBoxLayout *channelLayout;
    QVBoxLayout *highpassLayout;

    QHBoxLayout *lownhighLayout;
    QHBoxLayout *notchLayout;
    QCheckBox *filterEnable[2];

    QLabel *filterLabel;
    QLabel *cutoffLabel[2];

    QLineEdit *cutoffFreq[2];

    QComboBox *notchFilter;

    int sylphSampleFreq = 1000;
    int neutrinoSampleFreq = 17850;
    int samplingFreq = neutrinoSampleFreq;

private slots:
    void on_highpassEnable_toggled(bool);
    void on_notchFilter_changed(int Index);
    void on_highpassFreq_changed();
};

#endif // FILTERDIALOG_H
