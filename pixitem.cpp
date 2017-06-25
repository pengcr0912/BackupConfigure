#include "pixitem.h"
#include <QGraphicsSceneHoverEvent>

PixItem::PixItem(QPixmap *pixmap, QGraphicsItem* parent)
         :QGraphicsItem(parent)
{
    pix = *pixmap;

    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    setAcceptHoverEvents(true);
}

QRectF PixItem::boundingRect() const
{
    return QRectF(-2-pix.width()/2,-2-pix.height()/2,pix.width()+4, pix.height()+4);
}

void PixItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget)
{
    painter->drawPixmap(-pix.width()/2,-pix.height()/2,pix);
}


void PixItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    PixItem *pixItem = dynamic_cast<PixItem *>(this);
    if(!pixItem)
        return ;

//    setCursor(Qt::SizeAllCursor);
    if(isSelected())
    {
        QPointF topLeft, topCenter, topRight, leftCenter, bottomLeft, bottomCenter, bottomRight, rightCenter;
        QRectF topLeftRect, topCenterRect, topRightRect, leftCenterRect, rightCenterRect, bottomLeftRect, bottomCenterRect, bottomRightRect;
/*
        QRectF rect = pixItem->rect();//return myRect；

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
*/
        QPointF hoverPos = event->pos();//返回的item中的坐标
        int adjustSize = 0.5;

/*        resizeHandlePressed = true; //初始为真
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
        }*/
    }
    QGraphicsItem::hoverMoveEvent(event);
}


void PixItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);

}

void PixItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
/*    if(resizeHandlePressed)
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
    else*/
        QGraphicsItem::mouseMoveEvent(event);
}
void PixItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

