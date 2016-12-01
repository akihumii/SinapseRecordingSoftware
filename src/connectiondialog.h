#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include "qtincludes.h"
#include "socketedison.h"
#include "mainwindow_ka.h"

class SocketEdison;

using namespace std;

class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    ConnectionDialog(SocketEdison *socketEdison_);
    ~ConnectionDialog();

private slots:
    void on_ConnectButton_clicked();
    void on_CancelButton_clicked();

private:
    void createLayout();

    SocketEdison *socketEdison;

    QLabel *IPLabel;
    QLabel *PortLabel;

    QLineEdit *IPAddress;
    QLineEdit *PortNumber;

    QPushButton *Connect;
    QPushButton *Cancel;

    QAction *exitAction;
};

#endif // CONNECTIONDIALOG_H
