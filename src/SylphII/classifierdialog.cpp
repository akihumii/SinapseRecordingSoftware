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
    QLabel *classifierLabel = new QLabel(tr("Classifier Program: "));
    classifierPath = new QLineEdit;
    classifierOpen = new QPushButton(tr("Öpen"));
    connect(classifierOpen, SIGNAL(clicked(bool)), this, SLOT(on_openClassifier_clicked()));
    classifierLayout->addWidget(classifierLabel);
    classifierLayout->addWidget(classifierPath);
    classifierLayout->addWidget(classifierOpen);

    QHBoxLayout *parameterLayout = new QHBoxLayout;
    QLabel *parameterLabel = new QLabel(tr("Classifier parameters: "));
    parameterPath = new QLineEdit;
    parameterOpen = new QPushButton(tr("Open"));
    connect(parameterOpen, SIGNAL(clicked(bool)), this, SLOT(on_openParameter_clicked()));
    parameterLayout->addWidget(parameterLabel);
    parameterLayout->addWidget(parameterPath);
    parameterLayout->addWidget(parameterOpen);

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
}
