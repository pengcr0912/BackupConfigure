#ifndef PIXITEM_H
#define PIXITEM_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPainter>

class PixItem : public QGraphicsItem
{

public:
    enum { Type = QGraphicsItem::UserType + 7 };
    PixItem(QPixmap *pixmap, QGraphicsItem* parent = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int type() const;

//private:
    QPixmap pix;     		//作为图元显示的图片
/*
protected:
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent * event);
//    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);*/
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};

#endif // PIXITEM_H
