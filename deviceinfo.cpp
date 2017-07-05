#include "deviceinfo.h"
//#include "ui_deviceinfo.h"
#include "customplotwindow.h"
#include "baseitem.h"

deviceinfo::deviceinfo(BaseItem *baseItem, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::deviceinfo)
{
    ui->setupUi(this);

    myItem = baseItem;

    myRowCnt=0;

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

    myRowCnt=0;

//    myItem->setPen(Qt::green);
//    myItem->setBrush(Qt::green);
//    mypixItem->setBrush(Qt::blue);
    mypixItem->update();

    Qt::WindowFlags flags = 0;
    flags |= Qt::WindowMinimizeButtonHint;
    setWindowFlags(flags); // 设置禁止最大化

    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setRowCount(myRowCnt);
    QStringList headers;
    headers << "参数名称" << "允许最小值" << "允许最大值" << "当前值";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);//自动填满控件

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

    for(int k=0;k<ui->tableWidget->rowCount();k++)
    ui->tableWidget->setRowHeight(k,20);

    ui->toolBox->setItemText(0,"日志显示");
    ui->toolBox->setItemText(1,"参数配置");
    ui->toolBox->setItemText(2,"控制指令");

    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(plotSlot(int,int)));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(deleteParam()));
//    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(statusConfirm(int,int)));

    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(setConfirm()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(addTable()));
}


deviceinfo::~deviceinfo()
{
    delete ui;
}

void deviceinfo::plotSlot(int i, int j)
{
/*    CustomPlotWindow* cp = new CustomPlotWindow;

    Qt::WindowFlags flags = 0;
    flags |= Qt::WindowMinimizeButtonHint;
    cp->setWindowFlags(flags); // 设置禁止最大化

    cp->show();*/
}
/*
void deviceinfo::statusConfirm(int i, int j)
{
    tableItem = ui->tableWidget->item(i,j);
    if (tableItem)//对内容为空到item设置颜色会导致程序崩溃，因此需判断
    {
        const QColor color = QColor(255,255,255);
        tableItem->setBackgroundColor(color);
    }
}
*/

void deviceinfo::deleteParam()
{
    int i = ui->lineEdit_3->text().toInt();
    if(i > 0 && i <= ui->tableWidget->rowCount())
    {
        ui->tableWidget->removeRow(i-1);
        myRowCnt--;
    }
    else
        QMessageBox::about(NULL, "warning", "请输入合适的行号");
}

void deviceinfo::setCode(QString deviceCode)
{
    ui->lineEdit->setText(deviceCode);
}

void deviceinfo::setName(QString deviceName)
{
    ui->lineEdit_2->setText(deviceName);
}

void deviceinfo::addTable()
{
//    myParam.paramName = ui->lineEdit_3->text();
//    myParam.paramMin = ui->lineEdit_4->text();
//    myParam.paramMax = ui->lineEdit_5->text();
    myRowCnt++;
    ui->tableWidget->setRowCount(myRowCnt);
//    ui->tableWidget->setEditTriggers ( QAbstractItemView::NoEditTriggers );//整个table不可编辑

    tableItem = ui->tableWidget->item(myRowCnt-1,3);
    if(tableItem)
        tableItem->setFlags(tableItem->flags() & ~Qt::ItemIsEditable);
//    mypixItem->deviceParamList.append(myParam);
//    myParamList.append(myParam);
//    setTable(myParamList);
//    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(ui->lineEdit_3->text()));
}

void deviceinfo::setTable(QList<DeviceParam> paramList)
{
    myRowCnt = paramList.count();
    ui->tableWidget->setRowCount(myRowCnt);
    for(int i=0; i<myRowCnt; i++)
    {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(paramList.at(i).paramName));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(paramList.at(i).paramMin));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(paramList.at(i).paramMax));
//        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(paramList.at(i).paramValue));
    }
}

void deviceinfo::setConfirm()
{
    int i,j;
    mypixItem->deviceCode = ui->lineEdit->text();
    mypixItem->deviceName = ui->lineEdit_2->text();
    mypixItem->deviceParamList.clear();
    int rowCnt = ui->tableWidget->rowCount();

    for(i=0;i<rowCnt;i++)
    {
        for(j=0;j<3;j++)
        {
            tableItem = ui->tableWidget->item(i,j);
            if (!tableItem)
            {
                QMessageBox::about(NULL, "warning", "参数及上下限不能为空");
                return;
            }
        }
    }

    for(i=0;i<rowCnt;i++)
    {
        myParam.paramName = ui->tableWidget->item(i,0)->text();
        myParam.paramMin = ui->tableWidget->item(i,1)->text();
        myParam.paramMax = ui->tableWidget->item(i,2)->text();
        mypixItem->deviceParamList.append(myParam);
    }

/*
    QString m_content;
    QStringList drivers = QSqlDatabase::drivers();
    qDebug() << drivers;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("jtgl");
    db.setUserName("root");
    db.setPassword("840912");
    if(db.open())
    {
        qDebug() << "succeed！";

        QSqlQuery query;
        QStringList strTables = db.tables();
        if(strTables.contains("DeviceParam")) //新建表时需注意，如果表已经存在会报错
        {
            insertline = QString("insert into DeviceParam values('遥控前端','TCF','CPU使用率','0','50')");
            bool flag = query.exec(insertline);

            query.exec("select count(*) from information_schema.COLUMNS  where table_schema = 'jtgl' and table_name = 'DeviceParam'");//获得表中共有几列
            query.next();
            int columns = query.value(0).toInt();

            query.exec("select * from DeviceParam");
            //query.exec("select * from student where id < 3");//条件查询
            //query.exec("select * from student where time < '2018-06-14'");//条件查询
            while(query.next())//QSqlQuery返回的数据集，record是停在第一条记录之前的。所以，在获得数据集后，必须执行next()或first()到第一条记录，这时候record才是有效的
            {
                for(int i=0;i<columns;i++)
                {
                    m_content = query.value(i).toString();
                    qDebug() << m_content;
                }
            }
            qDebug() << flag;

            query.exec("SELECT FROM_UNIXTIME(1249488000,'%Y年%m月%d')");
            query.next();
            qDebug() << query.value(0).toString();

        }
        else
            qDebug() << "jtgl表不存在";

    }
    else
        qDebug() << db.lastError();
*/
}
