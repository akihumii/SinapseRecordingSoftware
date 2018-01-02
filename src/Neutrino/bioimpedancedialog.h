#ifndef BIOIMPEDANCEDIALOG_H
#define BIOIMPEDANCEDIALOG_H

#include <QDialog>
#include "../common/qtincludes.h"
#include "bioimpedance.h"

class BioImpedanceDialog : public QDialog
{
    Q_OBJECT

public:
    BioImpedanceDialog(BioImpedance *bioImpedance_);
    ~BioImpedanceDialog();

private:
    BioImpedance *bioImpedance;

    QString temp;
    QFile *File;
    QString directory = QDir::homePath() + "/Desktop/";
    QTextStream *out;
    QString fileName;

    QPushButton *saveButton;
    QPushButton *exitButton;

    void createLayout();

private slots:
    void on_saveButton_clicked();
    void on_exitButton_clicked();
};

#endif // BIOIMPEDANCEDIALOG_H
