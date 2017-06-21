#include "itemmimedata.h"
#include "rectitem.h"
#include "ellipseitem.h"

ItemMimeData::ItemMimeData(const QGraphicsScene *scene,
                           const QList<QGraphicsItem*> selectItems)
{
    myScene = scene;
    mySelectItems = selectItems;
    myFormats << "ByteArray";
}

QVariant ItemMimeData::retrieveData(const QString &format,
                                    QVariant::Type preferredType) const
{
   if(format == "ByteArray")
   {
       return toMyFormat(mySelectItems);
   }
   else
       QMimeData::retrieveData(format , preferredType);
}
QVariant ItemMimeData::toMyFormat( const QList<QGraphicsItem*> selectItems ) const
{//¹Ø¼üÔÚÕâ£º
    QString result;
    foreach (QGraphicsItem* item, selectItems)
    {
        switch (item->type())
        case RectItem::Type:
        {

        }
    }
    return result;
}
