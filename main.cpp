#include "mainwindow.h"
#include <QApplication>
#pragma execution_character_set("utf-8")
int
main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("图形视图框架Demo收集");
    w.show();
    return a.exec();
}
