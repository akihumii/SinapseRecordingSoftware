#ifndef TREEDIALOG_H
#define TREEDIALOG_H

#include <QMainWindow>
#include "../common/qtincludes.h"
#include "mainwindow.h"
#include "../Odin/odinwindow.h"
#include "../Cat/catwindow.h"
#include "recordingdialog.h"

namespace SylphX {

class TreeDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit TreeDialog();
    ~TreeDialog();

    MainWindow *sylphxGUI;
    Odin::OdinWindow *odinGUI;
    Cat::CatWindow *catGUI;
    RecordingDialog *recordingGUI;

private:
    QPushButton *button_recordingGUI;
    QPushButton *button_catGUI;
    QPushButton *button_stimulatorGUI;
    QPushButton *button_startGUI;

    void createLayout();

private slots:
    void on_recordingGUI_clicked();
    void on_stimulatorGUI_clicked();
    void on_catGUI_clicked();

signals:
    void sendParameters();
};

}

#endif // TREEDIALOG_H
