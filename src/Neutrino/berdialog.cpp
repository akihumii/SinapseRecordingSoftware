#include "berdialog.h"

BERDialog::BERDialog(DataProcessor *dataProcessor_)
{
    dataProcessor = dataProcessor_;
    timer.start();
    connect(&berTimer, SIGNAL(timeout()), this, SLOT(updateData()));
    berTimer.start(1);
}

BERDialog::~BERDialog()
{
}

void BERDialog::updateData(){
//    qDebug() << "Incorrect count : " << dataProcessor->getIncorrectCount();
//    qDebug() << "Total     count : " << dataProcessor->getTotalCount();
}
