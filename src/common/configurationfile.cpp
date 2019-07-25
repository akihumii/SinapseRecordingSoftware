#include "configurationfile.h"

ConfigurationFile::ConfigurationFile(){
}

void ConfigurationFile::on_write_settings_changed(){
    filenameSettingsTemp = QFileDialog::getSaveFileName(this, tr("Save settings as..."), filenameSettingsDir, tr("Config Files (*.ini)"));
    if(!filenameSettingsTemp.isEmpty()){
        filenameSettings = filenameSettingsTemp;
//        addFilenameSettingsAll();
        changeFilenameSettingsDir();
        qDebug() << "User is writing configuration file as: " << filenameSettings;
        emit writeSettingsSignal();
    }
}

void ConfigurationFile::on_read_settings_changed(){
    filenameSettingsTemp = QFileDialog::getOpenFileName(this, tr("Open settings..."), filenameSettingsDir, tr("Config Files (*.ini)"));
    if(!filenameSettingsTemp.isEmpty()){
        filenameSettings = filenameSettingsTemp;
//        addFilenameSettingsAll();
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

//    readFilenameSettingsAll();
}

void ConfigurationFile::addFilenameSettingsAll(){
    filenameSettingsAll.prepend(filenameSettings);
    filenameSettingsAll.removeDuplicates();

//    QSettings settings(filenameSettingsMostRecent, QSettings::IniFormat);
//    settings.beginWriteArray("filenameSettingsAll");
//    for(int i = 0; i < filenameSettingsAll->size(); i++){
//        settings.setArrayIndex(i);
//        settings.setValue("filenameRecent", filenameSettingsAll->at(i));
//    }
//    settings.endArray();

    qDebug() << "local filename!!!!!!!!!!!!!!!!!!!!!!!!!!!!: " << filenameSettings;
    qDebug() << "local filenameSettingsAll!!!!!!!!!!!!!!!!!!!!!!!!!!!!: " << filenameSettingsAll;
//    qDebug() << "config file filenameSettingsAll!!!!!!!!!!!!!!!!!!!!!!!!!!!!: " << settings.value("filenameSettingsAll");
}

void ConfigurationFile::readFilenameSettingsAll(){
//    QSettings settings(filenameSettings, QSettings::IniFormat);
//    int size = settings.beginReadArray("filenameSettingsAll");
//    for(int i = 0; i < size; i++){
//        settings.setArrayIndex(i);
//        filenameSettingsAll.prepend(settings.value("filenameRecent").toString());
//    }
//    settings.endArray();
//    filenameSettingsAll->removeDuplicates();

//    qDebug() << "local filenameSettingsAll!!!!!!!!!!!!!!!!!!!!!!!!!!!!: " << filenameSettingsAll;
//    qDebug() << "config file filenameSettingsAll~: " << settings.value("filenameSettingsAll");
    qDebug() << "config file filenameSettingsAll~: ";
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
