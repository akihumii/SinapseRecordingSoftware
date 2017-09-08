#include "classifierdialog.h"

ClassifierDialog::ClassifierDialog(DataProcessor *dataProcessor_){
    dataProcessor = dataProcessor_;
    createLayout();
}

ClassifierDialog::~ClassifierDialog(){
}

void ClassifierDialog::createLayout(){
    QVBoxLayout *mainLayout = new QVBoxLayout;

    QHBoxLayout *classifierLayout = new QHBoxLayout;
//    QLabel *classifierLabel = new QLabel(tr("Classifier Program: "));
    classifierPath = new QLineEdit;
    classifierOpen = new QPushButton(tr("Öpen Classifier Program"));
    connect(classifierOpen, SIGNAL(clicked(bool)), this, SLOT(on_openClassifier_clicked()));
//    classifierLayout->addWidget(classifierLabel);
//    classifierLayout->addWidget(classifierPath);
    classifierLayout->addWidget(classifierOpen);

    QHBoxLayout *parameterLayout = new QHBoxLayout;
//    QLabel *parameterLabel = new QLabel(tr("Classifier parameters: "));
    parameterPath = new QLineEdit;
    parameterOpen = new QPushButton(tr("Open parameters"));
    connect(parameterOpen, SIGNAL(clicked(bool)), this, SLOT(on_openParameter_clicked()));
//    parameterLayout->addWidget(parameterLabel);
//    parameterLayout->addWidget(parameterPath);
    parameterLayout->addWidget(parameterOpen);

    QLabel *instruction = new QLabel(tr("Step 1: ENSURE ROBOT ARM IS CONNECTED BEFORE RUNNING THE FOLLOWING STEPS! \n"
                                        "         IF NOT CONNECTED, RE-RUN THE PROGRAM AFTER CONNECTING TO ROBOT ARM! \n"
                                        "Step 2: Click Open Classifier Program and search for ClassifierProgram.exe. \n"
                                        "Step 3: Select TWO datasets only, that comprises of TWO different amplitudes. \n"
                                        "Step 4: Wait for prompt to allow you to save the parameters. \n"
                                        "Step 5: Click Open parameters and select the text file that was generated in Step 4. \n"
                                        "Step 6: Close this dialog and enable classifier through the file menu. \n"));

    mainLayout->addWidget(instruction);
    mainLayout->addLayout(classifierLayout);
    mainLayout->addLayout(parameterLayout);
    setLayout(mainLayout);
}

void ClassifierDialog::on_openClassifier_clicked(){
    QProcess *process = new QProcess(this);
    QString file;
    if(classifierPath->text() == ""){
        file = QFileDialog::getOpenFileName(this,
                                                tr("Open Classifier Program"),
                                                QDir::currentPath(),
                                                tr("Classifier Program (*.exe)"));
        classifierPath->setText(file);
        qDebug() << "Opening classifier program";
    }
    else{
        file = classifierPath->text();
    }
    process->start(file);
}

void ClassifierDialog::on_openParameter_clicked(){
    QString file;
    if(parameterPath->text() == ""){
        file = QFileDialog::getOpenFileName(this,
                                           tr("Open parameter text file"),
                                           QDir::currentPath(),
                                           tr(".txt file (*.txt)"));
        parameterPath->setText(file);
    }
    else{
        file = parameterPath->text();
    }
    QFile parameterFile(file);
    if(!parameterFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&parameterFile);
    QString temp;
    temp = in.readLine();
    dataProcessor->setClassifierK(temp.toFloat());
    qDebug() << "Classifier value K set to : " << temp;
    temp = in.readLine();
    dataProcessor->setClassifierL(temp.toFloat());
    qDebug() << "Classifier value L set to: " << temp;
    temp = in.readLine();
    dataProcessor->setClassifierWindowLength(temp.toFloat());
    qDebug() << "Window Length set to: " << temp;
    temp = in.readLine();
    dataProcessor->setClassifierThreshold(temp.toFloat());
    qDebug() << "Threshold set to: " << temp;
    temp = in.readLine();
    dataProcessor->setClassifierChannel(temp.toInt());
    qDebug() << "Classifier will perform on Channel " << temp;
    QMessageBox::information(this, "Parameters set!", "K Value: " + QString::number(dataProcessor->getClassifierK()) + "\n"
                                                            + "L Value: " + QString::number(dataProcessor->getClassifierL()) + "\n"
                                                            + "Window Length: " + QString::number(dataProcessor->getClassifierWindowLength()) + "\n"
                                                            + "Threshold: " + QString::number(dataProcessor->getClassifierThreshold()) + "\n"
                                                            + "Channel Selected: " + QString::number(dataProcessor->getClassifierChannel()) + "\n"
                                                            + "You may close the dialog!");
}
