#ifndef PIXITEM_H
#define PIXITEM_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPainter>

struct DeviceParam
{
    QString paramName;
    QString paramMin;
    QString paramMax;
    QString paramValue;
};

class PixItem : public QGraphicsItem
{

public:
    enum { Type = QGraphicsItem::UserType + 7 };
    PixItem(QPixmap *pixmap, QGraphicsItem* parent = 0);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int type() const;

//private:
    QPixmap pix;     		//作为图元显示的图片
    QString deviceCode;
    QString deviceName;
    QList<DeviceParam> deviceParamList;


/*
protected:
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent * event);
//    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);*/
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
};


inline QDataStream &operator << (QDataStream &out, const QList<DeviceParam> &deviceParamList)
{
    out << deviceParamList.count();
    for(int i=0; i<deviceParamList.count(); i++)
    {
        out << deviceParamList.at(i).paramName;
        out << deviceParamList.at(i).paramMin;
        out << deviceParamList.at(i).paramMax;
    }
    return out;
}

inline QDataStream &operator >> (QDataStream &in, QList<DeviceParam> &deviceParamList)
{
    int count;
    DeviceParam deviceParamTemp;
    in >> count;
    for(int i=0; i<count; i++)
    {
        in >> deviceParamTemp.paramName;
        in >> deviceParamTemp.paramMin;
        in >> deviceParamTemp.paramMax;
        deviceParamList.append(deviceParamTemp);
    }
    return in;
}

#endif // PIXITEM_H
