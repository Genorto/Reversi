#include "GameEngine.h"
#include <limits>
#include <vector>
#include <utility>

static bool inBounds(int row, int col) {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

static Board initBoard() {
    Board board;
    for (int i = 0; i < BOARD_SIZE; i++) {
        board[i].fill(0);
    }
    board[3][3] = 1;
    board[3][4] = -1;
    board[4][3] = -1;
    board[4][4] = 1;
    return board;
}

static const int dx[8] = {-1, -1, -1,  0, 0, 1, 1, 1};
static const int dy[8] = {-1,  0,  1, -1, 1,-1, 0, 1};

static bool isValidMove(const Board &board, int row, int col, int player) {
    if (!inBounds(row, col) || board[row][col] != 0)
         return false;
    for (int d = 0; d < 8; ++d) {
         int r = row + dx[d], c = col + dy[d];
         if (!inBounds(r, c) || board[r][c] != -player)
              continue;
         r += dx[d];
         c += dy[d];
         while (inBounds(r, c)) {
              if (board[r][c] == 0)
                   break;
              if (board[r][c] == player)
                   return true;
              r += dx[d];
              c += dy[d];
         }
    }
    return false;
}

static std::vector<std::pair<int,int>> calculateLegalMoves(const Board &board, int player) {
    std::vector<std::pair<int,int>> moves;
    for (int i = 0; i < BOARD_SIZE; i++) {
         for (int j = 0; j < BOARD_SIZE; j++) {
              if (isValidMove(board, i, j, player))
                  moves.push_back({i, j});
         }
    }
    return moves;
}

static void makeMove(Board &board, int row, int col, int player) {
    board[row][col] = player;
    for (int d = 0; d < 8; d++) {
         int r = row + dx[d], c = col + dy[d];
         if (!inBounds(r, c) || board[r][c] != -player)
              continue;
         std::vector<std::pair<int,int>> cellsToFlip;
         cellsToFlip.push_back({r, c});
         r += dx[d]; c += dy[d];
         bool valid = false;
         while (inBounds(r, c)) {
              if (board[r][c] == 0)
                   break;
              if (board[r][c] == player) {
                   valid = true;
                   break;
              }
              cellsToFlip.push_back({r, c});
              r += dx[d];
              c += dy[d];
         }
         if (valid) {
              for (auto &cell : cellsToFlip)
                  board[cell.first][cell.second] = player;
         }
    }
}

static int evaluate(const Board &board, int player) {
    int sum = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
         for (int j = 0; j < BOARD_SIZE; j++)
              sum += board[i][j];
    return sum * player;
}

static int alphaBeta(Board board, int depth, int alpha, int beta, int player) {
    std::vector<std::pair<int,int>> moves = calculateLegalMoves(board, player);
    if (depth == 0 || (moves.empty() && calculateLegalMoves(board, -player).empty()))
         return evaluate(board, player);
    if (moves.empty())
         return -alphaBeta(board, depth, -beta, -alpha, -player);
    for (auto move : moves) {
         Board newBoard = board;
         makeMove(newBoard, move.first, move.second, player);
         int score = -alphaBeta(newBoard, depth - 1, -beta, -alpha, -player);
         if (score > alpha)
              alpha = score;
         if (alpha >= beta)
              break;
    }
    return alpha;
}

static std::pair<int,int> findBestMove(const Board &board, int player, int depth) {
    std::vector<std::pair<int,int>> moves = calculateLegalMoves(board, player);
    std::pair<int,int> bestMove = {-1, -1};
    int bestScore = std::numeric_limits<int>::min();
    const int INF = std::numeric_limits<int>::max();
    for (auto move : moves) {
         Board newBoard = board;
         makeMove(newBoard, move.first, move.second, player);
         int score = -alphaBeta(newBoard, depth - 1, -INF, INF, -player);
         if (score > bestScore) {
             bestScore = score;
             bestMove = move;
         }
    }
    return bestMove;
}

GameEngine::GameEngine() {
    newGame(PvB, Medium);
}

void GameEngine::newGame(Mode m, Difficulty diff) {
    mode = m;
    if (mode == BvB) difficulty = Medium;
    else difficulty = diff;
    switch (difficulty) {
         case Easy:   aiDepth = 3; break;
         case Medium: aiDepth = 5; break;
         case Hard:   aiDepth = 7; break;
    }
    board = initBoard();
    currentPlayer = 1;
    history.clear();
    saveState();
}

bool GameEngine::playerMove(int row, int col) {
    std::vector<std::pair<int,int>> legalMoves = calculateLegalMoves(board, currentPlayer);
    if (!legalMoves.empty()) {
        if (isValidMove(board, row, col, currentPlayer)) {
            makeMove(board, row, col, currentPlayer);
            currentPlayer = -currentPlayer;
            saveState();
            return true;
        }
        return false;
    } else {
        int ownCount = 0;
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == currentPlayer)
                    ownCount++;
            }
        }
        if (ownCount > 0) {
            currentPlayer = -currentPlayer;
            saveState();
            return false;
        } else {
            if (board[row][col] == -currentPlayer) {
                board[row][col] = currentPlayer;
                currentPlayer = -currentPlayer;
                saveState();
                return true;
            }
            return false;
        }
    }
}

void GameEngine::botMove() {
    std::vector<std::pair<int,int>> legalMoves = calculateLegalMoves(board, currentPlayer);
    if (!legalMoves.empty()) {
        std::pair<int,int> move;
        move = findBestMove(board, currentPlayer, aiDepth);
        if (move.first != -1) makeMove(board, move.first, move.second, currentPlayer);
        currentPlayer = -currentPlayer;
        saveState();
        return;
    } else {
        int ownCount = 0;
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (board[i][j] == currentPlayer)
                    ownCount++;
            }
        }
        if (ownCount > 0) {
            currentPlayer = -currentPlayer;
            saveState();
            return;
        } else {
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    if (board[i][j] == -currentPlayer) {
                        board[i][j] = currentPlayer;
                        currentPlayer = -currentPlayer;
                        saveState();
                        return;
                    }
                }
            }
        }
    }
}

bool GameEngine::isGameOver() const {
    std::vector<std::pair<int, int>> legal1 = calculateLegalMoves(board, currentPlayer);
    std::vector<std::pair<int, int>> legal2 = calculateLegalMoves(board, -currentPlayer);
    return legal1.empty() && legal2.empty();
}

std::pair<int,int> GameEngine::hintMove() {
    return findBestMove(board, currentPlayer, aiDepth);
}

void GameEngine::undoMove() {
    if (mode == PvB && history.size() >= 3) {
         history.pop_back();
         history.pop_back();
         GameState last = history.back();
         board = last.board;
         currentPlayer = last.currentPlayer;
    }
}

void GameEngine::saveCurrentState() {
    savedState.board = board;
    savedState.currentPlayer = currentPlayer;
}

void GameEngine::loadSavedState() {
    board = savedState.board;
    currentPlayer = savedState.currentPlayer;
    saveState();
}

void GameEngine::saveState() {
    GameState state;
    state.board = board;
    state.currentPlayer = currentPlayer;
    history.push_back(state);
}

std::vector<std::pair<int, int>> GameEngine::getLegalMoves() const {
    return calculateLegalMoves(board, currentPlayer);
}
