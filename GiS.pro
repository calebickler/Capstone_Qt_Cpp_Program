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
    Memory\Memory.cpp \
    CPU\CPUusage.cpp \
    CPU\CPUusagethread.cpp \
    Settings\settings.cpp \
    CPU\CPUspeedthread.cpp \
    CPU\CPUspeed.cpp \
    displaysettings.cpp

HEADERS  += mainwindow.h \
    CPU\CPUusage.h \
    Memory\Memory.h \
    CPU\CPUusagethread.h \
    Settings\settings.h \
    CPU\CPUspeedthread.h \
    CPU\CPUspeed.h \
    displaysettings.h

FORMS    += mainwindow.ui \
    displaysettings.ui

win32: LIBS += -L$$PWD/Libraries/sigar-bin/lib/ -lsigar-x86-winnt

INCLUDEPATH += $$PWD/Libraries/sigar-bin/include
DEPENDPATH += $$PWD/Libraries/sigar-bin/include
