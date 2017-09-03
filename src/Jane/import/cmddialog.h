#ifndef CMDDIALOG_H
#define CMDDIALOG_H

#include <QMainWindow>
#include "../common/qtincludes.h"
#include "import/amwflash_mainwindow.h"
#include "import/commandj.h"
#include "import/stimulator.h"
#include "socketneutrino.h"
#include "serialchannel.h"


class CommandJ;


class CmdDialog : public QMainWindow
{
    Q_OBJECT

public:
    CmdDialog(SocketNeutrino *socketNeutrino_, Channel *NeutrinoChannel_, SerialChannel *NeutrinoSerial_);
    ~CmdDialog();
    void createLayout();
    void createAction();

private slots:
    void loadLastCommand();
    void loadDefault();
    void on_mode_changed(int mode);
    void on_chipID_changed(int IDNum);
    void on_BER_TextEditted();
    void on_BioImp_toggled();
    void on_DCL_toggled();
    void on_sendCommand_clicked();
    void on_chipReset_clicked();

    void on_subSequenceChannel_selected();
    void on_subSeqParamSpinBox_changed();

    void on_subSeqTimeStartEdit_changed(int channel);
    void on_subSeqMultipleStartComboBox_selected(int index);
    void on_subSeqTimeStopEdit_changed(int channel);
    void on_subSeqMultipleStopComboBox_selected(int index);

private:
    SocketNeutrino *thorSocket;
    Channel *thorChannel;
    SerialChannel *thorSerial;
    CommandJ *thorCommand;
    Stimulator *thorParam;
    amwFlash_mainWindow *amwFlash_init;

    QComboBox *modeComboBox;
    QComboBox *chipIDComboBox;

    QLineEdit *BER_byte[8];

    QCheckBox *BioImpData[8];
    QString BioImpName[8] = { "BIOCHSEL<1>",
                              "BIOCHSEL<2>",
                              "BIOCHSEL<3>",
                              "BIOCHSEL<4>",
                              "ENBIOPIN",
                              "ETIRST"
                            };
    
    QRadioButton *DCLEnter_RadioButtun;
    QRadioButton *DCLExit_RadioButton;

    QPushButton *sendCommandButton;
    QPushButton *chipResetButton;
    QPushButton *amwFlashButton;
    QVBoxLayout *mainLayout;

    QHBoxLayout *subSeqLine[8];
    QCheckBox *subSeqCheckBox[8];
    QSpinBox *subSeqParamSpinBox[8];
    
    QSignalMapper *timeStartMapper;
    QLineEdit *subSeqTimeStartEdit[8];
    QSignalMapper *multipleStartMapper;
    QComboBox *subSeqMultipleStartComboBox[8];
    
    QSignalMapper *timeStopMapper;
    QLineEdit *subSeqTimeStopEdit[8];
    QSignalMapper *multipleStopMapper;
    QComboBox *subSeqMultipleStopComboBox[8];
    QVBoxLayout *subSequenceLayout;

    QHBoxLayout *allLayout;
};

#endif // CMDDIALOG_H
