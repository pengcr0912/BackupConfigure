#include "queryresult.h"
#include "ui_queryresult.h"

QueryResult::QueryResult(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QueryResult)
{
    ui->setupUi(this);
}

QueryResult::~QueryResult()
{
    delete ui;
}

void QueryResult::setTable(QStringList timeList, QStringList logList)
{
    int rowCnt = timeList.count();
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(rowCnt);

    QStringList headers;
    headers << "时间" << "日志内容";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    //ui->tableWidget->horizontalHeader()->setStretchLastSection(true);//自动填满控件

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//先自适应宽度
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);//在将第一列根据内容调整宽度
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);

    for(int k=0;k<ui->tableWidget->rowCount();k++)
        ui->tableWidget->setRowHeight(k,20);

    for(int i=0;i<rowCnt;i++)
    {
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(timeList.at(i)));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(logList.at(i)));
    }
}
