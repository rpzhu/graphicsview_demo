﻿#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>
#include "diagramitem.h"

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

class Arrow : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 4 };

    Arrow(DiagramItem *startItem, DiagramItem *endItem, QGraphicsItem *parent = 0);

    int type() const Q_DECL_OVERRIDE
    {
        return Type;
    }

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;

    void setColor(const QColor &color)
    {
        myColor = color;
    }

    //后面加const的函数不能对其数据成员进行修改操作
    DiagramItem *startItem() const
    {
        return myStartItem;
    }

    DiagramItem *endItem() const
    {
        return myEndItem;
    }

    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;

private:
    DiagramItem *myStartItem;
    DiagramItem *myEndItem;
    QColor myColor;
    QPolygonF arrowHead;//箭头 三角形
};

#endif // ARROW_H