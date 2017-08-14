#include "mainwindow.h"
#include "amwflash_mainwindow.h"

MainWindow::MainWindow()
{
    amwFlash_init = new amwFlash_mainWindow;

    createLayout();
    createAction();
}

MainWindow::~MainWindow()
{

}

void MainWindow::createLayout()
{   QLabel *modeLabel = new QLabel;
    modeLabel -> setText("Mode: ");

    modeComboBox = new QComboBox;
    modeComboBox -> addItem("Reset (housekeeping)");
    modeComboBox -> addItem("Digital Command Loopback");
    modeComboBox -> addItem("Data BER Assessment");
    modeComboBox -> addItem("JTAG Programming Mode");
    modeComboBox -> addItem("Stimulator Parameter Mode");
    modeComboBox -> addItem("Stimulator Sequence Program Mode");
    modeComboBox -> addItem("Stimulator Trigger Command");
    modeComboBox -> addItem("Oscillator Clock Tuning Mode");
    modeComboBox -> addItem("Analog Measurement Mode (8 Bits)");
    modeComboBox -> addItem("Bioimpedance Measurement Mode (8 Bits)");

    QLabel *chipIDLabel = new QLabel;
    chipIDLabel -> setText("chip ID: ");

    chipIDComboBox = new QComboBox;
    for(int i = 0; i < 8; i++)
        chipIDComboBox -> addItem("Chip " + QString::number(i));

    QGroupBox *modeGroupbox = new QGroupBox(tr("Set 'Mode' and 'Chip ID'"));
    QFormLayout *modeLayout = new QFormLayout;
    modeLayout -> addRow(modeLabel, modeComboBox);
    modeLayout -> addRow(chipIDLabel, chipIDComboBox);
    modeGroupbox -> setLayout(modeLayout);


    amwFlashButton = new QPushButton(tr("amw flash"));

    mainLayout = new QVBoxLayout();
    mainLayout -> addWidget(modeGroupbox);
    mainLayout -> addWidget(amwFlashButton);

    setCentralWidget(new QWidget);
    centralWidget() -> setLayout(mainLayout);
}

void MainWindow::createAction()
{
    connect(modeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_mode_changed(int)));
    connect(amwFlashButton, SIGNAL(clicked()), amwFlash_init, SLOT(show()));
}

void MainWindow::on_mode_changed(int mode)
{

}

