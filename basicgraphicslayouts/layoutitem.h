﻿#ifndef LAYOUTITEM_H
#define LAYOUTITEM_H

#include <QGraphicsLayoutItem>
#include <QGraphicsItem>

class LayoutItem : public QGraphicsLayoutItem, public QGraphicsItem//图形项基类
{
public:
    LayoutItem(QGraphicsItem *parent = 0);
    ~LayoutItem();

    void setGeometry(const QRectF &geom) Q_DECL_OVERRIDE;
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const Q_DECL_OVERRIDE;

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;

private:
    QPixmap *m_pix;
};

#endif // LAYOUTITEM_H
