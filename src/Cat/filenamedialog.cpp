#include "filenamedialog.h"

FilenameDialog::FilenameDialog(){
    createLayout();
}

void FilenameDialog::createLayout(){
    QHBoxLayout *mainLayout = new QHBoxLayout;

//    QLabel *label = new QLabel(tr("Filename: "));
    filenameLine = new QLineEdit;
    filenameLine->setMinimumWidth(300);
    connect(filenameLine, SIGNAL(returnPressed()), this, SLOT(on_filename_changed()));

//    mainLayout->addWidget(label);
    mainLayout->addWidget(filenameLine);

    setLayout(mainLayout);
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    setWindowTitle(tr("Filename"));
}

void FilenameDialog::on_filename_changed(){
    filename = filenameLine->text();
    emit filenameChanged();
}

void FilenameDialog::setFilename(QString name){
    filename = name;
    filenameLine->setText(name);
}

void FilenameDialog::reject(){
    emit filenameChanged();
}

QString FilenameDialog::getFilename(){
    return filename;
}

FilenameDialog::~FilenameDialog(){
}
