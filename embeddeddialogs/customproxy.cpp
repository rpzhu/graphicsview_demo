#include "customproxy.h"
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>
#include <QDebug>

//QGraphicsProxyWidget：可将QWidget部件转换为QGraphicsItem图形项
CustomProxy::CustomProxy(QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QGraphicsProxyWidget(parent, wFlags), popupShown(false), currentPopup(0)
{
    timeLine = new QTimeLine(250, this);//图形项动画持续时间0.25秒 QTimeLine提供动画时间轴控制
    connect(timeLine, SIGNAL(valueChanged(qreal)), this, SLOT(updateStep(qreal)));
    connect(timeLine, SIGNAL(stateChanged(QTimeLine::State)), this, SLOT(stateChanged(QTimeLine::State)));
}

QRectF CustomProxy::boundingRect() const
{
    return QGraphicsProxyWidget::boundingRect().adjusted(0, 0, 10, 10);
}

//重绘由窗体转成的图形项
void CustomProxy::paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                   QWidget *widget)
{
    const QColor color(0, 0, 0, 64);

    QRectF r = windowFrameRect();//窗体图形项矩形范围
    QRectF right(r.right(), r.top() + 10, 10, r.height() - 10);
    QRectF bottom(r.left() + 10, r.bottom(), r.width(), 10);
    bool intersectsRight = right.intersects(option->exposedRect);
    bool intersectsBottom = bottom.intersects(option->exposedRect);
    if (intersectsRight && intersectsBottom)
    {
        QPainterPath path;
        path.addRect(right);
        path.addRect(bottom);
        painter->setPen(Qt::NoPen);
        painter->setBrush(color);
        painter->drawPath(path);
    }
    else if (intersectsBottom)
    {
        painter->fillRect(bottom, color);
    }
    else if (intersectsRight)
    {
        painter->fillRect(right, color);
    }

    QGraphicsProxyWidget::paintWindowFrame(painter, option, widget);
}

//鼠标悬停事件
void CustomProxy::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsProxyWidget::hoverEnterEvent(event);
    scene()->setActiveWindow(this);//设置图形项所处场景：该场景的活动窗口为本窗口
    if (timeLine->currentValue() != 1)
    {
        zoomIn();
    }
}

//悬停离开
void CustomProxy::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsProxyWidget::hoverLeaveEvent(event);
    if (!popupShown && (timeLine->direction() != QTimeLine::Backward || timeLine->currentValue() != 0))
    {
        zoomOut();
    }
}

//场景事件过滤器
//https://www.cnblogs.com/findumars/p/5990703.html
bool CustomProxy::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    if (watched->isWindow() && (event->type() == QEvent::UngrabMouse || event->type() == QEvent::GrabMouse))
    {
        popupShown = watched->isVisible();
        if (!popupShown && !isUnderMouse())
        {
            zoomOut();
        }
    }
    return QGraphicsProxyWidget::sceneEventFilter(watched, event);
}

QVariant CustomProxy::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemChildAddedChange || change == ItemChildRemovedChange)
    {
        if (change == ItemChildAddedChange)
        {
            currentPopup = qvariant_cast<QGraphicsItem *>(value);
            currentPopup->setCacheMode(ItemCoordinateCache);
            if (scene())
            {
                currentPopup->installSceneEventFilter(this);
            }
        }
        else if (scene())
        {
            currentPopup->removeSceneEventFilter(this);
            currentPopup = 0;
        }
    }
    else if (currentPopup && change == ItemSceneHasChanged)
    {
        currentPopup->installSceneEventFilter(this);
    }
    return QGraphicsProxyWidget::itemChange(change, value);
}

void CustomProxy::updateStep(qreal step)
{
    QRectF r = boundingRect();
    //按一定角度变换
    setTransform(QTransform()
                 .translate(r.width() / 2, r.height() / 2)
                 .rotate(step * 30, Qt::XAxis)
                 .rotate(step * 10, Qt::YAxis)
                 .rotate(step * 5, Qt::ZAxis)
                 .scale(1 + 1.5 * step, 1 + 1.5 * step)
                 .translate(-r.width() / 2, -r.height() / 2));
}

void CustomProxy::stateChanged(QTimeLine::State state)
{
    if (state == QTimeLine::Running)
    {
        if (timeLine->direction() == QTimeLine::Forward)
        {
            setCacheMode(ItemCoordinateCache);
        }
    }
    else if (state == QTimeLine::NotRunning)
    {
        if (timeLine->direction() == QTimeLine::Backward)
        {
            setCacheMode(DeviceCoordinateCache);
        }
    }
}

void CustomProxy::zoomIn()
{
    if (timeLine->direction() != QTimeLine::Forward)
    {
        timeLine->setDirection(QTimeLine::Forward);
    }
    if (timeLine->state() == QTimeLine::NotRunning)
    {
        timeLine->start();
    }
}

void CustomProxy::zoomOut()
{
    if (timeLine->direction() != QTimeLine::Backward)
    {
        timeLine->setDirection(QTimeLine::Backward);
    }
    if (timeLine->state() == QTimeLine::NotRunning)
    {
        timeLine->start();
    }
}
