#ifndef ARROWITEM_H
#define ARROWITEM_H

#include <QGraphicsItem>
#include <QWidget>

class ArrowItem : public QGraphicsItem
{
public:
    const static float LINE_WIDTH;
    const static float ARROW_SIZE;
public:
    ArrowItem(QGraphicsItem* parent = 0);
    ~ArrowItem();
    void setData(const QPointF& from,const QRectF& rect);
public:
    QRectF boundingRect()const;
    void paint(QPainter* painter,const QStyleOptionGraphicsItem* style,QWidget* widget);
private:
    QPointF computeTo(const QPointF& from,const QRectF& to);
private:
    bool mValid;
    QPointF mFrom;
    QPointF mTo;
    QPointF mP1;
    QPointF mP2;
    QRectF  mRect;
};

#endif // ARROWITEM_H
