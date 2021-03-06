﻿#include "view.h"
#ifndef QT_NO_PRINTER
#include <QPrinter>
#include <QPrintDialog>
#endif
#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#else
#include <QtWidgets>
#endif
#include <qmath.h>

#ifndef QT_NO_WHEELEVENT

//视图的鼠标滚轮事件
void GraphicsView::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() & Qt::ControlModifier)//如果滚动滚轮且按下了Ctrl键
    {
        if (e->delta() > 0)//滚轮向上滚动
        {
            view->zoomIn(6);
        }
        else    //滚轮向下滚动
        {
            view->zoomOut(6);
        }
        e->accept();
    }
    else    //只滚动滚轮 上下移动视图
    {
        QGraphicsView::wheelEvent(e);
    }
}
#endif

drilldownView::drilldownView(const QString &name, QWidget *parent)
    : QFrame(parent)
{
    setFrameStyle(Sunken | StyledPanel);//设置frame的视觉效果为下沉
    graphicsView = new GraphicsView(this);
    graphicsView->setRenderHint(QPainter::Antialiasing, false);//视图内的图形项抗锯齿（反走样）效果设置 默认为不打开
    graphicsView->setDragMode(QGraphicsView::RubberBandDrag);//设置视图内图形项的拖拽模式
    /*
    QGraphicsView::NoDrag ：忽略鼠标事件，不可以拖动。
    QGraphicsView::ScrollHandDrag ：光标变为手型，可以拖动场景进行移动。
    QGraphicsView::RubberBandDrag ：使用橡皮筋效果，进行区域选择，可以选中一个区域内的所有图元
    */

    graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);//视图性能调优
    graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);//视图更新方式
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);//当转换时候视图如何摆放场景的位置

    //设置滑动条两端的两个按钮的属性
    //按钮尺寸
    int size = style()->pixelMetric(QStyle::PM_ToolBarIconSize);
    QSize iconSize(size, size);
    //放大按钮
    QToolButton *zoomInIcon = new QToolButton;
    zoomInIcon->setAutoRepeat(true);//允许按住放大按钮时持续放大，默认为否
    zoomInIcon->setAutoRepeatInterval(33);//设置持续放大的时间间隔为33毫秒
    zoomInIcon->setAutoRepeatDelay(0);//设置按下放大按钮后开始放大的延迟时间
    zoomInIcon->setIcon(QPixmap(":/zoomin.png"));//设置按钮图标图像
    zoomInIcon->setIconSize(iconSize);
    //缩小按钮
    QToolButton *zoomOutIcon = new QToolButton;
    zoomOutIcon->setAutoRepeat(true);
    zoomOutIcon->setAutoRepeatInterval(33);
    zoomOutIcon->setAutoRepeatDelay(0);
    zoomOutIcon->setIcon(QPixmap(":/zoomout.png"));
    zoomOutIcon->setIconSize(iconSize);

    //绘制右侧缩放滑条
    zoomSlider = new QSlider;
    zoomSlider->setMinimum(0);//最小值
    zoomSlider->setMaximum(500);//最大值
    zoomSlider->setValue(250);//设置初始值
    zoomSlider->setTickPosition(QSlider::TicksRight);//设置刻度在滑槽的右侧

    //垂直布局
    QVBoxLayout *zoomSliderLayout = new QVBoxLayout;
    zoomSliderLayout->addWidget(zoomInIcon);
    zoomSliderLayout->addWidget(zoomSlider);
    zoomSliderLayout->addWidget(zoomOutIcon);

    //向左旋转按钮
    QToolButton *rotateLeftIcon = new QToolButton;
    rotateLeftIcon->setIcon(QPixmap(":/rotateleft.png"));
    rotateLeftIcon->setIconSize(iconSize);
    //向右旋转按钮
    QToolButton *rotateRightIcon = new QToolButton;
    rotateRightIcon->setIcon(QPixmap(":/rotateright.png"));
    rotateRightIcon->setIconSize(iconSize);
    //旋转滑动条
    rotateSlider = new QSlider;
    rotateSlider->setOrientation(Qt::Horizontal);
    rotateSlider->setMinimum(-360);
    rotateSlider->setMaximum(360);
    rotateSlider->setValue(0);
    rotateSlider->setTickPosition(QSlider::TicksBelow);//在滑块下方绘制刻度线

    //水平布局
    QHBoxLayout *rotateSliderLayout = new QHBoxLayout;
    rotateSliderLayout->addWidget(rotateLeftIcon);
    rotateSliderLayout->addWidget(rotateSlider);
    rotateSliderLayout->addWidget(rotateRightIcon);

    //还原按钮
    resetButton = new QToolButton;
    resetButton->setText(tr("还原"));
    resetButton->setEnabled(false);

    label = new QLabel(name);
    label2 = new QLabel(tr("图形项选择模式："));

    //四个功能设置按钮
    selectModeButton = new QToolButton;
    selectModeButton->setText(tr("鼠标选中"));
    selectModeButton->setCheckable(true);//设置按钮为可复选
    selectModeButton->setChecked(false);

    dragModeButton = new QToolButton;
    dragModeButton->setText(tr("整体拖拽"));
    dragModeButton->setCheckable(true);
    dragModeButton->setChecked(false);

    antialiasButton = new QToolButton;
    antialiasButton->setText(tr("抗锯齿处理"));
    antialiasButton->setCheckable(true);
    antialiasButton->setChecked(false);

    openGlButton = new QToolButton;
    openGlButton->setText(tr("OpenGL渲染"));
    openGlButton->setCheckable(true);
#ifndef QT_NO_OPENGL
    openGlButton->setEnabled(QGLFormat::hasOpenGL());
#else
    openGlButton->setEnabled(false);
#endif

    //打印按钮
    printButton = new QToolButton;
    printButton->setIcon(QIcon(QPixmap(":/fileprint.png")));

    QButtonGroup *pointerModeGroup = new QButtonGroup;//按钮容器 无视觉效果
    pointerModeGroup->setExclusive(true);
    pointerModeGroup->addButton(selectModeButton);
    pointerModeGroup->addButton(dragModeButton);

    QHBoxLayout *labelLayout = new QHBoxLayout;
    labelLayout->addWidget(label);
    labelLayout->addStretch();//添加分隔器
    labelLayout->addWidget(label2);
    labelLayout->addWidget(selectModeButton);
    labelLayout->addWidget(dragModeButton);
    labelLayout->addStretch();
    labelLayout->addSpacing(20);//添加20像素的间距
    labelLayout->addWidget(antialiasButton);
    labelLayout->addWidget(openGlButton);
    labelLayout->addWidget(printButton);

    //整体布局
    QGridLayout *topLayout = new QGridLayout;
    topLayout->addLayout(labelLayout, 0, 0);
    topLayout->addWidget(graphicsView, 1, 0);
    topLayout->addLayout(zoomSliderLayout, 1, 1);
    topLayout->addLayout(rotateSliderLayout, 2, 0);
    topLayout->addWidget(resetButton, 2, 1);
    setLayout(topLayout);

    //还原按钮
    connect(resetButton, SIGNAL(clicked()), this, SLOT(resetView()));
    //缩放滑动条
    connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
    //旋转滑动条
    connect(rotateSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
    //当拖动视图的垂直滑动条时，设置还原按钮可用
    connect(graphicsView->verticalScrollBar(), SIGNAL(valueChanged(int)),this, SLOT(setResetButtonEnabled()));
    //当拖动视图的水平滑动条时，设置还原按钮可用
    connect(graphicsView->horizontalScrollBar(), SIGNAL(valueChanged(int)),this, SLOT(setResetButtonEnabled()));
    //"鼠标选中"按钮
    connect(selectModeButton, SIGNAL(toggled(bool)), this, SLOT(togglePointerMode()));
    //"整体拖拽"按钮
    connect(dragModeButton, SIGNAL(toggled(bool)), this, SLOT(togglePointerMode()));
    //"抗锯齿处理"按钮
    connect(antialiasButton, SIGNAL(toggled(bool)), this, SLOT(toggleAntialiasing()));
    //"OpenGL增强"按钮
    connect(openGlButton, SIGNAL(toggled(bool)), this, SLOT(toggleOpenGL()));
    //"左旋转"按钮
    connect(rotateLeftIcon, SIGNAL(clicked()), this, SLOT(rotateLeft()));
    //"右旋转"按钮
    connect(rotateRightIcon, SIGNAL(clicked()), this, SLOT(rotateRight()));
    //"放大"按钮
    connect(zoomInIcon, SIGNAL(clicked()), this, SLOT(zoomIn()));
    //"缩小"按钮
    connect(zoomOutIcon, SIGNAL(clicked()), this, SLOT(zoomOut()));
    //"打印"按钮
    connect(printButton, SIGNAL(clicked()), this, SLOT(print()));

    setupMatrix();
}

QGraphicsView *drilldownView::view() const
{
    return static_cast<QGraphicsView *>(graphicsView);
}

//还原按钮
void drilldownView::resetView()
{
    zoomSlider->setValue(250);//两个滑动条的值设置到初始值（在滑动条中央）
    rotateSlider->setValue(0);
    //setupMatrix();
    graphicsView->ensureVisible(QRectF(0, 0, 0, 0));//调整视图的内容，使视图rect中的内容是可见的 矩形(0,0,0,0)可见
    resetButton->setEnabled(false);
}

//还原按钮设为可用
void drilldownView::setResetButtonEnabled()
{
    resetButton->setEnabled(true);
}

//缩放/旋转
void drilldownView::setupMatrix()
{
    //2的[(zoomSlider->value()-250)/50 ]次方  [2^-5,2^5]
    qreal scale = qPow(qreal(2), (zoomSlider->value() - 250) / qreal(50));

    QMatrix matrix;//变换矩阵
    matrix.scale(scale, scale);//缩放 将横坐标、纵坐标扩大/缩小sacle倍
    matrix.rotate(rotateSlider->value());//旋转到的角度 30 旋转到30°

    graphicsView->setMatrix(matrix);//视图按照旋转矩阵进行旋转
    setResetButtonEnabled();
}

//选择图形项模式 区域选择或拖动
void drilldownView::togglePointerMode()
{
    graphicsView->setDragMode(selectModeButton->isChecked()
                              ? QGraphicsView::RubberBandDrag
                              : QGraphicsView::ScrollHandDrag);
    /*
    QGraphicsView::ScrollHandDrag ：光标变为手型，可以拖动场景进行移动。
    QGraphicsView::RubberBandDrag ：使用橡皮筋效果，进行区域选择，可以选中一个区域内的所有图元
    */
    graphicsView->setInteractive(selectModeButton->isChecked());
}

//视图使用OpenGL增强
void drilldownView::toggleOpenGL()
{
#ifndef QT_NO_OPENGL
    graphicsView->setViewport(openGlButton->isChecked() ? new QGLWidget(QGLFormat(QGL::SampleBuffers)) : new QWidget);
#endif
}

//抗锯齿处理
void drilldownView::toggleAntialiasing()
{
    graphicsView->setRenderHint(QPainter::Antialiasing, antialiasButton->isChecked());
}

//打印
void drilldownView::print()
{
#if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
    QPrinter printer;
    QPrintDialog dialog(&printer, this);
    //对话框调用exec方法时，弹出对话框并等待用户响应，
    //用户可能点击确定，那么exec返回QDialog::Accepted值
    //如果点击取消则返回QDialog::Rejected
    if (dialog.exec() == QDialog::Accepted)
    {
        QPainter painter(&printer);
        graphicsView->render(&painter);
    }
#endif
}

//放大
void drilldownView::zoomIn(int level)
{
    zoomSlider->setValue(zoomSlider->value() + level);
}

//缩小
void drilldownView::zoomOut(int level)
{
    zoomSlider->setValue(zoomSlider->value() - level);
}

//左旋转
void drilldownView::rotateLeft()
{
    rotateSlider->setValue(rotateSlider->value() - 10);
}

//右旋转
void drilldownView::rotateRight()
{
    rotateSlider->setValue(rotateSlider->value() + 10);//旋转10°
}
