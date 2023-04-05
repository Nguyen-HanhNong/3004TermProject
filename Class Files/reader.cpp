/* Author: Nguyen-Hanh Nong */
/* File Name: reader.cpp */
/* Purpose: This is the source file for the Reader class. It contains the implementation for the Reader class. */
/* Functionality: The Reader class is less of a class itself but more a collection of methods and variables that are involved in the graphing functionality of the heart rate monitor. It contains functions to generate data for either a coherent heart rate or a incoherent heart rate by either generating the data itself or reading from a file. */

#include "reader.h"

/* Initialize the static variables */
QVector<double> Reader::timeDataPoints = QVector<double>();
QVector<double> Reader::heartRateDataPoints = QVector<double>();
QVector<double> Reader::weirdHeartRateDataPoints = QVector<double>();
int Reader::weirdHeartRateDataPointsIndex = 0;

/* Default constructor */
Reader::Reader()
{
}

/* This function is used to generate a data point that is simulating a coherent heart rate. */
void Reader::generateNormalDataPoint(double multiplierValue, double periodValue, int heightValue) {
  /* Generate a new data point starting at 65 if the timeDataPoints vector is empty. */
  if(timeDataPoints.size() == 0) {
    timeDataPoints.push_back(1);
    heartRateDataPoints.push_back(65);
  }
  /* If the timeDataPoints vector is not empty, then generate a new data point by using the current time elapsed and applying the qSin function and other modifiers to the data point to get a valid heart rate data point. */ 
  else {
    timeDataPoints.push_back(timeDataPoints.last() + 1);
    heartRateDataPoints.push_back((multiplierValue * qSin((timeDataPoints.last()) / periodValue)) + heightValue);
  }
}

/* This function is used to generate a data point that is simulating an incoherent heart rate. */
void Reader::generateWeirdDataPoint(int adjustmentValue) {
  /* Generate a new data point if the timeDataPoints vector is empty. Otherwise, generate a new data point by incrementing the last data point in the vector by 1 */
  if(timeDataPoints.size() == 0) {
    timeDataPoints.push_back(1);
  } else {
    timeDataPoints.push_back(timeDataPoints.last() + 1);
  }

  /* Check if the indexing variable is equal to the size of the weirdHeartRateDataPoints vector. If it is, then reset the indexing variable to 0 and generate a new data point. Otherwise, generate a new data point and incrementing the indexing variable by 1 */
  if(weirdHeartRateDataPointsIndex == weirdHeartRateDataPoints.size()) {
    weirdHeartRateDataPointsIndex = 0;
    heartRateDataPoints.push_back(weirdHeartRateDataPoints[weirdHeartRateDataPointsIndex] + adjustmentValue);
  }
  else {
    heartRateDataPoints.push_back(weirdHeartRateDataPoints[weirdHeartRateDataPointsIndex] + adjustmentValue);
    weirdHeartRateDataPointsIndex++;
  }
}

/* This function is used to read data from a file and store it in the weirdHeartRateDataPoints vector */
void Reader::readDataFromFile(QString fileName) {
  QFile dataFile(fileName); // This is the file that contains the data

  /* If the file cannot be opened, then exit the function */
  if (!dataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
      return;
  }

  /* Iterate through the file and store each entry in the weirdHeartRateDataPoints vector */
  while (!dataFile.atEnd()) {
      QByteArray line = dataFile.readLine();
      QString str(line);
      QStringList list = str.split(" ");

      weirdHeartRateDataPoints.push_back(list[0].toDouble());
  }

  dataFile.close(); // Close the file

  /* Print a debug message to indicate that all the data has been read from the file */
  qDebug() << "Size of the weirdHeartRateDataPoints vector: " << weirdHeartRateDataPoints.size();
}

/* This function is used to return the last data point generated in the heartRateDataPoints vector */
int Reader::getLatestDataPoint() {
  return heartRateDataPoints.back();
}

/* This function is used to return the timeDataPoints vector */
QVector<double> Reader::getTimeDataPoints() {
  return timeDataPoints;
}

/* This function is used to return the heartRateDataPoints vector */
QVector<double> Reader::getHeartRateDataPoints() {
  return heartRateDataPoints;
}

/* This function is used to reset the timeDataPoints and heartRateDataPoints to their initial state by emptying both vectors. It also resets the index variable to 0 */
void Reader::resetData() {
  timeDataPoints.clear();
  heartRateDataPoints.clear();
  weirdHeartRateDataPointsIndex = 0;
}
