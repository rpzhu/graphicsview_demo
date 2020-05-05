#ifndef FADEMESSAGE_H
#define FADEMESSAGE_H

#include <QGraphicsView>
class QGraphicsColorizeEffect;
class QPropertyAnimation;
#pragma execution_character_set("utf-8")
class FadeMessage : public QGraphicsView
{
    Q_OBJECT

public:
    FadeMessage(QWidget* parent = 0);

private:
    void setupScene();

private slots:
    void togglePopup();

private:
    QGraphicsScene m_scene;
    QGraphicsColorizeEffect* m_effect;
    QGraphicsItem* m_message;
    QPropertyAnimation* m_animation;
};

#endif // FADEMESSAGE_H
