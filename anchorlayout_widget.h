#ifndef ANCHORLAYOUT_WIDGET_H
#define ANCHORLAYOUT_WIDGET_H

#include <QGraphicsWidget>
#include <QPainter>
#include <QWidget>
#pragma execution_character_set("utf-8")
class anchorlayout_widget : public QWidget
{
    Q_OBJECT
public:
    explicit anchorlayout_widget(QWidget* parent = nullptr);
    QGraphicsProxyWidget* createItem(const QSizeF& minimum = QSizeF(100.0, 100.0),
                                     const QSizeF& preferred = QSize(150.0, 100.0),
                                     const QSizeF& maximum = QSizeF(200.0, 100.0),
                                     const QString& name = "0");
signals:
};

class anchorlayout_widget2 : public QWidget
{
    Q_OBJECT
public:
    explicit anchorlayout_widget2(QWidget* parent = nullptr);
};

//QGraphicsWidget是图形项也是布局项、也是窗口部件
class Widget : public QGraphicsWidget
{
public:
    Widget(const QColor& color, const QColor& textColor, const QString& caption, QGraphicsItem* parent = 0)
      : QGraphicsWidget(parent)
      , caption(caption)
      , color(color)
      , textColor(textColor)
    {
    }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget* = 0) Q_DECL_OVERRIDE
    {
        QFont font;
        font.setPixelSize(0.75 * qMin(boundingRect().width(), boundingRect().height()));

        painter->fillRect(boundingRect(), color);
        painter->save();
        painter->setFont(font);
        painter->setPen(textColor);
        painter->drawText(boundingRect(), Qt::AlignCenter, caption);
        painter->restore();
    }

private:
    QString caption;  //文字内容
    QColor color;     //背景色
    QColor textColor; //文字颜色
};
#endif // ANCHORLAYOUT_WIDGET_H
