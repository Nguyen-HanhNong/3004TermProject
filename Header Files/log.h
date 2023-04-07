#include <QString>
#include <QDateTime>
#include <QTime>
#include <QVector>

#include "qcustomplot.h"

#ifndef LOG_H
#define LOG_H

/* Purpose of Class: To store the session summary of all sessions that were run so they can be viewed or deleted
 *
 * Data Members:
 * - int logNum: an integer that keeps track of the chronological position of this session relative to others
 * - int challengeLevel: an integer that represents the challenge level used for the session
 * - double timeAtLow: a double that represents the PERCENTAGE of time at low coherence level
 * - double timeAtMedium: a double that represents the PERCENTAGE of time at medium coherence level
 * - double timeAtHigh: a double that represents the PERCENTAGE of time at high coherence level
 * - double avgCoherence: a double that represents the average coherence score of the session
 * - int length: an integer that represents the length in seconds of the session
 * - double achievementScore: a double used to store the achievement score of the session
 * - QDateTime logTime: an object to keep track of the date and time of the session completion
 *
 * - Class Functions:
 * - Getters and Setters
 *
 */

class Log {

public:
    Log(const int logNum, const int challengeLevel, const double timeAtLow, const double timeAtMedium, const double timeAtHigh, const double avgCoherence, const int length, const double achievementScore, const QDateTime& logTime);

    QString toString();

    int getLogNum();
    int getChallengeLevel();
    double getTimeAtLow();
    double getTimeAtMedium();
    double getTimeAtHigh();
    double getAvgCoherence();
    int getLength(); //Seconds
    double getAchievementScore();
    QDateTime getLogTime();

private:
    int logNum;
    int challengeLevel;
    double timeAtLow;
    double timeAtMedium;
    double timeAtHigh;
    double avgCoherence;
    int length;
    double achievementScore;
    QDateTime logTime;
    //Reader reader;
};

#endif // LOG_H
