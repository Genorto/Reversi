#pragma once
#include <QWidget>

class QComboBox;
class QLabel;

class MenuWidget : public QWidget {
    Q_OBJECT
public:
    explicit MenuWidget(QWidget *parent = nullptr);

signals:
    void startGame(int mode, int difficulty);
};
