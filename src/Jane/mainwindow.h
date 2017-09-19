#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../common/qtincludes.h"
#include "amwflash_mainwindow.h"
#include "command.h"
#include "stimulator.h"
#include "dataprocessor.h"
#include "socketthor.h"
#include "serialchannel.h"
#include "measurementdialog.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();
    void createLayout();
    void createAction();

private slots:
    void loadLastCommand();
    void loadDefault();
    void on_mode_changed(int mode);
    void on_chipID_changed(int IDNum);
    void on_BER_TextEditted();
    void on_OP_Trigger_TextEditted();
    void on_BioImp_toggled();
    void on_DCL_toggled();
    void on_sendCommand_clicked();
    void on_chipReset_clicked();
    void on_JTAG_toggled();

    void on_subSequenceChannel_selected();
    void on_subSeqParamSpinBox_changed();
    void on_paramValueChange();

    void on_subSeqTimeStartEdit_changed(int channel);
    void on_subSeqMultipleStartComboBox_selected(int index);
    void on_subSeqTimeStopEdit_changed(int channel);
    void on_subSeqMultipleStopComboBox_selected(int index);


private:
    SocketThor *socketThor;
    Command *thorCommand;
    Stimulator *thorParam;
    SerialChannel *serialThor;
    Channel *thorChannel;
    DataProcessor *data;
    bool isWired = true;
    bool isConnected = false;
    bool ok = true;

    amwFlash_mainWindow *amwFlash_init;
//    MeasurementDialog *graph_init;

    QComboBox *modeComboBox;
    QComboBox *chipIDComboBox;

    QLineEdit *BER_byte[8];
    QLineEdit *OP_bit[8];


    QCheckBox *BioImpData[8];
    QString BioImpName[8] = { "BIOCHSEL<1>",
                              "BIOCHSEL<2>",
                              "BIOCHSEL<3>",
                              "BIOCHSEL<4>",
                              "ENBIOPIN",
                              "ETIRST"
                            };
    
    QRadioButton *DCLEnter_RadioButton;
    QRadioButton *DCLExit_RadioButton;

    QPushButton *sendCommandButton;
    QPushButton *chipResetButton;
    QPushButton *amwFlashButton;
    QPushButton *graphButton;
    QVBoxLayout *mainLayout;

    QHBoxLayout *subSeqLine[9];
    QCheckBox *subSeqCheckBox[8];
    QCheckBox *triggerCheckBox[8];

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
    QWidget *subsequenceWidget;

    QVBoxLayout *StimulatorParamLayout;
    QWidget *StimulatorParamWidget;
    QLineEdit *paramEdit[16];
    QHBoxLayout *paramLine[16];

    QTabWidget *JTAGTabWidget;
    QWidget *DATAMOD;
    QWidget *OSCINT;
    QWidget *BIOIMPEDANCE;
    QWidget *NEUAMPChannel;
    QWidget *NEUAMP;
    QWidget *TUNEADC;
    QWidget *SPILLOVER;
    QWidget *VTUNEDAX;
    QWidget *JTAGREGISTER;
    QCheckBox *JTAG[48];

    QHBoxLayout *allLayout;

    QList<QSerialPortInfo> portInfo;
    QString connectionStatus;

    QString BioImpNames[8] = {  "BIOCHSEL<1>",
                                "BIOCHSEL<2>",
                                "BIOCHSEL<3>",
                                "BIOCHSEL<4>",
                                "ENBIOPIN",
                                "ETIRST"
                                };
    QString JTAGNames[48] = {  "INVDAT",           "OFFDATA",          "D3",               "D2",               "D1",
                                "OSCINT<8>",        "OSCINT<7>",        "OSCINT<6>",        "OSCINT<5>",        "OSCINT<4>",        "OSCINT<3>",        "OSCINT<2>",    "OSCINT<1>",
                                "D<7>",             "D<6>",             "D<5>",             "D<4>",             "D<3>",             "D<2>",             "D<1>",
                                "D<5>",             "D<4>",             "D<3>",             "D<2>",             "D<1>",
                                "Reg1_RFU<6>",      "Reg1_RFU<5>",      "Reg1_RFU<4>",      "Reg1_RFU<3>",      "Reg1_RFU<2>",      "Reg1_RFU<1>",      "Reg1_DLSTIM",  "Reg1_DUSTIM",
                                "Reg2_RFU<8>",      "Reg2_RFU<7>",      "Reg2_RFU<6>",      "Reg2_RFU<5>",      "Reg2_RFU<4>",      "Reg2_RFU<3>",      "Reg2_RFU<2>",  "Reg2_RFU<1>",
                                "Reg3_RFU<2>",      "Reg3_RFU<1>",      "Reg3_INTRES4",     "Reg3_INTRES3",     "Reg3_D<4>",     "Reg3_INTRES2",     "Reg3_INTRES1",
                                };  //5+8+7+5 + 8+8+7
//    QString JTAGNames[112] = {  "DATAMOD",          "OFFDATABUFFS",     "DATAMOD<2>",       "DATAMOD<1>",       "DATAMOD<0>",       "OSCINT<5>",        "OSCINT<4>",    "OSCINT<3>",
//                                "OSCINT<2>",        "OSCINT<1>",        "BIO<7>",           "BIO<6>",           "BIO<5>",           "BIO<4>",           "BIO<3>",       "BIO<2>",
//                                "BIO<1>",           "Ch10<5>",          "Ch10<4>",          "Ch10<3>",          "Ch10<2>",          "Ch10_OFF",         "Ch9<5>",       "Ch9<4>",
//                                "Ch9<3>",           "Ch9<2>",           "Ch9_OFF",          "Ch8<5>",           "Ch8<4>",           "Ch8<3>",           "Ch8<2>",       "Ch8_OFF",
//                                "Ch7<5>",           "Ch7<4>",           "Ch7<3>",           "Ch7<2>",           "Ch7_OFF",          "Ch6<5>",           "Ch6<4>",       "Ch6<3>",
//                                "Ch6<2>",           "Ch6_OFF",          "Ch5<5>",           "Ch5<4>",           "Ch5<3>",           "Ch5<2>",           "Ch5_OFF",      "Ch4<5>",
//                                "Ch4<4>",           "Ch4<3>",           "Ch4<2>",           "Ch4_OFF",          "Ch3<5>",           "Ch3<4>",           "Ch3<3>",       "Ch3<2>",
//                                "Ch3_OFF",          "Ch2<5>",           "Ch2<4>",           "Ch2<3>",           "Ch2<2>",           "Ch2_OFF",          "Ch1<5>",       "Ch1<4>",
//                                "Ch1<3>",           "Ch1<2>",           "Ch1_OFF",          "REFPWRDWN",        "IP_CAP1",          "IP_CAP0",          "IEchannelSel", "TSTDSEL2",
//                                "TSTDSEL1",         "TSTDSEL0",         "FREQ1",            "FREQ2",            "S2GAIN0",          "S2GAIN1",          "S1G1",         "S1G0",
//                                "S1Control1",       "S1Control0",       "OFFCMFB",          "INTRES4",          "INTRES3",          "INTRES2",          "INTRES1",      "EnEXTVolt",
//                                "ExEXTRes",         "INVBIASRESET",     "S2RESET",          "S1RESET",          "S1REFRESET",       "CMRRESET",         "PUTESTAMP",    "PDS3",
//                                "PDS2",             "PDS1",             "PDBIAS",           "TUNEADC<5>",       "TUNEADC<4>",       "TUNEADC<3>",       "TUNEADC<2>",   "TUNEADC<1>",
//                                "SpilloverRFU<6>",  "SELRES0",          "SELRES1",          "   ",              "INCOSCfreq",       "DECOSCfreq",       "OFFOSC",       "BIOPASSTHRU"
//                             };


    void on_wired_triggered();
    void connectThor();
    void createModeComboBox();
    void createSubsequenceWidget();
    void createStimulatorParamWidget();
    void createJTAGWidget();

};

#endif // MAINWINDOW_H
