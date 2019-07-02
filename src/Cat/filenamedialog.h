#ifndef FILENAMEDIALOG_H
#define FILENAMEDIALOG_H

#include <QDialog>
#include "../common/qtincludes.h"

namespace Ui {
class FilenameDialog;
}

class FilenameDialog : public QDialog
{
    Q_OBJECT

public:
    FilenameDialog();
    ~FilenameDialog();
    void setFilename(QString name);
    QString getFilename();

private:
    void createLayout();
    void reject();

    QString filename;

    QLineEdit *filenameLine;

private slots:
    void on_filename_changed();

signals:
    void filenameChanged();
};

#endif // FILENAMEDIALOG_H
