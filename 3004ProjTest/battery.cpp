#include "battery.h"
#include <QVBoxLayout>

Battery::Battery(QWidget *p) : QWidget(p), percent(100)
{
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
    timer.start(100);
}

void Battery::updateBattery(){
    if(percent == 0 && bSlider->sliderPosition() != 0) emit batteryCharged();
    percent = bSlider->sliderPosition();
    percentLabel->setText(QString::number(percent) + "%");
}

void Battery::drainBattery(){
    if(percent>0) --percent;
    bSlider->setSliderPosition(percent);
    if(percent == 0) emit batteryDrained();
}
