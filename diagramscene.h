#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QObject>
#include "rectitem.h"
#include "lineitem.h"
#include "textitem.h"
#include "mainwindow.h"
#include "ellipseitem.h"


#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QPaintEvent;
class QPointF;
class QWidget;
QT_END_NAMESPACE



class DiagramScene : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode { InsertCircleItem , InsertRectItem , InsertLineItem , InsertTextItem, MoveItem};
    DiagramScene(QObject *parent = 0);
    void setMode(Mode mode );
    void setStartPoint(QPointF point);
    void setItemColor(const QColor &color);
    void setLineColor(const QColor &color);
    void setLineWidth(const int width);
    void setTextColor(const QColor &color);
    void setFont(const QFont &font);
    QFont font() const
        { return myTextFont; }
    GroupItem *myCreatItemGroup(const QList<QGraphicsItem *> &items);
    void myDestroyItemGroup(GroupItem *group);
public slots:
    void cut();
    void copy();
    void paste();


    bool writeFile(QFile &file);
    bool readFile(QFile &file);
    void editorLostFocus(TextItem *item);
signals:
    void itemSelected(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    bool isItemChange(int type);

    QColor myItemColor;
    QColor myLineColor;
    int myLineWidth;
    EllipseItem *circle;
    RectItem *Rect;
    LineItem *line;
    TextItem *textItem;
    QPointF startPoint;
    Mode myMode;
    QColor myTextColor;
    QFont myTextFont;
//signals:
    //void itemMoved(DiagramItem *movedItem,const QPointF &movedFromPosition);

};
#endif // DIAGRAMSCENE_H