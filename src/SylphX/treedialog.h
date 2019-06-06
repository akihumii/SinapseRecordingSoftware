#ifndef TREEDIALOG_H
#define TREEDIALOG_H

#include <QDialog>
#include "../common/qtincludes.h"

namespace SylphX {

class TreeDialog : public QDialog
{
    Q_OBJECT

public:
    TreeDialog();
    ~TreeDialog();

private:
    QPushButton *recordingGUI;
    QPushButton *linkGUI;
    QPushButton *stimulatorGUI;
    QPushButton *startGUI;

    void createLayout();
};

}

#endif // TREEDIALOG_H
