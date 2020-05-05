#include <QtWidgets>
#include "coloritem.h"

ColorItem::ColorItem()
    : color(qrand() % 256, qrand() % 256, qrand() % 256)
{
    //鼠标移动到图形项上会显示此提示信息
    setToolTip(QString("QColor(%1, %2, %3)\n%4")
              .arg(color.red()).arg(color.green()).arg(color.blue())
              .arg("点击并拖动这个颜色到机器人上"));
    setCursor(Qt::OpenHandCursor);
    setAcceptedMouseButtons(Qt::LeftButton);
}

QRectF ColorItem::boundingRect() const
{
    return QRectF(-15.5, -15.5, 34, 34);
}

void ColorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-12, -12, 30, 30);
    painter->setPen(QPen(Qt::black, 1));
    painter->setBrush(QBrush(color));
    painter->drawEllipse(-15, -15, 30, 30);
}

void ColorItem::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    setCursor(Qt::ClosedHandCursor);//鼠标按下 光标样式变化
}

//鼠标移动
void ColorItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton))
        .length() < QApplication::startDragDistance())
    {
        return;
    }

    QDrag *drag = new QDrag(event->widget());//拖拽对象
    QMimeData *mime = new QMimeData;
    drag->setMimeData(mime);

    static int n = 0;

    //有一定的几率拖拽时候拖拽的数据是狗头图片
    if (n++ > 2 && (qrand() % 3) == 0)
    {
        QImage image(":/images/head.png");
        mime->setImageData(image);

        drag->setPixmap(QPixmap::fromImage(image).scaled(30, 40));
        drag->setHotSpot(QPoint(15, 30));
    }
    else//否则拖拽的信息是颜色
    {
        mime->setColorData(color);
        mime->setText(QString("#%1%2%3")
                      .arg(color.red(), 2, 16, QLatin1Char('0'))
                      .arg(color.green(), 2, 16, QLatin1Char('0'))
                      .arg(color.blue(), 2, 16, QLatin1Char('0')));

        QPixmap pixmap(34, 34);
        pixmap.fill(Qt::white);

        QPainter painter(&pixmap);
        painter.translate(15, 15);
        painter.setRenderHint(QPainter::Antialiasing);//抗锯齿
        paint(&painter, nullptr, nullptr);
        painter.end();

        //设置掩码 去掉多余边界
        pixmap.setMask(pixmap.createHeuristicMask());

        drag->setPixmap(pixmap);
        drag->setHotSpot(QPoint(15, 20));
    }

    drag->exec();
    setCursor(Qt::OpenHandCursor);
}

void ColorItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    setCursor(Qt::OpenHandCursor);//松开鼠标 换光标图形
}
