#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include "../common/qtincludes.h"
#include "socketneutrino.h"
#include "mainwindow.h"

class SocketNeutrino;

using namespace std;

class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    ConnectionDialog(SocketNeutrino *socketNeutrino_);
    ~ConnectionDialog();

private slots:
    void on_ConnectButton_clicked();
    void on_CancelButton_clicked();

private:
    void createLayout();

    SocketNeutrino *socketNeutrino;

    QLabel *IPLabel;
    QLabel *PortLabel;

    QLineEdit *IPAddress;
    QLineEdit *PortNumber;

    QPushButton *Connect;
    QPushButton *Cancel;

    QAction *exitAction;
};

#endif // CONNECTIONDIALOG_H
