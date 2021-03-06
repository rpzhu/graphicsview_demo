﻿#include "chip.h"
#include <QtWidgets>
#include <QDebug>

Chip::Chip(const QColor &color, int x, int y)
{
    this->x = x;
    this->y = y;
    this->color = color;
    setZValue((x + y) % 2);//一个对象是一个图形项 如果图形项重叠 ZValue值高的会显示在上面

    setFlags(ItemIsSelectable | ItemIsMovable);//设置图形项可移动 可选中
    setAcceptHoverEvents(true);//接受鼠标悬停事件，当鼠标置于某个芯片时导致亮度变化 默认为false
}

//返回绘制图形项的矩形区域 图形项必须绘制在此矩形区域中
QRectF Chip::boundingRect() const
{
    return QRectF(0, 0, 110, 70);//如果长宽值太小将导致芯片无法移动及放大时定位。
}

//作用是返回图形项的大概形状
QPainterPath Chip::shape() const
{
    QPainterPath path;
    path.addRect(14, 14, 82, 42);
    return path;
}

//绘制图形项  图形项在paint函数中绘制 不必显示调用
//                  绘图设备的painter                   绘图参数                 绘图设备
//鼠标指针移动到一个图形项上时 图形项的亮度会改变 这时候会重绘该图形项
//缩放 旋转等操作也要改变图形项的大小 重绘
void Chip::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);//Q_UNUSED避免编译器警告widget是未引用的参数

    QColor fillColor = (option->state & QStyle::State_Selected) ? color.dark(150) : color;
    if (option->state & QStyle::State_MouseOver)
    {
        fillColor = fillColor.light(125);//设备移到到该图形项 亮度改变
    }

    //分析一個 QPainter 的 worldTransform 資料，來取得目前在 view 裡面的縮放比例
    //取得的 qScale 就是代表目前這個 item 的縮放比例
    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    if (lod < 0.2)
    {
        if (lod < 0.125)
        {
            painter->fillRect(QRectF(0, 0, 110, 70), fillColor);//缩得太小则将此图形项绘制成一个矩形
            return;
        }

        QBrush b = painter->brush();
        painter->setBrush(fillColor);
        painter->drawRect(13, 13, 97, 57);
        painter->setBrush(b);
        return;
    }

    QPen oldPen = painter->pen();
    QPen pen = oldPen;
    int width = 0;
    if (option->state & QStyle::State_Selected)//如果选中图形项
    {
        width += 2;
    }

    pen.setWidth(width);
    QBrush b = painter->brush();
    painter->setBrush(QBrush(fillColor.dark(option->state & QStyle::State_Sunken ? 120 : 100)));

    painter->drawRect(QRect(14, 14, 79, 39));
    painter->setBrush(b);

    if (lod >= 1)
    {
        painter->setPen(QPen(Qt::gray, 1));//设置画笔
        painter->drawLine(15, 54, 94, 54);//画线
        painter->drawLine(94, 53, 94, 15);
        painter->setPen(QPen(Qt::black, 0));
    }

    //缩放倍数>=2则显示芯片上的字
    if (lod >= 2)
    {
        QFont font("Times", 10);
        font.setStyleStrategy(QFont::ForceOutline);
        painter->setFont(font);
        painter->save();
        painter->scale(0.1, 0.1);
        painter->drawText(170, 180, QString("位置：(%1,%2)").arg(x).arg(y));
        painter->drawText(170, 200, QString("序列号: SIYUAN-709579619"));
        painter->drawText(170, 220, QString("日期：2018-09-12"));
        painter->restore();
    }

    //画线
    QVarLengthArray<QLineF, 36> lines;
    if (lod >= 0.5)
    {
        for (int i = 0; i <= 10; i += (lod > 0.5 ? 1 : 2))
        {
            lines.append(QLineF(18 + 7 * i, 13, 18 + 7 * i, 5));
            lines.append(QLineF(18 + 7 * i, 54, 18 + 7 * i, 62));
        }
        for (int i = 0; i <= 6; i += (lod > 0.5 ? 1 : 2))
        {
            lines.append(QLineF(5, 18 + i * 5, 13, 18 + i * 5));
            lines.append(QLineF(94, 18 + i * 5, 102, 18 + i * 5));
        }
    }
    if (lod >= 0.4)
    {
        const QLineF lineData[] =
        {
            QLineF(25, 35, 35, 35),
            QLineF(35, 30, 35, 40),
            QLineF(35, 30, 45, 35),
            QLineF(35, 40, 45, 35),
            QLineF(45, 30, 45, 40),
            QLineF(45, 35, 55, 35)
        };
        lines.append(lineData, 6);
    }
    painter->drawLines(lines.data(), lines.size());  
}

//按下事件
void Chip::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    update();
}

//移动事件
void Chip::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ShiftModifier)//如果按shift键再拖动则无法拖动只刷新
    {
        update();
        return;
    }
    QGraphicsItem::mouseMoveEvent(event);
}

//鼠标松开事件
void Chip::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    update();
}
