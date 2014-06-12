#-------------------------------------------------
#
# Project created by QtCreator 2014-05-18T06:31:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LPCXpressoBurner
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        lpcconfig.cpp \
    lpccommand.cpp

HEADERS  += mainwindow.h \
        lpcconfig.h \
    lpccommand.h

FORMS    += mainwindow.ui
