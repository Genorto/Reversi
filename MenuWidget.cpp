#include "MenuWidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QVariant>

MenuWidget::MenuWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    QLabel *title = new QLabel("Реверси");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 24pt; color: #004D40;"); // темно-бирюзовый цвет
    layout->addWidget(title);
    
    QLabel *modeLabel = new QLabel("Выберите режим игры:");
    layout->addWidget(modeLabel);
    
    QComboBox *modeCombo = new QComboBox();
    modeCombo->addItem("Игрок против Бота", QVariant::fromValue(0));
    modeCombo->addItem("Бот против Бота", QVariant::fromValue(1));
    layout->addWidget(modeCombo);
    
    QLabel *difficultyLabel = new QLabel("Выберите сложность:");
    layout->addWidget(difficultyLabel);
    
    QComboBox *difficultyCombo = new QComboBox();
    difficultyCombo->addItem("Легкий", QVariant::fromValue(0));
    difficultyCombo->addItem("Средний", QVariant::fromValue(1));
    difficultyCombo->addItem("Тяжелый", QVariant::fromValue(2));
    layout->addWidget(difficultyCombo);
    
    QPushButton *startButton = new QPushButton("Начать игру");
    layout->addWidget(startButton);
    
    // Если выбран режим "Бот против Бота", сложность неактивна.
    connect(modeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int){
        if (modeCombo->currentData().toInt() == 1)
            difficultyCombo->setEnabled(false);
        else
            difficultyCombo->setEnabled(true);
    });
    
    connect(startButton, &QPushButton::clicked, this, [=](){
        int mode = modeCombo->currentData().toInt();
        int diff = difficultyCombo->currentData().toInt();
        emit startGame(mode, diff);
    });
    
    layout->addStretch();
    setLayout(layout);
    
    setStyleSheet("QWidget { background-color: #E0F7FA; }"
                  "QPushButton { background-color: #80DEEA; border-radius: 8px; padding: 8px; font-size: 12pt; }"
                  "QPushButton:hover { background-color: #4DD0E1; }"
                  "QComboBox { font-size: 12pt; }"
                  "QLabel { font-size: 14pt; }");
}
