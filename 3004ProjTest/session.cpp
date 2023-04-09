#include "session.h"

Session::Session(QWidget* p): QWidget(p) {
    this->challengeLevel = challengeLevel;
    this->length = 0;
    this->achievement = 0;
    this->currentCoherence = 0;
    this->coherenceScores = QVector<double>();
    this->hrvData = QVector<double>();
    this->timeData = QVector<double>();
    timer.setInterval(5000);
    QObject::connect(&timer, &QTimer::timeout, this, &Session::onTimerTimeout);
    timer.start();
}

Session::~Session() {

}

double Session::calculateCoherence(QVector<double> hrvData, QVector<double> timeData) {
    QVector<double> timeIntervals;
    QVector<double> diffIntervals;

    // Find positions of normal heartbeats and calculate time intervals
    for (int i = 1; i < hrvData.size(); i++) {
        // Check if both current and previous data points are normal heartbeats
        if (hrvData[i] > 0 && hrvData[i-1] > 0) {
            double interval = timeData[i] - timeData[i-1];
            timeIntervals.push_back(interval);
        }
    }

    // Calculate differences between successive time intervals and their squares
    for (int i = 1; i < timeIntervals.size(); i++) {
        double diff = timeIntervals[i] - timeIntervals[i-1];
        double diffSquared = diff * diff;
        diffIntervals.push_back(diffSquared);
    }

    // Calculate the mean of the squared differences
    double meanSquaredDiff = 0.0;
    for (int i = 0; i < diffIntervals.size(); i++) {
        meanSquaredDiff += diffIntervals[i];
    }
    meanSquaredDiff /= diffIntervals.size();

    // Calculate the root mean square of successive differences
    double rmsd = std::sqrt(meanSquaredDiff);
    return rmsd;
}

void Session::update() {
    //Calculate current coherence
    this->currentCoherence = calculateCoherence(this->hrvData, this->timeData);
    //Add currentCoherence to achievement
    this->achievement += this->currentCoherence;
    //Increment length variable by time elapsed
    this->length += 5;
    //Add calculated current coherence to coherenceScores vector
    this->coherenceScores.push_back(currentCoherence);
}

void Session::stop() {
    //Save all new values, prevent updating until further input is received
}

void Session::end() {
    //Transfer all information to a new log object
    update();
}

void Session::onTimerTimeout() {
    //Call update
    update();
    //Restart timer
    timer.start();
}

QString Session::toString() {
    QString newString =
            "CURRENT SESSION\n   Challenge Level: "
            + QString::number(challengeLevel) + "\n"
            + "   Length of Session(in seconds): " + QString::number(length) + "\n"
            + "   Achievement: " + QString::number(achievement) + "\n";

    return newString;
}

// getters
int Session::getChallengeLevel() { return challengeLevel; }
double Session::getTimeAtLow() { return timeAtLow; }
double Session::getTimeAtMedium() { return timeAtMedium; }
double Session::getTimeAtHigh() { return timeAtHigh; }
int Session::getLength() { return length; } //Seconds
double Session::getAchievement() { return achievement; }
