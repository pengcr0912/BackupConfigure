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

int PixItem::type() const
{
    return Type;
}

void PixItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

//    PropertiesDialog dialog(this);
//    dialog.exec();

    DeviceInfo* deviceWindow = new DeviceInfo(this);
    deviceWindow->setCode(deviceCode);
    deviceWindow->setName(deviceName);
    deviceWindow->setTable(deviceParamList);
    deviceWindow->show();
}
