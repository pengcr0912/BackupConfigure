#ifndef LINEITEM_H
#define LINEITEM_H

#include <QGraphicsLineItem>

class LineItem : public QGraphicsLineItem
{
public:
    LineItem();
    ~LineItem();
    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
};

#endif // LINEITEM_H
