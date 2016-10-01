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

LIBS += -L"$$PWD/lib" -lswipl -lgcc_s_sjlj-1 -lgmp-10 -lwinpthread-1
INCLUDEPATH += "$$PWD/include"


SOURCES += main.cpp\
        mainwindow.cpp \
    mazescene.cpp \
    cellobject.cpp \
    parameterswidget.cpp \
    parameterswidgetaction.cpp \
    mazeview.cpp \
    generationstate.cpp \
    prolog.cpp

HEADERS  += mainwindow.h \
    mazescene.h \
    cellobject.h \
    global.h \
    parameterswidget.h \
    parameterswidgetaction.h \
    mazeview.h \
    generationstate.h \
    prolog.h

FORMS    += mainwindow.ui \
    parameterswidget.ui

RESOURCES += \
    icons.qrc

DISTFILES +=

CODECFORTR = UTF-8
CODECFORSRC = UTF-8
