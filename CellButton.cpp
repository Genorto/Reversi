#include "CellButton.h"
#include <QPainter>
#include <QStyleOptionButton>
#include <QStyle>

CellButton::CellButton(QWidget *parent)
    : QPushButton(parent),
      value(0)
{
    // Убираем стандартное оформление и устанавливаем нужный стиль для ячейки.
    // Здесь устанавливается фон, рамка (1px solid тёмного цвета) и отсутствие округления.
    setStyleSheet("background-color: #B2DFDB; border: 1px solid #004D40; border-radius: 0px;");
    setFlat(true);
}

void CellButton::setValue(int val) {
    value = val;
    update();  // перерисовываем кнопку при изменении значения
}

int CellButton::getValue() const {
    return value;
}

void CellButton::paintEvent(QPaintEvent *event) {
    // Рисуем стандартное оформление кнопки (это нужно, если вы хотите сохранить эффект нажатия)
    QStyleOptionButton option;
    option.initFrom(this);
    QPainter p(this);
    style()->drawControl(QStyle::CE_PushButton, &option, &p, this);

    // Если ячейка пуста, отрисовывать ничего не нужно.
    if (value == 0)
        return;

    // Включаем сглаживание
    p.setRenderHint(QPainter::Antialiasing, true);

    // Определяем прямоугольник для отрисовки диска
    QRect rect = this->rect();
    int margin = 10;  // внутренний отступ; можно настроить по вкусу
    QRect circleRect = rect.adjusted(margin, margin, -margin, -margin);

    // Задаём цвет для фигур: value==1 – белый диск, value==-1 – чёрный
    if (value == 1)
        p.setBrush(Qt::white);
    else if (value == -1)
        p.setBrush(Qt::black);

    p.setPen(Qt::NoPen);
    p.drawEllipse(circleRect);
}
