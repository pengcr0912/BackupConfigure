
#include "baseitem.h"
#include "propertiesdialog.h"
#include <groupitem.h>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsScene>
#include "deviceinfo.h"

const int BaseItem::resizeHandleWidth ;
const QSizeF BaseItem::minSize(0.0, 0.0);
/*
QPainterPath qt_graphicsItem_shapeFromPath(const QPainterPath &path, const QPen &pen)
{
    // We unfortunately need this hack as QPainterPathStroker will set a width of 1.0
    // if we pass a value of 0.0 to QPainterPathStroker::setWidth()
    const qreal penWidthZero = qreal(0.00000001);

    if (path == QPainterPath())
        return path;
    QPainterPathStroker ps;
    ps.setCapStyle(pen.capStyle());
    if (pen.widthF() <= 0.0)
        ps.setWidth(penWidthZero);
    else
        ps.setWidth(pen.widthF());
    ps.setJoinStyle(pen.joinStyle());
    ps.setMiterLimit(pen.miterLimit());
    QPainterPath p = ps.createStroke(path);
    p.addPath(path);
    return p;
}
*/
BaseItem::BaseItem(QGraphicsItem* parent)
    :QGraphicsItem(parent)
{
   myHoverPosId = None;
   IsMemberOfGroup = false;
   resizeHandlePressed = false;
   setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
   setAcceptHoverEvents(true);
}
BaseItem::~BaseItem()
{

}
QPen BaseItem::pen() const
{
    return myPen;
}
void BaseItem::setPen(const QPen &pen)
{
    prepareGeometryChange();
    myPen = pen;
    myBoundingRect = QRectF();
    update();
}
void BaseItem::setPen(const QColor &color)
{
    prepareGeometryChange();
    myPen.setBrush(color);
    myBoundingRect = QRectF();
    update();
}
void BaseItem::setPen(const int width)
{
    prepareGeometryChange();
    myPen.setWidth(width);
    myBoundingRect = QRectF();
    update();
}
QBrush BaseItem::brush() const
{
    return myBrush;
}
void BaseItem::setBrush(const QBrush &brush)
{
    myBrush = brush;
    update();
}
void BaseItem::setRect(const QRectF &rect)
{
    if (myRect == rect)
        return;
    prepareGeometryChange();
    myRect = rect;
    myBoundingRect = QRectF();
    update();
}
QRectF BaseItem::rect() const
{
    return myRect;
}

void BaseItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    BaseItem *baseItem = dynamic_cast<BaseItem *>(this);
    if(!baseItem)
        return ;

    setCursor(Qt::SizeAllCursor);
    if(isSelected())
    {
        QPointF topLeft, topCenter, topRight, leftCenter, bottomLeft, bottomCenter, bottomRight, rightCenter;
        QRectF topLeftRect, topCenterRect, topRightRect, leftCenterRect, rightCenterRect, bottomLeftRect, bottomCenterRect, bottomRightRect;
        QRectF rect = baseItem->rect();//return myRect；

        //分别对应边框上的8个小方格
        topLeft = rect.topLeft();
        topLeftRect = QRectF((topLeft - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                             QSizeF(resizeHandleWidth, resizeHandleWidth) );
        topCenter = QPointF(rect.center().x(), rect.top());
        topCenterRect = QRectF((topCenter - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                               QSizeF(resizeHandleWidth, resizeHandleWidth) );
        topRight = rect.topRight();
        topRightRect = QRectF((topRight - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                              QSizeF(resizeHandleWidth, resizeHandleWidth) );
        leftCenter = QPointF(rect.left(), rect.center().y());
        leftCenterRect = QRectF((leftCenter - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                QSizeF(resizeHandleWidth, resizeHandleWidth) );
        rightCenter = QPointF(rect.right(), rect.center().y());
        rightCenterRect = QRectF((rightCenter - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                 QSizeF(resizeHandleWidth, resizeHandleWidth) );
        bottomLeft = rect.bottomLeft();
        bottomLeftRect = QRectF((bottomLeft - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                QSizeF(resizeHandleWidth, resizeHandleWidth) );
        bottomCenter = QPointF(rect.center().x(), rect.bottom());
        bottomCenterRect = QRectF((bottomCenter - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                  QSizeF(resizeHandleWidth, resizeHandleWidth) );
        bottomRight = rect.bottomRight();
        bottomRightRect = QRectF((bottomRight - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                 QSizeF(resizeHandleWidth, resizeHandleWidth) );

        QPointF hoverPos = event->pos();//返回的item中的坐标
        int adjustSize = 0.5;

        resizeHandlePressed = true; //初始为真
        if(rect.width() >= 18 && rect.height() >= 18)//item的边长都大于18，则显示所有的8个小方格
        {
            if(topLeftRect.adjusted(-adjustSize, -adjustSize, adjustSize, adjustSize).contains(hoverPos))
            {
                //TopLeft
                setCursor(Qt::SizeFDiagCursor);
                myHoverPosId = TopLeft;
            }
            else if(topCenterRect.adjusted(-adjustSize, -adjustSize, adjustSize, adjustSize).contains(hoverPos))
            {
                //TopCenter
                this->setCursor(Qt::SizeVerCursor);
                myHoverPosId = TopCenter;
            }
            else if(topRightRect.adjusted(-adjustSize, -adjustSize, adjustSize, adjustSize).contains(hoverPos))
            {
                    //TopRight
                    this->setCursor(Qt::SizeBDiagCursor);
                    myHoverPosId = TopRight;
            }
            else if(rightCenterRect.adjusted(-adjustSize, -adjustSize, adjustSize, adjustSize).contains(hoverPos))
            {
                    //RightCenter
                    this->setCursor(Qt::SizeHorCursor);
                    myHoverPosId = RightCenter;
            }
            else if(leftCenterRect.adjusted(-adjustSize, -adjustSize, adjustSize, adjustSize).contains(hoverPos))
            {
                    //LeftCenter
                    this->setCursor(Qt::SizeHorCursor);
                    myHoverPosId = LeftCenter;
            }
            else if(bottomLeftRect.adjusted(-adjustSize, -adjustSize, adjustSize, adjustSize).contains(hoverPos))
            {
                    //BottomLeft
                    this->setCursor(Qt::SizeBDiagCursor);
                    myHoverPosId = BottomLeft;
            }
            else if(bottomCenterRect.adjusted(-adjustSize, -adjustSize, adjustSize, adjustSize).contains(hoverPos))
            {
                    //BottomCenter
                    this->setCursor(Qt::SizeVerCursor);
                    myHoverPosId = BottomCenter;
            }
            else if(bottomRightRect.adjusted(-adjustSize, -adjustSize, adjustSize, adjustSize).contains(hoverPos))
            {
                    //BottomRight
                    this->setCursor(Qt::SizeFDiagCursor);
                    myHoverPosId = BottomRight;
            }
            else
            {
                    //Revert
                resizeHandlePressed = false;
            }
        }
        else if ((rect.width() > 0 && rect.width() < 18) ||(rect.height() > 0 && rect.height() < 18))
        {
            if(topCenterRect.adjusted(-adjustSize, -adjustSize, adjustSize, adjustSize).contains(hoverPos))
            {
                //TopCenter
                this->setCursor(Qt::SizeVerCursor);
                myHoverPosId = TopCenter;
            }
            else if(rightCenterRect.adjusted(-adjustSize, -adjustSize, adjustSize, adjustSize).contains(hoverPos))
            {
                    //RightCenter
                    this->setCursor(Qt::SizeHorCursor);
                    myHoverPosId = RightCenter;
            }
            else if(leftCenterRect.adjusted(-adjustSize, -adjustSize, adjustSize, adjustSize).contains(hoverPos))
            {
                    //LeftCenter
                    this->setCursor(Qt::SizeHorCursor);
                    myHoverPosId = LeftCenter;
            }
            else if(bottomCenterRect.adjusted(-adjustSize, -adjustSize, adjustSize, adjustSize).contains(hoverPos))
            {
                    //BottomCenter
                    this->setCursor(Qt::SizeVerCursor);
                    myHoverPosId = BottomCenter;
            }
            else
            {
                    //Revert
                resizeHandlePressed = false;
            }
        }
        else if (rect.width() == 0 && rect.height() != 0 )
        {
            if(topCenterRect.adjusted(-adjustSize, -adjustSize, adjustSize, adjustSize).contains(hoverPos))
            {
                //TopCenter
                this->setCursor(Qt::SizeVerCursor);
                myHoverPosId = TopCenter;
            }
            else if(rightCenterRect.adjusted(-adjustSize, -adjustSize, adjustSize, adjustSize).contains(hoverPos))
            {
                //RightCenter
                this->setCursor(Qt::SizeHorCursor);
                myHoverPosId = RightCenter;
            }
            else if(bottomCenterRect.adjusted(-adjustSize, -adjustSize, adjustSize, adjustSize).contains(hoverPos))
            {
                //BottomCenter
                this->setCursor(Qt::SizeVerCursor);
                myHoverPosId = BottomCenter;
            }
            else
            {
                resizeHandlePressed = false;
            }
        }
        else if (rect.height() == 0 && rect.width() != 0 )
        {
            if(leftCenterRect.adjusted(-adjustSize, -adjustSize, adjustSize, adjustSize).contains(hoverPos))
            {
                    //LeftCenter
                    this->setCursor(Qt::SizeHorCursor);
                    myHoverPosId = LeftCenter;
            }
            else if(topCenterRect.adjusted(-adjustSize, -adjustSize, adjustSize, adjustSize).contains(hoverPos))
            {
                //TopCenter
                this->setCursor(Qt::SizeVerCursor);
                myHoverPosId = TopCenter;
            }
            else if(rightCenterRect.adjusted(-adjustSize, -adjustSize, adjustSize, adjustSize).contains(hoverPos))
            {
                    //RightCenter
                    this->setCursor(Qt::SizeHorCursor);
                    myHoverPosId = RightCenter;
            }
            else
            {
                resizeHandlePressed = false;
            }
        }
        else
        {
            if(bottomCenterRect.adjusted(-adjustSize, -adjustSize, adjustSize, adjustSize).contains(hoverPos))
            {
                //BottomCenter
                this->setCursor(Qt::SizeVerCursor);
                myHoverPosId = BottomCenter;
            }
            else
                resizeHandlePressed = false;
        }
    }
    QGraphicsItem::hoverMoveEvent(event);
}
void BaseItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);

}

void BaseItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(resizeHandlePressed)
    {
        QPointF   myMovePos = event->pos();//获得的是移动点(item coordinates)
        QRectF rect = this->rect();

        switch(myHoverPosId)
        {
        case TopLeft:
            if(rect.right() - myMovePos.x() >= 0 &&
                    rect.bottom() - myMovePos.y() >= 0)
                rect.setTopLeft(myMovePos);
            break;
        case TopCenter:
            if(rect.bottom() - myMovePos.y() >= 0)
                rect.setTop(myMovePos.y());
            break;
        case TopRight:
            if(rect.bottom() - myMovePos.y() >= 0 &&
                    myMovePos.x() - rect.left() >= 0)
                rect.setTopRight(myMovePos);
            break;
        case LeftCenter:
            if(rect.right() - myMovePos.x() >= 0)
                rect.setLeft(myMovePos.x());
            else
                rect.setLeft(rect.right());
            break;
        case RightCenter:
            if(myMovePos.x() - rect.left() >= 0)
                rect.setRight(myMovePos.x());
            else
                rect.setRight(rect.left());
            break;
        case BottomLeft:
            if(rect.right() - myMovePos.x() >= 0 &&
                    myMovePos.y() - rect.top() >=0 )
                rect.setBottomLeft(myMovePos);
            break;
        case BottomCenter:
            if(myMovePos.y() - rect.top() >= 0)
                rect.setBottom(myMovePos.y());
            break;
        case BottomRight:
            if(myMovePos.y() - rect.top() >= 0 &&
                    myMovePos.x() - rect.left() >= 0)
                rect.setBottomRight(myMovePos);
            break;
        default:
            break;
        }
        setRect(rect);
        update();
    }
    else
        QGraphicsItem::mouseMoveEvent(event);
}
void BaseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

void BaseItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

    PropertiesDialog dialog(this);
    dialog.exec();

//    DeviceInfo* devicewindow = new DeviceInfo(this);
//    devicewindow->show();
}

void BaseItem::paintShape(QPainter *, const QStyleOptionGraphicsItem *,
                         QWidget *)
{

}
void BaseItem::paintBoundingRect(QPainter *painter, const QStyleOptionGraphicsItem *,
                  QWidget *widget)
{
        Q_UNUSED(widget);
        QRectF rect = this->rect();//return myRect;

        //绘制外围的矩形框
        painter->setPen(QPen(Qt::green, 1, Qt::DashLine));
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(rect);

        QPointF topLeft, topCenter, topRight, leftCenter, bottomLeft, bottomCenter, bottomRight, rightCenter;
        topLeft = rect.topLeft();//(0,0)
        topCenter = QPointF(rect.center().x(), rect.top());
        topRight = rect.topRight();
        leftCenter = QPointF(rect.left(), rect.center().y());
        rightCenter = QPointF(rect.right(), rect.center().y());
        bottomLeft = rect.bottomLeft();
        bottomCenter = QPointF(rect.center().x(), rect.bottom());
        bottomRight = rect.bottomRight();

        //绘制拖动的手柄---小矩形框
        painter->setPen(QPen(Qt::black));
        painter->setBrush(Qt::green);

        if(rect.width() >= 18 && rect.height() >= 18)
        {
            painter->drawRect(QRectF((topLeft - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                     QSizeF(resizeHandleWidth, resizeHandleWidth) ));
            painter->drawRect(QRectF((topCenter - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                     QSizeF(resizeHandleWidth, resizeHandleWidth) ));
            painter->drawRect(QRectF((topRight - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                     QSizeF(resizeHandleWidth, resizeHandleWidth) ));
            painter->drawRect(QRectF((leftCenter - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                     QSizeF(resizeHandleWidth, resizeHandleWidth) ));
            painter->drawRect(QRectF((rightCenter - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                     QSizeF(resizeHandleWidth, resizeHandleWidth) ));
            painter->drawRect(QRectF((bottomLeft - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                     QSizeF(resizeHandleWidth, resizeHandleWidth) ));
            painter->drawRect(QRectF((bottomCenter - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                     QSizeF(resizeHandleWidth, resizeHandleWidth) ));
            painter->drawRect(QRectF((bottomRight - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                     QSizeF(resizeHandleWidth, resizeHandleWidth) ));
        }
        else if ((rect.width() > 0  && rect.width() < 18)
                 || (rect.height() > 0 && rect.height() < 18))
        {

            painter->drawRect(QRectF((topCenter - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                     QSizeF(resizeHandleWidth, resizeHandleWidth) ));
            painter->drawRect(QRectF((leftCenter - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                     QSizeF(resizeHandleWidth, resizeHandleWidth) ));
            painter->drawRect(QRectF((rightCenter - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                     QSizeF(resizeHandleWidth, resizeHandleWidth) ));
            painter->drawRect(QRectF((bottomCenter - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                     QSizeF(resizeHandleWidth, resizeHandleWidth) ));
        }
        else if (rect.width() == 0 && rect.height() != 0)
        {
            painter->drawRect(QRectF((topLeft - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                     QSizeF(resizeHandleWidth, resizeHandleWidth) ));
            if(myHoverPosId == LeftCenter)
                painter->drawRect(QRectF((leftCenter - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                     QSizeF(resizeHandleWidth, resizeHandleWidth) ));
            else
                painter->drawRect(QRectF((rightCenter - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                     QSizeF(resizeHandleWidth, resizeHandleWidth) ));
            painter->drawRect(QRectF((bottomRight - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                     QSizeF(resizeHandleWidth, resizeHandleWidth) ));
        }
        else if (rect.height() == 0 && rect.width() != 0)
        {
            painter->drawRect(QRectF((topLeft - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                     QSizeF(resizeHandleWidth, resizeHandleWidth) ));
            painter->drawRect(QRectF((topCenter/*bottomCenter*/ - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                     QSizeF(resizeHandleWidth, resizeHandleWidth) ));
            painter->drawRect(QRectF((bottomRight - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                     QSizeF(resizeHandleWidth, resizeHandleWidth) ));
        }
        else
        {
            painter->drawRect(QRectF((topLeft - QPointF(resizeHandleWidth/2, resizeHandleWidth/2)),
                                     QSizeF(resizeHandleWidth, resizeHandleWidth) ));
        }

}

void BaseItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if (!isSelected() && scene())
    {
        scene()->clearSelection();
        setSelected(true);
    }

//    QMenu contextMenu;
//    QAction *delAction = contextMenu.addAction("Delete");
//    QAction *newAction = contextMenu.addAction("New");
//    QAction *growAction = contextMenu.addAction("Grow");
//    QAction *shrinkAction = contextMenu.addAction("Shrink");

//    QAction *selectedAction = contextMenu.exec(event->screenPos());

//    if (selectedAction == delAction)
//       emit delActionTriggered(this);
//    else if (selectedAction == newAction)
//        duplicateSelectedItems(scene());
//    else if (selectedAction == growAction)
//        growSelectedItems(scene());
//    else if (selectedAction == shrinkAction)
//        shrinkSelectedItems(scene());

}
void BaseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget)
{
    Q_UNUSED(widget);
    paintShape(painter, option, widget);
    if(isSelected())
        paintBoundingRect(painter, option, widget);
}
int BaseItem::type() const
{
    return Type;
}

void BaseItem::mySetIsMemberOfGroup(bool enabled)
{
    IsMemberOfGroup = enabled;
    if (!qgraphicsitem_cast<GroupItem *>(this))
    {
        foreach (QGraphicsItem *child, childItems())
        {
            if(BaseItem *baseChild = dynamic_cast<BaseItem *>(child))
                baseChild->mySetIsMemberOfGroup(enabled);
        }
    }
}

GroupItem* BaseItem::myGroup() const
{
    if(!IsMemberOfGroup)
        return 0;

    BaseItem *item = const_cast<BaseItem *>(this);
    QGraphicsItem *parent = item->parentItem();
    if(GroupItem *group = qgraphicsitem_cast<GroupItem *>(parent))
        return group;

    return 0;
}

void BaseItem::mySetGroup(GroupItem *group)
{
    if(!group)
    {
        if(GroupItem *group = this->myGroup())
            group->removeFromGroup(this);
    }
    else
    {
        group->addToGroup(this);
    }
}

QDataStream &operator <<(QDataStream &out, const BaseItem &baseitem)
{
    out << baseitem.type()
        << baseitem.rect()
        << baseitem.pos()
        << baseitem.pen().color()<< qint8(baseitem.pen().width())
        << baseitem.brush().color();
    return out;//78 bytes
}

QDataStream &operator >>(QDataStream &in, BaseItem &baseitem)
{
   qint32 itemType;
   QRectF myRect;
   QPointF mypos;
   QColor myPenColor;
   qint8 myPenWidth;
   QColor myBrushColor;
   in >> itemType >> myRect >> mypos >> myPenColor >> myPenWidth >> myBrushColor;
   baseitem.setRect(myRect);
   baseitem.setPen(myPenColor);
   baseitem.setPen(myPenWidth);
   baseitem.setBrush(QBrush(myBrushColor));
   baseitem.setPos(mypos);
   return in;

}
