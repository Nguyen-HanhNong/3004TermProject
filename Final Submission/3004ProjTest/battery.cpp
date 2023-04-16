/*Author: Ashok Sivathayalan
File name: battery.cpp
Purpose: Stores the device's charge level and drains it as the application runs
         Sends signals to disable device when charge runs out*/

#include "battery.h"
#include <QVBoxLayout>

//Constructor, initializing all members
Battery::Battery(QWidget *p) : QWidget(p), percent(100)
{  
    //Creating slider to control charge, and label to display charge percent
    bSlider = new QSlider(Qt::Vertical);
    percentLabel = new QLabel("100%");
    bSlider->setRange(0, 100);
    bSlider->setSliderPosition(100);

    //Adding the widgets to the layout
    QVBoxLayout* lay = new QVBoxLayout(this);
    lay->addWidget(percentLabel);
    lay->addWidget(bSlider);

    //Connecting slots
    QObject::connect(&timer, &QTimer::timeout, this, &Battery::drainBattery);
    QObject::connect(bSlider, &QSlider::valueChanged, this, &Battery::updateBattery);

    //Starting battery drain
    timer.start(1800);
}

void Battery::updateBattery(){
    //If battery was at 0 and now is not, send charged signal
    if(percent == 0 && bSlider->sliderPosition() != 0) emit batteryCharged();
    //If battery was not at 0 and now is, send drained signal
    if(percent != 0 && bSlider->sliderPosition() == 0) emit batteryDrained();
    //Updating the percent member and the UI label
    percent = bSlider->sliderPosition();
    percentLabel->setText(QString::number(percent) + "%");
}

void Battery::drainBattery(){
    //If battery is going to hit 0, send drained signal
    if(percent == 1) emit batteryDrained();

    //Reducing battery level and updating slider to be accurate to charge level
    if(percent>0) --percent;
    bSlider->setSliderPosition(percent);
}

//Stopping the timer so that the battery will no longer drain
void Battery::stopBattery(){
    timer.stop();
}

//Starting the timer so the battery will start draining again
void Battery::startBattery(){
    timer.start(1000);
}
