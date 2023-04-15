/* Author: Awwab Mahdi */
/* File Name: session.h */
/* Purpose: To serve as the object representation of active and saved sessions, notably allowing them to be stored in an array as session logs */

/* Including all the necessary headers and libraries */
#include <QString>
#include <QDateTime>
#include <QTime>
#include <QVector>
#include <QTimer>
#include <QObject>
#include <QWidget>

#include "qcustomplot.h"
#include "reader.h"

#ifndef SESSION_H
#define SESSION_H

class Session: public QWidget {

public:
    Session(QWidget* = nullptr, int challengeLevel = 1); //Default constructor
    ~Session(); //Default destructor

    void calculateSummaryData(); //Function to calculate and store the values of all unevaluated member variables
    void update(); //Function to update the running session

    //getters
    int getChallengeLevel(); //Getter for challengeLevel
    int getLength(); //Getter for length
    double getAchievement(); //Getter for achievement
    int getSessionID(); //Getter for sessionID
    double getAverageCoherenceScore(); //Getter for averageCoherenceScore
    double getPercentageOfHighCoherence(); //Getter for percentageOfHighCoherence
    double getPercentageOfMediumCoherence(); //Getter for percentageOfMediumCoherence
    double getPercentageOfLowCoherence(); //Getter for percentageOfLowCoherence
    QString getDateCreated(); //Getter for timeCreated
    Reader getReader(); //Getter for reader

    double getTimeAtLow(); //Getter for time in low coherence value from reader
    double getTimeAtMedium(); //Getter for time in medium coherence value from reader
    double getTimeAtHigh(); //Getter for time in high coherence value from reader
    double getCurrentCoherence(); //Getter for the latest coherence score from the reader

    QString getCoherenceLevel(); //Function that returns the value of reader.getCoherenceLevel() for the value of challenge level currently being used

private:
    int challengeLevel; // An integer that represents the challenge level used for the session
    int length; // An integer that represents the duration in seconds of the session
    double achievement; // A double that represents the achievement value, which is the sum of all coherence scores calculated in the session

    static int uniqueSessionID; // A static integer that is incremented for each new session
    int sessionID; // An integer that represents a unique value for each session

    double averageCoherenceScore; // A double that represents the average coherence score obtained over the course of the session
    double percentageOfHighCoherence; // A double that represents the percentage of the duration of the session spent at high coherence
    double percentageOfMediumCoherence; // A double that represents the percentage of the duration of the session spent at medium coherence
    double percentageOfLowCoherence; // A double that represents the percentage of the duration of the session spent at low coherence

    QString timeCreated; // A QString object that holds value of the date and time the session was created as a string

    Reader reader; // A reader object(sourced from reader.h) that reads from file and performs calculations for each session
};

#endif // SESSION_H
