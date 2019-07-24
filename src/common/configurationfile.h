#ifndef CONFIGURATIONFILE_H
#define CONFIGURATIONFILE_H

#include <QMainWindow>
#include "qtincludes.h"

class ConfigurationFile : public QMainWindow
{
    Q_OBJECT
public:
    ConfigurationFile();
    QString filenameSettings;

    QString getFilenameSettings();
    QStringList *getFilenameSettingsAll();

    void writeMostRecentSettings();
    void readMostRecentSettings();
    void setFilenameSettingsMostRecent(QString filename);

private:
    QStringList *filenameSettingsAll;
    QString filenameSettingsTemp;
    QString filenameSettingsDir = QDir::currentPath();
    QString filenameSettingsMostRecent = "mostRecent.ini";

    void changeFilenameSettingsDir();
    void addFilenameSettingsAll();
    void readFilenameSettingsAll();

public slots:
    void on_write_settings_changed();
    void on_read_settings_changed();

signals:
    void writeSettingsSignal();
    void readSettingsSignal();

};

#endif // CONFIGURATIONFILE_H
