#include "mousewindow.h"
#include "mouse.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QTime>
#include <QTimer>
#include <math.h>

static const int MouseCount = 15;
#pragma execution_character_set("utf-8")
mouseWindow::mouseWindow(QWidget* parent)
  : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowFlags(windowFlags() | Qt::Dialog);
    setWindowModality(Qt::ApplicationModal); //阻塞除当前窗体之外的所有的窗体

    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

    QGraphicsScene* scene = new QGraphicsScene;
    scene->setSceneRect(-300, -300, 600, 600);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    for (int i = 0; i < MouseCount; ++i) {
        Mouse* mouse = new Mouse;
        mouse->setPos(::sin((i * 6.28) / MouseCount) * 200,
                      ::cos((i * 6.28) / MouseCount) * 200);
        scene->addItem(mouse);
    }

    QGraphicsView* view = new QGraphicsView(scene, this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setBackgroundBrush(QPixmap(":/images/cheese.jpg"));
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->resize(600, 600);
    view->show();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, scene, &QGraphicsScene::advance);
    timer->start(33);

    QHBoxLayout* HB = new QHBoxLayout;
    HB->setMargin(0);
    HB->addWidget(view);
    setLayout(HB);
}
