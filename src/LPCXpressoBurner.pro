#-------------------------------------------------
#
# Project created by QtCreator 2014-05-18T06:31:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG   += warn_on release
CONFIG(debug, debug|release){
    DESTDIR = ./debug
    OBJECTS_DIR = debug/.obj
    MOC_DIR = debug/.moc
    RCC_DIR = debug/.rcc
    UI_DIR = debug/.ui
}
CONFIG(release, debug|release){
    DESTDIR = ./release
    OBJECTS_DIR = release/.obj
    MOC_DIR = release/.moc
    RCC_DIR = release/.rcc
    UI_DIR = release/.ui
}

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
