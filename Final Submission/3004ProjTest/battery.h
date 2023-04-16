/*Author: Ashok Sivathayalan
File name: battery.h
Purpose: Stores the device's charge level and drains it as the application runs
         Sends signals to disable device when charge runs out*/

#ifndef BATTERY_H
#define BATTERY_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QTimer>

class Battery : public QWidget
{
    //Stating that it is a QObject, so signals/slots will work
    Q_OBJECT

public:
    //Constructor: takes in parent widget as argument
    Battery(QWidget *parent = nullptr);

    //Getter
    int getBattery(){return percent;}

    //To start/stop battery draining
    void stopBattery();
    void startBattery();

signals:
    //Used to indicate that the device has no charge left and should not function
    void batteryDrained();
    //Used to indicate that the device has been charged and can now function again
    void batteryCharged();

public slots:
    //Used to update battery levels when user changes it
    void updateBattery();
    //Used to continuously drain battery while device is on
    void drainBattery();

private:
    QLabel* percentLabel; //UI element to show user how much battery is left
    QSlider* bSlider; //UI element to let user recharge battery
    QTimer timer; //Used to continuously drain battery
    int percent; //Stores current charge level for simplicity
};

#endif // BATTERY_H
