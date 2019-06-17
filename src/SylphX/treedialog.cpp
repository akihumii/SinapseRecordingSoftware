#include "treedialog.h"

namespace SylphX {

TreeDialog::TreeDialog(){
    QString version(APP_VERSION);
    setWindowTitle(tr("Tree Software V") + version);
    createLayout();

    sylphxGUI = new MainWindow;  //SylphX
    sylphxGUI->setMinimumSize(1366, 768);
    sylphxGUI->hide();

    odinGUI = new Odin::OdinWindow();  //Odin
    odinGUI->setFixedSize(odinGUI->sizeHint());
    odinGUI->hide();

    catGUI = new Cat::CatWindow();  //Cat
    catGUI->setFixedSize(catGUI->sizeHint());
    catGUI->hide();

    connect(sylphxGUI, SIGNAL(showCatSignal()), this, SLOT(on_catGUI_clicked()));
    connect(sylphxGUI, SIGNAL(showOdinSignal()), this, SLOT(on_stimulatorGUI_clicked()));

    connect(odinGUI, SIGNAL(commandSent(char*)), sylphxGUI->dataProcessor, SLOT(setLastSentBytes(char*)));
    connect(odinGUI, SIGNAL(amplitudeChanged(double*)), sylphxGUI->dataProcessor, SLOT(setLastSentAmplitudes(double*)));
    connect(odinGUI, SIGNAL(debounceEditted(int)), sylphxGUI->dataProcessor, SLOT(setDebounce(int)));
    connect(odinGUI, SIGNAL(upperThresholdEditted(double)), sylphxGUI->dataProcessor, SLOT(setUpperThreshold(double)));
    connect(odinGUI, SIGNAL(lowerThresholdEditted(double)), sylphxGUI->dataProcessor, SLOT(setLowerThreshold(double)));
    connect(odinGUI, SIGNAL(commandSent(char*)), catGUI, SLOT(setRpiCommand(char*)));
    connect(odinGUI, SIGNAL(showSylphXSignal()), this, SLOT(on_recordingGUI_clicked()));
    connect(odinGUI, SIGNAL(showCatSignal()), this, SLOT(on_catGUI_clicked()));

    connect(catGUI, SIGNAL(commandSent(char*)), sylphxGUI, SLOT(sendParameter(char*)));
    connect(catGUI, SIGNAL(highpassSent(QVector<double>*)), sylphxGUI->dataProcessor, SLOT(sendHighpassFilter(QVector<double>*)));
    connect(catGUI, SIGNAL(lowpassSent(QVector<double>*)), sylphxGUI->dataProcessor, SLOT(sendLowpassFilter(QVector<double>*)));
    connect(catGUI, SIGNAL(notchSent(QVector<double>*)), sylphxGUI->dataProcessor, SLOT(sendNotchFilter(QVector<double>*)));
    connect(catGUI, SIGNAL(showOdinSignal()), this, SLOT(on_stimulatorGUI_clicked()));
    connect(catGUI, SIGNAL(showSylphSignal()), this, SLOT(on_recordingGUI_clicked()));

    connect(this, SIGNAL(sendParameters()), odinGUI, SLOT(sendParameter()));

    emit sendParameters();  //send initial parameters to Rpi
}

void TreeDialog::createLayout(){
    QWidget *mainWidget = new QWidget;

    //buttons
    int buttonWidth= 200;
    int buttonHeight = 50;
    button_recordingGUI = new QPushButton(tr("&Recording GUI"));
    button_recordingGUI->setFixedSize(buttonWidth, buttonHeight);
    button_recordingGUI->setShortcut(tr("Ctrl+R"));
    connect(button_recordingGUI, SIGNAL(clicked(bool)), this, SLOT(on_recordingGUI_clicked()));

    button_catGUI = new QPushButton(tr("Classifica&tion GUI"));
    button_catGUI->setFixedSize(buttonWidth, buttonHeight);
    button_catGUI->setShortcut(tr("Ctrl+T"));
    connect(button_catGUI, SIGNAL(clicked(bool)), this, SLOT(on_catGUI_clicked()));

    button_stimulatorGUI = new QPushButton(tr("St&imulator GUI"));
    button_stimulatorGUI->setFixedSize(buttonWidth, buttonHeight);
    button_stimulatorGUI->setShortcut(tr("Ctrl+I"));
    connect(button_stimulatorGUI, SIGNAL(clicked(bool)), this, SLOT(on_stimulatorGUI_clicked()));

    button_startGUI = new QPushButton(tr("Start Integration"));
    button_startGUI->setFixedHeight(buttonHeight);

    //layout
    QHBoxLayout *mainSubLayout = new QHBoxLayout;
    mainSubLayout->addWidget(button_recordingGUI);
    mainSubLayout->addWidget(button_catGUI);
    mainSubLayout->addWidget(button_stimulatorGUI);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(mainSubLayout);
    mainLayout->addWidget(button_startGUI);

    mainWidget->setLayout(mainLayout);
//    setLayout(mainLayout);
    setCentralWidget(mainWidget);
    mainLayout->setSizeConstraint( QLayout::SetFixedSize);
}

void TreeDialog::on_recordingGUI_clicked(){
    sylphxGUI->show();
}

void TreeDialog::on_stimulatorGUI_clicked(){
    odinGUI->show();
}

void TreeDialog::on_catGUI_clicked(){
    catGUI->show();
}

TreeDialog::~TreeDialog(){
}

}
