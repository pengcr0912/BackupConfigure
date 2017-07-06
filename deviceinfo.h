#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include "ui_deviceinfo.h"
#include <QMainWindow>
#include "pixitem.h"

class BaseItem;
class PixItem;
class QColor;

namespace Ui {
class DeviceInfo;
}

class DeviceInfo : public QMainWindow
{
    Q_OBJECT

public:
    QString myName;
    QList<DeviceParam> myParamList;
    DeviceParam myParam;
    int myRowCnt;

public:
    explicit DeviceInfo(BaseItem *baseItem, QWidget *parent = 0);
    explicit DeviceInfo(PixItem *pixItem, QWidget *parent = 0);
    ~DeviceInfo();

public slots:
    void setCode(QString code);
    void setName(QString name);
    void setTable(QList<DeviceParam> paramList);

private:
    Ui::DeviceInfo *ui;

    QTableWidgetItem* tableItem;
    QListWidgetItem* listItem;

    BaseItem *myItem;
    PixItem *mypixItem;

private slots:
    void plotSlot(int i,int j);
    void deleteParam();
    void setConfirm();
    void addTable();
//    void statusConfirm(int i,int j);

};

#endif // DEVICEINFO_H
