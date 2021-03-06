﻿#ifndef LIGHTING_H
#define LIGHTING_H

#include <QGraphicsEffect>
#include <QGraphicsView>
#pragma execution_character_set("utf-8")
class Lighting : public QGraphicsView
{
    Q_OBJECT

public:
    Lighting(QWidget* parent = 0);

private slots:
    void animate();

private:
    void setupScene();

protected:
    void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;

private:
    qreal angle;
    QGraphicsScene m_scene;
    QGraphicsItem* m_lightSource;
    QList<QGraphicsItem*> m_items;
};

#endif // LIGHTING_H
