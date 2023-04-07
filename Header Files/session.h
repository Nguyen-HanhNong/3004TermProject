#ifndef SESSION_H
#define SESSION_H

#include <QString>
#include <QDateTime>
#include <QTime>
#include <QTimer>

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

class Session {

public:
    Session(const int challengeLevel);

    QString toString();

    double calculateCoherence();
    void update();
    void stop();
    void end();

    int getChallengeLevel();
    double getTimeAtLow();
    double getTimeAtMedium();
    double getTimeAtHigh();
    double getCurrentCoherence();
    int getLength(); //Seconds
    double getAchievement();

public slots:
    void onTimerTimeout();

private:
    int challengeLevel;
    double timeAtLow;
    double timeAtMedium;
    double timeAtHigh;
    double currentCoherence;
    int length;
    double achievement;
    QTimer timer;
    QVector<double> coherenceScores;
    //Reader reader;
};

#endif // SESSION_H
