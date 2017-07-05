#ifndef PIXITEM_H
#define PIXITEM_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPainter>
#include <QtSql/QSqlDatabase>
#include <QStringList>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

struct DeviceParam
{
    QString paramName;
    QString paramMin;
    QString paramMax;
    QString paramValue;
};
//class deviceinfo;

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


inline QDataStream &operator << (QDataStream &out, const QList<DeviceParam> &deviceParam)
{
    out << deviceParam.count();
    for(int i=0; i<deviceParam.count(); i++)
    {
        out << deviceParam.at(i).paramName;
        out << deviceParam.at(i).paramMin;
        out << deviceParam.at(i).paramMax;
    }
    return out;
}

inline QDataStream &operator >> (QDataStream &in, QList<DeviceParam> &deviceParam)
{
    int count;
    DeviceParam deviceParamtemp;
    in >> count;
    for(int i=0; i<count; i++)
    {
        in >> deviceParamtemp.paramName;
        in >> deviceParamtemp.paramMin;
        in >> deviceParamtemp.paramMax;
        deviceParam.append(deviceParamtemp);
    }
    return in;
}

#endif // PIXITEM_H
