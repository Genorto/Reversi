#include "CellButton.h"
#include <QPainter>
#include <QStyleOptionButton>
#include <QStyle>

CellButton::CellButton(QWidget *parent) : QPushButton(parent), value(0) {
    setStyleSheet("background-color: #B2DFDB; border: 1px solid #004D40; border-radius: 0px;");
    setFlat(true);
}

void CellButton::setValue(int val) {
    value = val;
    update();
}

int CellButton::getValue() const {
    return value;
}

void CellButton::paintEvent(QPaintEvent *event) {
    QStyleOptionButton option;
    option.initFrom(this);
    QPainter p(this);
    style()->drawControl(QStyle::CE_PushButton, &option, &p, this);
    if (value == 0) return;
    p.setRenderHint(QPainter::Antialiasing, true);
    QRect rect = this->rect();
    int margin = 10;
    QRect circleRect = rect.adjusted(margin, margin, -margin, -margin);
    if (value == 1) p.setBrush(Qt::white);
    else if (value == -1) p.setBrush(Qt::black);
    p.setPen(Qt::NoPen);
    p.drawEllipse(circleRect);
}
