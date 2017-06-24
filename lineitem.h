#ifndef LINEITEM_H
#define LINEITEM_H

#include <QGraphicsLineItem>

class LineItem : public QGraphicsLineItem
{
public:
    LineItem();
    ~LineItem();
    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
    enum { Type = QGraphicsItem::UserType + 6 };
    int type() const;
};

#endif // LINEITEM_H
