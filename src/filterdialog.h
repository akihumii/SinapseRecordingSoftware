#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include "qtincludes.h"
#include "dataprocessor.h"

namespace Ui {
class FilterDialog;
}

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    FilterDialog(DataProcessor *_dataProcessor);
    ~FilterDialog();

private:
    void createLayout();
    void loadPrevSetting();

    DataProcessor *dataProcessor;

    QGroupBox *channel;

    QVBoxLayout *channelLayout;

//    QVBoxLayout *lowpassLayout;
    QVBoxLayout *highpassLayout;

    QHBoxLayout *lownhighLayout;
    QHBoxLayout *notchLayout;

//    QCheckBox *channelFilterEnable;
    QCheckBox *filterEnable[2];

    QLabel *filterLabel;
    QLabel *cutoffLabel[2];

    QLineEdit *cutoffFreq[2];

    QComboBox *notchFilter;

    int sylphSampleFreq = 20000;
    int neutrinoSampleFreq = 17000;

#ifdef SYLPH
    int samplingFreq = sylphSampleFreq;
#endif
#ifdef NEUTRINO_II
    int samplingFreq = neutrinoSampleFreq;
#endif

private slots:
//    void on_channelFilter_toggled();
    void on_highpassEnable_toggled(bool);
    void on_notchFilter_changed(int Index);
    void on_highpassFreq_changed();
//    void on_lowpassEnable_toggled(bool enableFlag);
//    void on_lowpassFreq_changed();

};

#endif // FILTERDIALOG_H
