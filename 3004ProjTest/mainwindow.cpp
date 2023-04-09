#include "mainwindow.h"
#include "ui_mainwindow.h"

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

    //session screen
    ui->sessionScreen->setVisible(false);

    //session list
    //initializeSessions();

    //main menu view initialization
    curQListWidget = ui->mainListView;
    curQListWidget->addItems(mainMenu->getMenuItems());
    curQListWidget->setCurrentRow(0);
    ui->menuLabel->setText(mainMenu->getName());

    //menu slots
    connect(ui->upButton, &QPushButton::pressed, this, &MainWindow::upButton);
    connect(ui->downButton, &QPushButton::pressed, this, &MainWindow::downButton);
    connect(ui->selectButton, &QPushButton::pressed, this, &MainWindow::select);
    connect(ui->menuButton, &QPushButton::pressed, this, &MainWindow::menu);
    connect(ui->backButton, &QPushButton::pressed, this, &MainWindow::goBack);

}

MainWindow::~MainWindow()
{
    delete parentMenu;
    delete ui;
}

//MENU FUNCTIONS
void MainWindow::initializeMainMenu(Menu* m){
    Menu* start = new Menu("Start Session", {}, m);
    Menu* logs = new Menu("Logs/History", {"Session 1", "Session 2", "Session 3"}, m);
    Menu* settings = new Menu("Settings", {"Challenge Level", "Breath Pacer", "Reset Device"}, m);
    m->addChildMenu(start);
    m->addChildMenu(logs);
    m->addChildMenu(settings);
    Menu* challengeLevels = new Menu("Challenge Level", {"1", "2", "3", "4", "5"}, settings);
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
    } else if(mainMenu->get(curr)->getName()=="Start Session"){ //if start session
        if(ui->selectButton->text()=="End Session"){
            endSession();
        } else {
            startSession();
        }
        return;
    } else if(mainMenu->getLength() > 0){ //if parent menu to a child menu
        if(mainMenu->get(curr)->getMenuItems().length() > 0){
            mainMenu = mainMenu->get(curr);
            MainWindow::updateMenu(mainMenu->getName(), mainMenu->getMenuItems());
        }
    } else {
        qInfo("NOPE");
    }
}

void MainWindow::updateMenu(const QString child, const QStringList childItems){
    //need to end session here somewhere
    //ui->breathPacerLights->stop();

    curQListWidget->clear();
    curQListWidget->addItems(childItems);
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
        qInfo("1");
    } else if(mainMenu->getMenuItems()[curr] == "2"){
        qInfo("2");
    } else if(mainMenu->getMenuItems()[curr] == "3"){
        qInfo("3");
    } else if(mainMenu->getMenuItems()[curr] == "4"){
        qInfo("4");
    } else if(mainMenu->getMenuItems()[curr] == "5"){
        qInfo("5");
    } else {
        qInfo("ERROR");
    }
}

//RESET DEVICE
void MainWindow::resetDevice(){

}

//LOG FUNCTIONS
void MainWindow::viewLog(int curr){
    qInfo("%d", curr);
}

//SHOW BREATH PACER
void MainWindow::setBreathPacer(){
    qInfo("breath pacer setting interval...");
    ui->mainListView->setVisible(false);
    ui->breathPacersetting->setVisible(true);
    ui->breathPacersetting->isEnabled();

}

//SESSION FUNCTIONS
void MainWindow::startSession(){
    //visuals
    ui->mainListView->setVisible(false);
    ui->selectorButtons->setDisabled(true);
    ui->selectButton->setDisabled(false);
    ui->selectButton->setText("End Session");

    ui->menuButton->setDisabled(true);
    ui->backButton->setDisabled(true);

    ui->sessionScreen->setVisible(true);
    ui->menuLabel->setText("Session");

    qInfo("Session starting...");

    //session mechanisms
    ui->breathPacerLights->setInterval(ui->breathSlider->sliderPosition()*1000);
    ui->breathPacerLights->start();


}

void MainWindow::endSession(){
    ui->mainListView->setVisible(true);
    ui->selectorButtons->setDisabled(false);
    ui->selectButton->setText("Select");

    ui->menuButton->setDisabled(false);
    ui->backButton->setDisabled(false);

    ui->sessionScreen->setVisible(false);
    ui->menuLabel->setText("Menu");

    //session mechanisms
    ui->breathPacerLights->stop();
}


