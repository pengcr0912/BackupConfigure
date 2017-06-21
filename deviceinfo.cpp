#include "deviceinfo.h"
//#include "ui_deviceinfo.h"
#include "customplotwindow.h"

deviceinfo::deviceinfo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::deviceinfo)
{
    ui->setupUi(this);

    Qt::WindowFlags flags = 0;
    flags |= Qt::WindowMinimizeButtonHint;
    setWindowFlags(flags); // 设置禁止最大化

    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(15);
    QStringList headers;
    headers << "参数名称" << "正常范围" << "当前值";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("CPU使用率"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("0～50%"));
    ui->tableWidget->setItem(0, 2, new QTableWidgetItem("20%"));
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("硬盘使用率"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("0～40%"));
    ui->tableWidget->setItem(1, 2, new QTableWidgetItem("30%"));
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("内存使用率"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("0～40%"));
    ui->tableWidget->setItem(2, 2, new QTableWidgetItem("45%"));
    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("网络带宽"));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem("0～100Mbps"));
    ui->tableWidget->setItem(3, 2, new QTableWidgetItem("50Mbps"));

//    ui->tableWidget->setStyleSheet("selection-background-color: red");//鼠标点击处显示红色
//    ui->tableWidget->setStyleSheet("QTableView::Item{background-color:#FF3EFF}");//行到背景色
//    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁止编辑

    item = ui->tableWidget->item(2,2);
    if (item)//对内容为空到item设置颜色会导致程序崩溃，因此需判断
    {
    const QColor color = QColor(255,0,0);
    item->setBackgroundColor(color);
    }

    ui->toolBox->setItemText(0,"参数配置");
    ui->toolBox->setItemText(1,"控制指令");

    connect(ui->tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(plotSlot(int,int)));
    connect(ui->tableWidget,SIGNAL(cellClicked(int,int)),this,SLOT(confirm(int,int)));
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

void deviceinfo::confirm(int i, int j)
{
    item = ui->tableWidget->item(i,j);
    if (item)//对内容为空到item设置颜色会导致程序崩溃，因此需判断
    {
        const QColor color = QColor(255,255,255);
        item->setBackgroundColor(color);
    }
}
