#include "filenamedialog.h"

FilenameDialog::FilenameDialog(){
    createLayout();
}

void FilenameDialog::createLayout(){
    QVBoxLayout *mainLayout = new QVBoxLayout;

    filenameLine = new QLineEdit;
    filenameLine->setMinimumWidth(300);
    connect(filenameLine, SIGNAL(returnPressed()), this, SLOT(on_filename_changed()));

    QLabel *emptyLabel = new QLabel;

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QHBoxLayout *subLayout = new QHBoxLayout;
    subLayout->addWidget(emptyLabel);
    subLayout->addWidget(buttonBox);

    mainLayout->addWidget(filenameLine);
    mainLayout->addLayout(subLayout);

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

void FilenameDialog::accept(){
    emit filenameChanged();
}

void FilenameDialog::reject(){
    QMessageBox msgBox;
    msgBox.setText("Your are leaving without saving...");
    msgBox.setInformativeText("Do you want to save your file " + filename + " ?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();

    switch(ret){
        case QMessageBox::Save:
            emit filenameChanged();
            break;
        case QMessageBox::Discard:
            emit filenameDiscard();
            break;
        case QMessageBox::Cancel:
            break;
        default:
            break;
    }
}

QString FilenameDialog::getFilename(){
    return filename;
}

FilenameDialog::~FilenameDialog(){
}
