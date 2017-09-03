#ifndef CONSOLE_H
#define CONSOLE_H

#include <QPlainTextEdit>
#include <QScrollBar>
#include <QtCore/QDebug>

class console : public QPlainTextEdit
{
    Q_OBJECT

signals:
    void getData(const QByteArray &data);

public:
    explicit console(QWidget *parent = nullptr);

    void putData(const QByteArray &data);
    void setLocalEchoEnabled(bool set);

protected:
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void contextMenuEvent(QContextMenuEvent *e) Q_DECL_OVERRIDE;

private:
    bool localEchoEnabled;
};

#endif // CONSOLE_H
