#include "splashitem.h"
#include <QPainter>

//图形项，一块区域，用做在上面绘制提示文字
SplashItem::SplashItem(QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
    text = tr("欢迎来到Pad导航器示例。您可以使用键盘箭头来导航图标，然后按回车键来激活项目。按任意键开始");
    setCacheMode(DeviceCoordinateCache);
}

QRectF SplashItem::boundingRect() const
{
    return QRectF(0, 0, 400, 175);
}

void SplashItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(QPen(Qt::black, 2));
    painter->setBrush(QColor(245, 245, 255, 220));
    painter->setClipRect(boundingRect());
    painter->drawRoundRect(3, -100 + 3, 400 - 6, 250 - 6);

    QRectF textRect = boundingRect().adjusted(10, 10, -10, -10);
    int flags = Qt::AlignTop | Qt::AlignLeft | Qt::TextWordWrap;

    QFont font;
    font.setPixelSize(18);
    painter->setPen(Qt::black);
    painter->setFont(font);
    painter->drawText(textRect, flags, text);//绘制文字
}
