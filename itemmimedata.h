#ifndef ITEMMIMEDATA_H
#define ITEMMIMEDATA_H

#include <QMimeData>
#include <QGraphicsItem>
#include <QGraphicsScene>



class ItemMimeData : public QMimeData
{
public:
    ItemMimeData(const QGraphicsScene *scene,
        const QList<QGraphicsItem*> selectItems);
    const QGraphicsScene *scene() const { return myScene;}
    QList<QGraphicsItem*> selectItems() const {return mySelectItems;}
    QStringList formats() const {return myFormats;}
protected:
    QVariant retrieveData(const QString &format, QVariant::Type preferredType) const;
private:
    QVariant toMyFormat(const QList<QGraphicsItem*> selectItems ) const;

    const QGraphicsScene *myScene;
    QList<QGraphicsItem*> mySelectItems;
    QStringList myFormats;

};

#endif // ITEMMIMEDATA_H
