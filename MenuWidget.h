#pragma once

#include <QWidget>

class QComboBox;
class QLabel;

class MenuWidget : public QWidget {
    Q_OBJECT
public:
    explicit MenuWidget(QWidget *parent = nullptr);

signals:
    // Сигнал для старта игры: передаются выбранный режим и сложность.
    void startGame(int mode, int difficulty);
};
