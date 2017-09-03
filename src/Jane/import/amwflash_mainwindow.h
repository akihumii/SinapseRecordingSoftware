#ifndef AMWFLASH_MAINWINDOW_H
#define AMWFLASH_MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QtCore/QtGlobal>
#include <QtSerialPort/QSerialPort>

#include <import/console.h>
#include <import/settingdialog.h>

#include <QThread>
#include <QLabel>
#include <QDialog>
#include <QWidget>
#include <QGridLayout>
#include <QAction>
#include <QMenuBar>
#include <QPlainTextEdit>
#include <QLineEdit>

#include <QFile>
#include <QFileDialog>
#include <QTextStream>

class settingsDialog;
class amwFlash_mainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit amwFlash_mainWindow(QWidget *parent = nullptr);

signals:

public slots:
    void openSerialPort();
    void closeSerialPort();
    void writeData(const QByteArray &data);
    void readData();
    void readFeedback();
    void on_search_clicked();
    void on_flash_clicked();
    void on_saveReboot_clicked();

private:
    void createMenuBar();
    void createFlashMode();

    bool saveRebootFlag = false;
    bool openFileFlag = false;
    QLabel *status;

    console *mainConsole;
    QGridLayout *flashMode;
    QPushButton *searchButton;
    QLineEdit *fileNameEntry;
    QPlainTextEdit *fileDataEntry;
    QPushButton *flashButton;
    QPushButton *saveRebootButton;
    QFile *file;
    QString fileName;

    settingsDialog *settingsInit;
    QMenuBar *menuBar;
    QMenu *fileMenu;
    QMenu *settingMenu;

    QAction *connectAction;
    QAction *disconnectAction;
    QAction *exitAction;
    QAction *settingAction;
    QAction *clearSettingAction;

    QSerialPort *serialPort;
};

#endif // AMWFLASH_MAINWINDOW_H
