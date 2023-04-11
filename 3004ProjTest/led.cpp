#include "led.h"

LED::LED(QWidget* p, const QColor onC, const QColor offC): QWidget(p), on(false), onColour(onC), offColour(offC){

}

void LED::setOn(bool newState){
    if(newState == on) return;
    //If the state has changed, update
    on = newState;
    update();
}
void LED::setColour(const QColor newC){
    this->onColour = newC;
    update();
}
void LED::paintEvent(QPaintEvent* ){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if(this->on) painter.setBrush(onColour);
    else painter.setBrush(offColour);
    painter.drawRect(0, 0, width(), height());
}
