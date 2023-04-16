/*Author: Ashok Sivathayalan
File name: led.h
Purpose: Acts as simulated light that can be turned on/off and to various colours
         Used for breath pacer lights, HRV signal indicator, and coherence level light*/

#ifndef LED_H
#define LED_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QDebug>

class LED: public QWidget
{
    //Stating that it is a QObject and has an 'on' property
    Q_OBJECT
    Q_PROPERTY(bool on READ isOn WRITE setOn)

public:
    //Constructor: takes in parent widget, oncolour, and offcolour: 
    //default onColour is yellow, default offColour is gray
    LED(QWidget* = nullptr, const QColor = QColor(255, 255, 0), const QColor = QColor(137, 137, 137));

    //Getters & Setters
    bool isOn(){return on;}
    void setOn(bool);
    void setColour(const QColor);

public slots:
    //Setters as slots so they can be used by other classes' signals
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
