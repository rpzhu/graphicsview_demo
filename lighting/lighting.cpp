#include "lighting.h"
#include <QtWidgets>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#pragma execution_character_set("utf-8")
Lighting::Lighting(QWidget* parent)
  : QGraphicsView(parent)
  , angle(0.0)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowFlags(windowFlags() | Qt::Dialog);
    setWindowModality(Qt::ApplicationModal); //阻塞除当前窗体之外的所有的窗体

    setScene(&m_scene);

    setupScene();

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(animate()));
    timer->setInterval(30); //时间间隔
    timer->start();

    setRenderHint(QPainter::Antialiasing, true); //抗锯齿
    setFrameStyle(QFrame::NoFrame);
}

void
Lighting::setupScene()
{
    m_scene.setSceneRect(-300, -200, 600, 460); //场景尺寸

    //场景背景使用线性渐变
    QLinearGradient linearGrad(QPointF(-100, -100), QPointF(100, 100));
    linearGrad.setColorAt(0, QColor(255, 255, 255));
    linearGrad.setColorAt(1, QColor(192, 192, 255));
    setBackgroundBrush(linearGrad);

    //辐射渐变
    QRadialGradient radialGrad(30, 30, 30);
    radialGrad.setColorAt(0, Qt::yellow);
    radialGrad.setColorAt(0.2, Qt::yellow);
    radialGrad.setColorAt(1, Qt::transparent); //终点处是透明的

    //绘制太阳
    QPixmap pixmap(60, 60);
    pixmap.fill(Qt::transparent); //图像用透明色填充

    QPainter painter(&pixmap); //在图像上绘画
    painter.setPen(Qt::NoPen);
    painter.setBrush(radialGrad);      //画刷使用辐射渐变效果
    painter.drawEllipse(0, 0, 60, 60); //画圆
    painter.end();

    m_lightSource = m_scene.addPixmap(pixmap); //太阳图片转成图形项添加到场景

    m_lightSource->setZValue(2);

    for (int i = -2; i < 3; ++i) {
        for (int j = -2; j < 3; ++j) {
            QAbstractGraphicsShapeItem* item;
            if ((i + j) & 1) {
                item = new QGraphicsEllipseItem(0, 0, 50, 50); //圆形图形项
            } else {
                item = new QGraphicsRectItem(0, 0, 50, 50); //矩形图形项
            }

            item->setPen(QPen(Qt::black, 1));
            item->setBrush(QBrush(Qt::white));

            //图形视图框架阴影效果
            QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect;

            effect->setBlurRadius(8);        //设置阴影的模糊度
            item->setGraphicsEffect(effect); //为该图形项设置阴影
            item->setZValue(1);              //在太阳下
            item->setPos(i * 80, j * 80);
            m_scene.addItem(item); //添加到场景
            m_items << item;       //加入集合
        }
    }
}

void
Lighting::animate()
{
    angle += (M_PI / 30);
    qreal xs = 200 * sin(angle) - 40 + 25;
    qreal ys = 200 * cos(angle) - 40 + 25;
    m_lightSource->setPos(xs, ys); //太阳的位置不断变化 不连续的 但是0.03秒变化一次 人眼分辨不出来 看起来像连续移动的

    for (int i = 0; i < m_items.size(); ++i) {
        QGraphicsItem* item = m_items.at(i);
        Q_ASSERT(item);

        //当前图形项的阴影效果对象
        QGraphicsDropShadowEffect* effect = static_cast<QGraphicsDropShadowEffect*>(item->graphicsEffect());
        Q_ASSERT(effect);

        QPointF delta(item->x() - xs, item->y() - ys);
        //setOffset (qreal dx,qreal dy)用于设定在哪个方向产生阴影效果，如果dx为负数，则阴影在图形元素的左边
        effect->setOffset(delta.toPoint() / 30);

        qreal dx = delta.x();
        qreal dy = delta.y();
        qreal dd = sqrt(dx * dx + dy * dy);
        QColor color = effect->color(); //阴影的颜色
        color.setAlphaF(qBound(0.4, 1 - dd / 200.0, 0.7));
        effect->setColor(color);
    }

    m_scene.update();
}

//这个窗口大小改变事件虽然没东西 但是有用的 有则拉大窗口时窗口内容不变 无则拉大窗口时窗口内容跟着变化
void
Lighting::resizeEvent(QResizeEvent* /* event */)
{
}
