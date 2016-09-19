#-------------------------------------------------
#
# Project created by QtCreator 2016-09-19T23:58:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = prolog_maze
TEMPLATE = app

message(LIB: "$$PWD/lib/libswipl.lib")

LIBS += -L "$$PWD/lib" -lswipl
INCLUDEPATH += "$$PWD/include"


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
