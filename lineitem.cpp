#include "lineitem.h"
#include "qpainter.h"

LineItem::LineItem()
{
   setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
}
LineItem::~LineItem()
{

}
void LineItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    int old = painter->renderHints();
    painter->setRenderHint(QPainter::Antialiasing,true);
    QGraphicsLineItem::paint(painter,option,widget);
    painter->setRenderHint((QPainter::RenderHint)old,true);
}

