#ifndef BATTERY_H
#define BATTERY_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QTimer>

class Battery : public QWidget
{
    Q_OBJECT
public:
    Battery(QWidget *parent = nullptr);

    int getBattery(){return percent;}

signals:
    //Used to indicate that the device has no charge left and should not function
    void batteryDrained();
    //Used to indicate that the device has been charged and can now function again
    void batteryCharged();

public slots:
    void updateBattery();
    void drainBattery();

private:
    QLabel* percentLabel; //UI element to show user how much battery is left
    QSlider* bSlider; //UI element to let user recharge battery
    QTimer timer; //Used to continuously drain battery
    int percent; //Stores current charge level for simplicity
};

#endif // BATTERY_H
