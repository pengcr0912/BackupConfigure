#ifndef BASEITEM_H
#define BASEITEM_H
#include <QtGui>
#include <QGraphicsItem>
//#include <groupitem.h>
class QMenu;
class RectItem;
class EllipseItem;
class GroupItem;
class BaseItem : public QGraphicsItem
{
public:
    enum HoverPos{None, TopLeft, TopCenter, TopRight, LeftCenter, RightCenter, BottomLeft, BottomCenter, BottomRight};
    enum ItemType{RectangleItemType, EllipseItemType};//暂时没用到
    BaseItem(QGraphicsItem* parent = 0);
//    BaseItem()
    ~BaseItem();

    QPen pen() const;
    virtual void setPen(const QPen &pen);
    virtual void setPen(const QColor &color);
    virtual void setPen(const int width);
    QBrush brush() const;
    virtual void setBrush(const QBrush &brush);

    virtual QRectF  rect() const;
    virtual void  setRect(const QRectF &rect);
    inline void setRect(qreal x, qreal y, qreal w, qreal h);

    void mySetIsMemberOfGroup(bool enabled);
    GroupItem *myGroup() const;
    void mySetGroup(GroupItem *group);

    enum { Type = QGraphicsItem::UserType + 2 };
    int type() const;
    static const QSizeF minSize;
protected:
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent * event);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);


   virtual void paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option,
                          QWidget *widget = 0);
   virtual void paintBoundingRect(QPainter *painter, const QStyleOptionGraphicsItem *option,
                                  QWidget *widget = 0);
   virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget = 0);

    mutable QRectF myBoundingRect;
    QRectF myRect;
    bool resizeHandlePressed;
    HoverPos myHoverPosId;
    static const int resizeHandleWidth = 6;
private:

    QBrush myBrush;
    QPen myPen;

    bool IsMemberOfGroup;

};

inline void BaseItem::setRect(qreal ax, qreal ay, qreal w, qreal h)
{ setRect(QRectF(ax, ay, w, h)); }

QDataStream &operator <<(QDataStream &out, const BaseItem &baseitem);
QDataStream &operator >>(QDataStream &in, BaseItem &baseitem);

#endif // BASEITEM_H
