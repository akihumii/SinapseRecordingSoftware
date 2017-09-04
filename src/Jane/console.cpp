#include "console.h"

console::console(QWidget *parent): QPlainTextEdit(parent), localEchoEnabled(false)
{
    document()->setMaximumBlockCount(150);
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::green);
    setPalette(p);
}

void console::putData(const QByteArray &data){
    insertPlainText(QString(data));
    QScrollBar *bar = verticalScrollBar();
    bar -> setValue(bar -> maximum());
}

void console::setLocalEchoEnabled(bool set)
{
    localEchoEnabled = set;
}

void console::keyPressEvent(QKeyEvent *e){
    switch (e->key()) {
    case Qt::Key_Backspace:
    case Qt::Key_Left:
    case Qt::Key_Right:
    case Qt::Key_Up:
    case Qt::Key_Down:
        break;
    default:
        if (localEchoEnabled)
            QPlainTextEdit::keyPressEvent(e);
        emit getData(e->text().toLocal8Bit());
    }
}
void console::mousePressEvent(QMouseEvent *e){
    Q_UNUSED(e)
    setFocus();
}

void console::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
}

void console::contextMenuEvent(QContextMenuEvent *e)
{
    Q_UNUSED(e)
}
