﻿#include <QApplication>
#include <QGraphicsAnchorLayout>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneResizeEvent>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#pragma execution_character_set("utf-8")
namespace weatheranchor {
//图形视图框架布局项
class PixmapWidget : public QGraphicsLayoutItem
{
public:
    PixmapWidget(const QPixmap& pix)
      : QGraphicsLayoutItem()
    {
        original = new QGraphicsPixmapItem(pix); //图片图形项
        setGraphicsItem(original);
        original->show();
        r = QRectF(QPointF(0, 0), pix.size());
    }

    ~PixmapWidget()
    {
        setGraphicsItem(nullptr);
        delete original;
    }

    void setZValue(qreal z)
    {
        original->setZValue(z);
    }

    void setGeometry(const QRectF& rect) Q_DECL_OVERRIDE
    {
        original->setTransform(QTransform::fromScale(rect.width() / r.width(),
                                                     rect.height() / r.height()),
                               true);
        original->setPos(rect.x(), rect.y());
        r = rect;
    }

protected:
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF& constraint = QSizeF()) const Q_DECL_OVERRIDE
    {
        Q_UNUSED(constraint);
        QSizeF sh;
        switch (which) {
        case Qt::MinimumSize:
            sh = QSizeF(0, 0);
            break;
        case Qt::PreferredSize:
            sh = QSizeF(50, 50);
            break;
        case Qt::MaximumSize:
            sh = QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
            break;
        default:
            break;
        }
        return sh;
    }

private:
    QGraphicsPixmapItem* original;
    QRectF r;
};

//是布局部件也是图形项
class PlaceWidget : public QGraphicsWidget
{
    Q_OBJECT

public:
    PlaceWidget(const QPixmap& pix)
      : QGraphicsWidget()
      , original(pix)
      , scaled(pix)
    {
    }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) Q_DECL_OVERRIDE
    {
        QPointF reflection = QPointF();
        reflection.setY(scaled.height() + 2);

        painter->drawPixmap(QPointF(), scaled);

        QPixmap tmp(scaled.size());
        tmp.fill(Qt::transparent);
        QPainter p(&tmp);

        //线性渐变
        QPoint p1(scaled.width() / 2, 0);
        QPoint p2(scaled.width() / 2, scaled.height());
        QLinearGradient linearGrad(p1, p2);
        linearGrad.setColorAt(0, QColor(0, 0, 0, 0));
        linearGrad.setColorAt(0.65, QColor(0, 0, 0, 127));
        linearGrad.setColorAt(1, QColor(0, 0, 0, 255));

        //掩码
        p.setBrush(linearGrad);
        p.fillRect(0, 0, tmp.width(), tmp.height(), QBrush(linearGrad));
        p.fillRect(0, 0, tmp.width(), tmp.height(), QBrush(linearGrad));

        //绘制翻转的图像
        //QPainter::CompositionMode_DestinationOver 两张图像混合，其中一张做掩码
        p.setCompositionMode(QPainter::CompositionMode_DestinationOver);
        p.drawPixmap(0, 0, QPixmap::fromImage(scaled.toImage().mirrored(false, true)));
        p.end();

        painter->drawPixmap(reflection, tmp);
    }

    void resizeEvent(QGraphicsSceneResizeEvent* event) Q_DECL_OVERRIDE
    {
        QSize newSize = event->newSize().toSize();
        newSize.setHeight(newSize.height() / 2);
        scaled = original.scaled(newSize);
    }

    QRectF boundingRect() const Q_DECL_OVERRIDE
    {
        QSize size(scaled.width(), scaled.height() * 2 + 2);
        return QRectF(QPointF(0, 0), size);
    }

private:
    QPixmap original;
    QPixmap scaled;
};

class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(QWidget* parent = 0)
      : QGraphicsView(parent)
    {
        setAttribute(Qt::WA_DeleteOnClose);
        setWindowFlags(Qt::WindowStaysOnTopHint);
        setWindowFlags(windowFlags() | Qt::Dialog);
        setWindowModality(Qt::ApplicationModal); //阻塞除当前窗体之外的所有的窗体

        PixmapWidget* title = new PixmapWidget(QPixmap(":/images/title.jpg"));
        title->setPreferredSize(QSizeF(348, 45));
        title->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

        PlaceWidget* place = new PlaceWidget(QPixmap(":/images/place.jpg"));
        place->setPreferredSize(QSizeF(96, 72));
        place->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

        PixmapWidget* details = new PixmapWidget(QPixmap(":/images/5days.jpg"));
        details->setMinimumSize(QSizeF(200, 112));
        details->setPreferredSize(QSizeF(200, 112));
        details->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

        PixmapWidget* sunnyWeather = new PixmapWidget(QPixmap(":/images/weather-few-clouds.png"));
        sunnyWeather->setPreferredSize(QSizeF(128, 97));
        sunnyWeather->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sunnyWeather->setZValue(9999);

        PixmapWidget* tabbar = new PixmapWidget(QPixmap(":/images/tabbar.jpg"));
        tabbar->setPreferredSize(QSizeF(70, 24));
        tabbar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

        //锚布局
        QGraphicsAnchorLayout* layout = new QGraphicsAnchorLayout;
        layout->setSpacing(0);

        QGraphicsWidget* widget = new QGraphicsWidget(nullptr, Qt::Window);
        QPalette p;
        p.setColor(QPalette::Window, Qt::black);
        widget->setPalette(p);
        widget->setPos(20, 20);
        widget->setLayout(layout);

        //垂直锚
        QGraphicsAnchor* anchor = layout->addAnchor(title, Qt::AnchorTop, layout, Qt::AnchorTop);
        anchor = layout->addAnchor(place, Qt::AnchorTop, title, Qt::AnchorBottom);
        anchor->setSpacing(12);
        anchor = layout->addAnchor(place, Qt::AnchorBottom, layout, Qt::AnchorBottom);
        anchor->setSpacing(12);
        anchor = layout->addAnchor(sunnyWeather, Qt::AnchorTop, title, Qt::AnchorTop);
        anchor = layout->addAnchor(sunnyWeather, Qt::AnchorBottom, layout, Qt::AnchorVerticalCenter);
        anchor = layout->addAnchor(tabbar, Qt::AnchorTop, title, Qt::AnchorBottom);
        anchor->setSpacing(5);
        anchor = layout->addAnchor(details, Qt::AnchorTop, tabbar, Qt::AnchorBottom);
        anchor->setSpacing(2);
        anchor = layout->addAnchor(details, Qt::AnchorBottom, layout, Qt::AnchorBottom);
        anchor->setSpacing(12);

        //水平锚
        anchor = layout->addAnchor(layout, Qt::AnchorLeft, title, Qt::AnchorLeft);
        anchor = layout->addAnchor(title, Qt::AnchorRight, layout, Qt::AnchorRight);
        anchor = layout->addAnchor(place, Qt::AnchorLeft, layout, Qt::AnchorLeft);
        anchor->setSpacing(15);
        anchor = layout->addAnchor(place, Qt::AnchorRight, details, Qt::AnchorLeft);
        anchor->setSpacing(35);
        anchor = layout->addAnchor(sunnyWeather, Qt::AnchorLeft, place, Qt::AnchorHorizontalCenter);
        anchor = layout->addAnchor(sunnyWeather, Qt::AnchorRight, layout, Qt::AnchorHorizontalCenter);
        anchor = layout->addAnchor(tabbar, Qt::AnchorHorizontalCenter, details, Qt::AnchorHorizontalCenter);
        anchor = layout->addAnchor(details, Qt::AnchorRight, layout, Qt::AnchorRight);

        QGraphicsScene* scene = new QGraphicsScene;
        scene->setSceneRect(0, 0, 800, 480);
        scene->addItem(widget);
        scene->setBackgroundBrush(Qt::white);
        setScene(scene);
        show();
    }
};

} //namespace end
