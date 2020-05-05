#include "flowlayout.h"
#include <QtWidgets>
#pragma execution_character_set("utf-8")
#pragma execution_character_set("utf-8")
FlowLayout::FlowLayout(QWidget* parent, int margin, int hSpacing, int vSpacing)
  : QLayout(parent)
  , m_hSpace(hSpacing)
  , m_vSpace(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
    //参数顺序是上下左右，设置左侧、顶部、右侧和底部边距，以便在布局周围使用
}

FlowLayout::FlowLayout(int margin_left,
                       int margin_top,
                       int margin_right,
                       int margin_bottom,
                       int hSpacing,
                       int vSpacing)
  : m_hSpace(hSpacing)
  , m_vSpace(vSpacing)
{
    setContentsMargins(margin_left, margin_top, margin_right, margin_bottom);
}

FlowLayout::~FlowLayout()
{
    QLayoutItem* item = nullptr;
    while ((item = takeAt(0))) {
        delete item;
    }
}

void
FlowLayout::addItem(QLayoutItem* item)
{
    itemList.append(item); //向布局中添加布局项即添加到list集合
}

//返回水平间距
int
FlowLayout::horizontalSpacing() const
{
    if (m_hSpace >= 0) {
        return m_hSpace;
    } else {
        return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
    }
}

//返回垂直间距
int
FlowLayout::verticalSpacing() const
{
    if (m_vSpace >= 0) {
        return m_vSpace;
    } else {
        return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
    }
}

//返回布局中部件个数
int
FlowLayout::count() const
{
    return itemList.size();
}

//返回第index个布局项
QLayoutItem*
FlowLayout::itemAt(int index) const
{
    return itemList.value(index);
}

//删除布局中第index个布局项
QLayoutItem*
FlowLayout::takeAt(int index)
{
    if (index >= 0 && index < itemList.size()) {
        return itemList.takeAt(index);
    } else {
        return 0;
    }
}

Qt::Orientations
FlowLayout::expandingDirections() const
{
    return 0;
}

bool
FlowLayout::hasHeightForWidth() const
{
    return true;
}

int
FlowLayout::heightForWidth(int width) const
{
    int height = doLayout(QRect(0, 0, width, 0), true);
    return height;
}

void
FlowLayout::setGeometry(const QRect& rect)
{
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

QSize
FlowLayout::sizeHint() const
{
    return minimumSize();
}

QSize
FlowLayout::minimumSize() const
{
    QSize size;
    QLayoutItem* item;
    foreach (item, itemList) {
        size = size.expandedTo(item->minimumSize());
    }

    //    size += QSize(2*margin(), 2*margin());
    //    size += QSize(5,5);

    return size;
}

//排列布局中的部件
int
FlowLayout::doLayout(const QRect& rect, bool testOnly) const
{
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom); //获取布局目前的上下左右边距值
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
    int x = effectiveRect.x();
    int y = effectiveRect.y();
    int lineHeight = 0;

    QLayoutItem* item;
    foreach (item, itemList) {
        QWidget* wid = item->widget();
        int spaceX = horizontalSpacing();
        if (spaceX == -1) {
            spaceX = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
        }
        int spaceY = verticalSpacing();
        if (spaceY == -1) {
            spaceY = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);
        }

        int nextX = x + item->sizeHint().width() + spaceX;
        if (nextX - spaceX > effectiveRect.right() && lineHeight > 0) {
            x = effectiveRect.x();
            y = y + lineHeight + spaceY;
            nextX = x + item->sizeHint().width() + spaceX;
            lineHeight = 0;
        }

        if (!testOnly) {
            item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));
        }

        x = nextX;
        lineHeight = qMax(lineHeight, item->sizeHint().height());
    }
    return y + lineHeight - rect.y() + bottom;
}

int
FlowLayout::smartSpacing(QStyle::PixelMetric pm) const
{
    QObject* parent = this->parent();
    if (!parent) {
        return -1;
    } else if (parent->isWidgetType()) //是部件
    {
        QWidget* pw = static_cast<QWidget*>(parent);
        return pw->style()->pixelMetric(pm, 0, pw);
    } else {
        return static_cast<QLayout*>(parent)->spacing();
    }
}
