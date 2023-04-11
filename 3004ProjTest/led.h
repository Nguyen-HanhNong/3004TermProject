#ifndef LED_H
#define LED_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QDebug>

class LED: public QWidget
{
    Q_OBJECT
    Q_PROPERTY(bool on READ isOn WRITE setOn)
public:
    //Default colour of yellow when on, gray when off
    LED(QWidget* = nullptr, const QColor = QColor(255, 255, 0), const QColor = QColor(137, 137, 137));

    //Getters & Setters
    bool isOn(){return on;}
    void setOn(bool);
    void setColour(const QColor);

public slots:
    void turnOn(){setOn(true);}
    void turnOff(){setOn(false);}

protected:
    //Used to redraw icon in desired colour
    void paintEvent(QPaintEvent* ) override;

private:
    bool on; //Stores state of the light
    QColor onColour; //Colour when light is on
    QColor offColour; //Colour when light is off
};

#endif // LED_H
