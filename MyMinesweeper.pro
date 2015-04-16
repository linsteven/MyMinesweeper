#-------------------------------------------------
#
# Project created by QtCreator 2014-10-26T02:08:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyMinesweeper
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    block.cpp \
    blockarea.cpp

HEADERS  += mainwindow.h \
    block.h \
    blockarea.h

FORMS    += mainwindow.ui

RESOURCES += \
    MyMinesweeper.qrc
