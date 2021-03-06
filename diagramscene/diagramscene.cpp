﻿#include "diagramscene.h"
#include "arrow.h"
#include <QTextCursor>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

//自定义场景
DiagramScene::DiagramScene(QMenu *itemMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    myItemMenu = itemMenu;
    myMode = MoveItem;//默认的模型是移动项模式
    myItemType = DiagramItem::Step;//默认插入的多边形是正方形
    line = 0;//插入的线段
    textItem = 0;
    myItemColor = Qt::white;//多边形颜色（填充颜色）
    myTextColor = Qt::black;//文字颜色
    myLineColor = Qt::black;//箭头颜色
}

//设置箭头颜色
void DiagramScene::setLineColor(const QColor &color)
{
    myLineColor = color;//先改变箭头颜色属性
    if (isItemChange(Arrow::Type))
    {
        //如果此时选中了箭头，也改变选中箭头的颜色
        Arrow *item = qgraphicsitem_cast<Arrow *>(selectedItems().first());
        item->setColor(myLineColor);
        update();
    }
}

//类似 改变文字颜色
void DiagramScene::setTextColor(const QColor &color)
{
    myTextColor = color;
    if (isItemChange(DiagramTextItem::Type))
    {
        DiagramTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        item->setDefaultTextColor(myTextColor);
    }
}

//类似 改变多边形填充颜色
void DiagramScene::setItemColor(const QColor &color)
{
    myItemColor = color;
    if (isItemChange(DiagramItem::Type))
    {
        DiagramItem *item = qgraphicsitem_cast<DiagramItem *>(selectedItems().first());
        item->setBrush(myItemColor);
    }
}

//类似 改变文本字体
void DiagramScene::setFont(const QFont &font)
{
    myFont = font;
    if (isItemChange(DiagramTextItem::Type))
    {
        QGraphicsTextItem *item = qgraphicsitem_cast<DiagramTextItem *>(selectedItems().first());
        if (item)
        {
            item->setFont(myFont);
        }
    }
}

void DiagramScene::setMode(Mode mode)
{
    myMode = mode;
}

void DiagramScene::setItemType(DiagramItem::DiagramType type)
{
    myItemType = type;
}

//文本图形项失去焦点
void DiagramScene::editorLostFocus(DiagramTextItem *item)
{
    QTextCursor cursor = item->textCursor();//文本光标
    if(cursor.hasSelection())//如果有选中文本内容，则取消选中
    {
        cursor.clearSelection();
    }
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty())//文本为空 移除此图形项
    {
        removeItem(item);
        item->deleteLater();
    }
}

//场景鼠标按下事件 只处理左键
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
    {
        return;
    }

    DiagramItem *item;
    switch (myMode)
    {
        //场景点击鼠标左键时，可能是插入多边形、文字、直线箭头
        case InsertItem://插入多边形
            item = new DiagramItem(myItemType, myItemMenu);
            item->setBrush(myItemColor);
            addItem(item);
            item->setPos(mouseEvent->scenePos());//多边形的中心在鼠标按下的位置
            emit itemInserted(item);
            break;

        case InsertLine://插入线段
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),mouseEvent->scenePos()));
            line->setPen(QPen(myLineColor, 2));
            addItem(line);
            break;

        case InsertText://插入文本
            textItem = new DiagramTextItem();
            textItem->setFont(myFont);
            textItem->setTextInteractionFlags(Qt::TextEditorInteraction);//可编辑
            textItem->setZValue(1000.0);//此值设的比较大 设置文本图形项在其他类型图形项的上层

            connect(textItem, SIGNAL(lostFocus(DiagramTextItem*)), this, SLOT(editorLostFocus(DiagramTextItem*)));
            connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)), this, SIGNAL(itemSelected(QGraphicsItem*)));
            addItem(textItem);
            textItem->setDefaultTextColor(myTextColor);
            textItem->setPos(mouseEvent->scenePos());
            emit textInserted(textItem);
        default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

//鼠标移动事件
void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (myMode == InsertLine && line != 0)//插入线段模式
    {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    }
    else if (myMode == MoveItem)//场景中的项可移动模式
    {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

//鼠标释放事件：如果插入的线段连接的两头在两个多边形中将线段换成箭头>--> 否则清除掉线段
void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != 0 && myMode == InsertLine)
    {
        //items返回场景中所有项目的有序列表，顺序决定堆叠顺序。 将线段的起点存入集合
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        //当只点击未移动鼠标时startItems.count()=0
        if (startItems.count() && startItems.first() == line)
        {
            startItems.removeFirst();
        }
        //线段的尾点存入集合
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
        {
            endItems.removeFirst();
        }

        removeItem(line);
        delete line;

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == DiagramItem::Type &&
            endItems.first()->type() == DiagramItem::Type &&
            startItems.first() != endItems.first())
        {
            DiagramItem *startItem = qgraphicsitem_cast<DiagramItem *>(startItems.first());
            DiagramItem *endItem = qgraphicsitem_cast<DiagramItem *>(endItems.first());
            Arrow *arrow = new Arrow(startItem, endItem);
            arrow->setColor(myLineColor);
            startItem->addArrow(arrow);
            endItem->addArrow(arrow);
            arrow->setZValue(-1000.0);
            addItem(arrow);
            arrow->updatePosition();
        }
    }

    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

bool DiagramScene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems())
    {
        if (item->type() == type)
        {
            return true;
        }
    }
    return false;
}
