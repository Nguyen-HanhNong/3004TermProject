#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QVector>

#include "menu.h"
#include "session.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static void plotDataPoints(QWidget* mainWidget, QCustomPlot *graphWidget, QLabel* coherenceScoreLabel, QLabel* lengthLabel, QLabel* achievementLabel, QVector<Session *>* sessionsVector, bool* updateSessionGraph, QPushButton* coherenceLight);

private:
    Ui::MainWindow *ui;

    //menu related
    Menu* mainMenu;
    Menu* parentMenu;

    QListWidget *curQListWidget;
    void updateMenu(const QString, QVector<QString>);
    void initializeMainMenu(Menu*);

    //challenge level
    void setChallengeLevel(int);

    //reset device
    void resetDevice();

    //log/history
    void viewLog(int);

    //breath pacer
    void setBreathPacer();
    void moveBPSliderLeft();
    void moveBPSliderRight();
    void updateSliderText();

    //session stuff
    QVector<Session*>* sessions;
    static int currentSession;
    int nextChallengeLevel;
    QTimer *plottingTimer;
    bool *updateSessionGraph;
    bool activeSession;
    bool on;
    bool hasSignal;

    void addNewSession();
    void startSession();
    void endSession();
    void returnToMenu();
    void deleteSession();

    void turnOn();
    void turnOff();

private slots:
    void upButton();
    void downButton();
    void select();
    void menu();
    void goBack();

    void changeSignal();

    //Battery slots
    void batteryDead();
    void batteryCharged();

    //Turning on/off
    void powerButton();

};
#endif // MAINWINDOW_H
