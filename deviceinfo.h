#ifndef DEVICEINFO_H
#define DEVICEINFO_H

//#include "ui_deviceinfo.h"
#include <QMainWindow>
#include "pixitem.h"
#include "deviceparam.h"
#include <QtWidgets>

class BaseItem;
class PixItem;
class QColor;
/*
namespace Ui {
class DeviceInfo;
}
*/
class DeviceInfo : public QMainWindow
{
    Q_OBJECT

public:
    QString myName;
    QList<DeviceParam> myParamList;
    DeviceParam myParam;
    int myRowCnt;

    QStringList* itemValueList;
    QTimer *timer;
    QPixmap *pixmap1;
    QPixmap *pixmap2;

    QTableWidget* tableWidget;
    QComboBox* comboBox;
    QTableWidgetItem* tableItem;
    QListWidgetItem* listItem;
    QToolBox* toolBox;
    QPushButton* pushButton_save;
    QPushButton* pushButton_add;
    QLineEdit* lineEdit_code;
    QLineEdit* lineEdit_name;
    QListWidget* listWidget;

public:
    explicit DeviceInfo(PixItem *pixItem, QWidget *parent = 0);
    ~DeviceInfo();

public slots:
    void setCode(QString code);
    void setName(QString name);
    void setTable(QList<DeviceParam> paramList);
    void setDevice();
    void modeChanged(QString mode);

private:
    //Ui::DeviceInfo *ui;

    BaseItem *myItem;
    PixItem *mypixItem;

private slots:
    void plotSlot(int i,int j);
    void addParam();
    void save();
    void statusConfirm(int i,int j);
    void onHeaderClicked(int i);
    void updateData();

};

#endif // DEVICEINFO_H
