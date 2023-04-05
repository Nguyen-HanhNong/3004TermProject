#include "lightstrip.h"
#include <QHBoxLayout>
#include <QLabel>

LightStrip::LightStrip(QWidget* p): QWidget(p), dir(true), curOn(0), interval(5000)
{
    //Connecting timer to call nextLight
    QObject::connect(&timer, &QTimer::timeout, this, &LightStrip::nextLight);

    //Creating and organizing UI elements
    lights = new LED*[5];
    QHBoxLayout* hbox = new QHBoxLayout(this);
    for(int i=0;i<5;i++){
        lights[i] = new LED();
        hbox->addWidget(lights[i]);
    }

}

LightStrip::~LightStrip(){
    for(int i=0;i<5;i++) delete lights[i];
    delete[] lights;
}

void LightStrip::start(){
    lights[0]->turnOn();
    curOn++;
    timer.start(interval/4);
}

void LightStrip::stop(){
    timer.stop();
    lights[curOn-1]->turnOff();
    curOn = 0;
}

void LightStrip::nextLight(){
    lights[curOn-1]->turnOff();
    if(dir) ++curOn;
    else --curOn;
    //If the currently active light is on the end, switch directions
    if(curOn == 1 || curOn == 5) dir = !dir;
    lights[curOn-1]->turnOn();
}

void LightStrip::setInterval(int newInt){
    this->interval = newInt;
}
