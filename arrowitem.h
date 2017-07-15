#ifndef ARROWITEM_H
#define ARROWITEM_H

//#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QWidget>

//class ArrowItem : public QGraphicsItem
class ArrowItem : public QGraphicsLineItem
{
public:
    const static float LINE_WIDTH;
    const static float ARROW_SIZE;
    int iType;
public:
    //ArrowItem(QGraphicsItem* parent = 0);
    ArrowItem();
    ~ArrowItem();

    void setData(const QPointF& from,const QRectF& rect);
    void setData(const QPointF& from,const QPointF& to, int lineWidth, int arrowType);

    enum { Type = QGraphicsItem::UserType + 8 };
public:
    //QRectF boundingRect()const;
    void paint(QPainter* painter,const QStyleOptionGraphicsItem* style,QWidget* widget = 0);
    int type() const;

private:
    QPointF computeTo(const QPointF& from,const QRectF& to);
private:
    bool mValid;
    QPointF mFrom;
    QPointF mTo;
    QPointF mP1;
    QPointF mP2;
    QPointF mP3;
    QPointF mP4;
    QRectF  mRect;
};

/*
inline QDataStream &operator << (QDataStream &out, int type)
{
    out << type;
    return out;
}

inline QDataStream &operator >> (QDataStream &in, int type)
{
    in >> type;
    return in;
}
*/
#endif // ARROWITEM_H
