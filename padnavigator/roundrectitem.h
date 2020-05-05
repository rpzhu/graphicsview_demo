#ifndef ROUNDRECTITEM_H
#define ROUNDRECTITEM_H

#include <QGraphicsObject>
#include <QLinearGradient>

class RoundRectItem : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY(bool fill READ fill WRITE setFill)
public:
    RoundRectItem(const QRectF &bounds, const QColor &color,
                  QGraphicsItem *parent = 0);

    QPixmap pixmap() const;
    void setPixmap(const QPixmap &pixmap);

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;

    bool fill() const;
    void setFill(bool fill);

private:
    QPixmap pix;
    bool fillRect;
    QRectF bounds;
    QLinearGradient gradient;
};

#endif // ROUNDRECTITEM_H
