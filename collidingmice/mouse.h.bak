#ifndef MOUSE_H
#define MOUSE_H

#include <QGraphicsItem>

class Mouse : public QGraphicsItem
{
public:
    Mouse();

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) Q_DECL_OVERRIDE;

protected:
    void advance(int step) Q_DECL_OVERRIDE;

private:
    qreal angle;
    qreal speed;
    qreal mouseEyeDirection;
    QColor color;
};

#endif
