#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include "ui_deviceinfo.h"
#include <QMainWindow>
class BaseItem;
class QColor;

namespace Ui {
class deviceinfo;
}

class deviceinfo : public QMainWindow
{
    Q_OBJECT

public:
    explicit deviceinfo(BaseItem *baseItem, QWidget *parent = 0);
    ~deviceinfo();

private:
    Ui::deviceinfo *ui;

    QTableWidgetItem* tableItem;
    QListWidgetItem* listItem;

    BaseItem *myItem;

private slots:
    void plotSlot(int i,int j);
    void statusConfirm(int i,int j);

};

#endif // DEVICEINFO_H
