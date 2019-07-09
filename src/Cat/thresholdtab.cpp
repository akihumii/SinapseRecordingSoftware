#include "thresholdtab.h"

ThresholdTab::ThresholdTab(){
    createLayout();
}

void ThresholdTab::createLayout(){
    //add button
    doneSettingsButton = new QPushButton;
    doneSettingsFlag ? doneSettingsButton->setText("Edit") : doneSettingsButton->setText("Done");
    doneSettingsButton->setFixedSize(35, 30);
    connect(doneSettingsButton, SIGNAL(clicked(bool)), this, SLOT(on_done_settings_changed()));

    QVBoxLayout *settingsButtonLayout = new QVBoxLayout();
    settingsButtonLayout->addWidget(doneSettingsButton);

    //channel labels
    QLabel *channelLabel[8];
    QHBoxLayout *settingsInputLabelLayout = new QHBoxLayout;
    for(int i = 0; i < 4; i++){
        channelLabel[i] = new QLabel("Ch " + QString::number(i+1));
        settingsInputLabelLayout->addWidget(channelLabel[i]);
    }

    QHBoxLayout *settingsOutputLabelLayout = new QHBoxLayout;
    for(int i = 4; i < 8; i++){
        channelLabel[i] = new QLabel("Ch " + QString::number(i-3));
        settingsOutputLabelLayout->addWidget(channelLabel[i]);
    }

    //Layout
    settingsInputLayout = new QVBoxLayout;
    settingsInputLayout->addLayout(settingsInputLabelLayout);

    settingsOutputLayout = new QVBoxLayout;
    settingsOutputLayout->addLayout(settingsOutputLabelLayout);

    //Mapper
    inputMapperCh1 = new QSignalMapper(this);
    inputMapperCh2 = new QSignalMapper(this);
    inputMapperCh3 = new QSignalMapper(this);
    inputMapperCh4 = new QSignalMapper(this);
    outputMapperCh1 = new QSignalMapper(this);
    outputMapperCh2 = new QSignalMapper(this);
    outputMapperCh3 = new QSignalMapper(this);
    outputMapperCh4 = new QSignalMapper(this);
    connect(inputMapperCh1, SIGNAL(mapped(int)), this, SLOT(on_input_ch1_changed(int)));
    connect(inputMapperCh2, SIGNAL(mapped(int)), this, SLOT(on_input_ch2_changed(int)));
    connect(inputMapperCh3, SIGNAL(mapped(int)), this, SLOT(on_input_ch3_changed(int)));
    connect(inputMapperCh4, SIGNAL(mapped(int)), this, SLOT(on_input_ch4_changed(int)));
    connect(outputMapperCh1, SIGNAL(mapped(int)), this, SLOT(on_output_ch1_changed(int)));
    connect(outputMapperCh2, SIGNAL(mapped(int)), this, SLOT(on_output_ch2_changed(int)));
    connect(outputMapperCh3, SIGNAL(mapped(int)), this, SLOT(on_output_ch3_changed(int)));
    connect(outputMapperCh4, SIGNAL(mapped(int)), this, SLOT(on_output_ch4_changed(int)));

    removeMapper = new QSignalMapper(this);
    connect(removeMapper, SIGNAL(mapped(int)), this, SLOT(on_remove_checkbox_clicked(int)));

    //Row of checkboxes
    QHBoxLayout *removeSubLayout[indexThreshold];
    QLabel *removeLabel[indexThreshold];
    for(int i = 0; i < indexThreshold; i++){
        createSettingsLayout(i);  // create input and output boxes
        removeSettingsButton[i] = new QPushButton("-");
        removeSettingsButton[i]->setFixedSize(boxWidth, boxHeight);
        connect(removeSettingsButton[i], SIGNAL(clicked(bool)), removeMapper, SLOT(map()));
        removeMapper->setMapping(removeSettingsButton[i], i);

        removeSubLayout[i] = new QHBoxLayout;
        removeLabel[i] = new QLabel(QString::number(i+1));
        removeSubLayout[i]->addWidget(removeLabel[i]);
        removeSubLayout[i]->addWidget(removeSettingsButton[i]);
        settingsButtonLayout->addLayout(removeSubLayout[i]);
//        settingsButtonLayout->setAlignment(removeSettingsButton[i], Qt::AlignRight);
    }
    QLabel *emptyInputRow = new QLabel;
    QLabel *emptyOutputRow = new QLabel;

    settingsInputLayout->addWidget(emptyInputRow);
    settingsOutputLayout->addWidget(emptyOutputRow);

    QLabel *emptyButton = new QLabel;
    emptyButton->setFixedSize(boxWidth, boxHeight);

    addSettingsButton = new QPushButton("+");
    addSettingsButton->setFixedSize(35, boxHeight);
    connect(addSettingsButton, SIGNAL(clicked(bool)), this, SLOT(on_add_checkbox_clicked()));
    if(indexThreshold < 30){
        settingsButtonLayout->addWidget(addSettingsButton);
//        settingsButtonLayout->setAlignment(addSettingsButton, Qt::AlignRight);
    }
    else{
        settingsButtonLayout->addWidget(emptyButton);
    }

    //Grouping
    settingsInputGroup = new QGroupBox(tr("Input"));
    settingsInputGroup->setLayout(settingsInputLayout);

    settingsOutputGroup = new QGroupBox(tr("Output"));
    settingsOutputGroup->setLayout(settingsOutputLayout);

    IOLayout = new QHBoxLayout;
    IOLayout->addLayout(settingsButtonLayout);
    IOLayout->addWidget(settingsInputGroup);
    IOLayout->addWidget(settingsOutputGroup);

    setLayout(IOLayout);
}

void ThresholdTab::on_done_settings_changed(){
    doneSettingsFlag = !doneSettingsFlag;

    if(doneSettingsFlag){
        if(!check_input_boxes()){  //no repeated input
            doneSettingsButton->setText("Edit");
            emit updateStatusBarSignal(tr("Ready..."));
        }
        else{
            doneSettingsFlag = !doneSettingsFlag;
            emit updateStatusBarSignal(tr("<b><FONT COLOR='#ff0000' FONT SIZE = 4> Repeated input sets: ") +
                                 QString::number(repeatedLocs[0]) + tr(", ") + QString::number(repeatedLocs[1]) + tr(" ..."));
        }
    }
    else{
        doneSettingsButton->setText("Done");
        emit updateStatusBarSignal(tr("Editting threshold input output..."));
    }

    // set enable
    settingsInputGroup->setEnabled(!doneSettingsFlag);
    settingsOutputGroup->setEnabled(!doneSettingsFlag);
    for(int i = 0; i < indexThreshold; i++){
        removeSettingsButton[i]->setEnabled(!doneSettingsFlag);
    }
    addSettingsButton->setEnabled(!doneSettingsFlag);
}

bool ThresholdTab::check_input_boxes(){
    bool seen[255] = {false};
    int loc[255] = {0};
    for(int i = 0; i < indexThreshold; i++){
        if(seen[inputCheckBoxValue[i]]){
            repeatedLocs[0] = loc[inputCheckBoxValue[i]];
            repeatedLocs[1] = i+1;
            return true;
        }
        else{
            seen[inputCheckBoxValue[i]] = true;
            loc[inputCheckBoxValue[i]] = i+1;
        }
    }
    return false;
}

void ThresholdTab::on_add_checkbox_clicked(){
    indexThreshold ++;
    emit refreshLayoutSignal();
}

void ThresholdTab::on_remove_checkbox_clicked(int index){
    for(int i = index; i < 29; i++){
        inputCheckBoxValue[i] = inputCheckBoxValue[i+1];
        outputCheckBoxValue[i] = outputCheckBoxValue[i+1];
    }
    inputCheckBoxValue[29] = 0;
    outputCheckBoxValue[29] = 0;

    indexThreshold --;
    emit refreshLayoutSignal();
}

void ThresholdTab::createSettingsLayout(int index){
    //Boxes
    inputBoxCh1[index] = new QCheckBox;
    inputBoxCh2[index] = new QCheckBox;
    inputBoxCh3[index] = new QCheckBox;
    inputBoxCh4[index] = new QCheckBox;
    outputBoxCh1[index] = new QCheckBox;
    outputBoxCh2[index] = new QCheckBox;
    outputBoxCh3[index] = new QCheckBox;
    outputBoxCh4[index] = new QCheckBox;

    //Size
    inputBoxCh1[index]->setMinimumSize(boxWidth, boxHeight);
    inputBoxCh2[index]->setMinimumSize(boxWidth, boxHeight);
    inputBoxCh3[index]->setMinimumSize(boxWidth, boxHeight);
    inputBoxCh4[index]->setMinimumSize(boxWidth, boxHeight);
    outputBoxCh1[index]->setMinimumSize(boxWidth, boxHeight);
    outputBoxCh2[index]->setMinimumSize(boxWidth, boxHeight);
    outputBoxCh3[index]->setMinimumSize(boxWidth, boxHeight);
    outputBoxCh4[index]->setMinimumSize(boxWidth, boxHeight);

    //Checks
    inputCheckBoxValue[index] & (1 << 0) ? inputBoxCh1[index]->setChecked(true) : inputBoxCh1[index]->setChecked(false);
    inputCheckBoxValue[index] & (1 << 1) ? inputBoxCh2[index]->setChecked(true) : inputBoxCh2[index]->setChecked(false);
    inputCheckBoxValue[index] & (1 << 2) ? inputBoxCh3[index]->setChecked(true) : inputBoxCh3[index]->setChecked(false);
    inputCheckBoxValue[index] & (1 << 3) ? inputBoxCh4[index]->setChecked(true) : inputBoxCh4[index]->setChecked(false);

    outputCheckBoxValue[index] & (1 << 0) ? outputBoxCh1[index]->setChecked(true) : outputBoxCh1[index]->setChecked(false);
    outputCheckBoxValue[index] & (1 << 1) ? outputBoxCh2[index]->setChecked(true) : outputBoxCh2[index]->setChecked(false);
    outputCheckBoxValue[index] & (1 << 2) ? outputBoxCh3[index]->setChecked(true) : outputBoxCh3[index]->setChecked(false);
    outputCheckBoxValue[index] & (1 << 3) ? outputBoxCh4[index]->setChecked(true) : outputBoxCh4[index]->setChecked(false);


    //Connect
    connect(inputBoxCh1[index], SIGNAL(toggled(bool)), inputMapperCh1, SLOT(map()));
    connect(inputBoxCh2[index], SIGNAL(toggled(bool)), inputMapperCh2, SLOT(map()));
    connect(inputBoxCh3[index], SIGNAL(toggled(bool)), inputMapperCh3, SLOT(map()));
    connect(inputBoxCh4[index], SIGNAL(toggled(bool)), inputMapperCh4, SLOT(map()));
    connect(outputBoxCh1[index], SIGNAL(toggled(bool)), outputMapperCh1, SLOT(map()));
    connect(outputBoxCh2[index], SIGNAL(toggled(bool)), outputMapperCh2, SLOT(map()));
    connect(outputBoxCh3[index], SIGNAL(toggled(bool)), outputMapperCh3, SLOT(map()));
    connect(outputBoxCh4[index], SIGNAL(toggled(bool)), outputMapperCh4, SLOT(map()));
    inputMapperCh1->setMapping(inputBoxCh1[index], index);
    inputMapperCh2->setMapping(inputBoxCh2[index], index);
    inputMapperCh3->setMapping(inputBoxCh3[index], index);
    inputMapperCh4->setMapping(inputBoxCh4[index], index);
    outputMapperCh1->setMapping(outputBoxCh1[index], index);
    outputMapperCh2->setMapping(outputBoxCh2[index], index);
    outputMapperCh3->setMapping(outputBoxCh3[index], index);
    outputMapperCh4->setMapping(outputBoxCh4[index], index);


    //Layout
    settingsInputSubLayout[index] = new QHBoxLayout;
    settingsOutputSubLayout[index] = new QHBoxLayout;

    settingsInputSubLayout[index]->addWidget(inputBoxCh1[index]);
    settingsInputSubLayout[index]->addWidget(inputBoxCh2[index]);
    settingsInputSubLayout[index]->addWidget(inputBoxCh3[index]);
    settingsInputSubLayout[index]->addWidget(inputBoxCh4[index]);

    settingsOutputSubLayout[index]->addWidget(outputBoxCh1[index]);
    settingsOutputSubLayout[index]->addWidget(outputBoxCh2[index]);
    settingsOutputSubLayout[index]->addWidget(outputBoxCh3[index]);
    settingsOutputSubLayout[index]->addWidget(outputBoxCh4[index]);

    settingsInputLayout->addLayout(settingsInputSubLayout[index]);
    settingsOutputLayout->addLayout(settingsOutputSubLayout[index]);
}

void ThresholdTab::on_input_ch1_changed(int index){
    inputBoxCh1[index]->isChecked() ? (inputCheckBoxValue[index] |= (1 << 0)) : (inputCheckBoxValue[index] &= ~(1 << 0));
}

void ThresholdTab::on_input_ch2_changed(int index){
    inputBoxCh2[index]->isChecked() ? (inputCheckBoxValue[index] |= (1 << 1)) : (inputCheckBoxValue[index] &= ~(1 << 1));
}

void ThresholdTab::on_input_ch3_changed(int index){
    inputBoxCh3[index]->isChecked() ? (inputCheckBoxValue[index] |= (1 << 2)) : (inputCheckBoxValue[index] &= ~(1 << 2));
}

void ThresholdTab::on_input_ch4_changed(int index){
    inputBoxCh4[index]->isChecked() ? (inputCheckBoxValue[index] |= (1 << 3)) : (inputCheckBoxValue[index] &= ~(1 << 3));
}

void ThresholdTab::on_output_ch1_changed(int index){
    outputBoxCh1[index]->isChecked() ? (outputCheckBoxValue[index] |= (1 << 0)) : (outputCheckBoxValue[index] &= ~(1 << 0));
}

void ThresholdTab::on_output_ch2_changed(int index){
    outputBoxCh2[index]->isChecked() ? (outputCheckBoxValue[index] |= (1 << 1)) : (outputCheckBoxValue[index] &= ~(1 << 1));
}

void ThresholdTab::on_output_ch3_changed(int index){
    outputBoxCh3[index]->isChecked() ? (outputCheckBoxValue[index] |= (1 << 2)) : (outputCheckBoxValue[index] &= ~(1 << 2));
}

void ThresholdTab::on_output_ch4_changed(int index){
    outputBoxCh4[index]->isChecked() ? (outputCheckBoxValue[index] |= (1 << 3)) : (outputCheckBoxValue[index] &= ~(1 << 3));
}



