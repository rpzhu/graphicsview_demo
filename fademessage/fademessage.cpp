#include "fademessage.h"
#include <QDebug>
#include <QGraphicsEffect>
#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <QPushButton>
#pragma execution_character_set("utf-8")
FadeMessage::FadeMessage(QWidget* parent)
  : QGraphicsView(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowFlags(windowFlags() | Qt::Dialog);
    setWindowModality(Qt::ApplicationModal); //阻塞除当前窗体之外的所有的窗体

    setScene(&m_scene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //设置隐藏水平、垂直滚动条
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setupScene();

    //为着色对象的strength设置动画效果
    //该属性默认强度为1。强度0等于没有效果，而1意味着完全着色。
    m_animation = new QPropertyAnimation(m_effect, "strength", this);
    m_animation->setDuration(10000); //持续时间
    m_animation->setEasingCurve(QEasingCurve::InOutSine);
    m_animation->setStartValue(0); //从不着色到完全着色
    m_animation->setEndValue(1);

    setRenderHint(QPainter::Antialiasing, true); //抗锯齿
    setFrameStyle(QFrame::NoFrame);              //无边框
    setFixedSize(400, 600);
}

void
FadeMessage::togglePopup()
{
    if (m_message->isVisible()) {
        m_message->setVisible(false); //此图形项设为不可见
        m_animation->setDirection(QAbstractAnimation::Backward);
        //反方向执行动画：从结束值到开始值
        //这里是值着色强度strength从1到0，从完全着色到不着色
    } else {
        m_message->setVisible(true); //此图形项设为可见
        m_animation->setDirection(QAbstractAnimation::Forward);
        //正方向执行动画：从开始值到结束值
        //从不着色到完全着色
    }
    m_animation->start(); //动画开始
}

void
FadeMessage::setupScene()
{
    //场景中添加一个800*600的矩形图形项
    QGraphicsRectItem* parent = m_scene.addRect(0, 0, 800, 600);
    parent->setPen(Qt::NoPen);
    parent->setZValue(0);

    //添加一个图像图形项
    QGraphicsPixmapItem* bg = m_scene.addPixmap(QPixmap(":/background.jpg"));
    bg->setParentItem(parent); //作为上面矩形图形项的子图形项
    bg->setZValue(-1);         //在所有图形项的下面作为背景

    //添加12个矩形图形项
    for (int i = 1; i < 5; ++i) {
        for (int j = 2; j < 5; ++j) {
            QGraphicsRectItem* item = m_scene.addRect(i * 50, (j - 1) * 50, 38, 38);
            item->setParentItem(parent);
            item->setZValue(1);
            int hue = 12 * (i * 5 + j);
            item->setBrush(QColor::fromHsv(hue, 128, 128));
        }
    }

    QFont font;
    font.setPointSize(font.pointSize() * 2);
    font.setBold(true);
    QFontMetrics fontMetrics(font);
    int fh = fontMetrics.height();

    QString sceneText = "Qt无处不在!";
    int sceneTextWidth = fontMetrics.width(sceneText);

    //添加一个矩形区域
    QGraphicsRectItem* block = m_scene.addRect(50, 300, sceneTextWidth, fh + 10);
    block->setPen(Qt::NoPen);
    //block->setBrush(QColor(102, 153, 51));
    block->setBrush(Qt::red);

    //添加一个文本图形项 不设置setZValue默认是0
    QGraphicsTextItem* text = m_scene.addText(sceneText, font);
    text->setDefaultTextColor(Qt::green); //white
    text->setPos(50, 300);                //移到上面添加的矩形区域处

    block->setZValue(2); //设置ZValue值为2，在文本图形项的上面，挡住了文本图形项
    block->hide();       //隐藏住此矩形图形项 这时候挡住文本的矩形隐藏 文本是可见的

    text->setParentItem(block); //将文本图形项设置为矩形图形项的子图形项，这是文本也随之隐藏不可见了
    m_message = block;

    //QGraphicsColorizeEffect是专用于图形项着色的对象
    m_effect = new QGraphicsColorizeEffect;
    //m_effect->setColor(QColor(122, 193, 66));
    m_effect->setColor(Qt::yellow);
    m_effect->setStrength(0);
    m_effect->setEnabled(true);
    parent->setGraphicsEffect(m_effect); //此图形项着色对象的作用于800*600的图形项

    QPushButton* press = new QPushButton;
    press->setText(tr("单击"));
    connect(press, SIGNAL(clicked()), SLOT(togglePopup())); //按下按钮此函数执行
    m_scene.addWidget(press);                               //场景中添加按钮

    press->move(300, 500);
}
