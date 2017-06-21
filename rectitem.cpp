#include "rectitem.h"
#include "QPainter"

RectItem::RectItem(QGraphicsItem *parent)
    :BaseItem(parent)
{
}
RectItem::RectItem(const QRectF &rect,QGraphicsItem *parent)
        :BaseItem(parent)
{
    setRect(rect);
}
RectItem::RectItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
        :BaseItem(parent)
{
    setRect(QRectF(x, y, w, h));
}



RectItem::~RectItem()
{

}

QRectF RectItem::boundingRect() const
{
    if (myBoundingRect.isNull())
    {
        qreal halfpw;
        if(pen().widthF() > 5)
            halfpw = pen().widthF() / 2;
        else
            halfpw = (pen().widthF() + BaseItem::resizeHandleWidth) / 2 ;
        myBoundingRect = BaseItem::rect();
        if (halfpw > 0.0)
            myBoundingRect.adjust(-halfpw, -halfpw, halfpw, halfpw);
    }
    return myBoundingRect;
}

QPainterPath RectItem::shape() const
{
    QPainterPath path;
    path.addRect(myRect);
    return qt_graphicsItem_shapeFromPath(path, this->pen());
}
void RectItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option,
                              QWidget *widget)
{
    Q_UNUSED(widget);
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->drawRect(myRect);
}
int RectItem::type() const
    {
        return Type;
    }
QDataStream &operator >>(QDataStream &in, RectItem &rectItem)
{
    qint32 itemType;
    QRectF rect;
    QPointF pos;
    QPen pen;
    QBrush brush;
    in >> itemType >> rect >> pos >> pen >> brush;
    rectItem.setRect(rect);
    rectItem.setPen(pen);
    rectItem.setBrush(brush);
    rectItem.setPos(pos);
    return in;
}
