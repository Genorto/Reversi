#pragma once
#include <array>
#include <vector>
#include <utility>

const int BOARD_SIZE = 8;
using Board = std::array<std::array<int, BOARD_SIZE>, BOARD_SIZE>;
struct GameState {
    Board board;
    int currentPlayer;
};
enum Mode { PvB, BvB };
enum Difficulty { Easy, Medium, Hard };

class GameEngine {
public:
    GameEngine();
    void newGame(Mode mode, Difficulty diff);
    bool playerMove(int row, int col);
    void botMove();
    bool isGameOver() const;
    std::pair<int,int> hintMove();
    void undoMove();
    void saveCurrentState();
    void loadSavedState();
    std::vector<std::pair<int, int>> getLegalMoves() const; 
    Board board;
    int currentPlayer;
    Mode mode;
    Difficulty difficulty;
    int aiDepth;
    std::vector<GameState> history;

private:
    GameState savedState;
    void saveState();
};
