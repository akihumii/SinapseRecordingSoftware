#ifndef CLASSIFIERDIALOG_H
#define CLASSIFIERDIALOG_H

#include <QDialog>
#include "dataprocessor.h"

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

    void createLayout();

    QLineEdit *classifierPath;
    QPushButton *classifierOpen;

    QLineEdit *parameterPath;
    QPushButton *parameterOpen;

private slots:
    void on_openClassifier_clicked();
    void on_openParameter_clicked();
};

#endif // CLASSIFERDIALOG_H
