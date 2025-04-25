#include "CellButton.h"
#include <QPainter>
#include <QStyleOptionButton>
#include <QStyle>

CellButton::CellButton(QWidget *parent)
    : QPushButton(parent),
      value(0)
{
    // Убираем стандартное оформление кнопки (чтобы не рисовалась рамка и фон),
    // либо можно оставить, если требуется.
    setFlat(true);
}

void CellButton::setValue(int val) {
    value = val;
    update();  // перерисовать кнопку
}

int CellButton::getValue() const {
    return value;
}

void CellButton::paintEvent(QPaintEvent *event) {
    // Рисуем стандартное оформление кнопки
    QStyleOptionButton option;
    option.initFrom(this);
    QPainter p(this);
    style()->drawControl(QStyle::CE_PushButton, &option, &p, this);

    // Если значения 0 – ничего не рисуем кроме стандартного оформления.
    if (value == 0)
        return;

    // Включаем сглаживание
    p.setRenderHint(QPainter::Antialiasing, true);

    // Определяем прямоугольник, в котором будет отрисован круг
    QRect rect = this->rect();
    int margin = 5; // Отступ внутри кнопки
    QRect circleRect = rect.adjusted(margin, margin, -margin, -margin);

    // Устанавливаем цвет заливки: белый для value==1, чёрный для value==-1.
    if (value == 1)
        p.setBrush(Qt::white);
    else if (value == -1)
        p.setBrush(Qt::black);

    p.setPen(Qt::NoPen);
    p.drawEllipse(circleRect);
}
