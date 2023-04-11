#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "session.h"
#include "menu.h"

int MainWindow::currentSession = -1;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //menu structure
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
    //curQListWidget->addItems(mainMenu->getMenuItems());

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
    connect(ui->deleteSessionButton, &QPushButton::pressed, this, &MainWindow::deleteSession);
    connect(ui->leftButton, &QPushButton::pressed, this, &MainWindow::moveBPSliderLeft);
    connect(ui->rightButton, &QPushButton::pressed, this, &MainWindow::moveBPSliderRight);
    connect(ui->breathSlider, &QSlider::valueChanged, this, &MainWindow::updateSliderText);

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
    QPushButton* coherenceLevelLight = ui->coherenceLight;

    //session screen slots
    connect(this->plottingTimer, &QTimer::timeout, this, [=]()
    { plotDataPoints(mainWidget, graphWidget, coherenceLabel, lengthLabel, achievementLabel, sessions, willUpdate, coherenceLevelLight); });

    this->plottingTimer->start(100); //just for testing, need to put it back to 1000ms (1 second) when done
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
void MainWindow::initializeMainMenu(Menu* m){
    Menu* start = new Menu("Start Session", {}, m);
    Menu* logs = new Menu("Logs/History", {}, m);
    Menu* settings = new Menu("Settings", {"Challenge Level", "Breath Pacer", "Reset Device"}, m);
    m->addChildMenu(start);
    m->addChildMenu(logs);
    m->addChildMenu(settings);
    Menu* challengeLevels = new Menu("Challenge Level", {"1", "2", "3", "4"}, settings);
    Menu* breathPacer = new Menu("Breath Pacer", {}, settings);
    Menu* resetDevice = new Menu("Reset Device", {"Wipe all logs and reset settings"}, settings);
    settings->addChildMenu(challengeLevels);
    settings->addChildMenu(breathPacer);
    settings->addChildMenu(resetDevice);
}

void MainWindow::upButton(){
    int next = curQListWidget->currentRow() - 1;
    if(next < 0){
        next = curQListWidget->count() - 1;
    }
    curQListWidget->setCurrentRow(next);
}

void MainWindow::downButton(){
    int next = curQListWidget->currentRow() + 1;
    if(next > curQListWidget->count() - 1){
        next = 0;
    }
    curQListWidget->setCurrentRow(next);
}

void MainWindow::select(){
    int curr = curQListWidget->currentRow();
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
        mainMenu = mainMenu->get(curr);
        setBreathPacer();
        return;
    }
    else if(ui->selectButton->text()=="Return To Menu") {
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

void MainWindow::updateMenu(const QString child, QVector<QString> childItems){
    curQListWidget->clear();
    //curQListWidget->addItems(childItems);

    for(int i = 0; i < childItems.length(); i++) {
        curQListWidget->addItem(childItems.at(i));
    }

    curQListWidget->setCurrentRow(0);

    ui->menuLabel->setText(child);
    ui->breathPacersetting->setVisible(false);
    ui->sessionScreen->setVisible(false);
    ui->mainListView->setVisible(true);
    ui->selectorButtons->setDisabled(false);
}

void MainWindow::menu(){
    while(mainMenu->getName() != "Menu"){
        mainMenu = mainMenu->getParentMenu();
    }
    updateMenu(mainMenu->getName(), mainMenu->getMenuItems());
}

void MainWindow::goBack(){
    if(mainMenu->getName() != "Menu"){
        mainMenu = mainMenu->getParentMenu();
        updateMenu(mainMenu->getName(), mainMenu->getMenuItems());
    } else {
        updateMenu(mainMenu->getName(), mainMenu->getMenuItems());
    }
}

//CHALLENGE LEVELS FUNCTIONS
void MainWindow::setChallengeLevel(int curr){
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
    //for each session in the vector, deleteSession()
    mainMenu = mainMenu->getParentMenu();
    mainMenu = mainMenu->getParentMenu();
    for(int i = 0; i<sessions->length(); i++){
        mainMenu->getChildMenus()[1]->removeMenuItem(0);
    }
    sessions->clear();

    //set breath pacer back to 10 seconds
    ui->breathSlider->setSliderPosition(10);

    updateMenu(mainMenu->getName(), mainMenu->getMenuItems());

    /* Set the challenge level, current session index and updateSessionGraph booelan back to their default values */
    this->nextChallengeLevel = 1;
    currentSession = -1;
    *(this->updateSessionGraph) = false;
}

//LOG FUNCTIONS
void MainWindow::viewLog(int curr){
    *(this->updateSessionGraph) = false;

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
void MainWindow::setBreathPacer(){
    qInfo("breath pacer setting interval.");
    ui->mainListView->setVisible(false);
    ui->breathPacersetting->setVisible(true);
    ui->breathPacersetting->isEnabled();

    ui->breathSlider->setRange(1, 30);
}

void MainWindow::moveBPSliderLeft(){
    int pos = ui->breathSlider->sliderPosition();
    ui->breathSlider->setSliderPosition(pos-5);
}

void MainWindow::moveBPSliderRight(){
    int pos = ui->breathSlider->sliderPosition();
    ui->breathSlider->setSliderPosition(pos+5);
}

void MainWindow::updateSliderText(){
    int val = ui->breathSlider->sliderPosition();
    ui->breathSliderText->setText("Breath Pacer: " + QString::number(val) + " seconds");
}


void MainWindow::plotDataPoints(QWidget* mainWidget, QCustomPlot *graphWidget, QLabel* coherenceScoreLabel, QLabel* lengthLabel, QLabel* achievementLabel, QVector<Session *>* sessionsVector, bool* updateSessionGraph, QPushButton* coherenceLight) {
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
        coherenceLight->setStyleSheet("background-color: red");
    }
    else if(sessionsVector->at(currentSession)->getCoherenceLevel() == "Medium") {
        coherenceLight->setStyleSheet("background-color: blue");
    }
    else if(sessionsVector->at(currentSession)->getCoherenceLevel() == "High") {
        coherenceLight->setStyleSheet("background-color: green");
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

//SESSION FUNCTIONS
void MainWindow::startSession(){

    addNewSession();

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

    //session mechanisms
    ui->breathPacerLights->setInterval(ui->breathSlider->sliderPosition()*1000);
    ui->breathPacerLights->start();

    ui->averageCoherenceScoreLabel->setVisible(false);
    ui->percentHighLabel->setVisible(false);
    ui->percentMediumLabel->setVisible(false);
    ui->percentLowLabel->setVisible(false);
    ui->dateLabel->setVisible(false);
    ui->deleteSessionButton->setVisible(false);

    ui->coherenceLight->setEnabled(false);
    ui->coherenceLight->setVisible(true);
    ui->coherenceLight->setStyleSheet("background-color: green");
}

void MainWindow::endSession(){
    *(this->updateSessionGraph) = false;

    ui->mainListView->setVisible(false);
    ui->selectorButtons->setDisabled(true);
    ui->selectButton->setDisabled(false);
    ui->selectButton->setText("Return To Menu");

    ui->menuButton->setDisabled(true);
    ui->backButton->setDisabled(true);

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

    ui->coherenceScoreLabel->setText("Coherence Score: " + QString::number(sessions->at(currentSession)->getCurrentCoherence()));
    ui->lengthLabel->setText("Length (in seconds): \n" + QString::number(sessions->at(currentSession)->getLength()));
    ui->achievementScoreLabel->setText("Achievement Score: " + QString::number(sessions->at(currentSession)->getAchievement()));
    ui->averageCoherenceScoreLabel->setText("Average Coherence Score: " + QString::number(sessions->at(currentSession)->getAverageCoherenceScore()));
    ui->percentHighLabel->setText("Percentage of Time in High Coherence: " + QString::number(sessions->at(currentSession)->getPercentageOfHighCoherence()) + "%");
    ui->percentLowLabel->setText("Percentage of Time in Low Coherence: " + QString::number(sessions->at(currentSession)->getPercentageOfLowCoherence()) + "%");
    ui->percentMediumLabel->setText("Percentage of Time in Medium Coherence: " + QString::number(sessions->at(currentSession)->getPercentageOfMediumCoherence()) + "%");
}

void MainWindow::returnToMenu() {
    ui->mainListView->setVisible(true);
    ui->selectorButtons->setDisabled(false);
    ui->selectButton->setText("SELECT");

    ui->menuButton->setDisabled(false);
    ui->backButton->setDisabled(false);

    ui->sessionScreen->setVisible(false);
    ui->menuLabel->setText("Menu");
}

void MainWindow::deleteSession() {
    int deleteSessionIndex = curQListWidget->currentRow();
    if(deleteSessionIndex < 0){return ;}

    ui->mainListView->setVisible(true);
    ui->selectorButtons->setDisabled(false);
    ui->selectButton->setText("SELECT");

    ui->menuButton->setDisabled(false);
    ui->backButton->setDisabled(false);

    ui->sessionScreen->setVisible(false);

    menu();

    mainMenu->getChildMenus()[1]->removeMenuItem(deleteSessionIndex);

    sessions->erase(sessions->begin() + deleteSessionIndex);
    currentSession -= 1;
}
