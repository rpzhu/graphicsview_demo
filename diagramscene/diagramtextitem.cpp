﻿#include "diagramtextitem.h"
#include "diagramscene.h"
#include <QDebug>

//自定义文本图形项  基类是文本图形项
DiagramTextItem::DiagramTextItem(QGraphicsItem *parent)
    : QGraphicsTextItem(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);//文本图形项可移动
    setFlag(QGraphicsItem::ItemIsSelectable);//文本图形项可选中
}

//项变化状态时触发：选中与否、文本改变
QVariant DiagramTextItem::itemChange(GraphicsItemChange change,const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
    {
        emit selectedChange(this);
    }
    return value;
}

//失去焦点
void DiagramTextItem::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}

//鼠标双击事件 可编辑文本
void DiagramTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::NoTextInteraction)
    {
        setTextInteractionFlags(Qt::TextEditorInteraction);//项目可编辑
    }
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}
