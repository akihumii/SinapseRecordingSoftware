#ifndef THRESHOLDTAB_H
#define THRESHOLDTAB_H

#include "../common/qtincludes.h"
#include "catwindow.h"

class ThresholdTab : public QWidget
{
    Q_OBJECT

public:
    ThresholdTab();

private:
    void createLayout();

    QHBoxLayout *settingsLabelSubLayout[2];

    QPushButton *doneSettingsButton;
    QPushButton *addSettingsButton;
    QPushButton *removeSettingsButton[30];
    bool doneSettingsFlag = false;

    QVBoxLayout *settingsInputLayout;
    QHBoxLayout *settingsInputSubLayout[30];
    QVBoxLayout *settingsOutputLayout;
    QHBoxLayout *settingsOutputSubLayout[30];
    QHBoxLayout *IOLayout;
    QGroupBox *settingsInputGroup;
    QGroupBox *settingsOutputGroup;

    QCheckBox *inputBoxCh1[30];
    QCheckBox *inputBoxCh2[30];
    QCheckBox *inputBoxCh3[30];
    QCheckBox *inputBoxCh4[30];
    QCheckBox *outputBoxCh1[30];
    QCheckBox *outputBoxCh2[30];
    QCheckBox *outputBoxCh3[30];
    QCheckBox *outputBoxCh4[30];
    bool check_input_boxes();
    int repeatedLocs[2] = {0};

    QSignalMapper *removeMapper;
    QSignalMapper *inputMapperCh1;
    QSignalMapper *inputMapperCh2;
    QSignalMapper *inputMapperCh3;
    QSignalMapper *inputMapperCh4;
    QSignalMapper *outputMapperCh1;
    QSignalMapper *outputMapperCh2;
    QSignalMapper *outputMapperCh3;
    QSignalMapper *outputMapperCh4;

    int indexThreshold = 2;
    int inputCheckBoxValue[30] = {0};
    int outputCheckBoxValue[30] = {0};
    int boxWidth = 22;
    int boxHeight = 22;

    void createSettingsLayout(int index);

private slots:
    void on_input_ch1_changed(int index);
    void on_input_ch2_changed(int index);
    void on_input_ch3_changed(int index);
    void on_input_ch4_changed(int index);
    void on_output_ch1_changed(int index);
    void on_output_ch2_changed(int index);
    void on_output_ch3_changed(int index);
    void on_output_ch4_changed(int index);
    void on_add_checkbox_clicked();
    void on_remove_checkbox_clicked(int index);
    void on_done_settings_changed();

signals:
    void updateStatusBarSignal(QString text);
    void refreshLayoutSignal();
};


#endif // THRESHOLDTAB_H
