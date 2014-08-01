#-------------------------------------------------
#
# Project created by QtCreator 2014-07-30T22:38:47
#
#-------------------------------------------------

QT       += core gui\
         network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ipMenu
TEMPLATE = app


SOURCES += main.cpp\
        menuicon.cpp\
    filedownload.cpp


HEADERS  += menuicon.h \
    filedownload.h

RESOURCES +=

ICON = ipLoc.icns
