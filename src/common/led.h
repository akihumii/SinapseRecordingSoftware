#ifndef LED_H
#define LED_H

#include <QPainter>
#include "qtincludes.h"

class Led : public QWidget{
    Q_OBJECT
    Q_PROPERTY(bool power READ power WRITE setPower NOTIFY powerChanged)
    Led(const Led&)=delete;
    Led& operator=(const Led&)=delete;
public:
    explicit Led(QWidget* parent=nullptr)
        :QWidget(parent)
        , m_power(false)
    {}
    bool power() const
    {
        return m_power;
    }
public slots:
    void setPower(bool power)
    {
        if(power!=m_power){
            m_power = power;
            emit powerChanged();
            update();
        }
    }
    void blink(int msecDuration){
        setPower(true);
        QTimer::singleShot(msecDuration, [=] {
                setPower(false);
        });
    }

signals:
    void powerChanged();
protected:
    virtual void paintEvent(QPaintEvent *event) override{
        Q_UNUSED(event)
        QPainter ledPainter(this);
        ledPainter.setPen(Qt::black);
        if(m_power)
            ledPainter.setBrush(Qt::green);
        else
            ledPainter.setBrush(Qt::gray);
        ledPainter.drawEllipse(0, 0, 12, 12);
    }
private:
    bool m_power;
};

#endif // LED_H
