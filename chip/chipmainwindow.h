#ifndef CHIPMAINWINDOW_H
#define CHIPMAINWINDOW_H
#include <QWidget>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QSplitter;
QT_END_NAMESPACE
#pragma execution_character_set("utf-8")
class ChipMainWindow : public QWidget
{
    Q_OBJECT
public:
    ChipMainWindow(QWidget* parent = 0);

private:
    void setupMatrix();
    void populateScene();

    QGraphicsScene* scene; //图形视图框架-场景对象
    QSplitter* h1Splitter; //切分窗口对象 可使窗口切分开
    QSplitter* h2Splitter;
};

#endif // MAINWINDOW_H
