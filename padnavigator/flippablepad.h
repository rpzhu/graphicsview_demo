#ifndef FLIPPABLEPAD_H
#define FLIPPABLEPAD_H

#include "roundrectitem.h"
#include <QGraphicsObject>
#include <QLinearGradient>
#include <QVector>

class FlippablePad : public RoundRectItem
{
public:
    explicit FlippablePad(const QSize &size, QGraphicsItem *parent = 0);

    RoundRectItem *iconAt(int column, int row) const;

private:
    QVector<QVector<RoundRectItem *> > iconGrid;
};

#endif // FLIPPABLEPAD_H
