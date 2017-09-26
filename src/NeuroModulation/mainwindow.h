#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "command.h"
#include "serialshuhao.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
private:
    Command *command;
    SerialShuHao *serialShuHao;

    void createLayout();

    QComboBox *channelComboBox;
    QComboBox *phaseTypeComboBox;
    QComboBox *polarityComboBox;

    QLineEdit *anodicAmpLineEdit;
    QLineEdit *anodicDurationLineEdit;
    QLineEdit *cathodicAmpLineEdit;
    QLineEdit *cathodicDurationLineEdit;
    QLineEdit *numPulseLineEdit;
    QLineEdit *adjDurationLineEdit;

    QPushButton *sendCommandButton;

    void sendStop();

private slots:
    void on_channelComboBox_changed(int index);
    void on_phaseComboBox_changed(int index);
    void on_polarityComboBox_changed(int index);
    void on_anodicAmp_editted();
    void on_anodicDuration_editted();
    void on_cathodicAmp_editted();
    void on_cathodicDuration_editted();
    void on_numPulse_editted();
    void on_adjDuration_editted();
    void on_sendCommand_clicked();
};

#endif // MAINWINDOW_H
