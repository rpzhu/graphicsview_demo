#include "showwindow.h"
#include "window.h"
#include <QGraphicsView>
#include <QHBoxLayout>
#pragma execution_character_set("utf-8")
showWindow::showWindow(QWidget* parent)
  : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowFlags(windowFlags() | Qt::Dialog);
    setWindowModality(Qt::ApplicationModal); //阻塞除当前窗体之外的所有的窗体

    QGraphicsScene* scene = new QGraphicsScene;
    Window* window = new Window;
    scene->addItem(window); //窗口添加到场景
    QGraphicsView* view = new QGraphicsView(scene, this);
    view->resize(600, 600);
    view->setWindowTitle("View");
    view->show();
    QHBoxLayout* HB = new QHBoxLayout;
    HB->setMargin(0);
    HB->addWidget(view);
    setLayout(HB);

    setMinimumSize(800, 800);
}
