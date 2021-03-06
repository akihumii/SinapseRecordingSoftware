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
    QTcpServer *tcpServer;
    QTcpSocket *tcpServerConnection;

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
    QLineEdit *interPhaseIntervalLineEdit;

    QPushButton *sendCommandButton;
    QPushButton *startStopButton;

    bool start = false;

    void sendStop();
    void sendStart();

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
    void on_interPhaseInterval_editted();
    void on_sendCommand_clicked();
    void on_startStop_clicked();
    void acceptConnection();
    void echo();
};

#endif // MAINWINDOW_H
