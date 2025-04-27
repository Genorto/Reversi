#include "MainWindow.h"
#include "MenuWidget.h"
#include "GameWidget.h"
#include "GameEngine.h"
#include <QStackedWidget>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Реверси");
    resize(600, 800);
    
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);
    
    menuWidget = new MenuWidget();
    stackedWidget->addWidget(menuWidget);
    
    gameEngine = new GameEngine();
    gameWidget = nullptr;
    
    connect(menuWidget, &MenuWidget::startGame, this, &MainWindow::startGame);
}

void MainWindow::startGame(int mode, int difficulty) {
    Mode m = (mode == 0 ? PvB : BvB);
    Difficulty d;
    if (difficulty == 0) d = Easy;
    else if (difficulty == 1) d = Medium;
    else d = Hard;
    gameEngine->newGame(m, d);
    if (gameWidget) {
        stackedWidget->removeWidget(gameWidget);
        gameWidget->deleteLater();
    }
    gameWidget = new GameWidget(gameEngine);
    stackedWidget->addWidget(gameWidget);
    stackedWidget->setCurrentWidget(gameWidget);
    
    connect(gameWidget, &GameWidget::returnToMenu, this, &MainWindow::backToMenu);
}

void MainWindow::backToMenu() {
    stackedWidget->setCurrentWidget(menuWidget);
}
