#include "GameWidget.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QMessageBox>

GameWidget::GameWidget(GameEngine *engine, QWidget *parent)
    : QWidget(parent), gameEngine(engine)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Метка для отображения текущего хода.
    turnLabel = new QLabel(this);
    turnLabel->setAlignment(Qt::AlignCenter);
    turnLabel->setStyleSheet("font-size: 16pt; color: #00695C;");
    mainLayout->addWidget(turnLabel);

    // Сетка игрового поля (8×8).
    QGridLayout *gridLayout = new QGridLayout();
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            // Создаем экземпляр CellButton вместо QPushButton.
            boardButtons[i][j] = new CellButton(this);
            boardButtons[i][j]->setFixedSize(50, 50);
            boardButtons[i][j]->setStyleSheet("background-color: #B2DFDB;");
            gridLayout->addWidget(boardButtons[i][j], i, j);
            connect(boardButtons[i][j], &CellButton::clicked, this, [=](){
                handleCellClick(i, j);
            });
        }
    }
    mainLayout->addLayout(gridLayout);

    // Панель управления.
    QHBoxLayout *controlLayout = new QHBoxLayout();
    QPushButton *menuButton = new QPushButton("Меню", this);
    QPushButton *undoButton = new QPushButton("Отмена хода", this);
    QPushButton *saveButton = new QPushButton("Сохранить", this);
    QPushButton *loadButton = new QPushButton("Загрузить", this);
    QPushButton *hintButton = new QPushButton("Подсказка", this);
    controlLayout->addWidget(menuButton);
    controlLayout->addWidget(undoButton);
    controlLayout->addWidget(saveButton);
    controlLayout->addWidget(loadButton);
    controlLayout->addWidget(hintButton);
    mainLayout->addLayout(controlLayout);

    setLayout(mainLayout);

    // Связь кнопок управления.
    connect(menuButton, &QPushButton::clicked, this, &GameWidget::backToMenu);
    connect(undoButton, &QPushButton::clicked, this, &GameWidget::undoMove);
    connect(saveButton, &QPushButton::clicked, this, &GameWidget::saveGame);
    connect(loadButton, &QPushButton::clicked, this, &GameWidget::loadGame);
    connect(hintButton, &QPushButton::clicked, this, &GameWidget::showHint);

    // Таймер для автоматического хода ИИ.
    botMoveTimer = new QTimer(this);
    botMoveTimer->setInterval(1000);
    connect(botMoveTimer, &QTimer::timeout, this, &GameWidget::processTurn);

    updateBoard();
    processTurn();

    setStyleSheet("QWidget { background-color: #E0F7FA; }"
                  "QPushButton { background-color: #80DEEA; border-radius: 8px; padding: 5px; font-size: 10pt; }"
                  "QPushButton:hover { background-color: #4DD0E1; }"
                  "QLabel { font-size: 12pt; }");
}

void GameWidget::handleCellClick(int row, int col) {
    if (gameEngine->mode == PvB && gameEngine->currentPlayer == -1) {
        if (gameEngine->playerMove(row, col)) {
            updateBoard();
            if (!gameEngine->isGameOver())
                QTimer::singleShot(1000, this, SLOT(processTurn()));
            else
                showResult();
        }
    }
}

void GameWidget::processTurn() {
    if (gameEngine->isGameOver()) {
        showResult();
        botMoveTimer->stop();
        return;
    }
    if (gameEngine->mode == PvB && gameEngine->currentPlayer == 1) {
        gameEngine->botMove();
        updateBoard();
    }
    if (gameEngine->mode == BvB) {
        gameEngine->botMove();
        updateBoard();
    }
    updateTurnLabel();
    if (gameEngine->mode == BvB || (gameEngine->mode == PvB && gameEngine->currentPlayer == 1))
        botMoveTimer->start();
    else
        botMoveTimer->stop();
}

void GameWidget::undoMove() {
    if (gameEngine->mode == PvB) {
        gameEngine->undoMove();
        updateBoard();
    } else {
        QMessageBox::information(this, "Отмена хода", "Отмена хода недоступна в режиме Бот против Бота.");
    }
}

void GameWidget::saveGame() {
    gameEngine->saveCurrentState();
}

void GameWidget::loadGame() {
    gameEngine->loadSavedState();
    updateBoard();
    updateTurnLabel();
}

void GameWidget::showHint() {
    std::pair<int, int> hint = gameEngine->hintMove();
    if (hint.first != -1) {
        // Для подсказки можно, например, временно изменить фон ячейки.
        boardButtons[hint.first][hint.second]->setStyleSheet("background-color: #FFEB3B;");
        QTimer::singleShot(1500, this, [=](){ updateBoard(); });
    } else {
        QMessageBox::information(this, "Подсказка", "Нет доступного хода для подсказки.");
    }
}

void GameWidget::backToMenu() {
    emit returnToMenu();
}

void GameWidget::updateBoard() {
    // Получаем список допустимых ходов для текущего игрока.
    std::vector<std::pair<int, int>> legalMoves = gameEngine->getLegalMoves();
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            // Обновляем значение ячейки (она затем сама перерисовывается, вызывая paintEvent в CellButton)
            boardButtons[i][j]->setValue(gameEngine->board[i][j]);

            bool enable = false;
            for (const auto &move : legalMoves) {
                if (move.first == i && move.second == j) {
                    enable = true;
                    break;
                }
            }
            // Разрешаем кликать по ячейке в режиме "Игрок против Бота", если это допустимый ход и сейчас ход игрока.
            boardButtons[i][j]->setEnabled(enable && (gameEngine->mode == PvB) && (gameEngine->currentPlayer == -1));
            // Можно задать стандартный стиль.
            boardButtons[i][j]->setStyleSheet("background-color: #B2DFDB;");
        }
    }
    updateTurnLabel();
}

void GameWidget::updateTurnLabel() {
    // Если текущий игрок равен 1 – играют белые, иначе – чёрные.
    QString playerStr = (gameEngine->currentPlayer == 1) ? "белые" : "чёрные";
    turnLabel->setText("Ход: " + playerStr);
}

void GameWidget::showResult() {
    int score = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            score += gameEngine->board[i][j];
    QString result;
    if (score > 0)
        result = "Победили белые.";
    else if (score < 0)
        result = "Победили чёрные.";
    else
        result = "Ничья.";
    QMessageBox::information(this, "Игра окончена", result);
}
