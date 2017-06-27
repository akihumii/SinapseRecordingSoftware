#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include "../common/qtincludes.h"
#include "socketabstract.h"

class SocketAbstract;

using namespace std;

class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    ConnectionDialog(SocketAbstract *socketAbstract_);
    ~ConnectionDialog();

private slots:
    void on_ConnectButton_clicked();
    void on_CancelButton_clicked();

private:
    void createLayout();

    SocketAbstract *socketAbstract;

    QLabel *IPLabel;
    QLabel *PortLabel;

    QLineEdit *IPAddress;
    QLineEdit *PortNumber;

    QPushButton *Connect;
    QPushButton *Cancel;

    QAction *exitAction;
};

#endif // CONNECTIONDIALOG_H
