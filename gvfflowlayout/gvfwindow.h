#include <QGraphicsView>
#include <QGraphicsWidget>
#pragma execution_character_set("utf-8")
class gvfWindow : public QGraphicsWidget
{
    Q_OBJECT

public:
    gvfWindow();
};

class showgvfWindow : public QGraphicsView
{
    Q_OBJECT

public:
    explicit showgvfWindow(QWidget* parent = nullptr);
};
