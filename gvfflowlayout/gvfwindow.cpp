#include "gvfwindow.h"
#include "gvfflowlayout.h"
#include <QGraphicsProxyWidget>
#include <QLabel>
#pragma execution_character_set("utf-8")
//QGraphicsWidget是窗口部件也是图形项
gvfWindow::gvfWindow()
  : QGraphicsWidget(0, Qt::Window)
{
    gvfFlowLayout* lay = new gvfFlowLayout;

    QString sentence(tr("哪 里 有 天 才 ， 我 是 把 别 人 喝 咖 啡 的 工 夫 都 用 在 了 工 作 上 了 。 —— 鲁迅"));

    QStringList words = sentence.split(QLatin1Char(' '), QString::SkipEmptyParts); //QString::SkipEmptyParts跳过空字符串
    for (int i = 0; i < words.count(); ++i) {
        //QGraphicsProxyWidget可将QWidget部件转换成图形项
        QGraphicsProxyWidget* proxy = new QGraphicsProxyWidget(this);

        QLabel* label = new QLabel(words.at(i));
        label->setFrameStyle(QFrame::Box | QFrame::Plain);

        proxy->setWidget(label); //转换成图形项
        lay->addItem(proxy);     //添加到自定义布局
    }
    setLayout(lay);
}

showgvfWindow::showgvfWindow(QWidget* parent)
  : QGraphicsView(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowFlags(windowFlags() | Qt::Dialog);
    setWindowModality(Qt::ApplicationModal); //阻塞除当前窗体之外的所有的窗体

    QGraphicsScene* scene = new QGraphicsScene;
    setScene(scene);
    gvfWindow* w = new gvfWindow;
    scene->addItem(w);
    resize(400, 300);
    show();
}
