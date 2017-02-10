#ifndef BERDIALOG_H
#define BERDIALOG_H

#include <QDialog>
#include "dataprocessor.h"

class BERDialog : public QDialog
{
    Q_OBJECT

public:
    BERDialog(DataProcessor *dataProcessor_);
    ~BERDialog();

private:
    DataProcessor *dataProcessor;
    QElapsedTimer timer;
    QTimer berTimer;
private slots:
    void updateData();
};

#endif // BERDIALOG_H
