/*Author: Ashok Sivathayalan
File name: lightstrip.cpp
Purpose: Simulated strip of lights that goes back and forth based on timer
         Used for breath pacer to indicate to user how long breaths should take*/

#include "lightstrip.h"
#include <QHBoxLayout>
#include <QLabel>

LightStrip::LightStrip(QWidget* p): QWidget(p), dir(true), curOn(0), interval(10000)
{
    //Connecting timer to call nextLight
    QObject::connect(&timer, &QTimer::timeout, this, &LightStrip::nextLight);

    //Creating layout to organize UI elements
    QHBoxLayout* hbox = new QHBoxLayout(this);

    //Creating the LED lights and adding them to the layout
    lights = new LED*[5];
    for(int i=0;i<5;i++){
        lights[i] = new LED();
        hbox->addWidget(lights[i]);
    }
}

//Deleting the lights and the array they are stored in
LightStrip::~LightStrip(){
    for(int i=0;i<5;i++) delete lights[i];
    delete[] lights;
}

void LightStrip::start(){
    //Do nothing if already running
    if(curOn != 0) return;

    //Turning on the first light and starting the timer to switch lights automatically
    lights[0]->turnOn();
    curOn++;
    timer.start(interval/4);
}

void LightStrip::stop(){
    //Do nothing if not running
    if(curOn == 0) return;

    //Stopping the timer, turning off the current light
    timer.stop();
    lights[curOn-1]->turnOff();
    curOn = 0;
}

void LightStrip::nextLight(){
    //Turning off the last light
    lights[curOn-1]->turnOff();
    //Going to the next light
    if(dir) ++curOn;
    else --curOn;
    //If the currently active light is on the end, switch directions
    if(curOn == 1 || curOn == 5) dir = !dir;
    //Turning on the light
    lights[curOn-1]->turnOn();
}

//To change the timing of the lights
//Takes in the time for every light to be lit, in milliseconds
void LightStrip::setInterval(int newInt){
    this->interval = newInt;
}
