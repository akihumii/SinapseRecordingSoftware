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
    QDialogButtonBox *buttonBox;

    QLineEdit *filenameLine;

private slots:
    void accept();
    void on_filename_changed();

signals:
    void filenameChanged();
    void filenameDiscard();
};

#endif // FILENAMEDIALOG_H
