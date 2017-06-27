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
    myTextColor = Qt::black;

}
void DiagramScene::setMode(Mode mode )
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
                lineItem->setPen(myLineColor);
        }
    }

}

void DiagramScene::setLineWidth(const int width)
{
    myLineWidth = width;
    QPen linePen;
    if(!selectedItems().isEmpty())
    {
        foreach(QGraphicsItem *item, selectedItems())
        {
            if(BaseItem *baseItem = dynamic_cast<BaseItem *>(item))
                baseItem->setPen(myLineWidth);
            else if(LineItem *lineItem = dynamic_cast<LineItem *>(item))
            {
                linePen.setWidth(myLineWidth);
                lineItem->setPen(linePen);
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

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_3);
    //    out << quint32(MagicNumber);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    foreach(QGraphicsItem* item, items())
    {
        if(BaseItem *baseItem = dynamic_cast<BaseItem *>(item))
        {
            out << *baseItem;
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
                << pixItem->pix;
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

        QLineF myLine;
        QColor linePenColor;
        qint8 linePenWidth;
        QPen linePen;

        QFont myFont;
        QString myString;

        QPixmap myPixmap;

        in >> itemType;
        if(itemType == (QGraphicsItem::UserType + 3) || itemType == (QGraphicsItem::UserType + 4))
        {
            in >> myRect >> mypos >> myPenColor >> myPenWidth >> myBrushColor;

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
            in >> mypos >> myPixmap;

            if(in.status() != QDataStream::Ok)
            {
                return false;
            }
            PixItem *pixItem = new PixItem(&myPixmap);
            pixItem->setPos(mypos);
//            pixItem->setPixmap(myPixmap);
            addItem(pixItem);

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
         }
         break;
    }
    case InsertRectItem:
         Rect = new RectItem(QRectF(QPointF(0,0), QSizeF(0,0)));
         if(Rect)
         {
             setStartPoint(QPointF(mouseEvent->scenePos()));
             addItem(Rect);
             Rect->setPos(startPoint);//设置起点
             Rect->setPen(QPen(Qt::DotLine));
         }
         break;
    case InsertLineItem:
         line = new LineItem;
         if(line)
         {
             setStartPoint(QPointF(mouseEvent->scenePos()));
             line->setPen(QPen(Qt::DashLine));
             addItem(line);
         }
         break;
    case InsertTextItem:
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
    case InsertPixItem:
         pixmap = new  QPixmap("/users/hyn/images/image.png");
         pixItem = new PixItem(pixmap);
         if(pixItem)
         {
             addItem(pixItem);
             pixItem->setPos(mouseEvent->scenePos());
         }
         //        emit textInserted(textItem);//主要是想改变场景的模式
         setMode(MoveItem);
         break;

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
        if(line)
        line->setLine(QLineF(startPoint , mouseEvent->scenePos()));
        break;
    case MoveItem:
        QGraphicsScene::mouseMoveEvent(mouseEvent);
        break;

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
            circle->setPen(QPen(Qt::black, 2, Qt::SolidLine));
            circle->setBrush(Qt::lightGray);
            circle = 0;
            setMode(MoveItem);
        }
        break;
    case InsertRectItem:
        if(Rect)
        {
            Rect->setPen(QPen(Qt::black, 2, Qt::SolidLine));
            Rect->setBrush(Qt::lightGray);
            Rect = 0;
            setMode(MoveItem);
        }
        break;
    case InsertLineItem:
        if(line)
        {
            line->setPen(QPen(Qt::black, 2, Qt::SolidLine));
            setMode(MoveItem);
            line = 0;
        }
    default:
           ;
    }
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
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
