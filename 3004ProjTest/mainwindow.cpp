/*
    Authors: Shreya Voore, Nguyen-Hanh Nong, Ashok Sivathalayan, Awwab Mahdi
    Purpose: The main window of the application and controls the application.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "session.h"
#include "menu.h"

int MainWindow::currentSession = -1;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), activeSession(false), on(true), hasSignal(true)
{
    ui->setupUi(this);

    //setting up menu tree by making the first menu
    mainMenu = new Menu("Menu", {"Start Session", "Logs/History", "Settings"}, nullptr);
    parentMenu = mainMenu;
    initializeMainMenu(mainMenu);

    //breath pacer settings
    ui->breathPacersetting->setVisible(false);
    ui->breathSlider->setValue(10);

    //session screen
    ui->sessionScreen->setVisible(false);

    //main menu view initialization
    curQListWidget = ui->mainListView;

    for(int i = 0; i<mainMenu->getLength(); i++){
        curQListWidget->addItem(mainMenu->get(i)->getName());
    }

    curQListWidget->setCurrentRow(0);
    ui->menuLabel->setText(mainMenu->getName());

    //menu slots
    connect(ui->upButton, &QPushButton::pressed, this, &MainWindow::upButton);
    connect(ui->downButton, &QPushButton::pressed, this, &MainWindow::downButton);
    connect(ui->selectButton, &QPushButton::pressed, this, &MainWindow::select);
    connect(ui->menuButton, &QPushButton::pressed, this, &MainWindow::menu);
    connect(ui->backButton, &QPushButton::pressed, this, &MainWindow::goBack);
    connect(ui->deleteSessionButton, &QPushButton::released, this, &MainWindow::deleteSession);
    connect(ui->leftButton, &QPushButton::pressed, this, &MainWindow::moveBPSliderLeft);
    connect(ui->rightButton, &QPushButton::pressed, this, &MainWindow::moveBPSliderRight);
    connect(ui->breathSlider, &QSlider::valueChanged, this, &MainWindow::updateSliderText);

    connect(ui->hrvSignalButton, &QPushButton::pressed, this, &MainWindow::changeSignal);
    ui->signalLight->setColour(QColor("green"));
    ui->signalLight->turnOn();
    connect(ui->powerButton, &QPushButton::pressed, this, &MainWindow::powerButton);

    //battery slots
    connect(ui->battery, &Battery::batteryDrained, this, &MainWindow::batteryDead);
    connect(ui->battery, &Battery::batteryCharged, this, &MainWindow::batteryCharged);

    //setup timer and other session related stuff
    this->plottingTimer = new QTimer(this);
    this->nextChallengeLevel = 1;
    this->updateSessionGraph = new bool(false);

    this->sessions = new QVector<Session*>();

    QWidget* mainWidget = ui->sessionScreen;
    QCustomPlot* graphWidget = ui->graphWidget;
    QLabel *coherenceLabel = ui->coherenceScoreLabel;
    QLabel *lengthLabel = ui->lengthLabel;
    QLabel *achievementLabel = ui->achievementScoreLabel;
    QVector<Session* >* sessions = this->sessions;
    bool* willUpdate = this->updateSessionGraph;
    LED* coherenceLevelLight = ui->coherenceLight;
    ui->coherenceLight->turnOn();

    //session screen slots
    connect(this->plottingTimer, &QTimer::timeout, this, [=]()
    { plotDataPoints(mainWidget, graphWidget, coherenceLabel, lengthLabel, achievementLabel, sessions, willUpdate, coherenceLevelLight); });

    this->plottingTimer->start(1000); //just for testing, need to put it back to 1000ms (1 second) when done
}

MainWindow::~MainWindow()
{
    delete parentMenu;
    delete ui;

    delete this->plottingTimer;
    delete this->updateSessionGraph;

    for(int i = 0; i < this->sessions->size(); i++){
        delete this->sessions->at(i);
    }

    this->sessions->clear();
    delete this->sessions;
}

//MENU FUNCTIONS

//intializes main menu
void MainWindow::initializeMainMenu(Menu* m){
    Menu* start = new Menu("Start Session", {}, m); //making each of the sub menus, empty list because it connects to a device action
    Menu* logs = new Menu("Logs/History", {}, m); //empty list because there are no sessions yet
    Menu* settings = new Menu("Settings", {"Challenge Level", "Breath Pacer", "Reset Device"}, m);
    m->addChildMenu(start); //adding the child menus to the main menu
    m->addChildMenu(logs);
    m->addChildMenu(settings);
    Menu* challengeLevels = new Menu("Challenge Level", {"1", "2", "3", "4"}, settings); //making the sub menus for the main sub menus
    Menu* breathPacer = new Menu("Breath Pacer", {}, settings); //empty list because connects to breath pacer slider widget
    Menu* resetDevice = new Menu("Reset Device", {"Wipe all logs and reset settings"}, settings);
    settings->addChildMenu(challengeLevels); //adding the child menus
    settings->addChildMenu(breathPacer);
    settings->addChildMenu(resetDevice);
}

//when the up button is clicked, the highlighted row changes (goes up)
void MainWindow::upButton(){
    int next = curQListWidget->currentRow() - 1;
    if(next < 0){ // if the highlighted row is at the top, when clicking up itll start at the bottom
        next = curQListWidget->count() - 1;
    }
    //sets the actual highlight (logic before finds the intended row index)
    curQListWidget->setCurrentRow(next);
}

//operates similar to the up button, instead moves downwards
void MainWindow::downButton(){
    int next = curQListWidget->currentRow() + 1;
    if(next > curQListWidget->count() - 1){
        next = 0;
    }
    curQListWidget->setCurrentRow(next);
}

//select middle button
void MainWindow::select(){
    int curr = curQListWidget->currentRow(); //getting the current row menu
    if(curr < 0){return ;}

    //if choosing challenge level
    if(mainMenu->getName()=="Challenge Level"){
        qInfo("Setting Challenge Level...");
        setChallengeLevel(curr);
        return;
    } else if (mainMenu->getName()=="Reset Device"){ //if reset device
        resetDevice();
        qInfo("Resetting Device...");
        return;
    } else if (mainMenu->getName()=="Logs/History"){ //if logs/history
        qInfo("Viewing Log...");
        viewLog(curr);
        return;
    } else if(mainMenu->get(curr)->getName()=="Breath Pacer"){ //if breath pacer
        //mainMenu = mainMenu->get(curr);
        setBreathPacer();
        return;
    }
    else if(ui->selectButton->text()=="Return To Menu") { //if return to menu
        returnToMenu();
    }
    else if(mainMenu->get(curr)->getName()=="Start Session"){ //if start session
        if(ui->selectButton->text()=="End Session"){
            endSession();
        }
        else {
            startSession();
        }
        return;
    }
    else if(mainMenu->getLength() > 0){ //if parent menu to a child menu
        if(mainMenu->get(curr)->getMenuItems().length() > 0){
            mainMenu = mainMenu->get(curr);
            MainWindow::updateMenu(mainMenu->getName(), mainMenu->getMenuItems());
        }
    } else {
        qDebug("The selected option is not handled yet. The program will now exit.");
        exit(1);
    }
}

//updates menu with the child items provided
void MainWindow::updateMenu(const QString child, QVector<QString> childItems){
    curQListWidget->clear();
    //clears the current lsit widget

    //adds all the items to the widget
    for(int i = 0; i < childItems.length(); i++) {
        curQListWidget->addItem(childItems.at(i));
    }

    //sets the current highlighted row to the first row
    curQListWidget->setCurrentRow(0);

    //visuals
    ui->menuLabel->setText(child);
    ui->breathPacersetting->setVisible(false);
    ui->sessionScreen->setVisible(false);
    ui->mainListView->setVisible(true);
    ui->selectorButtons->setDisabled(false);
}

//when the menu button is clicked, return to the main original menu
void MainWindow::menu(){
    //goes back to the parent menus until the main menu is the original menu with the name "Menu"
    while(mainMenu->getName() != "Menu"){
        mainMenu = mainMenu->getParentMenu();
    }
    //once we get the right menu, update the list widget
    updateMenu(mainMenu->getName(), mainMenu->getMenuItems());
}

//back button is clicked, goes back to the parent menu
void MainWindow::goBack(){
    if(mainMenu->getName() != "Menu"){ //if its not the original menu, go back to parent menu
        mainMenu = mainMenu->getParentMenu();
        updateMenu(mainMenu->getName(), mainMenu->getMenuItems());
    } else { //if its the original menu, everything stays the same
        updateMenu(mainMenu->getName(), mainMenu->getMenuItems());
    }
}

//CHALLENGE LEVELS FUNCTIONS
void MainWindow::setChallengeLevel(int curr){ //sets the challenge level based on the int chosen
    if(mainMenu->getMenuItems()[curr] == "1"){
        this->nextChallengeLevel = 1;
        menu();
    } else if(mainMenu->getMenuItems()[curr] == "2"){
        this->nextChallengeLevel = 2;
        menu();
    } else if(mainMenu->getMenuItems()[curr] == "3"){
        this->nextChallengeLevel = 3;
        menu();
    } else if(mainMenu->getMenuItems()[curr] == "4"){
        this->nextChallengeLevel = 4;
        menu();
    } else {
        qDebug("ERROR. The challenge level should not be set to this value. The program will now exit.");
        exit(1);
    }
}

//RESET DEVICE
void MainWindow::resetDevice(){
    //first delete all logs
    //for each session in the vector, remove the menu item
    mainMenu = mainMenu->getParentMenu();
    mainMenu = mainMenu->getParentMenu();
    for(int i = 0; i<sessions->length(); i++){
        mainMenu->getChildMenus()[1]->removeMenuItem(0);
    }
    //clear the sessions vector
    sessions->clear();

    //set breath pacer back to 10 seconds
    ui->breathSlider->setSliderPosition(10);

    updateMenu(mainMenu->getName(), mainMenu->getMenuItems());

    /* Set the challenge level, current session index and updateSessionGraph boolean back to their default values */
    this->nextChallengeLevel = 1;
    currentSession = -1;
    *(this->updateSessionGraph) = false;
}

//LOG FUNCTIONS
void MainWindow::viewLog(int curr){
    *(this->updateSessionGraph) = false;

    //visuals
    ui->mainListView->setVisible(false);

    ui->menuButton->setDisabled(false);
    ui->menuButton->setVisible(true);

    ui->backButton->setDisabled(false);
    ui->backButton->setVisible(true);

    ui->sessionScreen->setVisible(true);
    ui->menuLabel->setText("Log Screen: Session " + QString::number(sessions->at(curr)->getSessionID()));

    ui->breathPacerLights->setVisible(false);
    ui->coherenceLight->setVisible(false);

    ui->averageCoherenceScoreLabel->setVisible(true);
    ui->percentHighLabel->setVisible(true);
    ui->percentMediumLabel->setVisible(true);
    ui->percentLowLabel->setVisible(true);
    ui->dateLabel->setVisible(true);
    ui->deleteSessionButton->setVisible(true);

    ui->coherenceScoreLabel->setText("Coherence Score: " + QString::number(sessions->at(curr)->getCurrentCoherence()));
    ui->lengthLabel->setText("Length (in seconds): \n" + QString::number(sessions->at(curr)->getLength()));
    ui->achievementScoreLabel->setText("Achievement Score: " + QString::number(sessions->at(curr)->getAchievement()));
    ui->averageCoherenceScoreLabel->setText("Average Coherence Score: " + QString::number(sessions->at(curr)->getAverageCoherenceScore()));
    ui->percentHighLabel->setText("Percentage of Time in High Coherence: " + QString::number(sessions->at(curr)->getPercentageOfHighCoherence()) + "%");
    ui->percentLowLabel->setText("Percentage of Time in Low Coherence: " + QString::number(sessions->at(curr)->getPercentageOfLowCoherence()) + "%");
    ui->percentMediumLabel->setText("Percentage of Time in Medium Coherence: " + QString::number(sessions->at(curr)->getPercentageOfMediumCoherence()) + "%");

    ui->dateLabel->setText("Date: " + sessions->at(curr)->getDateCreated());

    ui->graphWidget->addGraph();
    ui->graphWidget->graph(0)->setPen(QPen(Qt::blue));

    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(ui->graphWidget->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->graphWidget->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->graphWidget->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->graphWidget->yAxis2, SLOT(setRange(QCPRange)));

    // pass data points to graphs:
    ui->graphWidget->graph(0)->setData(sessions->at(curr)->getReader().getTimeDataPoints(), sessions->at(curr)->getReader().getHeartRateDataPoints());

    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    ui->graphWidget->rescaleAxes();
    ui->graphWidget->yAxis->setRange(0, 180);

    ui->graphWidget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->graphWidget->axisRect()->setRangeDrag(Qt::Horizontal);
    ui->graphWidget->axisRect()->setRangeZoom(Qt::Horizontal);
    ui->graphWidget->axisRect()->setRangeZoomAxes(ui->graphWidget->xAxis, NULL); //To block y axis zoom NULL axis as horizontal
    ui->graphWidget->setSelectionRectMode(QCP::srmZoom);

    ui->graphWidget->xAxis->setLabel("Time Elapsed (in seconds) ");
    ui->graphWidget->yAxis->setLabel("Heart Rate (in BPM)");

    ui->graphWidget->replot(QCustomPlot::rpRefreshHint);
}


//BREATH PACER FUNCTIONS

//displays the breath pacer slider
void MainWindow::setBreathPacer(){
    qInfo("breath pacer setting interval.");
    //
    ui->mainListView->setVisible(false);
    ui->breathPacersetting->setVisible(true);
    ui->breathPacersetting->isEnabled();

    ui->breathSlider->setRange(1, 30);
}

//when left button is clicked, the slider moves left by an increment
void MainWindow::moveBPSliderLeft(){
    int pos = ui->breathSlider->sliderPosition();
    ui->breathSlider->setSliderPosition(pos-5);
}

//when the right button is clicked, the slider moves right by an increment
void MainWindow::moveBPSliderRight(){
    int pos = ui->breathSlider->sliderPosition();
    ui->breathSlider->setSliderPosition(pos+5);
}

//updates slider text based on the slider value
void MainWindow::updateSliderText(){
    int val = ui->breathSlider->sliderPosition();
    ui->breathSliderText->setText("Breath Pacer: " + QString::number(val) + " seconds");
}


//SESSION FUNCTIONS
//plots the data points for the session graph
void MainWindow::plotDataPoints(QWidget* mainWidget, QCustomPlot *graphWidget, QLabel* coherenceScoreLabel, QLabel* lengthLabel, QLabel* achievementLabel, QVector<Session *>* sessionsVector, bool* updateSessionGraph, LED* coherenceLight) {
    //if there are no sessions or the main widget is not visible, return
    if(currentSession == -1 || mainWidget->isVisible() == false || *updateSessionGraph == false) {
        return;
    }

    sessionsVector->at(currentSession)->update();

    if(sessionsVector->at(currentSession)->getLength() % 5 == 0) {
        coherenceScoreLabel->setText("Coherence Score: \n" + QString::number(sessionsVector->at(currentSession)->getCurrentCoherence()));
        achievementLabel->setText("Achievement Score: \n" + QString::number(sessionsVector->at(currentSession)->getAchievement()));
    }

    if(sessionsVector->at(currentSession)->getCoherenceLevel() == "Low") {
        coherenceLight->setColour(QColor("red"));
    }
    else if(sessionsVector->at(currentSession)->getCoherenceLevel() == "Medium") {
        coherenceLight->setColour(QColor("blue"));
    }
    else if(sessionsVector->at(currentSession)->getCoherenceLevel() == "High") {
        coherenceLight->setColour(QColor("green"));
    }

    lengthLabel->setText("Length (in seconds): \n" + QString::number(sessionsVector->at(currentSession)->getLength()));

    graphWidget->addGraph();
    graphWidget->graph(0)->setPen(QPen(Qt::blue));

    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(graphWidget->xAxis, SIGNAL(rangeChanged(QCPRange)), graphWidget->xAxis2, SLOT(setRange(QCPRange)));
    connect(graphWidget->yAxis, SIGNAL(rangeChanged(QCPRange)), graphWidget->yAxis2, SLOT(setRange(QCPRange)));

    // pass data points to graphs:
    graphWidget->graph(0)->setData(sessionsVector->at(currentSession)->getReader().getTimeDataPoints(), sessionsVector->at(currentSession)->getReader().getHeartRateDataPoints());

    // let the ranges scale themselves so graph 0 fits perfectly in the visible area:
    graphWidget->rescaleAxes();
    graphWidget->yAxis->setRange(0, 180);

    graphWidget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    graphWidget->axisRect()->setRangeDrag(Qt::Horizontal);
    graphWidget->axisRect()->setRangeZoom(Qt::Horizontal);
    graphWidget->axisRect()->setRangeZoomAxes(graphWidget->xAxis, NULL); //To block y axis zoom NULL axis as horizontal
    graphWidget->setSelectionRectMode(QCP::srmZoom);

    graphWidget->xAxis->setLabel("Time Elapsed (in seconds) ");
    graphWidget->yAxis->setLabel("Heart Rate (in BPM)");

    graphWidget->replot(QCustomPlot::rpRefreshHint);
}

/* This function creates a new session and adds it to the sessions vector. */
void MainWindow::addNewSession() {
    sessions->push_back(new Session(nullptr, this->nextChallengeLevel));
    MainWindow::currentSession += 1;

    qDebug("what is current session? %d", currentSession);

    mainMenu->getChildMenus()[1]->addNewMenuItem("Session " + QString::number(sessions->at(currentSession)->getSessionID()));
}

//starts the session when the start session button is clicked
void MainWindow::startSession(){
    if(!hasSignal){ //checks if the hrv signal is on
        qDebug()<<"Needs active signal to start session!";
        return;
    }
    addNewSession(); //adds new session to the vector
    activeSession = true;

    //visuals
    ui->mainListView->setVisible(false);
    ui->selectorButtons->setDisabled(true);
    ui->selectButton->setDisabled(false);
    ui->selectButton->setText("End Session");
    ui->breathPacerLights->setVisible(true);

    ui->menuButton->setDisabled(true);
    ui->backButton->setDisabled(true);

    ui->sessionScreen->setVisible(true);
    ui->menuLabel->setText("Session" + QString::number(sessions->at(currentSession)->getSessionID()));

    *(this->updateSessionGraph) = true;

    //starts breath pacer animation/lights
    ui->breathPacerLights->setInterval(ui->breathSlider->sliderPosition()*1000);
    ui->breathPacerLights->start();

    //visuals
    ui->averageCoherenceScoreLabel->setVisible(false);
    ui->percentHighLabel->setVisible(false);
    ui->percentMediumLabel->setVisible(false);
    ui->percentLowLabel->setVisible(false);
    ui->dateLabel->setVisible(false);
    ui->deleteSessionButton->setVisible(false);

    ui->coherenceLight->setEnabled(false);
    ui->coherenceLight->setVisible(true);
    ui->coherenceLight->setColour(QColor("green"));
}

//ends session when select button is clicked
void MainWindow::endSession(){
    *(this->updateSessionGraph) = false; //stop updating the session graph
    activeSession = false;
    //visuals
    ui->mainListView->setVisible(false);
    ui->selectorButtons->setDisabled(true);
    ui->selectButton->setDisabled(false);
    ui->selectButton->setText("Return To Menu");
    ui->menuButton->setDisabled(true);
    ui->backButton->setDisabled(true);
    //shows the session data recorded
    ui->sessionScreen->setVisible(true);
    ui->menuLabel->setText("Summary Screen: Session " + QString::number(sessions->at(currentSession)->getSessionID()) + " Ended");
    ui->breathPacerLights->stop();
    this->sessions->at(currentSession)->calculateSummaryData();
    ui->averageCoherenceScoreLabel->setVisible(true);
    ui->percentHighLabel->setVisible(true);
    ui->percentMediumLabel->setVisible(true);
    ui->percentLowLabel->setVisible(true);
    ui->dateLabel->setVisible(false);
    ui->deleteSessionButton->setVisible(false);
    qDebug()<<4;
    ui->coherenceScoreLabel->setText("Coherence Score: " + QString::number(sessions->at(currentSession)->getCurrentCoherence()));
    ui->lengthLabel->setText("Length (in seconds): \n" + QString::number(sessions->at(currentSession)->getLength()));
    ui->achievementScoreLabel->setText("Achievement Score: " + QString::number(sessions->at(currentSession)->getAchievement()));
    ui->averageCoherenceScoreLabel->setText("Average Coherence Score: " + QString::number(sessions->at(currentSession)->getAverageCoherenceScore()));
    ui->percentHighLabel->setText("Percentage of Time in High Coherence: " + QString::number(sessions->at(currentSession)->getPercentageOfHighCoherence()) + "%");
    ui->percentLowLabel->setText("Percentage of Time in Low Coherence: " + QString::number(sessions->at(currentSession)->getPercentageOfLowCoherence()) + "%");
    ui->percentMediumLabel->setText("Percentage of Time in Medium Coherence: " + QString::number(sessions->at(currentSession)->getPercentageOfMediumCoherence()) + "%");
}

//when the middle button is clicked, the list widget is set to visible
void MainWindow::returnToMenu() {
    ui->mainListView->setVisible(true);
    ui->selectorButtons->setDisabled(false);
    ui->selectButton->setText("SELECT");

    ui->menuButton->setDisabled(false);
    ui->backButton->setDisabled(false);

    ui->sessionScreen->setVisible(false);
    ui->menuLabel->setText("Menu");
}

//removes session
void MainWindow::deleteSession() {
    int deleteSessionIndex = curQListWidget->currentRow(); //gets the index of the session meant to be deleted
    if(deleteSessionIndex < 0){return ;}

    ui->mainListView->setVisible(true);
    ui->selectorButtons->setDisabled(false);
    ui->selectButton->setText("SELECT");

    ui->menuButton->setDisabled(false);
    ui->backButton->setDisabled(false);

    ui->sessionScreen->setVisible(false);

    menu(); //goes back to main menu

    mainMenu->getChildMenus()[1]->removeMenuItem(deleteSessionIndex); //removes the menu item at the index

    sessions->erase(sessions->begin() + deleteSessionIndex); //removes the session from the session vector
    currentSession -= 1; //updates the id to be used for the next session
}

//if the battery is dead, the device turns off
void MainWindow::batteryDead(){
   turnOff();
}

//if the battery has charge, the device turns on
void MainWindow::batteryCharged(){
    turnOn();
}

//changes the signal for hrv
void MainWindow::changeSignal(){
    //Switching the signal
    hasSignal = !hasSignal;

    if(!hasSignal){
        if(activeSession){
            //Pause session if signal is lost
            *(this->updateSessionGraph) = false;
            ui->breathPacerLights->stop();
        }
        ui->signalLight->setColour(QColor("red"));
    }
    else{
        if(activeSession){
            //resume session if signal is regained
            *(this->updateSessionGraph) = true;
            ui->breathPacerLights->start();
        }
        ui->signalLight->setColour(QColor("green"));
    }
}

//turns on device
void MainWindow::turnOn(){
    on = true;
    //Enabling all the device buttons
    ui->mainListView->setEnabled(true);
    ui->selectButton->setEnabled(true);
    ui->selectorButtons->setEnabled(true);
    ui->menuButton->setEnabled(true);
    ui->backButton->setEnabled(true);
    ui->sessionScreen->setEnabled(true);
    ui->mainListView->setVisible(true);

    //Draining battery
    ui->battery->startBattery();
}

//turns off device
void MainWindow::turnOff(){
    on = false;
    //Ending session if one is active
    if(activeSession){
        activeSession = false;
        ui->sessionScreen->setEnabled(false);
        endSession();
    }
    returnToMenu();
    //Disabling the buttons and screen
    ui->mainListView->setEnabled(false);
    ui->selectButton->setEnabled(false);
    ui->selectorButtons->setEnabled(false);
    ui->menuButton->setEnabled(false);
    ui->backButton->setEnabled(false);

    ui->mainListView->setVisible(false);
    ui->battery->stopBattery();
}

//power button slot, if the power button is clicked, device turns either on or off
void MainWindow::powerButton(){
    if(!on) turnOn();
    else turnOff();
}
