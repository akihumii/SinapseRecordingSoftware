#ifndef FILENAMEDIALOG_H
#define FILENAMEDIALOG_H

#include <QDialog>
#include "qtincludes.h"
#include "data.h"

using namespace std;

class FileNameDialog : public QDialog
{
    Q_OBJECT

public:
    FileNameDialog(Data *data_);
    ~FileNameDialog();

private:
    Data *data;
    QLabel *FileNameLabel;
    QLineEdit *FileNameLineEdit;
    QPushButton *Ok;
    QPushButton *Cancel;

private slots:
    void on_OkButton_clicked();
    void on_CancelButton_clicked();
};

#endif // FILENAMEDIALOG_H
