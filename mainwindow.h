#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class FlowLayout;
class QDesktopWidget;
#pragma execution_character_set("utf-8")
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow* ui;
    FlowLayout* flowlayout;
    void init_add_btn();
    QDesktopWidget* desktop;
};
#endif // MAINWINDOW_H
