/*Author: Ashok Sivathayalan
File name: lightstrip.h
Purpose: Simulated strip of lights that goes back and forth based on timer
         Used for breath pacer to indicate to user how long breaths should take*/
         
#ifndef LIGHTSTRIP_H
#define LIGHTSTRIP_H

#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QDebug>
#include "led.h"

class LightStrip: public QWidget
{
    Q_OBJECT
public:
    LightStrip(QWidget* = nullptr);
    ~LightStrip();

public slots:

    //Used to start/stop the lights at the start and end of a session
    void start();
    void stop();
    
    //Adjusts timing of lights
    //Pass in time for full breath in milliseconds
    void setInterval(int);

private:
    LED** lights; //UI element to simulate lighting
    QTimer timer; //Used to control lighting of elements
    bool dir; //Indicates which direction the lighting is moving
    int curOn; //Stores the light that is on, 1-indexed with 0 meaning all are off
    int interval; //Interval in milliseconds for each breath

private slots:
    //Used privately by timer to change lights
    void nextLight();
};

#endif // LIGHTSTRIP_H
