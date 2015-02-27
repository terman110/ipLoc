#-------------------------------------------------
#
# Project created by QtCreator 2014-07-30T22:38:47
#
#-------------------------------------------------

QT       += core\
        network
QT       -= gui

#CONFIG   += console
#CONFIG   -= app_bundle
CONFIG   += no_icon # this will do the trick for hiding application icon from menu

#QT       += core gui\
 #        network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ipLoc
TEMPLATE = app


SOURCES += main.cpp\
        menuicon.cpp\
    filedownload.cpp \
    dlg.cpp


HEADERS  += menuicon.h \
    filedownload.h \
    dlg.h

RESOURCES +=

ICON = ipLoc.icns

FORMS += \
    dlg.ui

OTHER_FILES += \
    ipLoc_icon.png
