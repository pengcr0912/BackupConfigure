#include <QtGui>
#include <QWidget>
#include <QGraphicsSceneMouseEvent>

#include "diagramscene.h"
#include "mainwindow.h"
#include "groupitem.h"

DiagramScene::DiagramScene(QObject *parent)
    :QGraphicsScene(parent)
{
    myMode =  MoveItem ;
    Rect = NULL;
    circle = NULL;
    line = NULL;
    textItem = NULL;
    arrow = NULL;
    myTextColor = Qt::black;
    myLineWidth = 2;
    numDegrees=1;
    arrowType=1;

    pixmap = new  QPixmap("/users/hyn/images/deviceItem.jpg");
}
void DiagramScene::setMode(Mode mode )//区分单次点击生成还是按起始和终点生成
{
    myMode = mode;
    if(mode == MoveItem)
        views().first()->setDragMode(QGraphicsView::RubberBandDrag);
    else
        views().first()->setDragMode(QGraphicsView::NoDrag);
}
void DiagramScene::setStartPoint(QPointF point)
{
    startPoint = point;
}

void DiagramScene::setItemColor(const QColor &color)
{
//    if(myItemColor == color)
//        return ;
    myItemColor = color;
//    if (isItemChange(BaseItem::Type))//有点问题
    if(!selectedItems().isEmpty())
    {
        foreach(QGraphicsItem *item, selectedItems())
        {
            if(BaseItem *baseItem = dynamic_cast<BaseItem *>(item))
                baseItem->setBrush(myItemColor);
        }
    }
}

void DiagramScene::setLineColor(const QColor &color)
{
    myLineColor = color;
    if(!selectedItems().isEmpty())
    {
        foreach(QGraphicsItem *item, selectedItems())
        {
            if(BaseItem *baseItem = dynamic_cast<BaseItem *>(item))
                baseItem->setPen(myLineColor);
            else if(LineItem *lineItem = dynamic_cast<LineItem *>(item))
            {
                myPen.setBrush(myLineColor);
                lineItem->setPen(myPen);
            }
            else if(ArrowItem *arrowItem = dynamic_cast<ArrowItem *>(item))
            {
                myPen.setBrush(myLineColor);
                arrowItem->setPen(myPen);
            }
        }
    }
}

void DiagramScene::setLineWidth(const int width)
{
    myLineWidth = width;
    if(!selectedItems().isEmpty())
    {
        foreach(QGraphicsItem *item, selectedItems())
        {
            if(BaseItem *baseItem = dynamic_cast<BaseItem *>(item))
                baseItem->setPen(myLineWidth);
            else if(LineItem *lineItem = dynamic_cast<LineItem *>(item))
            {
                myPen.setWidth(myLineWidth);
                lineItem->setPen(myPen);
            }
            else if(ArrowItem *arrowItem = dynamic_cast<ArrowItem *>(item))
            {
                myPen.setWidth(myLineWidth);
                arrowItem->setPen(myPen);
                arrowItem->setData(arrowItem->line().p1(),arrowItem->line().p2(),myLineWidth,arrowItem->iType);//只改变线宽会导致箭头大小不变，需根据新的线宽重绘箭头
            }
        }
    }
}
void DiagramScene::setTextColor(const QColor &color)
{
    myTextColor = color;
    if(isItemChange(TextItem::Type))
    {
        TextItem *textItem = (TextItem *)(selectedItems().first());
        if(textItem)
            textItem->setDefaultTextColor(color);
    }
}
bool DiagramScene::writeFile(QFile &file)
{
    //QPixmap pixmapSaved("/users/hyn/images/deviceItem.jpg");
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_3);
    //    out << quint32(MagicNumber);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    foreach(QGraphicsItem* item, items())
    {
        if(BaseItem *baseItem = dynamic_cast<BaseItem *>(item))
        {
            out << *baseItem;
            out << baseItem->opacity();
        }
        else if(LineItem *lineItem = dynamic_cast<LineItem *>(item))
        {
            out << lineItem->type()
                //<< lineItem->boundingRect()
                << lineItem->pos()
                << lineItem->line()
                << lineItem->pen();
//                << lineItem->pen().color()
//                << lineItem->pen().width();
        }
        else if(TextItem *textItem = dynamic_cast<TextItem *>(item))
        {
            out << textItem->type()
                << textItem->pos()
                << textItem->toPlainText()
                << textItem->font();
        }
        else if(PixItem *pixItem = dynamic_cast<PixItem *>(item))
        {
            out << pixItem->type()
                << pixItem->pos()
                //<< pixItem->pix//pix此时可能为报错状态，保存后不是初始状态，且此时pix可能正在重绘，不能操作
                //<< pixmapSaved
                << pixItem->deviceCode;
//                << pixItem->deviceName
//                << pixItem->deviceParamList;
        }
        else if(ArrowItem *arrowItem = dynamic_cast<ArrowItem *>(item))
        {
            out << arrowItem->type()
                << arrowItem->pos()
                << arrowItem->line()
                << arrowItem->pen()
                << arrowItem->iType;
        }
    }

    QApplication::restoreOverrideCursor();
    return true;
}
bool DiagramScene::readFile(QFile &file)
{
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_3);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    clear();
    while(!in.atEnd())
    {
        qint32 itemType;
        QRectF myRect;
        QPointF mypos;
        QColor myPenColor;
        qint8 myPenWidth;
        QColor myBrushColor;

        double opacity;

        QLineF myLine;
        //QColor linePenColor;
        //qint8 linePenWidth;
        QPen linePen;

        QLineF myArrow;
        QPen arrowPen;
        int myType;

        QFont myFont;
        QString myString;

        //QPixmap myPixmap;
        //QList<DeviceParam> myDeviceParamList;

        in >> itemType;
        if(itemType == (QGraphicsItem::UserType + 3) || itemType == (QGraphicsItem::UserType + 4))
        {
            in >> myRect >> mypos >> myPenColor >> myPenWidth >> myBrushColor >> opacity;

            if(in.status() != QDataStream::Ok)
            {
                return false;
            }
            switch(itemType)
            {
            case QGraphicsItem::UserType + 3://rectItem
            {
                RectItem *rectItem = new RectItem;
                addItem(rectItem);
                rectItem->setRect(myRect);
                rectItem->setPen(myPenColor);
                rectItem->setPen(myPenWidth);
                rectItem->setBrush(QBrush(myBrushColor));
                rectItem->setPos(mypos);
                rectItem->setOpacity(opacity);
                break;
            }
            case QGraphicsItem::UserType + 4:
            {
                EllipseItem *ellipseItem = new EllipseItem;
                addItem(ellipseItem);
                ellipseItem->setRect(myRect);
                ellipseItem->setPen(myPenColor);
                ellipseItem->setPen(myPenWidth);
                ellipseItem->setBrush(QBrush(myBrushColor));
                ellipseItem->setPos(mypos);
                ellipseItem->setOpacity(opacity);
                break;
            }
            default:
                ;
            }
        }
        else if(itemType == QGraphicsItem::UserType + 5)
        {
            in >> mypos >> myString >> myFont;

            if(in.status() != QDataStream::Ok)
            {
                return false;
            }
            TextItem *textItem = new TextItem;
            textItem->setPos(mypos);
            textItem->setPlainText(myString);
            textItem->setFont(myFont);
            addItem(textItem);
        }
        else if(itemType == QGraphicsItem::UserType + 6)
        {
//            in >> myRect >> myPen;
            in >> mypos >> myLine >> linePen;
            //>> linePenColor >> linePenWidth;

            if(in.status() != QDataStream::Ok)
            {
                return false;
            }
            LineItem *lineItem = new LineItem;
//            linePen.setColor(linePenColor);
//            linePen.setWidth(linePenWidth);
//            lineItem->setLine(myRect.x(),myRect.y(),myRect.x()+myRect.width(),myRect.y()+myRect.height());
            lineItem->setPos(mypos);
            lineItem->setLine(myLine);
            lineItem->setPen(linePen);
            addItem(lineItem);
        }
        else if(itemType == QGraphicsItem::UserType + 7)//
        {
//            in >> mypos >> myPixmap >> code >> name >> myDeviceParamList;
//            in >> mypos >> myPixmap >> code;
            in >> mypos >> code;

            if(in.status() != QDataStream::Ok)
            {
                return false;
            }
            //PixItem *pixItem = new PixItem(&myPixmap);
            PixItem *pixItem = new PixItem(pixmap);
            pixItem->setPos(mypos);
            pixItem->deviceCode = code;
//            pixItem->deviceName = name;
//            pixItem->deviceParamList = myDeviceParamList;
            addItem(pixItem);
            pixItem->statusCheck();
        }
        else if(itemType == QGraphicsItem::UserType + 8)
        {
            in >> mypos >> myArrow >> arrowPen >> myType;
            if(in.status() != QDataStream::Ok)
            {
                return false;
            }
            ArrowItem *arrowItem = new ArrowItem;
            arrowItem->setPos(mypos);
            arrowItem->setLine(myArrow);
            arrowItem->setPen(arrowPen);
            arrowItem->setData(myArrow.p1(),myArrow.p2(),arrowPen.width(),myType);
            addItem(arrowItem);
        }
    }

    QList<QGraphicsItem *>  myitemlist = items();
    int itemcnt=myitemlist.count();
    int pixcnt=0;
    for(int i=0;i<itemcnt;i++)
    {
        if(myitemlist.at(i)->type() == PixItem::Type)
        {
            mypixlist.insert(pixcnt, dynamic_cast<PixItem *> (myitemlist.at(i)));
            pixcnt++;
        }
    }

    QApplication::restoreOverrideCursor();
    return true;
}
bool DiagramScene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems())
    {
        if(type == BaseItem::Type)
        {
            if (item->type() != RectItem::Type && item->type() != EllipseItem::Type )
                return false;
        }
        else if (item->type() == type)
        {
            return true;
        }
    }
    return false;
}
void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

    if (mouseEvent->button() != Qt::LeftButton)
        return;
    switch (myMode)
    {
//        setStartPoint(QPointF(mouseEvent->scenePos()));
    case InsertCircleItem:
    {
         circle = new EllipseItem(QRectF(QPointF(0,0), QSizeF(0,0)));
         if(circle)
         {
             setStartPoint(QPointF(mouseEvent->scenePos()));
             circle->setPos(mouseEvent->scenePos());//设置起点
             circle->setPen(QPen(Qt::DotLine));
             addItem(circle);
             //circle->setBrush(myItemColor);
         }
         break;
    }
    case InsertRectItem:
    {
         Rect = new RectItem(QRectF(QPointF(0,0), QSizeF(0,0)));
         if(Rect)
         {
             setStartPoint(QPointF(mouseEvent->scenePos()));
             addItem(Rect);
             Rect->setPos(startPoint);//设置起点
             Rect->setPen(QPen(Qt::DotLine));
             //Rect->setBrush(myItemColor);
         }
         break;
    }
    case InsertLineItem:
    {
         line = new LineItem;
         if(line)
         {
             setStartPoint(QPointF(mouseEvent->scenePos()));
             line->setPen(QPen(Qt::DashLine));
             addItem(line);
         }
         break;
    }
    case InsertArrowItem:
    {
         arrow = new ArrowItem;
         if(arrow)
         {
             setStartPoint(QPointF(mouseEvent->scenePos()));
             arrow->setPen(QPen(Qt::DashLine));
             addItem(arrow);
         }
         break;
    }
    case InsertTextItem:
    {
         textItem = new TextItem;
         if(textItem)
         {
             textItem->setFont(myTextFont);
             textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
             textItem->setZValue(1000.0);
             connect(textItem, SIGNAL(lostFocus(TextItem *)),//在focusOutEvent中发射
                     this, SLOT(editorLostFocus(TextItem *)));
             connect(textItem, SIGNAL(selectedChange(QGraphicsItem*)),//在itemChange中发射
                     this, SIGNAL(itemSelected(QGraphicsItem*)));
             addItem(textItem);
             textItem->setDefaultTextColor(myTextColor);
             textItem->setPos(mouseEvent->scenePos());
         }
         setMode(MoveItem);
         break;
    }
    case InsertPixItem:
    {
         pixItem = new PixItem(pixmap);
         if(pixItem)
         {
             addItem(pixItem);
             pixItem->setPos(mouseEvent->scenePos());
             pixItem->setOpacity(0.2);
         }
         //        emit textInserted(textItem);//主要是想改变场景的模式
         setMode(MoveItem);
         break;
    }

    default:
        ;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}
void DiagramScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    switch (myMode)
    {
    case InsertCircleItem:
    {
        if(circle)
            circle->setRect(QRectF(QPointF(0,0), mouseEvent->scenePos() - startPoint).toRect().normalized());
        break;
    }
    case InsertRectItem:
    {
        if(Rect)
            Rect->setRect(QRectF(QPointF(0,0) , mouseEvent->scenePos() - startPoint).normalized());
        break;
    }
    case InsertLineItem:
    {
        if(line)
            line->setLine(QLineF(startPoint , mouseEvent->scenePos()));
        break;
    }
    case InsertArrowItem:
    {
        if(arrow)
        {
            arrow->setLine(QLineF(startPoint , mouseEvent->scenePos()));
            //arrow->setData(startPoint,QPointF(mouseEvent->scenePos()),myLineWidth,arrowType);
            //update();
        }
        break;
    }
    case MoveItem:
    {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
        break;
    }

    default:
        ;
    }
}
void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    switch (myMode)
    {
    case InsertCircleItem:
        if(circle)
        {
            circle->setPen(QPen(myLineColor, myLineWidth, Qt::SolidLine));
            circle->setBrush(myItemColor);
            circle = 0;
            setMode(MoveItem);
        }
        break;
    case InsertRectItem:
        if(Rect)
        {
            Rect->setPen(QPen(myLineColor, myLineWidth, Qt::SolidLine));
            Rect->setBrush(myItemColor);
            Rect = 0;
            setMode(MoveItem);
        }
        break;
    case InsertLineItem:
        if(line)
        {
            line->setPen(QPen(myLineColor, myLineWidth, Qt::SolidLine));
            setMode(MoveItem);
            line = 0;
        }
        break;
    case InsertArrowItem:
        if(arrow)
        {
            arrow->setPen(QPen(myLineColor, myLineWidth, Qt::SolidLine));
            arrow->setData(startPoint,QPointF(mouseEvent->scenePos()),myLineWidth,arrowType);
            update();
            setMode(MoveItem);
            arrow = 0;
        }
        break;
    default:
           ;
    }
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void DiagramScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    numDegrees = numDegrees + (event->delta()/120)*0.25;
    numDegrees = (numDegrees > 1.5) ? 1.5 : numDegrees;
    numDegrees = (numDegrees < 0.5) ? 0.5 : numDegrees;
    wheelChanged(numDegrees);
    //qDebug() << numDegrees;
}

/*DiagramScene::itemMoved(DiagramItem *movedItem, const QPointF &movedFromPosition)
{

}*/
void DiagramScene::editorLostFocus(TextItem *item)
{
    if(item)
    {
        QTextCursor cursor = item->textCursor();
        cursor.clearSelection();
        item->setTextCursor(cursor);

        if (item->toPlainText().isEmpty())
        {
            removeItem(item);
            item->deleteLater();
        }
    }
}

void DiagramScene::cut()
{
    QList<QGraphicsItem *> items = selectedItems();
    if(!items.isEmpty())
    {
        copy();
        clearSelection();
    }
    qDeleteAll(items);

}
void DiagramScene::copy()
{
    QList<QGraphicsItem *> items = selectedItems();
    QString str;
    for(int i = 0; i != items.count(); i++)
    {
        if(i > 0)
            str += "\n";
        if(BaseItem *baseItem = dynamic_cast<BaseItem *>(items.at(i)))
        {
            str += QString("item %1 %2 %3 %4 %5 %6 %7 %8 %9 %10")
                    .arg(QString::number(baseItem->type()))
                    .arg(QString::number(baseItem->rect().topLeft().x()))
                    .arg(QString::number(baseItem->rect().topLeft().y()))
                    .arg(QString::number(baseItem->rect().width()))
                    .arg(QString::number(baseItem->rect().height()))
                    .arg(baseItem->pen().color().name())
                    .arg(QString::number(baseItem->pen().width()))
                    .arg(baseItem->brush().color().name())
                    .arg(QString::number(baseItem->pos().x()))
                    .arg(QString::number(baseItem->pos().y()));
        }

    }
    QApplication::clipboard()->setText(str);

}
void DiagramScene::paste()
{
    QString str = QApplication::clipboard()->text();
    QStringList strList = str.split('\n');
    for(int i = 0; i != strList.count(); i++)
    {
        QStringList itemStr = strList.at(i).split(' ');
        if (itemStr.count() >= 9 && itemStr.first() == "item")
        {
            switch(itemStr.at(1).toInt())
            {
            case QGraphicsItem::UserType + 3:
            {
                RectItem *rect = new RectItem;
                if(rect)
                {
                    rect->setRect(QRectF(QPointF(itemStr.at(2).toDouble(),itemStr.at(3).toDouble())
                                        ,QSize(itemStr.at(4).toDouble(), itemStr.at(5).toDouble())));
                    rect->setPen(QColor(itemStr.at(6)));
                    rect->setPen(int(itemStr.at(7).toInt()));
                    rect->setBrush(QBrush(QColor(itemStr.at(8))));
                    rect->setPos(itemStr.at(9).toDouble()+10.0,itemStr.at(10).toDouble()+10.0);
                    addItem(rect);
                }
                break;
            }
            case QGraphicsItem::UserType + 4:
            {
                EllipseItem *circle = new EllipseItem;
                if(circle)
                {
                    circle->setRect(QRectF(QPointF(itemStr.at(2).toDouble(),itemStr.at(3).toDouble())
                                        ,QSize(itemStr.at(4).toDouble(), itemStr.at(5).toDouble())));
                    circle->setPen(QColor(itemStr.at(6)));
                    circle->setPen(int(itemStr.at(7).toInt()));
                    circle->setBrush(QBrush(QColor(itemStr.at(8))));
                    circle->setPos(itemStr.at(9).toDouble()+10.0,itemStr.at(10).toDouble()+10.0);
                    addItem(circle);
                }
                break;
            }
            default:
                ;
            }
        }
    }
}
void DiagramScene::setFont(const QFont &font)
{
    myTextFont = font;

    if (isItemChange(TextItem::Type))//主要是判断当前的正在改变的item是否是自认为的item(也就是参数)
    {
        TextItem *item =
            qgraphicsitem_cast<TextItem *>(selectedItems().first());
        //At this point the selection can change so the first selected item might not be a DiagramTextItem
        if (item)
            item->setFont(myTextFont);
    }
}
GroupItem *DiagramScene::myCreatItemGroup(const QList<QGraphicsItem *> &items)
{
    {
        // Build a list of the first item's ancestors
        QList<QGraphicsItem *> ancestors;
        int n = 0;
        if (!items.isEmpty())
        {
            QGraphicsItem *parent = items.at(n++);
            while ((parent = parent->parentItem()))
                ancestors.append(parent);
        }

        // Find the common ancestor for all items
        QGraphicsItem *commonAncestor = 0;
        if (!ancestors.isEmpty())
        {
            while (n < items.size())
            {
                int commonIndex = -1;
                QGraphicsItem *parent = items.at(n++);
                do {
                    int index = ancestors.indexOf(parent, qMax(0, commonIndex));
                    if (index != -1)
                    {
                        commonIndex = index;
                        break;
                    }
                } while ((parent = parent->parentItem()));

                if (commonIndex == -1)
                {
                    commonAncestor = 0;
                    break;
                }
                commonAncestor = ancestors.at(commonIndex);
            }
        }

        // Create a new group at that level
        GroupItem *group = new GroupItem(commonAncestor);
        if (!commonAncestor)
        {
            addItem(group);
        }

        foreach (QGraphicsItem *item, items)
        {
            group->addToGroup(item);
        }

        return group;
    }
}
void DiagramScene::myDestroyItemGroup(GroupItem *group)
{
    //foreach (QGraphicsItem *item, group->children())
    foreach (QGraphicsItem *item, selectedItems())
    {
        group->removeFromGroup(item);
    }
    removeItem(group);
    delete group;
}
