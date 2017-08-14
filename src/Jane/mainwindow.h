#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../common/qtincludes.h"
#include "amwflash_mainwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();
    void createLayout();
    void createAction();

private slots:
    void on_mode_changed(int mode);
    //void on_chipID_changed(int IDNum);

private:
    QComboBox *modeComboBox;
    QComboBox *chipIDComboBox;

    QPushButton *amwFlashButton;
    QVBoxLayout *mainLayout;

    amwFlash_mainWindow *amwFlash_init;
};

#endif // MAINWINDOW_H
