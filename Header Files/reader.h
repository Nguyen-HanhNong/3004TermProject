/* Author: Nguyen-Hanh Nong */
/* File Name: reader.h */
/* Purpose: This is the header file for the Reader class. It contains the function prototypes and member variables for the Reader class. */
/* Functionality: The Reader class is less of a class itself but more a collection of methods and variables that are involved in the graphing functionality of the heart rate monitor. It contains functions to generate data for various levels of coherence by either generating the data itself or reading from a file. It also contains the corresponding coherence score for the heart rates. */

/* Include all the necessary headers and libraries */
#include <QMainWindow>
#include <QElapsedTimer>
#include <QSpinBox>

#include "qcustomplot.h"

#ifndef READER_H
#define READER_H

class Reader {
public:
    Reader(); // Default constructor
    ~Reader(); // Default destructor

    /* Function to read the incoherent heart rate data from a file */
    void setupClass();

    /* Functions to generate data points */
    void generateNextDataPoint(double multiplierValue = 20.0, double periodValue = 5.0, int heightValue = 80, int adjustmentValue = 0); // function to generate the next data point

    void generateHighCoherenceDataPoint(double multiplierValue = 20.0, double periodValue = 5.0, int heightValue = 80); //function to generate the high coherence heart rate data points
    void generateMediumCoherenceDataPoint(int adjustmentValue = 0); //function to generate the medium coherence heart rate data points
    void generateLowCoherenceDataPoint(int adjustmentValue = 0); //function to generate the low coherence heart rate data points

    /* Functions to read heart rate data from a file */
    void readLowCoherenceDataPoints(QString fileName); //function to read the low coherence heart rate data points from a file
    void readMediumCoherenceDataPoints(QString fileName); //function to read the medium coherence heart rate data points from a file
    void readAllCoherenceScores(QString lowCoherenceFileName, QString mediumCoherenceFileName, QString highCoherenceFileName); //function to read all the coherence scores from a file

    void calculateCoherenceScore(); //function to calculate the latest coherence score

    /* Function to reset to reset the Reader instance to the initial state */
    void resetData();

    /* Getter functions */
    QVector<double> getTimeDataPoints(); //returns the timeDataPoints vector
    QVector<double> getHeartRateDataPoints(); //returns the heartRateDataPoints vector
    QVector<double> getCurrentCoherenceScore(); //returns the currentCoherenceScore vector

    double getLatestCoherenceScore(); //returns the latestCoherenceScore double
    int getLatestDataPoint(); //returns the latest data point generated by the Reader instance
    int getSwitchBetweenCoherence(); //returns the switchBetweenCoherence integer

private:

    QVector<double> timeDataPoints; //the vector that stores the x-axis data points
    QVector<double> heartRateDataPoints; //the vector that stores the y-axis data points
    QVector<double> currentCoherenceScore; //the vector that stores the current coherence score

    QVector<double> lowCoherenceVector; //the vector that stores the low coherence y-axis data points
    QVector<double> mediumCoherenceVector; //the vector that stores the medium coherence y-axis data points

    QVector<double> timeInEachCoherence; //the vector that stores the time spent in each coherence level

    int lowCoherenceVectorIndex; //integer storing the current index of the low coherence vector
    int mediumCoherenceVectorIndex; //integer storing the current index of the medium coherence vector

    int currentCoherenceScoreIndex; //integer storing the current index of the coherence score vector

    int switchBetweenCoherence; //integer that is used to switch outputting heart rate data between the coherence levels

    double latestCoherenceScore; //double storing the latest coherence score
};

#endif // READER_H
