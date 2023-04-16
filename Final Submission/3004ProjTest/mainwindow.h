/*
    Authors: Shreya Voore, Nguyen-Hanh Nong, Ashok Sivathalayan, Awwab Mahdi
    Purpose: The main window of the application, intializes and controls the application.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QVector>

#include "menu.h"
#include "session.h"
#include "led.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static void plotDataPoints(QWidget* mainWidget, QCustomPlot *graphWidget, QLabel* coherenceScoreLabel, QLabel* lengthLabel, QLabel* achievementLabel, QVector<Session *>* sessionsVector, bool* updateSessionGraph, LED* coherenceLight);

private:
    Ui::MainWindow *ui;

    //menu related
    Menu* mainMenu; //the menu the user is currently on
    Menu* parentMenu; //the initial/original menu node, for destructor

    QListWidget *curQListWidget; //the list widget the menus are displayed on
    void updateMenu(const QString, QVector<QString>); //updates the list widget with the selected menu items
    void initializeMainMenu(Menu*); //initializes the main menu, sets up the menu hierarchy for the user

    //challenge level
    void setChallengeLevel(int); //sets the challenge level for the next sessions

    //reset device
    void resetDevice();

    //log/history
    void viewLog(int); //view all the information pertaining to a session

    //breath pacer
    void setBreathPacer(); //displays the breath pacer slider widget when selected
    void updateSliderText(); //changes the slider text depending on what percent the slider is set to

    //session
    QVector<Session*>* sessions; //vector containing all the sessions recorded
    static int currentSession; //used for the id of a session
    int nextChallengeLevel; //the challenge level of the next session
    QTimer *plottingTimer;
    bool *updateSessionGraph; //updates the session graph
    bool activeSession; //signals whether a session is happening or not currently
    bool on; //boolean that indicates whether or not the device is on
    bool hasSignal; //indicates whether the hrv signal is on or not

    void addNewSession();//adds a new session to the session list
    void startSession();//starts a session
    void endSession();//ends a session
    void returnToMenu();//once the session is over, upon clicking the return to menu button, displays main menu
    void deleteSession();//deletes a session from the menu tree and the session vector

    void turnOn();//turns device on
    void turnOff();//turns device off

private slots:
    //Menu slots
    void upButton();
    void downButton();
    void select();
    void menu();
    void goBack();

    //Breath Pacer slots
    void moveBPSliderLeft();
    void moveBPSliderRight();

    //Changes signal
    void changeSignal();

    //Battery slots
    void batteryDead();
    void batteryCharged();

    //Turning on/off
    void powerButton();

};
#endif // MAINWINDOW_H
