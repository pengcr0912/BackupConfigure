#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include "ui_deviceinfo.h"
#include <QMainWindow>
#include "pixitem.h"
class BaseItem;
class PixItem;
class QColor;

namespace Ui {
class deviceinfo;
}

class deviceinfo : public QMainWindow
{
    Q_OBJECT

public:
    QString myName;
    QList<DeviceParam> myParamList;
    DeviceParam myParam;
    int myRowCnt;
    QString insertline;

public:
    explicit deviceinfo(BaseItem *baseItem, QWidget *parent = 0);
    explicit deviceinfo(PixItem *pixItem, QWidget *parent = 0);
    ~deviceinfo();

public slots:
    void setCode(QString deviceCode);
    void setName(QString deviceName);
    void setTable(QList<DeviceParam> paramList);

private:
    Ui::deviceinfo *ui;

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
