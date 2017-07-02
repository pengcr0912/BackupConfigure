#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include "ui_deviceinfo.h"
#include <QMainWindow>
#include "pixitem.h"
class BaseItem;
class PixItem;
class QColor;
class table;

namespace Ui {
class deviceinfo;
}

class deviceinfo : public QMainWindow
{
    Q_OBJECT

public:
    QString myname;
    QList<DeviceParam> myParamList;
    DeviceParam myParam;
public:
    explicit deviceinfo(BaseItem *baseItem, QWidget *parent = 0);
    explicit deviceinfo(PixItem *pixItem, QWidget *parent = 0);
    ~deviceinfo();

public slots:
    void setid(int deviceID);
    void setname(QString devicename);
    void setTable(QList<DeviceParam> paramList);
    void getname();
    void getid();
    void addTable();

private:
    Ui::deviceinfo *ui;

    QTableWidgetItem* tableItem;
    QListWidgetItem* listItem;

    BaseItem *myItem;
    PixItem *mypixItem;
    table *mytable;

private slots:
    void plotSlot(int i,int j);
    void statusConfirm(int i,int j);

};

#endif // DEVICEINFO_H
