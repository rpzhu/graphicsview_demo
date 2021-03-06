﻿#include "mouse.h"
#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <math.h>
#include <QDebug>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

static qreal normalizeAngle(qreal angle)
{
    if (angle < 0)
    {
        angle += TwoPi;
    }
    if (angle > TwoPi)
    {
        angle -= TwoPi;
    }
    return angle;
}

Mouse::Mouse()
    : angle(0), speed(0), mouseEyeDirection(0),
      color(qrand() % 256, qrand() % 256, qrand() % 256)
{
    setRotation(qrand() % (360 * 16));//设置旋转角度
}

QRectF Mouse::boundingRect() const
{
    qreal adjust = 0.5;
    return QRectF(-18 - adjust, -22 - adjust,
                  36 + adjust, 60 + adjust);
}

QPainterPath Mouse::shape() const
{
    QPainterPath path;
    path.addRect(-10, -20, 20, 40);
    return path;
}

//paint只负责绘制老鼠 至于老鼠怎么运动、向哪个方向运动是不管的
void Mouse::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //身体
    painter->setBrush(color);
    painter->drawEllipse(-10, -20, 20, 40);

    //眼睛
    painter->setBrush(Qt::white);
    painter->drawEllipse(-10, -17, 8, 8);
    painter->drawEllipse(2, -17, 8, 8);

    //鼻子
    painter->setBrush(Qt::black);
    painter->drawEllipse(QRectF(-2, -22, 4, 4));

    //瞳孔
    painter->drawEllipse(QRectF(-8.0 + mouseEyeDirection, -17, 4, 4));
    painter->drawEllipse(QRectF(4.0 + mouseEyeDirection, -17, 4, 4));

    //耳朵
    //collidingItems()获取与本图形项碰撞的所有图形项列表 若有碰撞耳朵变红色
    painter->setBrush(scene()->collidingItems(this).isEmpty() ? Qt::darkYellow : Qt::red);
    painter->drawEllipse(-17, -12, 16, 16);
    painter->drawEllipse(1, -12, 16, 16);

    //尾部
    QPainterPath path(QPointF(0, 20));
    path.cubicTo(-5, 22, -5, 22, 0, 25);
    path.cubicTo(5, 27, 5, 32, 0, 30);
    path.cubicTo(-5, 32, -5, 42, 0, 35);
    painter->setBrush(Qt::NoBrush);
    painter->drawPath(path);
}

void Mouse::advance(int step)
{
    if (!step)
    {
        return;
    }

    //图形项的原点(0,0)是图形项的中心点  场景的原点(0,0)是场景的中心
    //lineToCenter是将图形项的原点映射到场景 图形项的原点距离图形项的中心的位置
    //也就是老鼠距离场景中心的距离
    QLineF lineToCenter(QPointF(0, 0), mapFromScene(0, 0));
    qDebug()<<lineToCenter.length();
    if (lineToCenter.length() > 150)
    {
        //老鼠位置与场景原点线段的角度
        qreal angleToCenter = ::acos(lineToCenter.dx() / lineToCenter.length());

        if (lineToCenter.dy() < 0)
        {
            angleToCenter = TwoPi - angleToCenter;
        }

        angleToCenter = normalizeAngle((Pi - angleToCenter) + Pi / 2);

        if (angleToCenter < Pi && angleToCenter > Pi / 4)
        {
            //向左旋转
            angle += (angle < -Pi / 2) ? 0.25 : -0.25;
        }
        else if (angleToCenter >= Pi && angleToCenter < (Pi + Pi / 2 + Pi / 4))
        {
            //向右旋转
            angle += (angle < Pi / 2) ? 0.25 : -0.25;
        }
    }
    else if (::sin(angle) < 0)
    {
        angle += 0.25;
    }
    else if (::sin(angle) > 0)
    {
        angle -= 0.25;
    }

    //本图形项所在场景的所有老鼠
    QList<QGraphicsItem *> dangerMice = scene()->items(QPolygonF()
                                                       << mapToScene(0, 0)
                                                       << mapToScene(-30, -50)
                                                       << mapToScene(30, -50));
    foreach (QGraphicsItem *item, dangerMice)
    {
        if (item == this)//该老鼠是本身
            continue;

        QLineF lineToMouse(QPointF(0, 0), mapFromItem(item, 0, 0));
        qreal angleToMouse = ::acos(lineToMouse.dx() / lineToMouse.length());
        if (lineToMouse.dy() < 0)
        {
            angleToMouse = TwoPi - angleToMouse;
        }
        angleToMouse = normalizeAngle((Pi - angleToMouse) + Pi / 2);

        //尽量不要和其他老鼠碰撞
        //快和其他老鼠碰到了转个弯
        if (angleToMouse >= 0 && angleToMouse < Pi / 2)
        {
            //右转
            angle += 0.5;
        }
        else if (angleToMouse <= TwoPi && angleToMouse > (TwoPi - Pi / 2))
        {
            //左转
            angle -= 0.5;
        }
    }

    //增加一些随机运动
    if (dangerMice.size() > 1 && (qrand() % 10) == 0)
    {
        if (qrand() % 1)
            angle += (qrand() % 100) / 500.0;
        else
            angle -= (qrand() % 100) / 500.0;
    }

    speed += (-50 + qrand() % 100) / 100.0;

    qreal dx = ::sin(angle) * 10;
    mouseEyeDirection = (qAbs(dx / 5) < 1) ? 0 : dx / 5;

    setRotation(rotation() + dx);//设置转动角度
    setPos(mapToParent(0, -(3 + sin(speed) * 3)));//设置位置
}
