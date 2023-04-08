#include "log.h"

Log::Log(const int logNum, const int challengeLevel, const double timeAtLow, const double timeAtMedium, const double timeAtHigh, const double avgCoherence, const int length, const double achievementScore, const QDateTime& logTime) {
    if (logTime.isValid()) {
        this->logNum = logNum;
        this->challengeLevel = challengeLevel;
        this->timeAtLow = timeAtLow;
        this->timeAtMedium = timeAtMedium;
        this->timeAtHigh = timeAtHigh;
        this->avgCoherence = avgCoherence;
        this->length = length;
        this->achievementScore = achievementScore;
        this->logTime = logTime;
    }
}

QString Log::toString() {
    QString newString =
            "Session #" + QString::number(logNum) + ": " + logTime.toString("MMMM d yyyy - h:mm AP") + "\n"
            + "   Challenge Level: " + QString::number(challengeLevel) + "\n"
            + "   Percentage of Time at Low Coherence: " + QString::number(timeAtLow) + "\n"
            + "   Percentage of Time at Medium Coherence: " + QString::number(timeAtMedium) + "\n"
            + "   Percentage of Time at High Coherence: " + QString::number(timeAtHigh) + "\n"
            + "   Average Coherence: " + QString::number(avgCoherence) + "\n"
            + "   Length of Session(in seconds): " + QString::number(length) + "\n"
            + "   Achievement Score: " + QString::number(achievementScore) + "\n";

    return newString;
}

// getters
int Log::getLogNum() { return logNum; }
int Log::getChallengeLevel() { return challengeLevel; }
double Log::getTimeAtLow() { return timeAtLow; }
double Log::getTimeAtMedium() { return timeAtMedium; }
double Log::getTimeAtHigh() { return timeAtHigh; }
double Log::getAvgCoherence() { return avgCoherence; }
int Log::getLength() { return length; } //Seconds
double Log::getAchievementScore() { return achievementScore; }
QDateTime Log::getLogTime() { return logTime; }

