#include "window.h"
#include "layoutitem.h"
#include <QGraphicsLinearLayout>
#include <QGraphicsGridLayout>

//QGraphicsWidget是窗口部件也是图形项 此窗口可作为图形项放入场景
Window::Window(QGraphicsWidget *parent) : QGraphicsWidget(parent, Qt::Window)
{
    //QGraphicsLinearLayout用在图形视图框架里面的布局管理器 处理水平、垂直布局
    QGraphicsLinearLayout *windowLayout = new QGraphicsLinearLayout(Qt::Vertical);

    QGraphicsLinearLayout *linear = new QGraphicsLinearLayout(Qt::Vertical);
    //创建两个图形项存入布局 按垂直布局存放 这两个图形项拖动时的伸缩比例为1:3
    LayoutItem *item = new LayoutItem;
    linear->addItem(item);
    linear->setStretchFactor(item, 1);
    item = new LayoutItem;
    linear->addItem(item);
    linear->setStretchFactor(item, 3);

    windowLayout->addItem(linear);

    //网格布局
    QGraphicsGridLayout *grid = new QGraphicsGridLayout(windowLayout);
    item = new LayoutItem;
    grid->addItem(item, 0, 0, 4, 1);
    item = new LayoutItem;
    item->setMaximumHeight(item->minimumHeight());
    grid->addItem(item, 0, 1, 2, 1, Qt::AlignVCenter);
    item = new LayoutItem;
    item->setMaximumHeight(item->minimumHeight());
    grid->addItem(item, 2, 1, 2, 1, Qt::AlignVCenter);
    item = new LayoutItem;
    grid->addItem(item, 0, 2);
    item = new LayoutItem;
    grid->addItem(item, 1, 2);
    item = new LayoutItem;
    grid->addItem(item, 2, 2);
    item = new LayoutItem;
    grid->addItem(item, 3, 2);

    windowLayout->addItem(grid);

    setLayout(windowLayout);//设置本QGraphicsWidget窗口的中心布局
    setWindowTitle(tr("基本图形视图框架布局管理器示例"));
}
