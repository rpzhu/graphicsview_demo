#include "drilldown_view.h"
#include "connection.h"
#include "imageitem.h"
#include "informationwindow.h"
#include <QSqlRelationalTableModel>
#pragma execution_character_set("utf-8")
drilldown_View::drilldown_View(QWidget* parent)
  : QGraphicsView(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowFlags(windowFlags() | Qt::Dialog);
    setWindowModality(Qt::ApplicationModal); //阻塞除当前窗体之外的所有的窗体

    createConnection();

    //创建数据库关系表模型 模型中的数据来源于数据库
    itemTable = new QSqlRelationalTableModel(this);
    itemTable->setTable("items"); //数据库的items表
    //items表的itemid列关联 与 images表的file列关联
    itemTable->setRelation(1, QSqlRelation("images", "itemid", "file"));
    itemTable->select(); //select * from items

    scene = new QGraphicsScene(this); //创建场景
    scene->setSceneRect(0, 0, 465, 365);
    setScene(scene); //为本视图设置场景

    addItems();

    setMinimumSize(470, 370);
    setMaximumSize(470, 370);

    QLinearGradient gradient(QPointF(0, 0), QPointF(0, 370));
    gradient.setColorAt(0, QColor("#0000FF"));
    gradient.setColorAt(1, QColor("#FF0000"));
    setBackgroundBrush(gradient); //视图的背景使用线性渐变

    show();
}

void
drilldown_View::addItems()
{
    int itemCount = itemTable->rowCount(); //行数

    int imageOffset = 150;
    int leftMargin = 70;
    int topMargin = 40;

    for (int i = 0; i < itemCount; i++) {
        ImageItem* image;
        QGraphicsTextItem* label;                 //文本图形项
        QSqlRecord record = itemTable->record(i); //数据库中一条记录 即一行

        //获取一行中的相应的列数据
        int id = record.value("id").toInt();
        QString file = record.value("file").toString();
        QString item = record.value("itemtype").toString();

        int columnOffset = ((i % 2) * 37);
        int x = ((i % 2) * imageOffset) + leftMargin + columnOffset;
        int y = ((i / 2) * imageOffset) + topMargin;

        //从数据库中获取图片名称->包装成地址->创建图像->创建图像图形项
        image = new ImageItem(id, QPixmap(":/" + file));
        image->setData(0, i);
        image->setPos(x, y);
        scene->addItem(image); //添加图形图形项到场景

        label = scene->addText(item); //字符串添加到场景，被包装成文本图形项
        label->setDefaultTextColor(QColor("#FF0000"));
        QPointF labelOffset((120 - label->boundingRect().width()) / 2, 120.0);
        label->setPos(QPointF(x, y) + labelOffset);
    }
}

//鼠标松开事件 效果：松开鼠标打开一个窗口
void
drilldown_View::mouseReleaseEvent(QMouseEvent* event)
{
    if (QGraphicsItem* item = itemAt(event->pos())) //如果鼠标松开处存在图像图形项
    {
        //该图片图形项能成功转换成自定义的ImageItem图形项
        if (ImageItem* image = qgraphicsitem_cast<ImageItem*>(item)) {
            showInformation(image);
        }
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void
drilldown_View::showInformation(ImageItem* image)
{
    int id = image->id();
    if (id < 0 || id >= itemTable->rowCount()) {
        return;
    }

    InformationWindow* window = findWindow(id);
    if (window && window->isVisible()) {
        window->raise();          //让窗口成为顶层窗口
        window->activateWindow(); //让窗口成为激活状态
    } else if (window && !window->isVisible()) {
        window->show();
    } else {
        InformationWindow* window;
        window = new InformationWindow(id, itemTable, this);

        connect(window, SIGNAL(imageChanged(int, QString)), this, SLOT(updateImage(int, QString)));

        window->move(pos() + QPoint(20, 40));
        window->show();
        informationWindows.append(window);
    }
}

//更改图像图形项显示的图片
void
drilldown_View::updateImage(int id, const QString& fileName)
{
    QList<QGraphicsItem*> items = scene->items();

    while (!items.empty()) {
        QGraphicsItem* item = items.takeFirst();

        if (ImageItem* image = qgraphicsitem_cast<ImageItem*>(item)) {
            if (image->id() == id) {
                image->setPixmap(QPixmap(":/" + fileName));
                image->adjust();
                break;
            }
        }
    }
}

InformationWindow*
drilldown_View::findWindow(int id)
{
    QList<InformationWindow*>::iterator i, beginning, end;

    beginning = informationWindows.begin();
    end = informationWindows.end();

    for (i = beginning; i != end; ++i) {
        InformationWindow* window = (*i);
        if (window && (window->id() == id)) {
            return window;
        }
    }
    return 0;
}
