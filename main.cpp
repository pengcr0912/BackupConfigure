//实现类似visio功能

#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
//    QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));

    MainWindow w;
    w.show();
    return a.exec();
}