/* Author: Nguyen-Hanh Nong */
/* File Name: reader.cpp */
/* Purpose: This is the source file for the Reader class. It contains the implementation for the Reader class. */
/* Functionality: The Reader class is less of a class itself but more a collection of methods and variables that are involved in the graphing functionality of the heart rate monitor. It contains functions to generate data for various levels of coherence by either generating the data itself or reading from a file. It also contains the corresponding coherence score for the heart rates. */

#include "reader.h"

/* Default constructor */
Reader::Reader() {
  /* Initialize all the different QVectors to defualt state */
  timeDataPoints = QVector<double>();
  heartRateDataPoints = QVector<double>();
  currentCoherenceScore = QVector<double>();

  lowCoherenceVector = QVector<double>();
  mediumCoherenceVector = QVector<double>();

  timeInEachCoherence = QVector<double>();

  /* Add three default values to the timeInEachCoherence vector */
  for(int i = 0; i < 3; i++) {
    timeInEachCoherence.push_back(0.0);
  }

  qDebug("At setup, timeInEachCoherence is: %f, %f, %f", timeInEachCoherence[0], timeInEachCoherence[1], timeInEachCoherence[2]);

  /* Initializing all the different integers and doubles to default values */
  lowCoherenceVectorIndex = 0;
  mediumCoherenceVectorIndex = 0;
  switchBetweenCoherence = 0;
  currentCoherenceScoreIndex = 0;
  
  latestCoherenceScore = 0.0;
  latestAchievementScore = 0.0;

  numberOfAchievementUpdates = 0;
  
  setupClass(); //Import relevant data from files
}

/* Default destructor */
Reader::~Reader() {
}

/* This function sets up the Reader instance by reading in the data points and coherence scores from the various files */
void Reader::setupClass() {
  /* Read in the low coherence and medium coherence data points from their respective files */
  readLowCoherenceDataPoints(":/low_coherence_data.txt");
  readMediumCoherenceDataPoints(":/medium_coherence_data.txt");

  /* Read in the coherence scores for low, medium and high coherence data points from their respective files */
  readAllCoherenceScores(":/low_coherence_scores.txt", ":/medium_coherence_scores.txt", ":/high_coherence_scores.txt");
}

/* This function is used to generate the next data point for the HRV graph depending on which coherence level should currently be displayed */
void Reader::generateNextDataPoint(double multiplierValue, double periodValue, int heightValue, int adjustmentValue) {
  /* Use the switchBetweenCoherence variable to determine which coherence level should be displayed. The Reader will always start off with the high coherence data points first, then medium coherence data points, then low coherence data points last. */
  if(switchBetweenCoherence <= 60) {
    generateHighCoherenceDataPoint(multiplierValue, periodValue, heightValue);
    timeInEachCoherence[0] = timeInEachCoherence[0] + 1;
    switchBetweenCoherence += 1;
  }
  else if (switchBetweenCoherence > 60 && switchBetweenCoherence <= 120) {
    generateMediumCoherenceDataPoint(adjustmentValue);
    timeInEachCoherence[1] = timeInEachCoherence[1] + 1;
    switchBetweenCoherence += 1;
  }
  else if (switchBetweenCoherence > 120 && switchBetweenCoherence <= 180) {
    generateLowCoherenceDataPoint(adjustmentValue);
    timeInEachCoherence[2] = timeInEachCoherence[2] + 1;
    switchBetweenCoherence += 1;
  }
  else {
    switchBetweenCoherence = 0;
  }

  /* Make sure that every 5 seconds, we calculate/update the coherence score and achievement score for the current data points. */
  if(switchBetweenCoherence % 5 == 0) {
    calculateCoherenceScore();
    calculateAchievementScore(); //Update the achievement score
  }
}

/* This function is used to generate a data point that is simulating a high coherent heart rate. */
void Reader::generateHighCoherenceDataPoint(double multiplierValue, double periodValue, int heightValue) {
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

/* This function is used to generate a data point that is simulating a medium coherent heart rate. */
void Reader::generateMediumCoherenceDataPoint(int adjustmentValue) {
  /* Generate a new data point if the timeDataPoints vector is empty. Otherwise, generate a new data point by incrementing the last data point in the vector by 1 */
  if(timeDataPoints.size() == 0) {
    timeDataPoints.push_back(1);
  } else {
    timeDataPoints.push_back(timeDataPoints.last() + 1);
  }

  /* Check if the indexing variable is equal to 60. If it is, then reset the indexing variable to 0 and generate a new data point. Otherwise, generate a new data point using the data from the medium_coherence_data.txt file and incrementing the indexing variable by 1 */
  if(mediumCoherenceVectorIndex == 60) {
    mediumCoherenceVectorIndex = 0;
    heartRateDataPoints.push_back(mediumCoherenceVector[mediumCoherenceVectorIndex] + adjustmentValue);
  }
  else {
    heartRateDataPoints.push_back(mediumCoherenceVector[mediumCoherenceVectorIndex] + adjustmentValue);
    mediumCoherenceVectorIndex++;
  }
}

/* This function is used to generate a data point that is simulating an incoherent heart rate. */
void Reader::generateLowCoherenceDataPoint(int adjustmentValue) {
  /* Generate a new data point if the timeDataPoints vector is empty. Otherwise, generate a new data point by incrementing the last data point in the vector by 1 */
  if(timeDataPoints.size() == 0) {
    timeDataPoints.push_back(1);
  } else {
    timeDataPoints.push_back(timeDataPoints.last() + 1);
  }

  /* Check if the indexing variable is equal to 60. If it is, then reset the indexing variable to 0 and generate a new data point. Otherwise, generate a new data point using the data from the low_coherence_data.txt file and incrementing the indexing variable by 1 */
  if(lowCoherenceVectorIndex == 60) {
    lowCoherenceVectorIndex = 0;
    heartRateDataPoints.push_back(lowCoherenceVector[lowCoherenceVectorIndex] + adjustmentValue);
  }
  else {
    heartRateDataPoints.push_back(lowCoherenceVector[lowCoherenceVectorIndex] + adjustmentValue);
    lowCoherenceVectorIndex++;
  }
}

/* This function is used to read data from a file and store it in the lowCoherenceVector vector */
void Reader::readLowCoherenceDataPoints(QString fileName) {
  QFile lowCoherenceDataFile(fileName); // File that contains the low coherence data points

  /* Check if the file can be opened/isn't corrupted, if the file cannot be opened, then print a debug message and exit the program */
  if (!lowCoherenceDataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
      qDebug() << "Error: Cannot open file. The program will now exit.";
      exit(1);
  }

  /* Iterate through the file and store each entry in the lowCoherenceVector vector */
  while (!lowCoherenceDataFile.atEnd()) {
      QByteArray currentLine = lowCoherenceDataFile.readLine();
      QString currentDataPoint(currentLine);
      lowCoherenceVector.push_back(currentDataPoint.toDouble());
  }

  lowCoherenceDataFile.close(); // Close the file
}

/* This function is used to read data from a file and store it in the mediumCoherenceVector vector */
void Reader::readMediumCoherenceDataPoints(QString fileName) {
  QFile mediumCoherenceDataFile(fileName); // File that contains the medium coherence data points

  /* Check if the file can be opened/isn't corrupted, if the file cannot be opened, then print a debug message and exit the program */
  if (!mediumCoherenceDataFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Error: Cannot open file. The program will now exit.";
    exit(1);
  }

  /* Iterate through the file and store each entry in the mediumCoherenceVector vector */
  while (!mediumCoherenceDataFile.atEnd()) {
      QByteArray currentLine = mediumCoherenceDataFile.readLine();
      QString currentDataPoint(currentLine);
      mediumCoherenceVector.push_back(currentDataPoint.toDouble());
  }

  mediumCoherenceDataFile.close(); // Close the file
}

/* This function is used to read all the different coherence scores that correspond to the low, medium and high coherence data points into their respective vectors in the Reader class. */
void Reader::readAllCoherenceScores(QString lowCoherenceFileName, QString mediumCoherenceFileName, QString highCoherenceFileName) {
  /* Create a QFile object for each of the files storing the coherence scores */
  QFile highCoherenceFile(highCoherenceFileName);
  QFile mediumCoherenceFile(mediumCoherenceFileName);
  QFile lowCoherenceFile(lowCoherenceFileName); 

  /* Check if the high coherence score file can be opened/isn't corrupted, if the file cannot be opened, then print a debug message and exit the program */
  if (!highCoherenceFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Error: Cannot open file. The program will now exit.";
    exit(1);
  }

  /* Iterate through the high coherence score file and store each entry in the currentCoherenceScore vector */
  while (!highCoherenceFile.atEnd()) {
      QByteArray currentLine = highCoherenceFile.readLine();
      QString currentDataPoint(currentLine);
      currentCoherenceScore.push_back(currentDataPoint.toDouble());
  }

  highCoherenceFile.close(); // Close the high coherence score file

  /* Check if the medium coherence score file can be opened/isn't corrupted, if the file cannot be opened, then print a debug message and exit the program */
  if (!mediumCoherenceFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Error: Cannot open file. The program will now exit.";
    exit(1);
  }

  /* Iterate through the medium coherence score file and store each entry in the currentCoherenceScore vector */
  while (!mediumCoherenceFile.atEnd()) {
      QByteArray currentLine = mediumCoherenceFile.readLine();
      QString currentDataPoint(currentLine);
      currentCoherenceScore.push_back(currentDataPoint.toDouble());
  }

  mediumCoherenceFile.close(); // Close the medium coherence score file

  /* Check if the low coherence score file can be opened/isn't corrupted, if the file cannot be opened, then print a debug message and exit the program */
  if (!lowCoherenceFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qDebug() << "Error: Cannot open file. The program will now exit.";
    exit(1);
  }

  /* Iterate through the low coherence score file and store each entry in the currentCoherenceScore vector */
  while (!lowCoherenceFile.atEnd()) {
      QByteArray currentLine = lowCoherenceFile.readLine();
      QString currentDataPoint(currentLine);
      currentCoherenceScore.push_back(currentDataPoint.toDouble());
  }

  lowCoherenceFile.close(); // Close the low coherence score file
}

/* This function calculates the coherence score for the current set of data. It does this by checking the currentCoherenceScore vector and getting the value that corresponds to the past 5 seconds of data. */
void Reader::calculateCoherenceScore() {
  /* If the currentCoherenceScore vector is empty, then do notrhing and exit the function */
  if(currentCoherenceScore.size() == 0) {
    return;
  }
  else {
    /* If the currentCoherenceScore vector is not empty, then get the value at the currentCoherenceScoreIndex and store it in the latestCoherenceScore variable */
    latestCoherenceScore = currentCoherenceScore.at(currentCoherenceScoreIndex);
    currentCoherenceScoreIndex++; // Increment the currentCoherenceScoreIndex variable by 1

    /* If the currentCoherenceScoreIndex variable is out of bounds of the currentCoherenceScore vector, then reset the currentCoherenceScoreIndex variable to 0 */
    if(currentCoherenceScoreIndex == currentCoherenceScore.size()) {
      currentCoherenceScoreIndex = 0;
    }
  }
}

void Reader::calculateAchievementScore() {
  latestAchievementScore += latestCoherenceScore; //Add the latest coherence score to the latest achievement score
  numberOfAchievementUpdates++; //Increment the number of achievement updates by 1
}

/* This function is used to reset the Reader instance back to its initial state. */
void Reader::resetData() {
  /* Empty the timeDataPoints, heartRateDataPoints and currentCoherenceScore vectors */
  timeDataPoints.clear();
  heartRateDataPoints.clear();
  currentCoherenceScore.clear();

  /* Reset the integers and double member variables to their initial values */
  lowCoherenceVectorIndex = 0;
  mediumCoherenceVectorIndex = 0;
  switchBetweenCoherence = 0;
  currentCoherenceScoreIndex = 0;

  latestCoherenceScore = 0.0;
}

/* This function is used to return the timeDataPoints vector */
QVector<double> Reader::getTimeDataPoints() {
  return timeDataPoints;
}

/* This function is used to return the heartRateDataPoints vector */
QVector<double> Reader::getHeartRateDataPoints() {
  return heartRateDataPoints;
}

/* This function is used to return the currentCoherenceScore vector */
QVector<double> Reader::getCurrentCoherenceScore() {
  return currentCoherenceScore;
}

/* This function is used to get the latest coherence score generated by the Reader instance by returning the latestCoherenceScore member variable.  */
double Reader::getLatestCoherenceScore() {
  return latestCoherenceScore;
}

/* This function is used to return the last data point generated by the Reader class. */
int Reader::getLatestDataPoint() {
  /* Returns the last data point in the heartRateDataPoints vector */
  return heartRateDataPoints.back();
}

/* This function is used to return the switchBetweenCoherence member variable. */
int Reader::getSwitchBetweenCoherence() {
  return switchBetweenCoherence;
}

double Reader::getLatestAchievementScore() {
  return latestAchievementScore;
}

QVector<double> Reader::getTimeInEachCoherence() {
  return timeInEachCoherence;
}

int Reader::getNumberOfAchievementUpdates() {
  return numberOfAchievementUpdates;
}

QString Reader::getCoherenceLevel(int challengeLevel) {
  switch (challengeLevel) {
    case 1:
      if(latestCoherenceScore < 0.5) {
        return "Low";
      }
      else if(latestCoherenceScore >= 0.5 && latestCoherenceScore <= 0.9) {
        return "Medium";
      }
      else if(latestCoherenceScore > 0.9) {
        return "High";
      }
      break;
    case 2:
      if(latestCoherenceScore < 0.6) {
        return "Low";
      }
      else if(latestCoherenceScore >= 0.6 && latestCoherenceScore <= 2.1) {
        return "Medium";
      }
      else if(latestCoherenceScore > 2.1) {
        return "High";
      }
      break;
    case 3:
      if(latestCoherenceScore < 1.8) {
        return "Low";
      }
      else if(latestCoherenceScore >= 1.8 && latestCoherenceScore <= 4.0) {
        return "Medium";
      }
      else if(latestCoherenceScore > 4.0) {
        return "High";
      }
      break;
    case 4:
      if(latestCoherenceScore < 4.0) {
        return "Low";
      }
      else if(latestCoherenceScore >= 4.0 && latestCoherenceScore <= 6.0) {
        return "Medium";
      }
      else if(latestCoherenceScore > 6.0) {
        return "High";
      }
      break;
    default:
      qDebug() << "Error: Invalid challenge level. The program will now exit";
      exit(1);
    }

    return "Error";
}