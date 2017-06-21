#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include "ui_deviceinfo.h"
#include <QMainWindow>

namespace Ui {
class deviceinfo;
}

class deviceinfo : public QMainWindow
{
    Q_OBJECT

public:
    explicit deviceinfo(QWidget *parent = 0);
    ~deviceinfo();

private:
    Ui::deviceinfo *ui;

    QTableWidgetItem *item;

private slots:
    void plotSlot(int i,int j);
    void confirm(int i,int j);

};

#endif // DEVICEINFO_H
