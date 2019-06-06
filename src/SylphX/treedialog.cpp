#include "treedialog.h"

namespace SylphX {

TreeDialog::TreeDialog(){
    QString version(APP_VERSION);
    setWindowTitle(tr("Tree Software V") + version);
    createLayout();
}

void TreeDialog::createLayout(){
//    QWidget *mainWidget = new QWidget;

    //buttons
    int buttonWidth= 200;
    int buttonHeight = 50;
    recordingGUI = new QPushButton(tr("Recording GUI"));
    recordingGUI->setFixedSize(buttonWidth, buttonHeight);
    linkGUI = new QPushButton(tr("Link GUIs"));
    linkGUI->setFixedSize(buttonWidth, buttonHeight);
    stimulatorGUI = new QPushButton(tr("Stimulator GUI"));
    stimulatorGUI->setFixedSize(buttonWidth, buttonHeight);
    startGUI = new QPushButton(tr("Start Integration"));
    startGUI->setFixedHeight(buttonHeight);

    //layout
    QHBoxLayout *mainSubLayout = new QHBoxLayout;
    mainSubLayout->addWidget(recordingGUI);
    mainSubLayout->addWidget(linkGUI);
    mainSubLayout->addWidget(stimulatorGUI);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(mainSubLayout);
    mainLayout->addWidget(startGUI);

//    mainWidget->setLayout(mainLayout);
    setLayout(mainLayout);
//    setCentralWidget(mainWidget);
    mainLayout->setSizeConstraint( QLayout::SetFixedSize);
}

TreeDialog::~TreeDialog(){
}

}
