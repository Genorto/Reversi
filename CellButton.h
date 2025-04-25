#pragma once

#include <QPushButton>

class CellButton : public QPushButton {
    Q_OBJECT
public:
    explicit CellButton(QWidget *parent = nullptr);

    // Устанавливает значение ячейки: 0 – пусто, 1 – белая фишка, -1 – чёрная фишка.
    void setValue(int val);

    // Получает значение ячейки.
    int getValue() const;

protected:
    // Переопределённый метод для отрисовки фишки (если значение не 0).
    void paintEvent(QPaintEvent *event) override;

private:
    int value; // 0, 1 или -1.
};
