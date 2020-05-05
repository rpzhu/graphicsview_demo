#ifndef MOUSEWINDOW_H
#define MOUSEWINDOW_H
#pragma execution_character_set("utf-8")
#include <QWidget>
class QTimer;
class mouseWindow : public QWidget
{
    Q_OBJECT
public:
    explicit mouseWindow(QWidget* parent = nullptr);
    QTimer* timer;
};

#endif // MOUSEWINDOW_H
