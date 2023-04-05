/* Author: Nguyen-Hanh Nong */
/* File Name: reader.h */
/* Purpose: This is the header file for the Reader class. It contains the function prototypes and member variables for the Reader class. */
/* Functionality: The Reader class is less of a class itself but more a collection of methods and variables that are involved in the graphing functionality of the heart rate monitor. It contains functions to generate data for either a coherent heart rate or a incoherent heart rate by either generating the data itself or reading from a file. */

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

    /* Functions to generate data points */
    static void generateNormalDataPoint(int multiplierValue = 20, double periodValue = 5.0, int heightValue = 80); 
    static void generateWeirdDataPoint(int adjustmentValue = 0); //function to generate the incoherent heart rate data points

    /* Function to read the incoherent heart rate data from a file */
    static void readDataFromFile(QString fileName);

    /* Function to get the latest data point generated */
    static int getLatestDataPoint();

    /* Function to reset to reset the time and heart rate vectors to their initial state */
    static void resetData();

    /* Getter functions */
    static QVector<double> getTimeDataPoints();
    static QVector<double> getHeartRateDataPoints();

private:

    static QVector<double> timeDataPoints;

    static QVector<double> heartRateDataPoints;
    static QVector<double> weirdHeartRateDataPoints;

    static int weirdHeartRateDataPointsIndex;
};

#endif // READER_H
