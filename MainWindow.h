#pragma once
#include <QMainWindow>

class QStackedWidget;
class MenuWidget;
class GameWidget;
class GameEngine;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void startGame(int mode, int difficulty);
    void backToMenu();

private:
    QStackedWidget *stackedWidget;
    MenuWidget *menuWidget;
    GameWidget *gameWidget;
    GameEngine *gameEngine;
};
