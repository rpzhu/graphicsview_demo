#include "embeddeddialog.h"
#include "ui_embeddeddialog.h"
#include <QStyleFactory>

EmbeddedDialog::EmbeddedDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EmbeddedDialog)
{
    ui->setupUi(this);
    ui->layoutDirection->setCurrentIndex(layoutDirection() != Qt::LeftToRight);

    //窗口风格类型加入到界面的下拉框
    foreach (QString styleName, QStyleFactory::keys())
    {
        ui->style->addItem(styleName);
        if (style()->objectName().toLower() == styleName.toLower())
        {
            ui->style->setCurrentIndex(ui->style->count() - 1);
        }
    }

    connect(ui->layoutDirection, SIGNAL(activated(int)), this, SLOT(layoutDirectionChanged(int)));
    connect(ui->spacing, SIGNAL(valueChanged(int)), this, SLOT(spacingChanged(int)));
    connect(ui->fontComboBox, SIGNAL(currentFontChanged(QFont)), this, SLOT(fontChanged(QFont)));
    connect(ui->style, SIGNAL(activated(QString)), this, SLOT(styleChanged(QString)));
}

EmbeddedDialog::~EmbeddedDialog()
{
    delete ui;
}

//对话框布局方向设置
void EmbeddedDialog::layoutDirectionChanged(int index)
{
    setLayoutDirection(index == 0 ? Qt::LeftToRight : Qt::RightToLeft);
}

//对话框布局间距设置
void EmbeddedDialog::spacingChanged(int spacing)
{
    layout()->setSpacing(spacing);
    adjustSize();
}

//对话框字体设置
void EmbeddedDialog::fontChanged(const QFont &font)
{
    setFont(font);
}

static void setStyleHelper(QWidget *widget, QStyle *style)
{
    widget->setStyle(style);//设置窗体的的风格
    widget->setPalette(style->standardPalette());//返回风格的标准调色板。
    foreach (QObject *child, widget->children())//给该窗体的所有子对象都设置成该种风格
    {
        if (QWidget *childWidget = qobject_cast<QWidget *>(child))
        {
            setStyleHelper(childWidget, style);
        }
    }
}

//窗体风格选择
void EmbeddedDialog::styleChanged(const QString &styleName)
{
    QStyle *style = QStyleFactory::create(styleName);
    if (style)
    {
        setStyleHelper(this, style);
    }
}
