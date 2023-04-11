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

/* Purpose of Class: To serve as the active object being updated while a session is running
 *
 * Data Members:
 * - int challengeLevel: an integer that represents the challenge level used for the session
 * - double timeAtLow: a double that represents the PERCENTAGE of time at low coherence level
 * - double timeAtMedium: a double that represents the PERCENTAGE of time at medium coherence level
 * - double timeAtHigh: a double that represents the PERCENTAGE of time at high coherence level
 * - double currentCoherence: a double that represents the most recent coherence score(calculated every five seconds)
 * - int length: an integer that represents the length in seconds of the session in seconds
 * - double achievement: a double used to represent the total value of the sum of all coherence scores
 *
 * - Class Functions:
 * - Getters and Setters
 *
 */

class Session: public QWidget {

public:
    Session(QWidget* = nullptr, int challengeLevel = 1);
    ~Session();

    void calculateSummaryData();
    void update();

    //getters
    int getChallengeLevel();
    double getTimeAtLow();
    double getTimeAtMedium();
    double getTimeAtHigh();
    double getCurrentCoherence();
    int getLength(); //Seconds
    double getAchievement();
    Reader getReader();

    double getAverageCoherenceScore();
    double getPercentageOfHighCoherence();
    double getPercentageOfMediumCoherence();
    double getPercentageOfLowCoherence();

    QString getCoherenceLevel();
    QString getDateCreated();

    int getSessionID();

private:
    int challengeLevel;
    int length;
    double achievement;

    static int uniqueSessionID;
    int sessionID;

    double averageCoherenceScore;
    double percentageOfHighCoherence;
    double percentageOfMediumCoherence;
    double percentageOfLowCoherence;

    QString timeCreated;

    Reader reader;
};

#endif // SESSION_H
