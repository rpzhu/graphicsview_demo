#include <QGraphicsLayout>

class gvfFlowLayout : public QGraphicsLayout
{
public:
    gvfFlowLayout();
    inline void addItem(QGraphicsLayoutItem *item);
    void insertItem(int index, QGraphicsLayoutItem *item);
    void setSpacing(Qt::Orientations o, qreal spacing);
    qreal spacing(Qt::Orientation o) const;

    void setGeometry(const QRectF &geom) Q_DECL_OVERRIDE;

    int count() const Q_DECL_OVERRIDE;
    QGraphicsLayoutItem *itemAt(int index) const Q_DECL_OVERRIDE;
    void removeAt(int index) Q_DECL_OVERRIDE;

protected:
    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const Q_DECL_OVERRIDE;

private:
    qreal doLayout(const QRectF &geom, bool applyNewGeometry) const;
    QSizeF minSize(const QSizeF &constraint) const;
    QSizeF prefSize() const;
    QSizeF maxSize() const;

    QList<QGraphicsLayoutItem*> m_items;
    qreal m_spacing[2];
};

inline void gvfFlowLayout::addItem(QGraphicsLayoutItem *item)
{
    insertItem(-1, item);
}
