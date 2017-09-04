#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QSignalMapper>
#include <QDialog>
#include <QDialogButtonBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QFormLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QDebug>

class QIntValidator;

class settingsDialog : public QDialog
{
    Q_OBJECT
public:
    void createLayout();

    struct SettingsStruct{
        QString name;
        qint32 baudRate;
        QString stringBaudRate;
        QSerialPort::DataBits dataBits;
        QString stringDataBits;
        QSerialPort::Parity parity;
        QString stringParity;
        QSerialPort::StopBits stopBits;
        QString stringStopBits;
        QSerialPort::FlowControl flowControl;
        QString stringFlowControl;
        bool localEchoEnabled;
    };
    explicit settingsDialog(QWidget *parent = nullptr);

    SettingsStruct settings() const;

public slots:
    void on_scan_triggered();
    void on_selected_triggered(int stateCheck);
    void on_apply_triggered(int stateCheck);
    void on_ok_triggered();

private:
    void createSelectportSetting();
    void createSerialportSetting();
    void updateSettings(int selectedPort);

    int portCount;
    int stateCurrentPort;

    SettingsStruct currentSettings;

    QGroupBox *selectPortGroupBox;
    QVBoxLayout *microLayout[4];
    QGroupBox *port[4];
    QSignalMapper *signalMapper;
    QCheckBox *selectedPort[4];
    QLabel *portNameLabel[4];
    QLabel *portDescLabel[4];
    QLabel *portManuLabel[4];
    QLabel *portSeriLabel[4];

    QGroupBox *serialPortGroupBox;
    QComboBox *baudRateComboBox;
    QComboBox *dataBitsComboBox;
    QComboBox *parityComboBox;
    QComboBox *stopBitsComboBox;
    QComboBox *flowControlComboBox;
};

#endif // SETTINGSDIALOG_H
