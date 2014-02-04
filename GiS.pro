#-------------------------------------------------
#
# Project created by QtCreator 2014-02-01T16:55:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GiS
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    CPU.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

win32: LIBS += -L$$PWD/Libraries/sigar-bin/lib/ -lsigar-x86-winnt

INCLUDEPATH += $$PWD/Libraries/sigar-bin/include
DEPENDPATH += $$PWD/Libraries/sigar-bin/include
