#ifndef CONNECTION_H
#define CONNECTION_H

#include <QApplication>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
/*
这个文件定义了一个辅助函数来打开连接到在内存SQLite数据库中创建一个测试表。
如果您想使用另一个数据库，只需修改代码即可。
*/
static bool
createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    if (!db.open()) {
        QMessageBox::critical(0, qApp->tr("Cannot open database"), qApp->tr("Unable to establish a database connection.\n"
                                                                            "This example needs SQLite support. Please read "
                                                                            "the Qt SQL driver documentation for information how "
                                                                            "to build it.\n\n"
                                                                            "Click Cancel to exit."),
                              QMessageBox::Cancel);
        return false;
    }

    QSqlQuery query;
    query.exec("create table person (id int primary key, "
               "firstname varchar(20), lastname varchar(20))");
    query.exec("insert into person values(101, 'Danny', 'Young')");
    query.exec("insert into person values(102, 'Christine', 'Holand')");
    query.exec("insert into person values(103, 'Lars', 'Gordon')");
    query.exec("insert into person values(104, 'Roberto', 'Robitaille')");
    query.exec("insert into person values(105, 'Maria', 'Papadopoulos')");

    query.exec("create table items (id int primary key,"
               "imagefile int,"
               "itemtype varchar(20),"
               "description varchar(100))");
    query.exec("insert into items "
               "values(0, 0, 'Qt',"
               "'Qt是一个完整的开发框架，其工具是为了精简创建惊人的应用程序而设计的。桌面、嵌入式和移动的用户界面平台')");
    query.exec("insert into items "
               "values(1, 1, 'Qt Quick',"
               "'Qt Quick是一系列旨在帮助开发人员使用CSS和JavaScript等语言创建直观、现代和流畅的用户界面的技术')");
    query.exec("insert into items "
               "values(2, 2, 'Qt Creator',"
               "'Qt Creator是一个强大的跨平台集成开发环境（IDE），包括UI设计工具和设备内调试')");
    query.exec("insert into items "
               "values(3, 3, 'Qt Project',"
               "'The Qt Project 管理Qt的开放源码开发，允许任何想要贡献的人通过审批者和维护者的精英结构加入到工作中')");

    query.exec("create table images (itemid int, file varchar(20))");
    query.exec("insert into images values(0, 'images/qt-logo.png')");
    query.exec("insert into images values(1, 'images/qt-quick.png')");
    query.exec("insert into images values(2, 'images/qt-creator.png')");
    query.exec("insert into images values(3, 'images/qt-project.png')");

    return true;
}

#endif
