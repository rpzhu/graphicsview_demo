#include "diagramscene_MainWindow.h"
#include "arrow.h"
#include "diagramscene.h"
#include "diagramtextitem.h"
#include <QDebug>
#include <QtWidgets>

const int InsertTextButton = 10;
#pragma execution_character_set("utf-8")
diagramscene_MainWindow::diagramscene_MainWindow()
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowFlags(windowFlags() | Qt::Dialog);
    setWindowModality(Qt::ApplicationModal); //阻塞除当前窗体之外的所有的窗体

    createActions();
    createToolBox();
    createMenus();

    scene = new DiagramScene(itemMenu, this); //创建场景
    scene->setSceneRect(QRectF(0, 0, 5000, 5000));
    connect(scene, SIGNAL(itemInserted(DiagramItem*)), this, SLOT(itemInserted(DiagramItem*)));
    connect(scene, SIGNAL(textInserted(QGraphicsTextItem*)), this, SLOT(textInserted(QGraphicsTextItem*)));
    connect(scene, SIGNAL(itemSelected(QGraphicsItem*)), this, SLOT(itemSelected(QGraphicsItem*)));
    createToolbars();

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(toolBox);
    view = new QGraphicsView(scene);
    layout->addWidget(view);

    QWidget* widget = new QWidget;
    widget->setLayout(layout);

    setCentralWidget(widget);
    setWindowTitle(tr("流程图绘制"));
    setUnifiedTitleAndToolBarOnMac(true);
}

//按下放置背景按钮组的任意按钮槽函数
void
diagramscene_MainWindow::backgroundButtonGroupClicked(QAbstractButton* button)
{
    QList<QAbstractButton*> buttons = backgroundButtonGroup->buttons();
    foreach (QAbstractButton* myButton, buttons) {
        if (myButton != button) {
            button->setChecked(false);
        }
    }
    QString text = button->text();
    if (text == tr("蓝色网格")) {
        scene->setBackgroundBrush(QPixmap(":/images/background1.png"));
    } else if (text == tr("白色网格")) {
        scene->setBackgroundBrush(QPixmap(":/images/background2.png"));
    } else if (text == tr("灰色网格")) {
        scene->setBackgroundBrush(QPixmap(":/images/background3.png"));
    } else {
        scene->setBackgroundBrush(QPixmap(":/images/background4.png"));
    }

    scene->update();
    view->update();
}

//多边形按钮集中的任意按钮按下了
void
diagramscene_MainWindow::buttonGroupClicked(int id)
{
    //该按钮集中所有按钮
    QList<QAbstractButton*> buttons = buttonGroup->buttons();
    foreach (QAbstractButton* button, buttons) {
        if (buttonGroup->button(id) != button) {
            button->setChecked(false);
        }
    }
    if (id == InsertTextButton) //按下的是文字按钮 更改场景模式为插入文本模式
    {
        scene->setMode(DiagramScene::InsertText);
    } else {
        scene->setItemType(DiagramItem::DiagramType(id));
        scene->setMode(DiagramScene::InsertItem); //场景模式为插入多边形
    }
}

//从流程图中删除此项
void
diagramscene_MainWindow::deleteItem()
{
    foreach (QGraphicsItem* item, scene->selectedItems()) {
        if (item->type() == Arrow::Type) {
            scene->removeItem(item);
            Arrow* arrow = qgraphicsitem_cast<Arrow*>(item);
            arrow->startItem()->removeArrow(arrow);
            arrow->endItem()->removeArrow(arrow);
            delete item;
        }
    }

    foreach (QGraphicsItem* item, scene->selectedItems()) {
        if (item->type() == DiagramItem::Type) {
            qgraphicsitem_cast<DiagramItem*>(item)->removeArrows();
        }
        scene->removeItem(item);
        delete item;
    }
}

void
diagramscene_MainWindow::pointerGroupClicked(int)
{
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}

//设置此图形项到最上层
void
diagramscene_MainWindow::bringToFront()
{
    if (scene->selectedItems().isEmpty()) {
        return;
    }

    QGraphicsItem* selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem*> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem* item, overlapItems) {
        if (item->zValue() >= zValue && item->type() == DiagramItem::Type) {
            zValue = item->zValue() + 0.1;
        }
    }
    selectedItem->setZValue(zValue);
}

//设置此图像移到最底层
void
diagramscene_MainWindow::sendToBack()
{
    if (scene->selectedItems().isEmpty()) {
        return;
    }

    QGraphicsItem* selectedItem = scene->selectedItems().first();
    QList<QGraphicsItem*> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem* item, overlapItems) {
        if (item->zValue() <= zValue && item->type() == DiagramItem::Type) {
            zValue = item->zValue() - 0.1;
        }
    }
    selectedItem->setZValue(zValue);
}

//场景发过来的插入多边形信号的槽函数
void
diagramscene_MainWindow::itemInserted(DiagramItem* item)
{
    pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
    buttonGroup->button(int(item->diagramType()))->setChecked(false);
}

//类似 插入文本
void
diagramscene_MainWindow::textInserted(QGraphicsTextItem*)
{
    buttonGroup->button(InsertTextButton)->setChecked(false);
    scene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}

void
diagramscene_MainWindow::currentFontChanged(const QFont&)
{
    handleFontChange();
}

//字体大小设置
void
diagramscene_MainWindow::fontSizeChanged(const QString&)
{
    handleFontChange();
}

//场景缩放
void
diagramscene_MainWindow::sceneScaleChanged(const QString& scale)
{
    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QMatrix oldMatrix = view->matrix();
    view->resetMatrix();
    view->translate(oldMatrix.dx(), oldMatrix.dy());
    view->scale(newScale, newScale);
}

//字体颜色按钮下拉菜单槽函数
void
diagramscene_MainWindow::textColorChanged()
{
    textAction = qobject_cast<QAction*>(sender());
    fontColorToolButton->setIcon(createColorToolButtonIcon(
        ":/images/textpointer.png",
        qvariant_cast<QColor>(textAction->data())));
    textButtonTriggered();
}

//多边形填充颜色按钮下拉菜单槽函数
void
diagramscene_MainWindow::itemColorChanged()
{
    fillAction = qobject_cast<QAction*>(sender());
    fillColorToolButton->setIcon(createColorToolButtonIcon(
        ":/images/floodfill.png",
        qvariant_cast<QColor>(fillAction->data())));
    fillButtonTriggered();
}

void
diagramscene_MainWindow::lineColorChanged()
{
    lineAction = qobject_cast<QAction*>(sender());
    lineColorToolButton->setIcon(createColorToolButtonIcon(
        ":/images/linecolor.png",
        qvariant_cast<QColor>(lineAction->data())));
    lineButtonTriggered();
}

//设置文字的颜色为字体颜色按钮的弹出式下拉菜单选中的颜色
void
diagramscene_MainWindow::textButtonTriggered()
{
    scene->setTextColor(qvariant_cast<QColor>(textAction->data()));
}

//填充多边形操作
void
diagramscene_MainWindow::fillButtonTriggered()
{
    scene->setItemColor(qvariant_cast<QColor>(fillAction->data()));
}

//设置线段颜色操作
void
diagramscene_MainWindow::lineButtonTriggered()
{
    scene->setLineColor(qvariant_cast<QColor>(lineAction->data()));
}

//改变字体
void
diagramscene_MainWindow::handleFontChange()
{
    QFont font = fontCombo->currentFont();
    font.setPointSize(fontSizeCombo->currentText().toInt());
    font.setWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(italicAction->isChecked());
    font.setUnderline(underlineAction->isChecked());
    scene->setFont(font);
}

//当选中文本图形项中的文字时的槽函数
void
diagramscene_MainWindow::itemSelected(QGraphicsItem* item)
{
    DiagramTextItem* textItem = qgraphicsitem_cast<DiagramTextItem*>(item);

    QFont font = textItem->font(); //文本图形项的字体
    //设置选中文本图形项中文本的字体
    fontCombo->setCurrentFont(font);
    fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
    boldAction->setChecked(font.weight() == QFont::Bold);
    italicAction->setChecked(font.italic());
    underlineAction->setChecked(font.underline());
}

void
diagramscene_MainWindow::createToolBox()
{
    //放置多边形/文字按钮
    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(false);
    //该QButtonGroup中任意按钮按下信号
    connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(buttonGroupClicked(int)));

    QGridLayout* layout = new QGridLayout;
    //多边形工具按钮
    layout->addWidget(createCellWidget(tr("棱形"), DiagramItem::Conditional), 0, 0);
    layout->addWidget(createCellWidget(tr("正方形"), DiagramItem::Step), 0, 1);
    layout->addWidget(createCellWidget(tr("平行四边形"), DiagramItem::Io), 1, 0);

    //文字工具按钮  开始
    QToolButton* textButton = new QToolButton;
    textButton->setCheckable(true);
    buttonGroup->addButton(textButton, InsertTextButton);
    textButton->setIcon(QIcon(QPixmap(":/images/textpointer.png")));
    textButton->setIconSize(QSize(50, 50));

    QGridLayout* textLayout = new QGridLayout;
    textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
    textLayout->addWidget(new QLabel(tr("文字")), 1, 0, Qt::AlignCenter);

    QWidget* textWidget = new QWidget;
    textWidget->setLayout(textLayout);
    layout->addWidget(textWidget, 1, 1);
    //文字工具按钮  终

    layout->setRowStretch(3, 10);
    layout->setColumnStretch(2, 10);

    QWidget* itemWidget = new QWidget;
    itemWidget->setLayout(layout);

    //放置背景按钮
    backgroundButtonGroup = new QButtonGroup(this);
    connect(backgroundButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(backgroundButtonGroupClicked(QAbstractButton*)));

    QGridLayout* backgroundLayout = new QGridLayout;
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("蓝色网格"), ":/images/background1.png"), 0, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("白色网格"), ":/images/background2.png"), 0, 1);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("灰色网格"), ":/images/background3.png"), 1, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("无网格"), ":/images/background4.png"), 1, 1);

    backgroundLayout->setRowStretch(2, 10);
    backgroundLayout->setColumnStretch(2, 10);

    QWidget* backgroundWidget = new QWidget;
    backgroundWidget->setLayout(backgroundLayout);

    toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    /*
    QSizePolicy.Fixed	widget 的实际尺寸只参考 sizeHint() 的返回值，不能伸展（grow）和收缩（shrink）
    QSizePolicy.Minimum	可以伸展和收缩，不过sizeHint() 的返回值规定了 widget 能缩小到的最小尺寸
    QSizePolicy.Maximum	可以伸展和收缩，不过sizeHint() 的返回值规定了 widget 能伸展到的最大尺寸
    QSizePolicy.Preferred	可以伸展和收缩，但没有优势去获取更大的额外空间使自己的尺寸比 sizeHint() 的返回值更大
    QSizePolicy.Expanding	可以伸展和收缩，它会尽可能多地去获取额外的空间，也就是比 Preferred 更具优势
    QSizePolicy.MinimumExpanding	可以伸展和收缩，不过sizeHint() 的返回值规定了 widget 能缩小到的最小尺寸,同时它比 Preferred 更具优势去获取额外空间
    QSizePolicy.Ignored	忽略 sizeHint() 的作用
    */
    toolBox->setMinimumWidth(itemWidget->sizeHint().width());
    toolBox->addItem(itemWidget, tr("基本流程图形状"));
    toolBox->addItem(backgroundWidget, tr("背景"));
}

void
diagramscene_MainWindow::createActions()
{
    toFrontAction = new QAction(QIcon(":/images/bringtofront.png"), tr("此项移到最上层"), this);
    //设置快捷键
    toFrontAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
    toFrontAction->setStatusTip(tr("此项移到最上层"));
    connect(toFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront()));

    sendBackAction = new QAction(QIcon(":/images/sendtoback.png"), tr("此项移到最底层"), this);
    sendBackAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_B));
    sendBackAction->setStatusTip(tr("此项移到最底层"));
    connect(sendBackAction, SIGNAL(triggered()), this, SLOT(sendToBack()));

    deleteAction = new QAction(QIcon(":/images/delete.png"), tr("删除"), this);
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setStatusTip(tr("从流程图中删除此项"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    exitAction = new QAction(tr("退出"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("退出程序"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    boldAction = new QAction(tr("加粗"), this);
    boldAction->setCheckable(true);
    QPixmap pixmap(":/images/bold.png");
    boldAction->setIcon(QIcon(pixmap));
    boldAction->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_B));
    connect(boldAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    italicAction = new QAction(QIcon(":/images/italic.png"), tr("斜体"), this);
    italicAction->setCheckable(true);
    italicAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));
    connect(italicAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    underlineAction = new QAction(QIcon(":/images/underline.png"), tr("下划线"), this);
    underlineAction->setCheckable(true);
    underlineAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_U));
    connect(underlineAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));
}

void
diagramscene_MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("文件"));
    fileMenu->addAction(exitAction);

    itemMenu = menuBar()->addMenu(tr("条目"));
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    itemMenu->addAction(toFrontAction);
    itemMenu->addAction(sendBackAction);
}

void
diagramscene_MainWindow::createToolbars()
{
    editToolBar = addToolBar(tr("退出"));
    editToolBar->addAction(deleteAction);
    editToolBar->addAction(toFrontAction);
    editToolBar->addAction(sendBackAction);

    fontCombo = new QFontComboBox();
    connect(fontCombo, SIGNAL(currentFontChanged(QFont)), this, SLOT(currentFontChanged(QFont)));

    //字体大小下拉框
    fontSizeCombo = new QComboBox;
    fontSizeCombo->setEditable(true); //下拉框可编辑
    for (int i = 8; i < 74; i = i + 2) {
        fontSizeCombo->addItem(QString().setNum(i));
    }
    QIntValidator* validator = new QIntValidator(2, 72, this);
    fontSizeCombo->setValidator(validator); //下拉框只能输入2-72的数字
    connect(fontSizeCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(fontSizeChanged(QString)));

    //字体颜色工具按钮
    fontColorToolButton = new QToolButton;
    fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup); //按钮样式：带有弹出样式的下拉菜单
    //创建此按钮的弹出样式下拉菜单 并为该菜单指定一个槽函数
    fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()), Qt::black));
    textAction = fontColorToolButton->menu()->defaultAction();
    fontColorToolButton->setIcon(createColorToolButtonIcon(":/images/textpointer.png", Qt::black));
    fontColorToolButton->setAutoFillBackground(true);
    connect(fontColorToolButton, SIGNAL(clicked()), this, SLOT(textButtonTriggered()));

    //多边形填充颜色按钮 类似
    fillColorToolButton = new QToolButton;
    fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()), Qt::white));
    fillAction = fillColorToolButton->menu()->defaultAction();
    fillColorToolButton->setIcon(createColorToolButtonIcon(
        ":/images/floodfill.png", Qt::white));
    connect(fillColorToolButton, SIGNAL(clicked()), this, SLOT(fillButtonTriggered()));

    //线段颜色设置按钮 类似
    lineColorToolButton = new QToolButton;
    lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    lineColorToolButton->setMenu(createColorMenu(SLOT(lineColorChanged()), Qt::black));
    lineAction = lineColorToolButton->menu()->defaultAction();
    lineColorToolButton->setIcon(createColorToolButtonIcon(
        ":/images/linecolor.png", Qt::black));
    connect(lineColorToolButton, SIGNAL(clicked()), this, SLOT(lineButtonTriggered()));

    textToolBar = addToolBar(tr("字体"));
    textToolBar->addWidget(fontCombo);
    textToolBar->addWidget(fontSizeCombo);
    textToolBar->addAction(boldAction);
    textToolBar->addAction(italicAction);
    textToolBar->addAction(underlineAction);

    colorToolBar = addToolBar(tr("颜色"));
    colorToolBar->addWidget(fontColorToolButton);
    colorToolBar->addWidget(fillColorToolButton);
    colorToolBar->addWidget(lineColorToolButton);

    //切换移动图形项模式和插入线段模式
    QToolButton* pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/images/pointer.png"));
    QToolButton* linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/images/linepointer.png"));

    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton, int(DiagramScene::InsertLine));
    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)), this, SLOT(pointerGroupClicked(int)));

    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(2);
    connect(sceneScaleCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(sceneScaleChanged(QString)));

    pointerToolbar = addToolBar(tr("鼠标指针类型"));
    pointerToolbar->addWidget(pointerButton);
    pointerToolbar->addWidget(linePointerButton);
    pointerToolbar->addWidget(sceneScaleCombo);
}

//场景设置背景的按钮部件
QWidget*
diagramscene_MainWindow::createBackgroundCellWidget(const QString& text, const QString& image)
{
    QToolButton* button = new QToolButton;
    button->setText(text);
    button->setIcon(QIcon(image));
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    backgroundButtonGroup->addButton(button);

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget* widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}

//用多边形的形状和名称创建一个部件
QWidget*
diagramscene_MainWindow::createCellWidget(const QString& text, DiagramItem::DiagramType type)
{
    DiagramItem item(type, itemMenu);
    QIcon icon(item.image()); //多边形图形项项的图案作为图标250*250大小的

    QToolButton* button = new QToolButton;
    button->setIcon(icon);
    button->setIconSize(QSize(50, 50)); //缩小尺寸到50*500
    button->setCheckable(true);
    buttonGroup->addButton(button, int(type));

    QGridLayout* layout = new QGridLayout; //水平部件
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget* widget = new QWidget;
    widget->setLayout(layout);

    return widget;
}

//工具按钮带有弹出样式的下拉菜单 创建弹出菜单
QMenu*
diagramscene_MainWindow::createColorMenu(const char* slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
    QStringList names;
    names << tr("黑") << tr("白") << tr("红") << tr("蓝") << tr("黄");

    QMenu* colorMenu = new QMenu(this);
    for (int i = 0; i < colors.count(); ++i) {
        QAction* action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(createColorIcon(colors.at(i)));
        connect(action, SIGNAL(triggered()), this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor) {
            colorMenu->setDefaultAction(action);
        }
    }
    return colorMenu;
}

//创建/刷新按钮图标（选了工具按钮的弹出菜单的颜色后用该颜色刷新重新创建按钮图标）
QIcon
diagramscene_MainWindow::createColorToolButtonIcon(const QString& imageFile, QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    //在按钮中心绘制图标
    QRect target(4, 0, 42, 43);
    QRect source(0, 0, 42, 43);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);

    return QIcon(pixmap);
}

//创建工具按钮弹出菜单的图标
QIcon
diagramscene_MainWindow::createColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), color);

    return QIcon(pixmap);
}
