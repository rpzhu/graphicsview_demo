#include "embeddeddialogwindow.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include "customproxy.h"
#include "embeddeddialog.h"
#include <QHBoxLayout>

embeddeddialogWindow::embeddeddialogWindow(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowFlags(windowFlags() |Qt::Dialog);
    setWindowModality(Qt::ApplicationModal); //阻塞除当前窗体之外的所有的窗体

    QGraphicsScene * scene = new QGraphicsScene;
    scene->setStickyFocus(true);

    for (int y = 0; y < 3; ++y)
    {
        for (int x = 0; x < 3; ++x)
        {
            CustomProxy *proxy = new CustomProxy(0, Qt::Window);
            proxy->setWidget(new EmbeddedDialog);

            QRectF rect = proxy->boundingRect();

            //设置窗体图形项位置
            proxy->setPos(x * rect.width() * 1.05, y * rect.height() * 1.05);
            proxy->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

            scene->addItem(proxy);
        }
    }
    scene->setSceneRect(scene->itemsBoundingRect());

    QGraphicsView * view = new QGraphicsView(scene,this);
    view->scale(0.5, 0.5);
    view->setRenderHints(view->renderHints() | QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    view->setBackgroundBrush(QPixmap(":/No-Ones-Laughing-3.jpg"));
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->show();

    QHBoxLayout * HB = new QHBoxLayout;
    HB->addWidget(view);
    HB->setMargin(0);
    setLayout(HB);
}
