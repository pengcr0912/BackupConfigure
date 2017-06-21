#include "textitem.h"
#include "diagramscene.h"

TextItem::TextItem(QGraphicsItem *parent, QGraphicsScene *scene)
    :QGraphicsTextItem(parent)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}
QVariant TextItem::itemChange(GraphicsItemChange change,
                     const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
        emit selectedChange(this);//用来实现
    return value;
}
void TextItem::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}
void TextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::NoTextInteraction)
        {
            setTextInteractionFlags(Qt::TextEditorInteraction);
        }
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}
