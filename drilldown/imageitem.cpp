#include "imageitem.h"
#include <QDebug>

ImageItem::ImageItem(int id, const QPixmap &pixmap, QGraphicsItem *parent)
    : QGraphicsPixmapItem(pixmap, parent)
{
    recordId = id;
    setAcceptHoverEvents(true);//图形项可以接收悬停事件

    timeLine.setDuration(1500);//动画效果持续1.5秒
    timeLine.setFrameRange(0, 150);//动画可以分为150帧静态图像

    //动画持续进行的时候会发送frameChanged(int)信号
    connect(&timeLine, SIGNAL(frameChanged(int)), this, SLOT(setFrame(int)));
    //结束后会发此信号
    connect(&timeLine, SIGNAL(finished()), this, SLOT(updateItemPosition()));

    adjust();
}

//悬停事件
void ImageItem::hoverEnterEvent(QGraphicsSceneHoverEvent * /*event*/)
{
    timeLine.setDirection(QTimeLine::Forward);

    if (z != 1.0)
    {
        z = 1.0;
        updateItemPosition();//将当期图形项放在最上层
    }

    if (timeLine.state() == QTimeLine::NotRunning)
    {
        timeLine.start();
    }
}

//悬停离开
void ImageItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * /*event*/)
{
    timeLine.setDirection(QTimeLine::Backward);

    //动画结束后会调用updateItemPosition将当期悬停的图形项的z值恢复 所有图形项存于同一层
    if (z != 0.0)
    {
        z = 0.0;
    }

    if (timeLine.state() == QTimeLine::NotRunning)
    {
        timeLine.start();
    }
}

void ImageItem::setFrame(int frame)
{
    adjust();
    QPointF center = boundingRect().center();

    //图形项按变换矩阵进行变换
    setTransform(QTransform::fromTranslate(center.x(), center.y()), true);
    setTransform(QTransform::fromScale(1 + frame / 330.0, 1 + frame / 330.0), true);
    setTransform(QTransform::fromTranslate(-center.x(), -center.y()), true);
}

void ImageItem::adjust()
{
    //设置变换矩阵:长宽尺寸扩大1.2倍
    QMatrix matrix;
    matrix.scale(120/ boundingRect().width(), 120/ boundingRect().height());
    setMatrix(matrix);
}

int ImageItem::id()
{
    return recordId;
}

void ImageItem::updateItemPosition()
{
    setZValue(z);
}
