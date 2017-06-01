#-------------------------------------------------
#
# Project created by QtCreator 2017-03-03T13:33:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ../Cap135Display
TEMPLATE = app

DEFINES += VOS_WINDOWS

DEPENDPATH += . \
    Gui \
    Pub
INCLUDEPATH += . \
    Gui \
    Pub


SOURCES += main.cpp\
        mainwindow.cpp \
    Pub/global.cpp \
    Gui/capTest.cpp \
    Gui/guiGlobal.cpp

HEADERS  += mainwindow.h \
    Pub/global.h \
    Gui/capTest.h \
    Pub/os_header.h \
    Gui/guiGlobal.h

FORMS    += mainwindow.ui
