#-------------------------------------------------
#
# Project created by QtCreator 2016-09-19T23:58:22
#
#-------------------------------------------------

QT       += core gui

CONFIG += exceptions static

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = prolog_maze
TEMPLATE = app

CONFIG(debug, debug|release) {
    DESTDIR = build/debug
} else {
    DESTDIR = build/release
}
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

DEST_PWD = $$OUT_PWD/$$DESTDIR
QT_LIBS_PWD = $$(QTDIR)/bin

prolog_libs.files = $$PWD/lib/*
prolog_libs.path = $$DEST_PWD

prolog_src.files = "$$PWD/maze.pl"
prolog_src.path = $$DEST_PWD

CONFIG(debug, debug|release) {
    qt_libs.files = $$QT_LIBS_PWD/Qt5Cored.dll $$QT_LIBS_PWD/Qt5Guid.dll $$QT_LIBS_PWD/Qt5Widgetsd.dll
} else {
    qt_libs.files = $$QT_LIBS_PWD/Qt5Core.dll $$QT_LIBS_PWD/Qt5Gui.dll $$QT_LIBS_PWD/Qt5Widgets.dll
}
qt_libs.path= $$DEST_PWD

INSTALLS += prolog_src prolog_libs qt_libs



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
    prolog.cpp \
    swipl32inputdialog.cpp

HEADERS  += mainwindow.h \
    mazescene.h \
    cellobject.h \
    global.h \
    parameterswidget.h \
    parameterswidgetaction.h \
    mazeview.h \
    generationstate.h \
    prolog.h \
    swipl32inputdialog.h

FORMS    += mainwindow.ui \
    parameterswidget.ui \
    swipl32inputdialog.ui

RESOURCES += \
    icons.qrc

DISTFILES +=

CODECFORTR = UTF-8
CODECFORSRC = UTF-8
