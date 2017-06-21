#include "groupitem.h"
#include <QtGui>
#include <QGraphicsTransform>
#include <QGraphicsSceneMouseEvent>

GroupItem::GroupItem(QGraphicsItem *parent)
    :BaseItem(parent)
{
    setHandlesChildEvents(true);//接收子类的所有事件
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
}

GroupItem::~GroupItem()
{
}

void GroupItem::addToGroup(QGraphicsItem *item)
{
    if (!item)
    {
            qWarning("QGraphicsItemGroup::addToGroup: cannot add null item");
            return;
        }
        if (item == this) {
            qWarning("QGraphicsItemGroup::addToGroup: cannot add a group to itself");
            return;
        }

        // COMBINE
        bool ok;
        QTransform itemTransform = item->itemTransform(this, &ok);

        if (!ok) {
            qWarning("QGraphicsItemGroup::addToGroup: could not find a valid transformation from item to group coordinates");
            return;
        }

        QTransform newItemTransform(itemTransform);
        item->setPos(mapFromItem(item, 0, 0));//实际上是this->mapFromItem(item, 0, 0)

        item->setParentItem(this);

        // removing position from translation component of the new transform
        if (!item->pos().isNull())//the same as QTransform().translate(dx, dy) but slightly faster.
            newItemTransform *= QTransform::fromTranslate(-item->x(), -item->y());
        //now: newItemTransform is an identity matrix
        // removing additional transformations properties applied with itemTransform()
        QPointF origin = item->transformOriginPoint(); //The default is QPointF(0,0).
        QMatrix4x4 m;//主要是用来显示3D效果的，一般transformList = NULL
        QList<QGraphicsTransform*> transformList = item->transformations();
        for (int i = 0; i < transformList.size(); ++i)
            transformList.at(i)->applyTo(&m);
        newItemTransform *= m.toTransform().inverted();

        newItemTransform.translate(origin.x(), origin.y());
        newItemTransform.rotate(-item->rotation());
        newItemTransform.scale(1/item->scale(), 1/item->scale());
        newItemTransform.translate(-origin.x(), -origin.y());

        // ### Expensive, we could maybe use dirtySceneTransform bit for optimization

        item->setTransform(newItemTransform);
//        item->d_func()->setIsMemberOfGroup(true);
        if(BaseItem *baseItem = dynamic_cast<BaseItem *>(item))
        {
            baseItem->mySetIsMemberOfGroup(true);
        }
        prepareGeometryChange();

        groupItemBoundingRect |= itemTransform.mapRect(item->boundingRect() | item->childrenBoundingRect());
        update();
}

void GroupItem::removeFromGroup(QGraphicsItem *item)
{
    if (!item) {
            qWarning("QGraphicsItemGroup::removeFromGroup: cannot remove null item");
            return;
        }

       QGraphicsItem *newParent = parentItem()/*d_ptr->parent*/;

        // COMBINE
        bool ok;
        QTransform itemTransform;
        if (newParent)
            itemTransform = item->itemTransform(newParent, &ok);
        else
            itemTransform = item->sceneTransform();

        QPointF oldPos = item->mapToItem(newParent, 0, 0);
        item->setParentItem(newParent);
        item->setPos(oldPos);

        // removing position from translation component of the new transform
        if (!item->pos().isNull())
            itemTransform *= QTransform::fromTranslate(-item->x(), -item->y());

        // removing additional transformations properties applied
        // with itemTransform() or sceneTransform()
        QPointF origin = item->transformOriginPoint();
        QMatrix4x4 m;
        QList<QGraphicsTransform*> transformList = item->transformations();
        for (int i = 0; i < transformList.size(); ++i)
            transformList.at(i)->applyTo(&m);
        itemTransform *= m.toTransform().inverted();
        itemTransform.translate(origin.x(), origin.y());
        itemTransform.rotate(-item->rotation());
        itemTransform.scale(1 / item->scale(), 1 / item->scale());
        itemTransform.translate(-origin.x(), -origin.y());

        // ### Expensive, we could maybe use dirtySceneTransform bit for optimization

        item->setTransform(itemTransform);
        item->setSelected(true);//自己加的
        if(BaseItem *baseItem = dynamic_cast<BaseItem *>(item))
            baseItem->mySetIsMemberOfGroup(baseItem->myGroup() != 0);
//        item->d_func()->setIsMemberOfGroup(item->group() != 0);

        // ### Quite expensive. But removeFromGroup() isn't called very often.
        prepareGeometryChange();
        groupItemBoundingRect = childrenBoundingRect();
}
void GroupItem::paintShape(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    foreach(QGraphicsItem *item, childItems())
    {
        item->paint(painter, option, widget);
    }
}
void GroupItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(resizeHandlePressed)
    {
        QPointF   myMovePos = event->pos();//获得的是移动点(item coordinates)

        QRectF groupRect = groupItemBoundingRect/*this->rect()*/;
        qreal wideChangeRate = 1;
        qreal heighChangeRate = 1;

        switch(myHoverPosId)
        {
        case TopLeft:
            if(groupRect.right() - myMovePos.x() >= 0.1 && groupRect.bottom() - myMovePos.y() >= 0.1)
            {
                wideChangeRate = (groupRect.bottomRight() - myMovePos).x() / groupRect.width();
                heighChangeRate = (groupRect.bottomRight() - myMovePos).y() / groupRect.height();
                groupRect.setTopLeft(myMovePos);
            }
            break;
        case TopCenter:
            if(groupRect.bottom() - myMovePos.y() >= 0.1)
            {
                wideChangeRate = 1;
                heighChangeRate = (groupRect.bottom() - myMovePos.y()) / groupRect.height();
                groupRect.setTop(myMovePos.y());
            }
            break;
        case TopRight:
            if(groupRect.bottom() - myMovePos.y() >= 0.1 &&
                    myMovePos.x() - groupRect.left() >= 0.1)
            {
                wideChangeRate = (myMovePos - groupRect.bottomLeft()).x() / groupRect.width();
                heighChangeRate = (groupRect.bottomLeft() - myMovePos).y() / groupRect.height();
                groupRect.setTopRight(myMovePos);
            }
            break;
        case LeftCenter:
            if(groupRect.right() - myMovePos.x() >= 0.1)
            {
                wideChangeRate = (groupRect.right() - myMovePos.x()) / groupRect.width();
                heighChangeRate = 1;
                groupRect.setLeft(myMovePos.x());
            }
            break;
        case RightCenter:
            if(myMovePos.x() - groupRect.left() >= 0.1)
            {
                wideChangeRate = (myMovePos.x() - groupRect.left()) / groupRect.width();
                heighChangeRate = 1;
                groupRect.setRight(myMovePos.x());
            }
             break;
        case BottomLeft:
            if(groupRect.right() - myMovePos.x() >= 0.1 && myMovePos.y() - groupRect.top() >=0.1 )
            {
                wideChangeRate = (groupRect.topRight() - myMovePos).x() / groupRect.width();
                heighChangeRate = (myMovePos - groupRect.topRight()).y() / groupRect.height();
                groupRect.setBottomLeft(myMovePos);
            }
            break;
        case BottomCenter:
            if(myMovePos.y() - groupRect.top() >= 0.1)
            {
                wideChangeRate = 1;
                heighChangeRate = (myMovePos.y() - groupRect.top()) / groupRect.height();
                groupRect.setBottom(myMovePos.y());
            }
            break;
        case BottomRight:
            if(myMovePos.y() - groupRect.top() >= 0.1 && myMovePos.x() - groupRect.left() >= 0.1)
            {
                wideChangeRate = ((myMovePos - groupRect.topLeft()).x()) / groupRect.width();
                heighChangeRate =((myMovePos - groupRect.topLeft()).y()) / groupRect.height();
                groupRect.setBottomRight(myMovePos);
            }
            break;
        default:
            break;
        }
        setRect(groupRect);
        update();
        foreach(QGraphicsItem *item, childItems())
        {
            if(BaseItem *baseItem = dynamic_cast<BaseItem *>(item))
            {
                QRectF oldRect = baseItem->rect();
                QRectF newRect;
                newRect.setWidth(oldRect.width() * wideChangeRate);
                newRect.setHeight(oldRect.height() * heighChangeRate);
                baseItem->setRect(newRect);

                QPointF oldPos = baseItem->pos();
                QPointF newPos(oldPos);
                switch(myHoverPosId)
                {
                case TopLeft:
                {
                    QPointF bottomRight = groupRect.bottomRight();
                    newPos.setX(bottomRight.x() + (oldPos.x() - bottomRight.x()) * wideChangeRate);
                    newPos.setY(bottomRight.y() + (oldPos.y() - bottomRight.y()) * heighChangeRate);
                    break;
                }
                case TopCenter:
                {
                    qreal bottom = groupRect.bottom();
                    newPos.setY(bottom + (oldPos.y() - bottom) * heighChangeRate);
                    break;
                }
                case TopRight:
                {
                    QPointF bottomLeft = groupRect.bottomLeft();
                    newPos.setX(bottomLeft.x() + (oldPos.x() - bottomLeft.x()) * wideChangeRate);
                    newPos.setY(bottomLeft.y() + (oldPos.y() - bottomLeft.y()) * heighChangeRate);
                    break;
                }
                case LeftCenter:
                {
                    qreal right = groupRect.right();
                    newPos.setX(right + (oldPos.x() - right) * wideChangeRate);
                    break;
                }
                case RightCenter:
                {
                    qreal left = groupRect.left();
                    newPos.setX(left + (oldPos.x() - left) * wideChangeRate);
                    break;
                }
                case BottomLeft:
                {
                    QPointF topRight = groupRect.topRight();
                    newPos.setX(topRight.x() + (oldPos.x() - topRight.x()) * wideChangeRate);
                    newPos.setY(topRight.y() + (oldPos.y() - topRight.y()) * heighChangeRate);
                    break;
                }
                case BottomCenter:
                {
                    qreal top = groupRect.top();
                    newPos.setY(top + (oldPos.y() - top) * heighChangeRate);
                    break;
                }
                case BottomRight:
                {
                    QPointF topLeft = groupRect.topLeft();
                    newPos.setX(topLeft.x() + (oldPos.x() - topLeft.x()) * wideChangeRate);
                    newPos.setY(topLeft.y() + (oldPos.y() - topLeft.y()) * heighChangeRate);
                    break;
                }
                default:
                    ;
                }
                baseItem->setPos(newPos);
            }
        }
        update();
    }
    else
        QGraphicsItem::mouseMoveEvent(event);
}

QRectF GroupItem::boundingRect() const
{
    qreal halfpw = (pen().widthF() + BaseItem::resizeHandleWidth+100) / 2 ;//5是拖动小正方形的边长的长度
    QRectF rect(groupItemBoundingRect);
    if (halfpw > 0.0)
    {
        rect.adjust(-halfpw, -halfpw, halfpw, halfpw);
    }
    return rect;
}

void GroupItem::setBrush(const QBrush &brush)
{
    if(childItems().empty())
        return;
    foreach(QGraphicsItem *item, childItems())
    {
        if(!item->isSelected())
            if(BaseItem *baseItem = dynamic_cast<BaseItem *>(item))
                baseItem->setBrush(brush);
    }
}
void GroupItem::setPen(const QPen &pen)
{
    foreach(QGraphicsItem *item, childItems())
    {
        if(!item->isSelected())
            if(BaseItem *baseItem = dynamic_cast<BaseItem *>(item))
                baseItem->setPen(pen);
    }
}
void GroupItem::setPen(const QColor &color)
{
    foreach(QGraphicsItem *item, childItems())
    {
        if(!item->isSelected())
            if(BaseItem *baseItem = dynamic_cast<BaseItem *>(item))
                baseItem->setPen(color);
    }
}
void GroupItem::setPen(const int width)
{
    foreach(QGraphicsItem *item, childItems())
    {
        if(!item->isSelected())
            if(BaseItem *baseItem = dynamic_cast<BaseItem *>(item))
                baseItem->setPen(width);
    }
}
void GroupItem::setRect(const QRectF &rect)
{
    groupItemBoundingRect = rect;
    update();
}
QRectF GroupItem::rect() const
{
    return groupItemBoundingRect;
}
