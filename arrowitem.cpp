#include "arrowitem.h"
#include <QPainter>
#include <QtCore/qmath.h>

const float ArrowItem::LINE_WIDTH = 1.5f;
const float ArrowItem::ARROW_SIZE = 8.0f;

const double PI=3.1415926;

//ArrowItem::ArrowItem(QGraphicsItem* parent):
//QGraphicsItem(parent),
//mValid(false)
ArrowItem::ArrowItem()
{
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
}

ArrowItem::~ArrowItem()
{
}

QPointF ArrowItem::computeTo(const QPointF& from,const QRectF& to)
{
    QPointF center(to.center());
    QPointF top(center.x(),to.top());
    QPointF right(to.right(),center.y());
    QPointF bottom(center.x(),to.bottom());
    QPointF left(to.left(),center.y());

    //left
    if(from.x() < to.left())
    {
        if(from.y() < to.top())
        {
            QLineF line1(from,top);
            QLineF line2(from,left);
            return line1.length() > line2.length() ? left : top;
        }
        else if(from.y() > to.bottom())
        {
            QLineF line1(from,left);
            QLineF line2(from,bottom);
            return line1.length() > line2.length() ? bottom : left;
        }
        return left;
    }
    //right
    if(from.x() > to.right())
    {
        if(from.y() < to.top())
        {
            QLineF line1(from,top);
            QLineF line2(from,right);
            return line1.length() > line2.length() ? right : top;
        }
        else if(from.y() > to.bottom())
        {
            QLineF line1(from,bottom);
            QLineF line2(from,right);
            return line1.length() > line2.length() ? right : bottom;
        }
        return right;
    }

    if(from.y() < to.top())
        return top;
    else if(from.y() > to.bottom())
        return bottom;

    Q_ASSERT(0);
    return QPointF();
}

void ArrowItem::setData(const QPointF& from,const QRectF& rect)
{
    mValid = true;
    mFrom = from;
    mRect = rect;

    if(rect.contains(from))
    {
        mValid = false;
        return;
    }

    mTo = computeTo(mFrom,mRect);

    QLineF line(mFrom,mTo);
    qreal angle = ::acos(line.dx()/line.length());

    if(line.dy() >= 0)
        angle = 3.14159*2 - angle;

    mP1 = mTo + QPointF(sin(angle-PI/3)*ARROW_SIZE,cos(angle-PI/3)*ARROW_SIZE);
    mP2 = mTo + QPointF(sin(angle-PI+PI/3)*ARROW_SIZE,cos(angle-PI+PI/3)*ARROW_SIZE);
}

void ArrowItem::setData(const QPointF& from,const QPointF& to, int lineWidth, int arrowType)
{
    mValid = true;
    mFrom = from;
    mTo = to;
    QLineF line(mFrom,mTo);
    qreal angle = ::acos(line.dx()/line.length());

    if(line.dy() >= 0)
        angle = 3.14159*2 - angle;

    //mP1 = mTo + QPointF(sin(angle-PI/3)*ARROW_SIZE,cos(angle-PI/3)*ARROW_SIZE);
    //mP2 = mTo + QPointF(sin(angle-PI+PI/3)*ARROW_SIZE,cos(angle-PI+PI/3)*ARROW_SIZE);
    mP1 = mTo + QPointF(sin(angle-PI/3)*lineWidth*5,cos(angle-PI/3)*lineWidth*5);
    mP2 = mTo + QPointF(sin(angle-PI+PI/3)*lineWidth*5,cos(angle-PI+PI/3)*lineWidth*5);

    if(arrowType == 2)
    {
        mP3 = mFrom - QPointF(sin(angle-PI/3)*lineWidth*5,cos(angle-PI/3)*lineWidth*5);
        mP4 = mFrom - QPointF(sin(angle-PI+PI/3)*lineWidth*5,cos(angle-PI+PI/3)*lineWidth*5);
        iType = 2;
    }
    else
    {
        mP3 = mFrom;
        mP4 = mFrom;
        iType = 1;
    }
}

/*
QRectF ArrowItem::boundingRect()const
{
    qreal extra = (LINE_WIDTH + ARROW_SIZE)/2.0;
    QRectF rect= QRectF(mFrom,QSizeF(mTo.x()-mFrom.x(),mTo.y()-mFrom.y())).normalized().
        adjusted(-extra,-extra,extra,extra);
    return rect;
}
*/

void ArrowItem::paint(QPainter* painter,const QStyleOptionGraphicsItem* style,QWidget* widget)
{
    QGraphicsLineItem::paint(painter,style,widget);
    if(!mValid)
        return;
    painter->setRenderHint(QPainter::Antialiasing);//反走样，提高绘图质量
    //QPen p(QColor::fromRgb(79,136,187));
    //painter->setBrush(QBrush(p.color()));
    //p.setWidthF(LINE_WIDTH);
    //painter->setPen(p);
    painter->drawLine(mFrom,mTo);
    painter->drawPolygon(QPolygonF()<<mTo<<mP1<<mP2);
    painter->drawPolygon(QPolygonF()<<mFrom<<mP3<<mP4);
}

int ArrowItem::type() const
{
    return Type;
}
