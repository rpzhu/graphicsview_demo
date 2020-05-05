#include "informationwindow.h"

InformationWindow::InformationWindow(int id, QSqlRelationalTableModel *items,
                                     QWidget *parent)
    : QDialog(parent)
{
    QLabel *itemLabel = new QLabel(tr("项目： "));
    QLabel *descriptionLabel = new QLabel(tr("描述： "));
    QLabel *imageFileLabel = new QLabel(tr("图片文件： "));

    createButtons();

    itemText = new QLabel;
    descriptionEditor = new QTextEdit;

    imageFileEditor = new QComboBox;
    imageFileEditor->setModel(items->relationModel(1));
    imageFileEditor->setModelColumn(items->relationModel(1)->fieldIndex("file"));

    //将数据库中的数据映射到窗口控件
    mapper = new QDataWidgetMapper(this);
    mapper->setModel(items);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setItemDelegate(new QSqlRelationalDelegate(mapper));
    mapper->addMapping(imageFileEditor, 1);
    mapper->addMapping(itemText, 2, "text");
    mapper->addMapping(descriptionEditor, 3);
    mapper->setCurrentIndex(id);

    connect(descriptionEditor, SIGNAL(textChanged()),
            this, SLOT(enableButtons()));
    connect(imageFileEditor, SIGNAL(currentIndexChanged(int)),
            this, SLOT(enableButtons()));

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(itemLabel, itemText);
    formLayout->addRow(imageFileLabel, imageFileEditor);
    formLayout->addRow(descriptionLabel, descriptionEditor);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);
    setLayout(layout);

    itemId = id;
    displayedImage = imageFileEditor->currentText();

    setWindowFlags(Qt::Window);
    enableButtons(false);
    setWindowTitle(itemText->text());
}

int InformationWindow::id()
{
    return itemId;
}

void InformationWindow::revert()
{
    mapper->revert();
    enableButtons(false);
}

void InformationWindow::submit()
{
    QString newImage(imageFileEditor->currentText());

    if (displayedImage != newImage)
    {
        displayedImage = newImage;
        emit imageChanged(itemId, newImage);
    }

    mapper->submit();
    mapper->setCurrentIndex(itemId);

    enableButtons(false);
}

void InformationWindow::createButtons()
{
    closeButton = new QPushButton(tr("关闭"));
    revertButton = new QPushButton(tr("恢复"));
    submitButton = new QPushButton(tr("提交"));

    closeButton->setDefault(true);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(revertButton, SIGNAL(clicked()), this, SLOT(revert()));
    connect(submitButton, SIGNAL(clicked()), this, SLOT(submit()));

    buttonBox = new QDialogButtonBox(this);
    buttonBox->addButton(submitButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(revertButton, QDialogButtonBox::ResetRole);
    buttonBox->addButton(closeButton, QDialogButtonBox::RejectRole);
}

void InformationWindow::enableButtons(bool enable)
{
    revertButton->setEnabled(enable);
    submitButton->setEnabled(enable);
}
