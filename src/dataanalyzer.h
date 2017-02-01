#ifndef DATAANALYZER_H
#define DATAANALYZER_H

#include <QDialog>
#include "qtincludes.h"
#include "qcustomplot.h"

using namespace std;

class DataAnalyzer : public QDialog
{
    Q_OBJECT

public:
    DataAnalyzer();
    ~DataAnalyzer();

private:
    void createAction();
    void createMenu();
    void createLayout();
    QPushButton *openFileButton;
    QLineEdit *openFileLineEdit;
    QCustomPlot *dataGraph;

private slots:
    void on_openFile_triggered();
    void selectionChanged();
    void mouseWheel();
};

#endif // DATAANALYZER_H
