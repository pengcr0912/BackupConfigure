#ifndef QUERYRESULT_H
#define QUERYRESULT_H

#include <QMainWindow>

namespace Ui {
class QueryResult;
}

class QueryResult : public QMainWindow
{
    Q_OBJECT

public:
    explicit QueryResult(QWidget *parent = 0);
    ~QueryResult();

    void setTable(QStringList timeList, QStringList logList);

private:
    Ui::QueryResult *ui;
};

#endif // QUERYRESULT_H
