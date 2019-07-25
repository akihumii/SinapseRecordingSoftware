#include "configurationfile.h"

ConfigurationFile::ConfigurationFile(){
}

void ConfigurationFile::on_write_settings_changed(){
    filenameSettingsTemp = QFileDialog::getSaveFileName(this, tr("Save settings as..."), filenameSettingsDir, tr("Config Files (*.ini)"));
    if(!filenameSettingsTemp.isEmpty()){
        filenameSettings = filenameSettingsTemp;
        changeFilenameSettingsDir();
        qDebug() << "User is writing configuration file as: " << filenameSettings;
        emit writeSettingsSignal();
    }
}

void ConfigurationFile::on_read_settings_changed(){
    filenameSettingsTemp = QFileDialog::getOpenFileName(this, tr("Open settings..."), filenameSettingsDir, tr("Config Files (*.ini)"));
    if(!filenameSettingsTemp.isEmpty()){
        filenameSettings = filenameSettingsTemp;
        changeFilenameSettingsDir();
        qDebug() << "User is opening configuration file: " << filenameSettings;
        emit readSettingsSignal();
    }
}

void ConfigurationFile::writeMostRecentSettings(){
    filenameSettings = filenameSettingsMostRecent;
    emit writeSettingsSignal();
    qDebug() << "Most recent configuration file has been saved as: " << filenameSettings;
}

void ConfigurationFile::readMostRecentSettings(){
    filenameSettings = filenameSettingsMostRecent;
    if(QFile::exists(filenameSettings)){
        emit readSettingsSignal();
        qDebug() << "Loaded most recent configuration file: " << filenameSettings;
    }
}

void ConfigurationFile::changeFilenameSettingsDir(){
    QFileInfo fi(filenameSettings);
    filenameSettingsDir = fi.filePath();
}

void ConfigurationFile::setFilenameSettingsMostRecent(QString filename){
    filenameSettingsMostRecent = filename;
}

QString ConfigurationFile::getFilenameSettings(){
    return filenameSettings;
}

QStringList ConfigurationFile::getFilenameSettingsAll(){
    return filenameSettingsAll;
}
