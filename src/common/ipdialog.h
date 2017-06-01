#ifndef IPDIALOG_H
#define IPDIALOG_H

#include <QDialog>
#include "qtincludes.h"

using namespace std;

namespace Ui {
class IPDialog;
}

class IPDialog : public QDialog
{
    Q_OBJECT

public:
    IPDialog();
    ~IPDialog();

private:
    QString ipString = "10.10.10.1";
    QLineEdit *ipAddress;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void createLayout();

private slots:
    void on_okClicked();
    void on_cancelClicked();
};

#endif // IPDIALOG_H
