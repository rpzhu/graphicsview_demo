#include "gvfflowlayout.h"
#include <qmath.h>
#include <QWidget>

//图像视图框架 可向其中插入布局项QGraphicsLayoutItem
gvfFlowLayout::gvfFlowLayout()
{
    m_spacing[0] = 6;
    m_spacing[1] = 6;
    QSizePolicy sp = sizePolicy();//QSizePolicy类是描述布局水平和垂直方向调整策略的属性
    sp.setHeightForWidth(true);//设置标志判断窗口部件的首选高度依赖于它的宽度
    setSizePolicy(sp);
}

//向QGraphicsLayout布局中插入QGraphicsLayoutItem布局项
void gvfFlowLayout::insertItem(int index, QGraphicsLayoutItem *item)
{
    item->setParentLayoutItem(this);//设置插入的布局项的父布局项为本布局
    if (uint(index) > uint(m_items.count()))
    {
        index = m_items.count();
    }
    m_items.insert(index, item);//布局项插入list集合
    invalidate();//现有布局失效
}

int gvfFlowLayout::count() const
{
    return m_items.count();//list集合的元素个数为本布局中元素的个数
}

QGraphicsLayoutItem *gvfFlowLayout::itemAt(int index) const
{
    return m_items.value(index);//返回list集合中第index个布局项
}

void gvfFlowLayout::removeAt(int index)
{
    m_items.removeAt(index);//移除第index个布局项
    invalidate();//现有布局失效
}

//布局项间距
qreal gvfFlowLayout::spacing(Qt::Orientation o) const
{
    return m_spacing[int(o) - 1];
}

//设置布局项间距
void gvfFlowLayout::setSpacing(Qt::Orientations o, qreal spacing)
{
    if (o & Qt::Horizontal)//水平布局
    {
        m_spacing[0] = spacing;
    }
    if (o & Qt::Vertical)//垂直布局
    {
        m_spacing[1] = spacing;
    }
}

//设置位置
void gvfFlowLayout::setGeometry(const QRectF &geom)
{
    QGraphicsLayout::setGeometry(geom);
    doLayout(geom, true);
}

//重新排列布局中的布局项
qreal gvfFlowLayout::doLayout(const QRectF &geom, bool applyNewGeometry) const
{
    qreal left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    const qreal maxw = geom.width() - left - right;

    qreal x = 0;
    qreal y = 0;
    qreal maxRowHeight = 0;
    QSizeF pref;
    for (int i = 0; i < m_items.count(); ++i)
    {
        QGraphicsLayoutItem *item = m_items.at(i);
        pref = item->effectiveSizeHint(Qt::PreferredSize);
        maxRowHeight = qMax(maxRowHeight, pref.height());

        qreal next_x;
        next_x = x + pref.width();
        if (next_x > maxw)
        {
            if (x == 0)
            {
                pref.setWidth(maxw);
            }
            else
            {
                x = 0;
                next_x = pref.width();
            }
            y += maxRowHeight + spacing(Qt::Vertical);
            maxRowHeight = 0;
        }

        if (applyNewGeometry)
        {
            item->setGeometry(QRectF(QPointF(left + x, top + y), pref));
        }
        x = next_x + spacing(Qt::Horizontal);
    }
    maxRowHeight = qMax(maxRowHeight, pref.height());
    return top + y + maxRowHeight + bottom;
}

QSizeF gvfFlowLayout::minSize(const QSizeF &constraint) const
{
    QSizeF size(0, 0);
    qreal left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    if (constraint.width() >= 0)
    {
        const qreal height = doLayout(QRectF(QPointF(0,0), constraint), false);
        size = QSizeF(constraint.width(), height);
    }
    else
    {
        QGraphicsLayoutItem *item;
        foreach (item, m_items)
        {
            size = size.expandedTo(item->effectiveSizeHint(Qt::MinimumSize));
        }
        size += QSize(left + right, top + bottom);
    }
    return size;
}

QSizeF gvfFlowLayout::prefSize() const
{
    qreal left, right;
    getContentsMargins(&left, 0, &right, 0);

    QGraphicsLayoutItem *item;
    qreal maxh = 0;
    qreal totalWidth = 0;
    foreach (item, m_items)
    {
        if (totalWidth > 0)
        {
            totalWidth += spacing(Qt::Horizontal);
        }
        QSizeF pref = item->effectiveSizeHint(Qt::PreferredSize);
        totalWidth += pref.width();
        maxh = qMax(maxh, pref.height());
    }
    maxh += spacing(Qt::Vertical);

    const qreal goldenAspectRatio = 1.61803399;
    qreal w = qSqrt(totalWidth * maxh * goldenAspectRatio) + left + right;

    return minSize(QSizeF(w, -1));
}

QSizeF gvfFlowLayout::maxSize() const
{
    QGraphicsLayoutItem *item;
    qreal totalWidth = 0;
    qreal totalHeight = 0;
    foreach (item, m_items)
    {
        if (totalWidth > 0)
        {
            totalWidth += spacing(Qt::Horizontal);
        }
        if (totalHeight > 0)
        {
            totalHeight += spacing(Qt::Vertical);
        }
        QSizeF pref = item->effectiveSizeHint(Qt::PreferredSize);
        totalWidth += pref.width();
        totalHeight += pref.height();
    }

    qreal left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    return QSizeF(left + totalWidth + right, top + totalHeight + bottom);
}

QSizeF gvfFlowLayout::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    QSizeF sh = constraint;
    switch (which)
    {
        case Qt::PreferredSize:
            sh = prefSize();
            break;
        case Qt::MinimumSize:
            sh = minSize(constraint);
            break;
        case Qt::MaximumSize:
            sh = maxSize();
            break;
        default:
            break;
    }
    return sh;
}
