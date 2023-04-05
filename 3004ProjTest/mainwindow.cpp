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
    delete ui;
}

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
    //need if statements for if they chose start session, or logs/history
    int curr = curQListWidget->currentRow();
    if(curr < 0){return ;}
    //qInfo("%d", curr);

    //if parent menu to child menu
    if(mainMenu->getLength() > 0){
        if(mainMenu->get(curr)->getMenuItems().length() > 0){
            mainMenu = mainMenu->get(curr);
            MainWindow::updateMenu(mainMenu->getName(), mainMenu->getMenuItems());
        }
    }
}

void MainWindow::updateMenu(const QString child, const QStringList childItems){
    curQListWidget->clear();
    curQListWidget->addItems(childItems);
    curQListWidget->setCurrentRow(0);

    ui->menuLabel->setText(child);
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
    }
}


