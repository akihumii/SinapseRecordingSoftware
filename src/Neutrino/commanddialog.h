#ifndef COMMANDDIALOG_H
#define COMMANDDIALOG_H

#include <QDialog>
#include "../common/qtincludes.h"
#include "socketneutrino.h"
#include "command.h"
#include "serialchannel.h"

using namespace std;

class Command;

class CommandDialog : public QDialog
{
    Q_OBJECT

public:
    CommandDialog(SocketNeutrino *socketNeutrino_, Command *NeutrinoCommand_, Channel *NeutrinoChannel_, SerialChannel *NeutrinoSerial_);
    ~CommandDialog();

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
    void on_BER_textEdited();
    void on_CMReset_clicked();

private:
    SocketNeutrino *socketNeutrino;
    Command *NeutrinoCommand;
    Channel *NeutrinoChannel;
    SerialChannel *NeutrinoSerial;
    void createLayout();
    void createJTAGLayout();
    void loadlastCommand();
    void loadDefault();

    QLabel *ModeLabel;
    QLabel *CIDLabel;
    QLabel *DataBERLabel;
    QLabel *BioImpLabel;
    QLabel *DigComLoopback;

    QComboBox *ModeComboBox;
    QComboBox *CIDComboBox;

    QCheckBox *ETIRST;
    QCheckBox *ENBIOPIN;
    QCheckBox *BIOCHSEL4;
    QCheckBox *BIOCHSEL3;
    QCheckBox *BIOCHSEL2;
    QCheckBox *BIOCHSEL1;

    QCheckBox *SELCHN[10];
    QCheckBox *BioImpData[8];

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

    QPushButton *SendCommand;
    QPushButton *ChipReset;
    QPushButton *CMReset;

    QPushButton *JTAGextension;
    QPushButton *JTAGreset;

    QRadioButton *Exit;
    QRadioButton *Enter;

    QLineEdit *BER_byte[8];

    QAction *exitAction;
    QString BioImpNames[8] = {  "ETIRST",
                                "ENBIOPIN",
                                "BIOCHSEL<4>",
                                "BIOCHSEL<3>",
                                "BIOCHSEL<2>",
                                "BIOCHSEL<1>"};
    QString JTAGNames[112] = {  "DATAMOD",
                                "OFFDATABUFFS",
                                "DATAMOD<2>",
                                "DATAMOD<1>",
                                "DATAMOD<0>",
                                "OSCINT<5>",
                                "OSCINT<4>",
                                "OSCINT<3>",

                                "OSCINT<2>",
                                "OSCINT<1>",
                                "BIO<7>",
                                "BIO<6>",
                                "BIO<5>",
                                "BIO<4>",
                                "BIO<3>",
                                "BIO<2>",

                                "BIO<1>",
                                "Ch10<5>",
                                "Ch10<4>",
                                "Ch10<3>",
                                "Ch10<2>",
                                "Ch10<1>",
                                "Ch9<5>",
                                "Ch9<4>",

                                "Ch9<3>",
                                "Ch9<2>",
                                "Ch9<1>",
                                "Ch8<5>",
                                "Ch8<4>",
                                "Ch8<3>",
                                "Ch8<2>",
                                "Ch8<1>",

                                "Ch7<5>",
                                "Ch7<4>",
                                "Ch7<3>",
                                "Ch7<2>",
                                "Ch7<1>",
                                "Ch6<5>",
                                "Ch6<4>",
                                "Ch6<3>",

                                "Ch6<2>",
                                "Ch6<1>",
                                "Ch5<5>",
                                "Ch5<4>",
                                "Ch5<3>",
                                "Ch5<2>",
                                "Ch5<1>",
                                "Ch4<5>",

                                "Ch4<4>",
                                "Ch4<3>",
                                "Ch4<2>",
                                "Ch4<1>",
                                "Ch3<5>",
                                "Ch3<4>",
                                "Ch3<3>",
                                "Ch3<2>",

                                "Ch3<1>",
                                "Ch2<5>",
                                "Ch2<4>",
                                "Ch2<3>",
                                "Ch2<2>",
                                "Ch2<1>",
                                "Ch1<5>",
                                "Ch1<4>",

                                "Ch1<3>",
                                "Ch1<2>",
                                "Ch1<1>",
                                "RFU",
                                "RFU",
                                "RFU",
                                "IEchannelSel",
                                "TSTDSEL2",

                                "TSTDSEL1",
                                "TSTDSEL0",
                                "FREQ1",
                                "FREQ2",
                                "S2GAIN0",
                                "S2GAIN1",
                                "S1G1",
                                "S1G0",

                                "S1Control1",
                                "S1Control0",
                                "OFFCMFB",
                                "INTRES4",
                                "INTRES3",
                                "INTRES2",
                                "INTRES1",
                                "EnEXTVolt",

                                "ExEXTRes",
                                "INVBIASRESET",
                                "S2RESET",
                                "S1RESET",
                                "S1REFRESET",
                                "CMRRESET",
                                "PUTESTAMP",
                                "PDS3",

                                "PDS2",
                                "PDS1",
                                "PDBIAS",
                                "TUNEADC<5>",
                                "TUNEADC<4>",
                                "TUNEADC<3>",
                                "TUNEADC<2>",
                                "TUNEADC<1>",

                                "SpilloverRFU<6>",
                                "SpilloverRFU<5>",
                                "SpilloverRFU<4>",
                                "SpilloverRFU<3>",
                                "SpilloverRFU<2>",
                                "SpilloverRFU<1>",
                                "OFFOSC",
                                "BIOPASSTHRU"};
};

#endif // COMMANDDIALOG_H
