#-------------------------------------------------
#
# Project created by QtCreator 2014-08-20T18:59:54
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport
QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qt_Plot
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h

FORMS    += mainwindow.ui
