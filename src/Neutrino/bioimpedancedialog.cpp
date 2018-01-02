#include "bioimpedancedialog.h"

BioImpedanceDialog::BioImpedanceDialog(BioImpedance *bioImpedance_)
{
    bioImpedance = bioImpedance_;
    createLayout();
}

BioImpedanceDialog::~BioImpedanceDialog()
{

}

void BioImpedanceDialog::createLayout(){
    QLabel *Impedance = new QLabel;
    for(int i = 0; i < 10; i++){
        temp = temp + "Magnitude for Channel " + QString::number(i + 1) + " is: " + QString::number(bioImpedance->calculateMagnitude(i)) + "\n";
        temp = temp + "Phase for Channel " + QString::number(i + 1) + " is: " + QString::number(bioImpedance->calculatePhase(i)) + "\n\n";
    }
    Impedance->setText(temp);

    QHBoxLayout *buttonLayout = new QHBoxLayout;

    saveButton = new QPushButton(tr("Save!"));
    connect(saveButton, SIGNAL(clicked(bool)), this, SLOT(on_saveButton_clicked()));

    exitButton = new QPushButton(tr("Exit!"));
    connect(exitButton, SIGNAL(clicked(bool)), this, SLOT(on_exitButton_clicked()));

    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(exitButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(Impedance);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
    mainLayout->setSizeConstraint( QLayout::SetFixedSize );
}

void BioImpedanceDialog::on_saveButton_clicked(){
    qDebug() << "Saving...";
    File = new QFile;
    fileName = directory + QDateTime::currentDateTime().toString("'impedance_'yyyyMMdd_HHmmss'.txt'");
    File->setFileName(fileName);
    if(File->open(QIODevice::WriteOnly|QIODevice::Text)){
        qDebug()<< "File opened";
    }
    else{
        qDebug() << "File failed to open";
    }
    out = new QTextStream(File);
    *out << temp;
    if(File->isOpen()){
        File->close();
        qDebug() << "File closed";
    }
}

void BioImpedanceDialog::on_exitButton_clicked(){
    qDebug() << "Exiting...";
    this->close();
}
