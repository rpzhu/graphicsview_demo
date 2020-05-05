#ifndef DRILLDOWN_VIEW_H
#define DRILLDOWN_VIEW_H

#include <QGraphicsView>

class ImageItem;
class InformationWindow;
class QSqlRelationalTableModel;
#pragma execution_character_set("utf-8")
class drilldown_View : public QGraphicsView
{
    Q_OBJECT

public:
    drilldown_View(QWidget* parent = nullptr);

protected:
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

private slots:
    void updateImage(int id, const QString& fileName);

private:
    void addItems();
    InformationWindow* findWindow(int id);
    void showInformation(ImageItem* image);

    QGraphicsScene* scene;
    QList<InformationWindow*> informationWindows;
    QSqlRelationalTableModel* itemTable;
};

#endif // DRILLDOWN_VIEW_H
