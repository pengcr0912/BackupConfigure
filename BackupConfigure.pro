#-------------------------------------------------
#
# Project created by QtCreator 2011-06-27T11:04:38
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = BackupConfigure
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    textitem.cpp \
    rectitem.cpp \
    propertiesdialog.cpp \
    lineitem.cpp \
    itemmimedata.cpp \
    groupitem.cpp \
    ellipseitem.cpp \
    diagramscene.cpp \
    baseitem.cpp \
    deviceinfo.cpp \
    customplotwindow.cpp \
    qcustomplot.cpp \
    pixitem.cpp \
    mysql.cpp

HEADERS  += mainwindow.h \
    textitem.h \
    rectitem.h \
    propertiesdialog.h \
    lineitem.h \
    itemmimedata.h \
    groupitem.h \
    ellipseitem.h \
    diagramscene.h \
    baseitem.h \
    deviceinfo.h \
    customplotwindow.h \
    qcustomplot.h \
    pixitem.h \
    mysql.h

OTHER_FILES += \
    MyConfigure.pro.user \
    BackupConfigure.pro.user

FORMS += \
    propertiesdialog.ui \
    deviceinfo.ui \
    customplotwindow.ui

RESOURCES += \
    image.qrc

DISTFILES += \
    BackupConfigure.pro.user.2.2pre5
