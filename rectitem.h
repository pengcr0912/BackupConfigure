#ifndef RECTITEM_H
#define RECTITEM_H

#include "baseitem.h"
class BaseItem;
//extern QPainterPath qt_graphicsItem_shapeFromPath(const QPainterPath &path, const QPen &pen);
class RectItem : public BaseItem
{
public:
     RectItem(QGraphicsItem *parent = 0);
     RectItem(const QRectF &rect,QGraphicsItem *parent = 0);
     RectItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = 0);
     ~RectItem();

     QRectF boundingRect() const;
     QPainterPath shape() const;
//        bool contains(const QPointF &point) const;

        void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

//        bool isObscuredBy(const QGraphicsItem *item) const;
//        QPainterPath opaqueArea() const;

        enum { Type = QGraphicsItem::UserType + 3 };
        int type() const;

private:

};
QDataStream &operator >>(QDataStream &in, RectItem &rectItem);


#endif // RECTITEM_H
