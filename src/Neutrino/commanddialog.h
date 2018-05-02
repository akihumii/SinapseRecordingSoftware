#ifndef COMMANDDIALOG_H
#define COMMANDDIALOG_H

#include <QDialog>
#include "../common/qtincludes.h"
#include "bioimpedance.h"
#include "socketneutrino.h"
#include "command.h"
#include "serialchannel.h"
#include "measurementdialog.h"
#include "delaythread.h"
#include "bioimpedancedialog.h"

using namespace std;

class Command;

class CommandDialog : public QDialog
{
    Q_OBJECT

public:
    CommandDialog(SocketNeutrino *socketNeutrino_,
                  Command *NeutrinoCommand_,
                  Channel *NeutrinoChannel_,
                  SerialChannel *NeutrinoSerial_,
                  DataProcessor *dataProcessor_);
    ~CommandDialog();

signals:
    void closeWaitingMBox();

private slots:
    void on_sendCommand_clicked();
    void on_chipReset_clicked();
    void on_SELALL_clicked();
    void on_SELNONE_clicked();
    void on_JTAGextension_clicked();
    void on_JTAGreset_clicked();
    void on_JTAG_toggled();
    void on_Mode_Changed(int Mode);
    void on_ChipID_Changed(int IDnum);
    void on_SELCHN_toggled();
    void on_BioImp_toggled();
    void on_DCL_toggled();
    void on_baud_toggled();
    void on_BER_textEdited();
    void on_CMReset_clicked();
    void on_amplifierSelectAll_clicked();
    void on_amplifierSelectNone_clicked();
    void on_shortRefToGND_clicked();
    void on_amplifierSelect_toggled();
    void on_bioImpDelay_changed();
    void on_startBioImpedanceMeasurement();
    void on_exitBioImpedanceMeasurement();

private:
    SocketNeutrino *socketNeutrino;
    Command *NeutrinoCommand;
    Channel *NeutrinoChannel;
    SerialChannel *NeutrinoSerial;
    DataProcessor *dataProcessor;
    QMessageBox *mboxWait;
    QByteArray bioImpedanceData;
    DelayThread *delayThread;
    BioImpedance *bioImpedance;

    void createLayout();
    void createJTAGLayout();
    void loadlastCommand();
    void loadDefault();
    void updateHeader();
    void bioReset();
    void setInlineQuad(int input);
    void setBioImpedanceChannel(int channel);
    void setCurrentType(CURRENT_TYPE type);
    void setBioImpedanceGain(GAIN gain);
    void measureResetVoltage(BioImpedance *bioImpedance, GAIN gain);
    void runCurrentMeasurement(int i, BioImpedance *bioImpedance, GAIN bioGain, CURRENT_TYPE bioCurrent);
    void runFullBioImpedanceMeasurement();
    void runAutoRangedBioImpedanceMeasurement();
    void delaySeconds();

    double bioImpGain = 80.0;
    double resolution = 5.0;

    QLabel *ModeLabel;
    QLabel *CIDLabel;
    QLabel *DataBERLabel;
    QLabel *BioImpLabel;
    QLabel *DigComLoopback;

    QComboBox *ModeComboBox;
    QComboBox *CIDComboBox;

    QLineEdit *bioImpDelayLineEdit;
    QCheckBox *ETIRST;
    QCheckBox *ENBIOPIN;
    QCheckBox *BIOCHSEL4;
    QCheckBox *BIOCHSEL3;
    QCheckBox *BIOCHSEL2;
    QCheckBox *BIOCHSEL1;

    QCheckBox *SELCHN[10];
    QCheckBox *BioImpData[8];
    QCheckBox *amplifierSelect[10];

    QTabWidget *JTAGTabWidget;
    QWidget *DATAMOD;
    QWidget *OSCINT;
    QWidget *BIOIMPEDANCE;
    QWidget *NEUAMPChannel;
    QWidget *NEUAMP;
    QWidget *TUNEADC;
    QWidget *SPILLOVER;
    QCheckBox *JTAG[112];

    QPushButton *SELALL;
    QPushButton *SELNONE;

    QPushButton *amplifierSelectAll;
    QPushButton *amplifierSelectNone;

    QPushButton *SendCommand;
    QPushButton *ChipReset;
    QPushButton *CMReset;
    QPushButton *shortRefToGND;

    QPushButton *JTAGextension;
    QPushButton *JTAGreset;

    QRadioButton *Exit;
    QRadioButton *Enter;

    QComboBox *baudRateComboBox;

    QLineEdit *BER_byte[8];

    QAction *exitAction;
    QString BioImpNames[8] = {  "BIOCHSEL<1>",
                                "BIOCHSEL<2>",
                                "BIOCHSEL<3>",
                                "BIOCHSEL<4>",
                                "ENBIOPIN",
                                "ETIRST"
                                };
    QString JTAGNames[112] = {  "DATAMOD",          "OFFDATABUFFS",     "SELRES1",          "SELRES0",          "iENPREBIAS",       "OSCINT<5>",        "OSCINT<4>",    "OSCINT<3>",    // 0-7
                                "OSCINT<2>",        "OSCINT<1>",        "ONBIOIMP",         "A1",               "A0",               "SEL_LNA GAIN",     "ISEL_1",       "ISEL_0",       // 8-15
                                "SEL_IQ",           "Ch10<5>",          "Ch10<4>",          "Ch10<3>",          "Ch10<2>",          "Ch10_OFF",         "Ch9<5>",       "Ch9<4>",       // 16-23
                                "Ch9<3>",           "Ch9<2>",           "Ch9_OFF",          "Ch8<5>",           "Ch8<4>",           "Ch8<3>",           "Ch8<2>",       "Ch8_OFF",      // 24-31
                                "Ch7<5>",           "Ch7<4>",           "Ch7<3>",           "Ch7<2>",           "Ch7_OFF",          "Ch6<5>",           "Ch6<4>",       "Ch6<3>",       // 32-39
                                "Ch6<2>",           "Ch6_OFF",          "Ch5<5>",           "Ch5<4>",           "Ch5<3>",           "Ch5<2>",           "Ch5_OFF",      "Ch4<5>",       // 40-47
                                "Ch4<4>",           "Ch4<3>",           "Ch4<2>",           "Ch4_OFF",          "Ch3<5>",           "Ch3<4>",           "Ch3<3>",       "Ch3<2>",       // 48-55
                                "Ch3_OFF",          "Ch2<5>",           "Ch2<4>",           "Ch2<3>",           "Ch2<2>",           "Ch2_OFF",          "Ch1<5>",       "Ch1<4>",       // 56-63
                                "Ch1<3>",           "Ch1<2>",           "Ch1_OFF",          "REFPWRDWN",        "IP_CAP1",          "IP_CAP0",          "IEchannelSel", "TSTDSEL2",     // 64-71
                                "TSTDSEL1",         "TSTDSEL0",         "FREQ1",            "FREQ2",            "S2GAIN0",          "S2GAIN1",          "S1G1",         "S1G0",         // 72-79
                                "S1Control1",       "S1Control0",       "OFFCMFB",          "INTRES4",          "INTRES3",          "INTRES2",          "INTRES1",      "EnEXTVolt",    // 80-87
                                "ExEXTRes",         "INVBIASRESET",     "S2RESET",          "S1RESET",          "S1REFRESET",       "CMRRESET",         "PUTESTAMP",    "PDS3",         // 88-95
                                "PDS2",             "PDS1",             "PDBIAS",           "TUNEADC<4>",       "TUNEADC<3>",       "TUNEADC<2>",       "TUNEADC<1>",   "ReduceLED_Prebias",   // 96-103
                                "DIVCL_0",          "PRODEL<0>",        "PRODEL<1>",        "PRODEL<2>",        "INCOSCfreq",       "DECOSCfreq",       "OFFOSC",       "BIOPASSTHRU"}; // 104-111
};

#endif // COMMANDDIALOG_H
