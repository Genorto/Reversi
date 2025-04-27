#pragma once
#include <QPushButton>

class CellButton : public QPushButton {
    Q_OBJECT
public:
    explicit CellButton(QWidget *parent = nullptr);
    void setValue(int val);
    int getValue() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int value;
};
