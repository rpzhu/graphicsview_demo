#include "arrow.h"
#include <math.h>
#include <QPen>
#include <QPainter>
#include <QDebug>

const qreal Pi = 3.14;

//https://blog.csdn.net/yansmile1/article/details/52370346
//自定义箭头图形项 基类是个直线图形项
Arrow::Arrow(DiagramItem *startItem, DiagramItem *endItem, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    //箭头两端连接的是两个多边形图形项
    myStartItem = startItem;// 这端>---->
    myEndItem = endItem; //    >---->这端
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    myColor = Qt::black;//箭头颜色
    setPen(QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

//此图形项的矩形范围
QRectF Arrow::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;
    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),line().p2().y() - line().p1().y())).normalized().adjusted(-extra, -extra, extra, extra);
}

//返回图形项的准确图形
QPainterPath Arrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);//多边形
    return path;
}

void Arrow::updatePosition()
{
    //QLinF 线段类
    //QGraphicsItem::mapFromItem将另一个图形项的坐标映射到本图形项坐标
    QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
    setLine(line);//设置直线图形项的线段
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (myStartItem->collidesWithItem(myEndItem))
    {
        return;
    }

    //设置画笔
    QPen myPen = pen();
    myPen.setColor(myColor);
    painter->setPen(myPen);
    painter->setBrush(myColor);

    //pos()返回父坐标中项目的位置,即该多边形图形项在场景中的位置
    //连接两个多边形中心位置的线段
    QLineF centerLine(myStartItem->pos(), myEndItem->pos());

    //>---->这端 的多边形 可能是矩形、棱形、平行四边形
    QPolygonF endPolygon = myEndItem->polygon();

    //>---->这端（右） 多边形的起点
    QPointF p1 = endPolygon.first() + myEndItem->pos();
    //painter->drawEllipse(p1,5,5);

    QPointF p2;
    QPointF intersectPoint;
    QLineF polyLine;

    //确定箭头 >---->这端 与多边形交点的具体位置，把线段延长，线段是连结两个多边形的中心的
    qDebug()<<endPolygon.count();
    for (int i = 1; i < endPolygon.count(); ++i)//多边形矢量的个数
    {
        //>---->这端 多边形的当前角点
        p2 = endPolygon.at(i) + myEndItem->pos();

        polyLine = QLineF(p1, p2);//一条边线

        //当前线段与指定线段的相交情况，返回值为IntersectType，若相交，则intersectPoint为交点，
        //若平行则intersectPoint未定义
        QLineF::IntersectType intersectType = polyLine.intersect(centerLine, &intersectPoint);

        //找到与两个中心连线相交的那条边
        if (intersectType == QLineF::BoundedIntersection)//两条线在每条线的起点和终点内相交 线段交叉
        {
            break;
        }
        p1 = p2;
    }

    //painter->drawEllipse(intersectPoint,5,5);
    //setLine(交点,这端>---->多边形的中心点)
    setLine(QLineF(intersectPoint, myStartItem->pos()));//设置本线段图形项的线段

    double angle = ::acos(line().dx() / line().length());
    if (line().dy() >= 0)
    {
        angle = (Pi * 2) - angle;
    }

    qreal arrowSize = 20;//箭头宽度

    QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize, cos(angle + Pi / 3) * arrowSize);
    QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize, cos(angle + Pi - Pi / 3) * arrowSize);

    painter->drawLine(line());//画线段 箭头的箭杆

    //画箭头
    arrowHead.clear();
    arrowHead << line().p1() << arrowP1 << arrowP2;
    painter->drawPolygon(arrowHead);

    if (isSelected())//若选中了这个图形项
    {
        //箭头围上一圈虚线
        painter->setPen(QPen(myColor, 1, Qt::DashLine));
        QLineF myLine = line();//本线段图形项的直线
        //效果：给线段图形项画了两条对称虚线
        myLine.translate(0, 4.0);//画线位置下移一点 图形项的下方画线
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);//再上移到上方的对称位置
        painter->drawLine(myLine);
    }
}
