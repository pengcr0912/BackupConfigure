#include "ellipseitem.h"

#include "QPainterPathStroker"

EllipseItem::EllipseItem(QGraphicsItem *parent)
    :BaseItem(parent)
    ,myStartAngle(0), mySpanAngle(360 * 16)
{
}
EllipseItem::EllipseItem(const QRectF &rect,QGraphicsItem *parent)
    :BaseItem(parent)
    ,myStartAngle(0), mySpanAngle(360 * 16)
{
    setRect(rect);
}
EllipseItem::EllipseItem(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
    :BaseItem(parent)
    ,myStartAngle(0), mySpanAngle(360 * 16)
{
    setRect(QRectF(x, y, w, h));
}
EllipseItem::~EllipseItem()
{

}


int EllipseItem::startAngle() const
{
    return myStartAngle;
}
void EllipseItem::setStartAngle(int angle)
{
    if (angle != myStartAngle)
    {
        prepareGeometryChange();
        myBoundingRect = QRectF();
        myStartAngle = angle;
        update();
    }
}

int EllipseItem::spanAngle() const
{
    return mySpanAngle;
}
void EllipseItem::setSpanAngle(int angle)
{
    if (angle != mySpanAngle)
    {
        prepareGeometryChange();
        myBoundingRect = QRectF();
        mySpanAngle = angle;
        update();
    }
}
QRectF EllipseItem::boundingRect() const
{
    if (myBoundingRect.isNull())
    {
        qreal halfpw;
        if(pen().widthF() > 5)
            halfpw = pen().widthF() / 2;
        else
            halfpw = (pen().widthF() +5 ) / 2 ;//5是拖动小正方形的边长的长度
        myBoundingRect = rect();

        if (halfpw > 0.0)
            myBoundingRect.adjust(-halfpw, -halfpw, halfpw, halfpw);
    }
    return myBoundingRect;
}
QPainterPath EllipseItem::shape() const
{
    QPainterPath path;
    if (myRect.isNull())
        return path;
    if (mySpanAngle != 360 * 16)
    {
        path.moveTo(myRect.center());
        path.arcTo(myRect, myStartAngle / 16.0, mySpanAngle / 16.0);
    }
    else
    {
        path.addEllipse(myRect);
    }
    if(isSelected())
    {
        QPointF topLeft, topCenter, topRight, leftCenter, bottomLeft, bottomCenter, bottomRight, rightCenter;
        qreal sX = 5 ;
        qreal sY = 5 ;

        QRectF rect = this->rect();

        topLeft = rect.topLeft();//(0,0)
        topCenter = QPointF(rect.center().x(), rect.top());
        topRight = rect.topRight();
        leftCenter = QPointF(rect.left(), rect.center().y());
        rightCenter = QPointF(rect.right(), rect.center().y());
        bottomLeft = rect.bottomLeft();
        bottomCenter = QPointF(rect.center().x(), rect.bottom());
        bottomRight = rect.bottomRight();

        path.addRect(QRectF((topLeft - QPointF(sX/2,sY/2)), QSizeF(sX,sY) ));
        path.addRect(QRectF((topCenter - QPointF(sX/2, sY/2)), QSizeF(sX, sY)));
        path.addRect(QRectF((topRight - QPointF(sX/2,sY/2)), QSizeF(sX,sY)));
        path.addRect(QRectF((leftCenter - QPointF(sX/2,sY/2)), QSizeF(sX,sY)));
        path.addRect(QRectF((rightCenter - QPointF(sX/2,sY/2)), QSizeF(sX,sY)));
        path.addRect(QRectF((bottomLeft - QPointF(sX/2,sY/2)), QSizeF(sX,sY)));
        path.addRect(QRectF((bottomCenter - QPointF(sX/2,sY/2)), QSizeF(sX,sY)));
        path.addRect(QRectF((bottomRight - QPointF(sX/2,sY/2)), QSizeF(sX,sY)));
    }
    return path;//有问题
}

void EllipseItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *widget)
{
    Q_UNUSED(widget);
    painter->setPen(pen());
    painter->setBrush(brush());
    painter->setRenderHint(QPainter::Antialiasing,true);//自己加的
    if ((mySpanAngle != 0) && (qAbs(mySpanAngle) % (360 * 16) == 0))
        painter->drawEllipse(rect());
    else
        painter->drawPie(myRect, myStartAngle, mySpanAngle);
    painter->setRenderHint(QPainter::Antialiasing,false);//自己加的
}

int EllipseItem::type() const
    {
        return Type;
    }
QDataStream &operator >>(QDataStream &in, EllipseItem &ellipseItem)
{
    qint32 itemType;
    QRectF rect;
    QPointF pos;
    QPen pen;
    QBrush brush;
    in >> itemType >> rect >> pos >> pen >> brush;
    ellipseItem.setRect(rect);
    ellipseItem.setPen(pen);
    ellipseItem.setBrush(brush);
    ellipseItem.setPos(pos);
    return in;
}
