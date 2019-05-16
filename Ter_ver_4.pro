#-------------------------------------------------
#
# Project created by QtCreator 2019-04-21T16:55:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Ter_ver_4
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        qcustomplot.cpp \
    plotwindow.cpp

HEADERS  += mainwindow.h \
            qcustomplot.h \
    plotwindow.h

FORMS    += mainwindow.ui \
    plotwindow.ui
