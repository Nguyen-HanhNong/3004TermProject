#include "session.h"

//Initializing the static member variable
int Session::uniqueSessionID = 1;

//Default constructor
Session::Session(QWidget* p, int challengeLevel): QWidget(p), challengeLevel(challengeLevel), length(0), reader(Reader())
{
    this->timeCreated = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    this->sessionID = uniqueSessionID;
    uniqueSessionID++;
}

//Default destructor
Session::~Session() {

}

//Function to calculate and store the values of all unevaluated member variables
void Session::calculateSummaryData() {
    //Evaluating averageCoherenceScore
    if(reader.getNumberOfAchievementUpdates() == 0) {
        averageCoherenceScore = 0;
    }
    else {
        averageCoherenceScore = reader.getLatestAchievementScore() / reader.getNumberOfAchievementUpdates();
    }

    //Evaluating percentageOfHighCoherence
    if(getTimeAtHigh() == 0.0) {
        percentageOfHighCoherence = 0.0;
    }
    else {
        percentageOfHighCoherence = (getTimeAtHigh() / length) * 100;
    }

    //Evaluating percentageOfMediumCoherence
    if(getTimeAtMedium() == 0.0) {
        percentageOfMediumCoherence = 0.0;
    }
    else {
        percentageOfMediumCoherence = (getTimeAtMedium() / length) * 100;
    }

    //Evaluating percentageOfLowCoherence
    if(getTimeAtLow() == 0.0) {
        percentageOfLowCoherence = 0.0;
    }
    else {
        percentageOfLowCoherence = (getTimeAtLow() / length) * 100;
    }

    return;
}

//Function to update the running session
void Session::update() {
    this->reader.generateNextDataPoint();
    this->length += 1;
}

// GETTERS

//Getter for challengeLevel
int Session::getChallengeLevel() { return challengeLevel; }
//Getter for length(in seconds)
int Session::getLength() { return length; }
//Getter for achievement
double Session::getAchievement() { return reader.getLatestAchievementScore(); }
//Getter for sessionID
int Session::getSessionID() { return sessionID; }
//Getter for averageCoherenceScore
double Session::getAverageCoherenceScore() { return averageCoherenceScore; }
//Getter for percentageOfHighCoherence
double Session::getPercentageOfHighCoherence() { return percentageOfHighCoherence; }
//Getter for percentageOfMediumCoherence
double Session::getPercentageOfMediumCoherence() { return percentageOfMediumCoherence; }
//Getter for percentageOfLowCoherence
double Session::getPercentageOfLowCoherence() { return percentageOfLowCoherence; }
//Getter for timeCreated
QString Session::getDateCreated() { return timeCreated; }
//Getter for reader
Reader Session::getReader() { return reader; }


//Getter for time in low coherence value from reader
double Session::getTimeAtLow() { return reader.getTimeInEachCoherence().at(2); }
//Getter for time in medium coherence value from reader
double Session::getTimeAtMedium() { return reader.getTimeInEachCoherence().at(1); }
//Getter for time in high coherence value from reader
double Session::getTimeAtHigh() { return reader.getTimeInEachCoherence().at(0); }
//Getter for the latest coherence score from the reader
double Session::getCurrentCoherence() { return reader.getLatestCoherenceScore(); }


//Function that returns the value of reader.getCoherenceLevel() for the value of challenge level currently being used
QString Session::getCoherenceLevel() { return reader.getCoherenceLevel(this->challengeLevel); }
