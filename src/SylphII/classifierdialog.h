#ifndef CLASSIFIERDIALOG_H
#define CLASSIFIERDIALOG_H

#include <QDialog>
#include "dataprocessor.h"
#include <iostream>

namespace Ui {
class ClassifierDialog;
}

class ClassifierDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClassifierDialog(DataProcessor *dataProcessor_);
    ~ClassifierDialog();

private:
    DataProcessor *dataProcessor;
    QLocalServer *matlabSocket;
    QStringList fortunes;

    void createLayout();

    QLineEdit *classifierPath;
    QPushButton *classifierOpen;

    QLineEdit *parameterPath;
    QPushButton *parameterOpen;

private slots:
    void on_openClassifier_clicked();
    void on_openParameter_clicked();
    void on_matlabData_ready();
};

#endif // CLASSIFERDIALOG_H
