#include "session.h"

int Session::uniqueSessionID = 1;

Session::Session(QWidget* p, int challengeLevel): QWidget(p), challengeLevel(challengeLevel), length(0), reader(Reader()) 
{
    this->timeCreated = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    this->sessionID = uniqueSessionID;
    uniqueSessionID++;
}

Session::~Session() {

}

void Session::calculateSummaryData() {
    if(reader.getNumberOfAchievementUpdates() == 0) {
        averageCoherenceScore = 0;
    }
    else {
        averageCoherenceScore = reader.getLatestAchievementScore() / reader.getNumberOfAchievementUpdates();
    }

    if(getTimeAtHigh() == 0.0) {
        percentageOfHighCoherence = 0.0;
    }
    else {
        percentageOfHighCoherence = (getTimeAtHigh() / length) * 100;
    }

    if(getTimeAtMedium() == 0.0) {
        percentageOfMediumCoherence = 0.0;
    }
    else {
        percentageOfMediumCoherence = (getTimeAtMedium() / length) * 100;
    }

    if(getTimeAtLow() == 0.0) {
        percentageOfLowCoherence = 0.0;
    }
    else {
        percentageOfLowCoherence = (getTimeAtLow() / length) * 100;
    }
    
    return;
}

void Session::update() {
    this->reader.generateNextDataPoint();
    this->length += 1;
}

// getters
int Session::getChallengeLevel() { return challengeLevel; }

double Session::getTimeAtLow() { return reader.getTimeInEachCoherence().at(2); }

double Session::getTimeAtMedium() { return reader.getTimeInEachCoherence().at(1); }

double Session::getTimeAtHigh() { return reader.getTimeInEachCoherence().at(0); }

double Session::getCurrentCoherence() {
    return reader.getLatestCoherenceScore();
}

int Session::getLength() { return length; } //Seconds

double Session::getAchievement() { return reader.getLatestAchievementScore(); }

Reader Session::getReader() { return reader; }

double Session::getAverageCoherenceScore() { return averageCoherenceScore; }

double Session::getPercentageOfHighCoherence() { return percentageOfHighCoherence; }

double Session::getPercentageOfMediumCoherence() { return percentageOfMediumCoherence; }

double Session::getPercentageOfLowCoherence() { return percentageOfLowCoherence; }

QString Session::getCoherenceLevel() {
    return reader.getCoherenceLevel(this->challengeLevel);
}

QString Session::getDateCreated() {
    return timeCreated;
}

int Session::getSessionID() {
    return sessionID;
}