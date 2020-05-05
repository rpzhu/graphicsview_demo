#include "layoutitem.h"
#include <QGradient>
#include <QGraphicsLinearLayout>
#include <QPainter>

LayoutItem::LayoutItem(QGraphicsItem *parent/* = 0*/)
    : QGraphicsLayoutItem(), QGraphicsItem(parent)
{
    m_pix = new QPixmap(QLatin1String(":/images/block.png"));
    setGraphicsItem(this);
}

LayoutItem::~LayoutItem()
{
    delete m_pix;
}

void LayoutItem::paint(QPainter *painter,
    const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    QRectF frame(QPointF(0,0), geometry().size());
    qreal w = m_pix->width();
    qreal h = m_pix->height();
    QGradientStops stops;//<qreal, QColor>

    //用线性渐变绘制一个背景矩形
    QLinearGradient gradient(frame.topLeft(), frame.topLeft() + QPointF(200,200));
    stops << QGradientStop(0.0, QColor(60, 60,  60));
    stops << QGradientStop(frame.height() / 2 / frame.height(), QColor(102, 176, 54));
    stops << QGradientStop(1.0, QColor(215, 215, 215));
    gradient.setStops(stops);
    painter->setBrush(QBrush(gradient));
    painter->drawRoundedRect(frame, 10.0, 10.0);

    //绘制一个围绕在骰子周围的矩形 使用线性渐变
    QPointF pixpos = frame.center() - (QPointF(w, h) / 2);
    QRectF innerFrame(pixpos, QSizeF(w, h));
    innerFrame.adjust(-4, -4, 4, 4);
    gradient.setStart(innerFrame.topLeft());
    gradient.setFinalStop(innerFrame.bottomRight());
    stops.clear();
    stops << QGradientStop(0.0, QColor(255, 0, 0));
    //stops << QGradientStop(0.0, QColor(215, 255, 200));
    stops << QGradientStop(0.5, QColor(102, 176, 54));
    stops << QGradientStop(1.0, QColor(0, 0,  0));
    gradient.setStops(stops);
    painter->setBrush(QBrush(gradient));
    painter->drawRoundedRect(innerFrame, 10.0, 10.0);

    painter->drawPixmap(pixpos, *m_pix);
}

QRectF LayoutItem::boundingRect() const
{
    return QRectF(QPointF(0,0), geometry().size());
}

//设置图形项位置
void LayoutItem::setGeometry(const QRectF &geom)
{
    prepareGeometryChange();
    QGraphicsLayoutItem::setGeometry(geom);
    setPos(geom.topLeft());
}

QSizeF LayoutItem::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    switch (which)
    {
        case Qt::MinimumSize://最小尺寸
        case Qt::PreferredSize://最佳尺寸
            return m_pix->size() + QSize(12, 12);
        case Qt::MaximumSize://最大尺寸
            return QSizeF(1000,1000);
        default:
            break;
    }
    return constraint;
}
