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

void QueryResult::setTable(QStringList timeList, QStringList typeList, QStringList logList)
{
    int rowCnt = timeList.count();
    QTableWidgetItem* tableItem;
    QColor color;
    QString str;
    tableWidget->setColumnCount(3);
    tableWidget->setRowCount(rowCnt);

    QStringList headers;
    headers << "时间" << "类型" << "日志内容";
    tableWidget->setHorizontalHeaderLabels(headers);

    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//先自适应宽度
    tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);//将第一列根据内容调整宽度
    tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    tableWidget->horizontalHeader()->setStretchLastSection(true);//自动填满控件

    for(int k=0;k<tableWidget->rowCount();k++)
        tableWidget->setRowHeight(k,20);

    for(int i=0;i<rowCnt;i++)
    {
        tableWidget->setItem(i,0,new QTableWidgetItem(timeList.at(i)));
        tableWidget->setItem(i,1,new QTableWidgetItem(typeList.at(i)));
        tableWidget->setItem(i,2,new QTableWidgetItem(logList.at(i)));

        str = typeList.at(i);
        if(str == "信息")
            color = QColor(150,255,150);
        else if(str == "报警")
            color = QColor(255,255,150);
        else if(str == "错误")
            color = QColor(255,150,150);

        for(int j=0;j<3;j++)
        {
            tableItem = tableWidget->item(i,j);
            if (tableItem)//对内容为空到item设置颜色会导致程序崩溃，因此需判断
                tableItem->setBackgroundColor(color);

        }
    }
}

void QueryResult::resizeEvent(QResizeEvent *event)
{

}
