#ifndef SERIALPORTDIALOG_H
#define SERIALPORTDIALOG_H

#include <QDialog>
#include "../common/qtincludes.h"
#include "serialchannel.h"

namespace SylphX {

class SerialPortDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SerialPortDialog(SerialChannel *serialChannel_);
    ~SerialPortDialog();

    SerialChannel *serialChannel;

private slots:
    void on_scanPorts_triggered();

private:
    void createLayout();
    int portCount;

    QGroupBox *port[4];

    QButtonGroup *implantGroup;
    QButtonGroup *ADCGroup;

    QVBoxLayout *portLayout[4];
    QHBoxLayout *portTypeLayout[4];

    QRadioButton *implantButton[4];
    QRadioButton *ADCButton[4];

    QLabel *portNameLabel[4];
    QLabel *portDescLabel[4];
    QLabel *portManuLabel[4];
    QLabel *enableLabel[4];

    QPushButton *scanPorts;

};

}

#endif // SERIALPORTDIALOG_H
