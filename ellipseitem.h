#ifndef ELLIPSEITEM_H
#define ELLIPSEITEM_H
#include "baseitem.h"

class QBrush;
class QPen;
class QRect;

class EllipseItem : public BaseItem
{
public:
    EllipseItem(QGraphicsItem *parent = 0);
    EllipseItem(const QRectF &rect,QGraphicsItem *parent = 0);
    EllipseItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = 0);
    ~EllipseItem();


    int startAngle() const;
    void setStartAngle(int angle);

    int spanAngle() const;
    void setSpanAngle(int angle);

    QRectF boundingRect() const;
    QPainterPath shape() const;
//    bool contains(const QPointF &point) const;//Ô´Âë¶¼ÊÇ¼Ì³ÐQGraphicsItem

    void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

//    bool isObscuredBy(const QGraphicsItem *item) const;
//    QPainterPath opaqueArea() const;

    enum { Type = QGraphicsItem::UserType + 4 };
    int type() const;
protected:

private:

    int myStartAngle;
    int mySpanAngle;
};

QDataStream &operator >>(QDataStream &in, EllipseItem &ellipseItem);
#endif // ELLIPSEITEM_H
