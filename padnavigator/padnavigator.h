#ifndef PADNAVIGATOR_H
#define PADNAVIGATOR_H

#include "ui_form.h"
#include <QGraphicsView>
#pragma execution_character_set("utf-8")
QT_BEGIN_NAMESPACE
class QState;
class QStateMachine;
QT_END_NAMESPACE

class PadNavigator : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PadNavigator(const QSize& size, QWidget* parent = 0);

protected:
    void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;

private:
    Ui::Form form;
};

#endif // PADNAVIGATOR_H
