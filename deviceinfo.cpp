#include "deviceinfo.h"
#include "customplotwindow.h"
#include "baseitem.h"
#include <QtSql/QSqlDatabase>
#include <QStringList>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

extern QSqlDatabase db;
extern QMap<QString, QMap<QString, QString>>* currentTable;

/*
DeviceInfo::DeviceInfo(PixItem *pixItem, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DeviceInfo)*/
DeviceInfo::DeviceInfo(PixItem *pixItem, QWidget *parent) :
    QMainWindow(parent)
{
    //ui->setupUi(this);

    mypixItem = pixItem;
    myRowCnt=0;

    tableWidget = new QTableWidget;
    toolBox = new QToolBox;
    pushButton_add = new QPushButton;
    pushButton_save = new QPushButton;
    lineEdit_code = new QLineEdit;
    lineEdit_name = new QLineEdit;
    listWidget = new QListWidget;
    comboBox = new QComboBox;

/*
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(toolBox);
    layout->addWidget(tableWidget);

    layout->setStretch(0,1);
    layout->setStretch(1,2);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);*/

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(toolBox);
    splitter->addWidget(tableWidget);
    splitter->setStretchFactor(0,1);
    splitter->setStretchFactor(1,2);
    setCentralWidget(splitter);

    resize(650, 300);

    //    myItem->setPen(Qt::green);
    //    myItem->setBrush(Qt::green);
    //    mypixItem->setBrush(Qt::blue);
    mypixItem->update();
    pixmap1 = new  QPixmap("/users/hyn/images/deviceItem.jpg");
    pixmap2 = new  QPixmap("/users/hyn/images/deviceItem2.jpg");

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateData()));
    timer->start(1000);

    itemValueList = new QStringList;

    Qt::WindowFlags flags = 0;
    flags |= Qt::WindowMinimizeButtonHint;
    setWindowFlags(flags); // 设置禁止最大化

    tableWidget->setColumnCount(4);
    tableWidget->setRowCount(myRowCnt);
    QStringList headers;
    headers << "参数名称" << "允许最小值" << "允许最大值" << "当前值";
    tableWidget->setHorizontalHeaderLabels(headers);
    tableWidget->horizontalHeader()->setStretchLastSection(true);//自动填满控件

    //    tableWidget->setStyleSheet("selection-background-color: red");//鼠标点击处显示红色
    //    tableWidget->setStyleSheet("QTableView::Item{background-color:#FF3EFF}");//行到背景色
    //    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止编辑

    for(int k=0;k<tableWidget->rowCount();k++)
        tableWidget->setRowHeight(k,20);

    QWidget *logWidget = new QWidget;
    QVBoxLayout* logLayout = new QVBoxLayout;
    logLayout->addWidget(listWidget);
    logWidget->setLayout(logLayout);

    listWidget->setAlternatingRowColors(true);//背景颜色隔行区分
    for(int i=0;i<10;i++)
        listWidget->addItem("2017-06-22 18:08:02:900 收到一条正常日志汇报");
    listWidget->addItem("2017-06-22 18:08:02:900 收到一条异常日志汇报");
    listItem = listWidget->item(10);
    if (listItem)
        listItem->setBackground(Qt::red);
    for(int j=0;j<10;j++)
        listWidget->addItem("2017-06-22 18:08:02:900 收到一条正常日志汇报");
    listWidget->scrollToBottom();


    QWidget *paramWidget = new QWidget;
    QVBoxLayout* layoutV = new QVBoxLayout;
    QLabel* label_code = new QLabel("设备代码");
    QLabel* label_name = new QLabel("设备名称");
    QGridLayout* paramLayout = new QGridLayout;
    paramLayout->addWidget(label_code, 0, 0);
    paramLayout->addWidget(label_name, 1, 0);
    paramLayout->addWidget(lineEdit_code, 0, 1);
    paramLayout->addWidget(lineEdit_name, 1, 1);
    layoutV->addWidget(comboBox);
    layoutV->addLayout(paramLayout);
    layoutV->addWidget(pushButton_add);
    layoutV->addWidget(pushButton_save);
    paramWidget->setLayout(layoutV);

    QWidget *commandWidget = new QWidget;

    toolBox->addItem(logWidget,"日志显示");
    toolBox->addItem(paramWidget,"参数配置");
    toolBox->addItem(commandWidget,"指令控制");

    pushButton_add->setText("添加参数");
    pushButton_save->setText("保存设置");

    //toolBox->setItemText(0,"日志显示");
    //toolBox->setItemText(1,"参数配置");
    //toolBox->setItemText(2,"控制指令");

    comboBox->addItem("监视模式");
    comboBox->addItem("配置模式");
    comboBox->setCurrentIndex(0);
    pushButton_add->setEnabled(false);
    pushButton_save->setEnabled(false);

    connect(tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(plotSlot(int,int)));
    //    connect(tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(statusConfirm(int,int)));
    connect(pushButton_save,SIGNAL(clicked()),this,SLOT(save()));
    connect(pushButton_add,SIGNAL(clicked()),this,SLOT(addParam()));
    connect(tableWidget->verticalHeader(), SIGNAL(sectionDoubleClicked(int)), this, SLOT(onHeaderClicked(int)));
    connect(comboBox, SIGNAL(currentIndexChanged(const QString &)),
            this, SLOT(modeChanged(const QString &)));
}


DeviceInfo::~DeviceInfo()
{
    //delete ui;
}

void DeviceInfo::plotSlot(int i, int j)
{
    if(j==3)
    {
        CustomPlotWindow* cp = new CustomPlotWindow;
        Qt::WindowFlags flags = 0;
        flags |= Qt::WindowMinimizeButtonHint;
        cp->setWindowFlags(flags); // 设置禁止最大化
        QString str="";
        tableItem = tableWidget->item(i,0);
        if(tableItem)
            str = tableItem->text();
        cp->drawRealtimeCurve(str, i, itemValueList);
        cp->show();
    }
}

void DeviceInfo::statusConfirm(int i, int j)
{
    tableItem = tableWidget->item(i,j);
    if (tableItem)//对内容为空到item设置颜色会导致程序崩溃，因此需判断
    {
        const QColor color = QColor(255,255,255);
        tableItem->setBackgroundColor(color);
    }
}

void DeviceInfo::setCode(QString code)
{
    lineEdit_code->setText(code);
}

void DeviceInfo::setName(QString name)
{
    lineEdit_name->setText(name);
}

void DeviceInfo::setTable(QList<DeviceParam> paramList)
{
    myRowCnt = paramList.count();
    tableWidget->setRowCount(myRowCnt);
    for(int i=0; i<myRowCnt; i++)
    {
        tableWidget->setRowHeight(i, 20);
        tableWidget->setItem(i, 0, new QTableWidgetItem(paramList.at(i).paramName));
        tableWidget->setItem(i, 1, new QTableWidgetItem(paramList.at(i).paramMin));
        tableWidget->setItem(i, 2, new QTableWidgetItem(paramList.at(i).paramMax));
        //        tableWidget->setItem(i, 3, new QTableWidgetItem(paramList.at(i).paramValue));
    }
}


void DeviceInfo::addParam()
{
    myRowCnt++;
    tableWidget->setRowCount(myRowCnt);
    tableWidget->setRowHeight(myRowCnt-1, 20);
    //    tableWidget->setEditTriggers ( QAbstractItemView::NoEditTriggers );//整个table不可编辑

    tableItem = tableWidget->item(myRowCnt-1,3);
    if(tableItem)
        tableItem->setFlags(tableItem->flags() & ~Qt::ItemIsEditable);
    //    mypixItem->deviceParamList.append(myParam);
    //    myParamList.append(myParam);
    //    setTable(myParamList);
    //    tableWidget->setItem(0, 0, new QTableWidgetItem(lineEdit_3->text()));
}

void DeviceInfo::setDevice()
{
    if(db.open())
    {
        //qDebug() << "succeed！";
        QSqlQuery query;
        QString insertline;
        QStringList strTables = db.tables();
        if(strTables.contains("DeviceParam")) //新建表时需注意，如果表已经存在会报错
        {
            query.exec("select count(*) from information_schema.COLUMNS  where table_schema = 'jtgl' and table_name = 'DeviceParam'");//获得表中共有几列
            query.next();
            int columns = query.value(0).toInt();

            myRowCnt = 0;
            insertline=QString("select * from DeviceParam where deviceCode = '%1'").arg(mypixItem->deviceCode);
            query.exec(insertline);

            while(query.next())//QSqlQuery返回的数据集，record是停在第一条记录之前的。所以，在获得数据集后，必须执行next()或first()到第一条记录，这时候record才是有效的
            {
                tableWidget->setRowCount(myRowCnt+1);
                setName(query.value(0).toString());
                setCode(query.value(1).toString());
                tableWidget->setItem(myRowCnt, 0, new QTableWidgetItem(query.value(2).toString()));
                tableWidget->setItem(myRowCnt, 1, new QTableWidgetItem(query.value(3).toString()));
                tableWidget->setItem(myRowCnt, 2, new QTableWidgetItem(query.value(4).toString()));
                myRowCnt++;
            }
            setWindowTitle(lineEdit_code->text());
        }
        else
            qDebug() << "jtgl表不存在";
    }
    else
        qDebug() << db.lastError();

}

void DeviceInfo::onHeaderClicked(int i)
{
    tableWidget->removeRow(i);
    myRowCnt--;
}

void DeviceInfo::save()
{
    int i,j;
    mypixItem->deviceCode = lineEdit_code->text();
    mypixItem->deviceName = lineEdit_name->text();
    mypixItem->deviceParamList.clear();
    int rowCnt = tableWidget->rowCount();

    if(mypixItem->deviceCode.count() == 0)
    {
        QMessageBox::about(NULL, "warning", "设备代号不能为空");
        return;
    }

    for(i=0;i<rowCnt;i++)
    {
        for(j=0;j<3;j++)
        {
            tableItem = tableWidget->item(i,j);
            if (!tableItem)
            {
                QMessageBox::about(NULL, "warning", "参数及上下限不能为空");
                return;
            }
        }
    }

    for(i=0;i<rowCnt;i++)
    {
        myParam.paramName = tableWidget->item(i,0)->text();
        myParam.paramMin = tableWidget->item(i,1)->text();
        myParam.paramMax = tableWidget->item(i,2)->text();
        mypixItem->deviceParamList.append(myParam);
    }

    if(db.open())
    {
        //qDebug() << "succeed！";
        QString m_content;
        QSqlQuery query;
        QString insertline;
        QStringList strTables = db.tables();
        if(strTables.contains("DeviceParam")) //新建表时需注意，如果表已经存在会报错
        {
            //insertline = QString("truncate table DeviceParam");//删除表中所有数据
            insertline = QString("delete from DeviceParam where deviceCode = '%1'").arg(mypixItem->deviceCode);
            bool flag = query.exec(insertline);
            //qDebug() << flag;

            for(i=0;i<rowCnt;i++)
            {
                insertline = QString("insert into DeviceParam values('%1','%2','%3','%4','%5')")
                        .arg(mypixItem->deviceName)
                        .arg(mypixItem->deviceCode)
                        .arg(tableWidget->item(i,0)->text())
                        .arg(tableWidget->item(i,1)->text())
                        .arg(tableWidget->item(i,2)->text());
                query.exec(insertline);
            }
        }
        else
            qDebug() << "DeviceParam表不存在";

        if(!strTables.contains(mypixItem->deviceCode))
        {
            insertline = QString("create table %1 (dateTime VARCHAR(50))").arg(mypixItem->deviceCode);
            query.exec(insertline);
        }
        for(i=0;i<rowCnt;i++)
        {
            insertline = QString("alter table %1 add column %2 VARCHAR(100)")
                    .arg(mypixItem->deviceCode)
                    .arg(tableWidget->item(i,0)->text());//参数名称中不能有()等字符！！
            query.exec(insertline);
        }
    }
    else
        qDebug() << db.lastError();
}

void DeviceInfo::updateData()
{
    const QColor color1 = QColor(255,200,200);
    const QColor color2 = QColor(255,255,255);
    itemValueList->clear();
    double min;
    double max;
    QMap<QString, QString> paramValueMap;
    QString paramName;
    paramValueMap = currentTable->value(lineEdit_code->text());
    QTableWidgetItem* nameItem = new QTableWidgetItem;
    QTableWidgetItem* minItem = new QTableWidgetItem;
    QTableWidgetItem* maxItem = new QTableWidgetItem;
    QString str;
    for(int i=0;i<myRowCnt;i++)
    {
        nameItem = tableWidget->item(i,0);
        minItem = tableWidget->item(i,1);
        maxItem = tableWidget->item(i,2);
        if(nameItem && minItem && maxItem)
        {
            paramName = nameItem->text();
            min = minItem->text().toDouble();
            max = maxItem->text().toDouble();
            str = paramValueMap.value(paramName);
            tableWidget->setItem(i, 3, new QTableWidgetItem(str));
            itemValueList->append(str);
            if(str.toDouble() > max || str.toDouble() < min)
            {
                tableWidget->item(i,3)->setBackgroundColor(color1);
                mypixItem->pix = *pixmap2;
                mypixItem->update();
                //mypixItem->setOpacity(0.1);
            }
            else
            {
                tableWidget->item(i,3)->setBackgroundColor(color2);
                mypixItem->pix = *pixmap1;
                mypixItem->update();
                //mypixItem->setOpacity(1);
            }
        }
    }
}

void DeviceInfo::modeChanged(QString mode)
{
    if(mode == "监视模式")
    {
        timer->start(1000);
        pushButton_add->setEnabled(false);
        pushButton_save->setEnabled(false);
    }
    else
    {
        timer->stop();
        pushButton_add->setEnabled(true);
        pushButton_save->setEnabled(true);
    }
}
