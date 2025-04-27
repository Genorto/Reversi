#pragma once
#include <QWidget>
#include "GameEngine.h"
#include "CellButton.h"

class QLabel;
class QTimer;

class GameWidget : public QWidget {
    Q_OBJECT
public:
    explicit GameWidget(GameEngine *engine, QWidget *parent = nullptr);

signals:
    void returnToMenu();

private slots:
    void handleCellClick(int row, int col);
    void processTurn();
    void undoMove();
    void saveGame();
    void loadGame();
    void showHint();
    void backToMenu();

private:
    void updateBoard();
    void updateTurnLabel();
    void showResult();
    GameEngine *gameEngine;
    CellButton* boardButtons[BOARD_SIZE][BOARD_SIZE];
    QLabel *turnLabel;
    QTimer *botMoveTimer;
};
