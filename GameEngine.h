#pragma once

#include <array>
#include <vector>
#include <utility>

// Размер доски 8×8.
const int BOARD_SIZE = 8;
using Board = std::array<std::array<int, BOARD_SIZE>, BOARD_SIZE>;

// Структура состояния игры: расположение фишек и чей ход.
struct GameState {
    Board board;
    int currentPlayer;
};

enum Mode { PvB, BvB };       // Режимы: Игрок против Бота и Бот против Бота.
enum Difficulty { Easy, Medium, Hard };

class GameEngine {
public:
    GameEngine();
    
    // Начать новую игру с выбранными параметрами.
    void newGame(Mode mode, Difficulty diff);
    
    // Ход игрока (для режима PvB, игрок ходит фишками -1).
    bool playerMove(int row, int col);
    
    // Выполняет ход бота.
    void botMove();
    
    // Проверка на окончание игры (если ни у кого нет допустимых ходов).
    bool isGameOver() const;
    
    // Подсказка: оптимальный ход для текущего игрока через alpha‑beta.
    std::pair<int,int> hintMove();
    
    // Отменить ход (работает только в режиме PvB).
    void undoMove();
    
    // Сохранение и загрузка состояния игры.
    void saveCurrentState();
    void loadSavedState();

    // Публичный метод для получения списка допустимых ходов.
    std::vector<std::pair<int, int>> getLegalMoves() const; 

    // Текущее состояние доски и чей ход.
    Board board;
    int currentPlayer;
    
    // Режим игры и сложность.
    Mode mode;
    Difficulty difficulty;
    int aiDepth;              // Глубина поиска для ИИ.
    
    // История состояний для возможности отмены.
    std::vector<GameState> history;

private:
    GameState savedState;
    void saveState();
};
