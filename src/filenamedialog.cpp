#include "filenamedialog.h"

FileNameDialog::FileNameDialog(Data *data_){
    data = data_;

    FileNameLabel = new QLabel;
    FileNameLabel->setText("Enter File Name");

    FileNameLineEdit = new QLineEdit(data->getFileName());

    QVBoxLayout *FileNameLayout = new QVBoxLayout;
    FileNameLayout->addWidget(FileNameLabel);
    FileNameLayout->addWidget(FileNameLineEdit);

    Ok = new QPushButton(tr("Ok"));
    Cancel = new QPushButton(tr("Cancel"));

    QVBoxLayout *ButtonsLayout = new QVBoxLayout;
    ButtonsLayout->addWidget(Ok);
    ButtonsLayout->addWidget(Cancel);

    connect(Ok, SIGNAL(clicked()), this, SLOT(on_OkButton_clicked()));
    connect(Cancel, SIGNAL(clicked()), this, SLOT(on_CancelButton_clicked()));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(FileNameLayout);
    mainLayout->addLayout(ButtonsLayout);

    setLayout(mainLayout);
    mainLayout->setSizeConstraint( QLayout::SetFixedSize );
}

FileNameDialog::~FileNameDialog(){

}

void FileNameDialog::on_OkButton_clicked(){
    data->setFileName(FileNameLineEdit->text());
    setWindowTitle(tr("File Name set, closing..."));
    QThread::msleep(500);
    FileNameDialog::close();
}

void FileNameDialog::on_CancelButton_clicked(){
    FileNameDialog::close();
}
