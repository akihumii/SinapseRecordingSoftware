#ifndef SERIALPORTDIALOG_H
#define SERIALPORTDIALOG_H

#include <QDialog>
#include "../common/qtincludes.h"

namespace Ui {
class SerialPortDialog;
}

class SerialPortDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SerialPortDialog(QSerialPort *port);
    ~SerialPortDialog();

private slots:
    void on_scanPorts_triggered();
    void on_portSelected();

private:
    void createLayout();
    int portCount;

    QSerialPort *serialPort;

    QGroupBox *port[4];

    QButtonGroup *implantGroup;
    QButtonGroup *ADCGroup;

    QVBoxLayout *portLayout[4];
    QHBoxLayout *portTypeLayout[4];

    QCheckBox *selected[4];

    QLabel *portNameLabel[4];
    QLabel *portDescLabel[4];
    QLabel *portManuLabel[4];
    QLabel *enableLabel[4];

    QPushButton *scanPorts;

};

#endif // SERIALPORTDIALOG_H
