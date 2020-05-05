#include "robot_window.h"
#include "coloritem.h"
#include "robot.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QTime>
#include <math.h>
#pragma execution_character_set("utf-8")
Robot_window::Robot_window(QWidget* parent)
  : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowFlags(windowFlags() | Qt::Dialog);
    setWindowModality(Qt::ApplicationModal); //阻塞除当前窗体之外的所有的窗体

    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    QGraphicsScene* scene = new QGraphicsScene(-200, -200, 400, 400);

    for (int i = 0; i < 10; ++i) {
        ColorItem* item = new ColorItem;
        item->setPos(::sin((i * 6.28) / 10.0) * 150, //围成一圈
                     ::cos((i * 6.28) / 10.0) * 150);

        scene->addItem(item);
    }

    Robot* robot = new Robot;
    robot->setTransform(QTransform::fromScale(1.2, 1.2), true); //机器人扩大一定尺寸
    robot->setPos(0, -20);
    scene->addItem(robot);

    QGraphicsView* view = new QGraphicsView(scene, nullptr);
    view->setRenderHint(QPainter::Antialiasing); //抗锯齿
    //场景重绘更新方式
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->setBackgroundBrush(QColor(230, 200, 167)); //设置背景颜色
    view->show();

    QHBoxLayout* HB = new QHBoxLayout;
    HB->addWidget(view);
    HB->setMargin(0);
    setLayout(HB);
}
