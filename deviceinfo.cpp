#include "deviceinfo.h"
//#include "ui_deviceinfo.h"
#include "customplotwindow.h"
#include "baseitem.h"
#include "table.h"

deviceinfo::deviceinfo(BaseItem *baseItem, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::deviceinfo)
{
    ui->setupUi(this);

    myItem = baseItem;

//    myItem->setPen(Qt::green);
//    myItem->setBrush(Qt::green);
    myItem->setBrush(Qt::blue);
    myItem->update();

    Qt::WindowFlags flags = 0;
    flags |= Qt::WindowMinimizeButtonHint;
    setWindowFlags(flags); // 设置禁止最大化

    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setRowCount(30);
    QStringList headers;
    headers << "参数名称" << "允许最小值" << "允许最大值" << "当前值";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);//自动填满控件

    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("CPU使用率"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(0, 2, new QTableWidgetItem("30%"));
    ui->tableWidget->setItem(0, 3, new QTableWidgetItem("20%"));
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("硬盘使用率"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(1, 2, new QTableWidgetItem("40%"));
    ui->tableWidget->setItem(1, 3, new QTableWidgetItem("30%"));
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("内存使用率"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(2, 2, new QTableWidgetItem("40%"));
    ui->tableWidget->setItem(2, 3, new QTableWidgetItem("45%"));
    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("网络带宽"));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(3, 2, new QTableWidgetItem("100Mbps"));
    ui->tableWidget->setItem(3, 3, new QTableWidgetItem("50Mbps"));

    ui->listWidget->setAlternatingRowColors(true);//背景颜色隔行区分

    for(int i=0;i<10;i++)
        ui->listWidget->addItem("2017-06-22 18:08:02:900 收到一条正常日志汇报");
    ui->listWidget->addItem("2017-06-22 18:08:02:900 收到一条异常日志汇报");

    listItem = ui->listWidget->item(10);
    if (listItem)
        listItem->setBackground(Qt::red);

    for(int j=0;j<10;j++)
        ui->listWidget->addItem("2017-06-22 18:08:02:900 收到一条正常日志汇报");

    ui->listWidget->scrollToBottom();

//    ui->tableWidget->setStyleSheet("selection-background-color: red");//鼠标点击处显示红色
//    ui->tableWidget->setStyleSheet("QTableView::Item{background-color:#FF3EFF}");//行到背景色
//    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止编辑

    tableItem = ui->tableWidget->item(2,3);
    if (tableItem)//对内容为空到item设置颜色会导致程序崩溃，因此需判断
    {
    const QColor color = QColor(255,0,0);
    tableItem->setBackgroundColor(color);
    }



    for(int k=0;k<ui->tableWidget->rowCount();k++)
    ui->tableWidget->setRowHeight(k,20);

    ui->toolBox->setItemText(0,"日志显示");
    ui->toolBox->setItemText(1,"参数配置");
    ui->toolBox->setItemText(2,"控制指令");

    connect(ui->tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(plotSlot(int,int)));
    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(statusConfirm(int,int)));
}


deviceinfo::deviceinfo(PixItem *pixItem, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::deviceinfo)
{
    ui->setupUi(this);

    mypixItem = pixItem;

//    myItem->setPen(Qt::green);
//    myItem->setBrush(Qt::green);
//    mypixItem->setBrush(Qt::blue);
    mypixItem->update();

    Qt::WindowFlags flags = 0;
    flags |= Qt::WindowMinimizeButtonHint;
    setWindowFlags(flags); // 设置禁止最大化

    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setRowCount(30);
    QStringList headers;
    headers << "参数名称" << "允许最小值" << "允许最大值" << "当前值";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);//自动填满控件


/*
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("CPU使用率"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(0, 2, new QTableWidgetItem("30%"));
    ui->tableWidget->setItem(0, 3, new QTableWidgetItem("20%"));
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("硬盘使用率"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(1, 2, new QTableWidgetItem("40%"));
    ui->tableWidget->setItem(1, 3, new QTableWidgetItem("30%"));
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("内存使用率"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(2, 2, new QTableWidgetItem("40%"));
    ui->tableWidget->setItem(2, 3, new QTableWidgetItem("45%"));
    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("网络带宽"));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(3, 2, new QTableWidgetItem("100Mbps"));
    ui->tableWidget->setItem(3, 3, new QTableWidgetItem("50Mbps"));
*/
    ui->listWidget->setAlternatingRowColors(true);//背景颜色隔行区分

    for(int i=0;i<10;i++)
        ui->listWidget->addItem("2017-06-22 18:08:02:900 收到一条正常日志汇报");
    ui->listWidget->addItem("2017-06-22 18:08:02:900 收到一条异常日志汇报");

    listItem = ui->listWidget->item(10);
    if (listItem)
        listItem->setBackground(Qt::red);

    for(int j=0;j<10;j++)
        ui->listWidget->addItem("2017-06-22 18:08:02:900 收到一条正常日志汇报");

    ui->listWidget->scrollToBottom();

//    ui->tableWidget->setStyleSheet("selection-background-color: red");//鼠标点击处显示红色
//    ui->tableWidget->setStyleSheet("QTableView::Item{background-color:#FF3EFF}");//行到背景色
//    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止编辑

    tableItem = ui->tableWidget->item(2,3);
    if (tableItem)//对内容为空到item设置颜色会导致程序崩溃，因此需判断
    {
    const QColor color = QColor(255,0,0);
    tableItem->setBackgroundColor(color);
    }



    for(int k=0;k<ui->tableWidget->rowCount();k++)
    ui->tableWidget->setRowHeight(k,20);

    ui->toolBox->setItemText(0,"日志显示");
    ui->toolBox->setItemText(1,"参数配置");
    ui->toolBox->setItemText(2,"控制指令");

    connect(ui->tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(plotSlot(int,int)));
    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(statusConfirm(int,int)));

//    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(getid()));
//    connect(ui->lineEdit_2,SIGNAL(textChanged(QString)),this,SLOT(getname()));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(getname()));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(getid()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(addTable()));
}


deviceinfo::~deviceinfo()
{
    delete ui;
}

void deviceinfo::plotSlot(int i, int j)
{
    CustomPlotWindow* cp = new CustomPlotWindow;

    Qt::WindowFlags flags = 0;
    flags |= Qt::WindowMinimizeButtonHint;
    cp->setWindowFlags(flags); // 设置禁止最大化

    cp->show();
}

void deviceinfo::statusConfirm(int i, int j)
{
    tableItem = ui->tableWidget->item(i,j);
    if (tableItem)//对内容为空到item设置颜色会导致程序崩溃，因此需判断
    {
        const QColor color = QColor(255,255,255);
        tableItem->setBackgroundColor(color);
    }
}


void deviceinfo::setid(int deviceID)
{
    ui->lineEdit->setText(QString("%1").arg(deviceID));
}

void deviceinfo::setname(QString devicename)
{
    ui->lineEdit_2->setText(devicename);
}

void deviceinfo::getname()
{
    mypixItem->name = ui->lineEdit_2->text();
}

void deviceinfo::getid()
{
    bool ok;
    mypixItem->id = (ui->lineEdit->text()).toInt(&ok,10);
}

void deviceinfo::addTable()
{
    myParam.paramName = ui->lineEdit_3->text();
    myParam.paramMin = ui->lineEdit_4->text();
    myParam.paramMax = ui->lineEdit_5->text();
    mypixItem->deviceParamList.append(myParam);
    myParamList.append(myParam);
    setTable(myParamList);
//    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(ui->lineEdit_3->text()));
}

void deviceinfo::setTable(QList<DeviceParam> paramList)
{
    for(int i=0; i<paramList.count(); i++)
    {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(paramList.at(i).paramName));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(paramList.at(i).paramMin));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(paramList.at(i).paramMax));
//        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(paramList.at(i).paramValue));
    }
}
