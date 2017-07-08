#ifndef QUERYRESULT_H
#define QUERYRESULT_H

#include <QMainWindow>
#include <QTableWidget>

class QueryResult : public QMainWindow
{
    Q_OBJECT

public:
    QueryResult(QWidget *parent = 0);
    ~QueryResult();

    QTableWidget* tableWidget;

    void setTable(QStringList timeList, QStringList logList);

    virtual void resizeEvent(QResizeEvent *event);

};

#endif // QUERYRESULT_H
