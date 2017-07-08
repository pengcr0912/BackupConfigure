#include "queryresult.h"
#include <QHBoxLayout>
#include <QHeaderView>

QueryResult::QueryResult(QWidget *parent) :
    QMainWindow(parent)
{
    tableWidget = new QTableWidget;
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(tableWidget);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
    resize(700, 500);
}

QueryResult::~QueryResult()
{

}

void QueryResult::setTable(QStringList timeList, QStringList logList)
{
    int rowCnt = timeList.count();

    tableWidget->setColumnCount(2);
    tableWidget->setRowCount(rowCnt);

    QStringList headers;
    headers << "时间" << "日志内容";
    tableWidget->setHorizontalHeaderLabels(headers);

    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//先自适应宽度
    tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);//将第一列根据内容调整宽度
    tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setStretchLastSection(true);//自动填满控件

    for(int k=0;k<tableWidget->rowCount();k++)
        tableWidget->setRowHeight(k,20);

    for(int i=0;i<rowCnt;i++)
    {
        tableWidget->setItem(i,0,new QTableWidgetItem(timeList.at(i)));
        tableWidget->setItem(i,1,new QTableWidgetItem(logList.at(i)));
    }
}

void QueryResult::resizeEvent(QResizeEvent *event)
{

}
