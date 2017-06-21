#ifndef GROUPITEM_H
#define GROUPITEM_H

#include "baseitem.h"
#include <QGraphicsItem>

class GroupItem : public BaseItem
{
public:
    GroupItem(QGraphicsItem *parent = 0);
    ~GroupItem();

    void addToGroup(QGraphicsItem *item);
    void removeFromGroup(QGraphicsItem *item);

    QRectF boundingRect() const;

    void setPen(const QPen &pen);
    void setPen(const QColor &color);
    void setPen(const int width);

    void setBrush(const QBrush &brush);

    void  setRect(const QRectF &rect);
    QRectF rect() const;


    enum { Type = QGraphicsItem::UserType + 10 };
    int type() const
           {return Type; }
protected:
    void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event){ }
private:
    QRectF groupItemBoundingRect;
};

#endif // GROUPITEM_H
