/*Author: Ashok Sivathayalan
File name: led.cpp
Purpose: Acts as simulated light that can be turned on/off and to various colours
         Used for breath pacer lights, HRV signal indicator, and coherence level light*/

#include "led.h"

//Initializing all the members in the constructor
LED::LED(QWidget* p, const QColor onC, const QColor offC): QWidget(p), on(false), onColour(onC), offColour(offC){
}

//Used to set the state of the light
void LED::setOn(bool newState){
    //No need to do anything if state is not changing
    if(newState == on) return;
    //If the state has changed, update displayed light
    on = newState;
    update();
}

//Setting the onColour of the light
void LED::setColour(const QColor newC){
    //Setting the member variable and redrawing the light
    this->onColour = newC;
    update();
}

//Used for displaying the light in the window
void LED::paintEvent(QPaintEvent* ){
    //Creating a QPainter and adding antialiasing for appearance
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    //Selecting which colour to draw in 
    if(this->on) painter.setBrush(onColour);
    else painter.setBrush(offColour);
    //Drawing the light
    painter.drawRect(0, 0, width(), height());
}
