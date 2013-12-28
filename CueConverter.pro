#-------------------------------------------------
#
# Project created by QtCreator 2013-06-18T18:41:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CueConverter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    droplistview.cpp \
    cuefiles.cpp \
    nregexp.cpp

HEADERS  += mainwindow.h \
    droplistview.h \
    cuefiles.h \
    nregexp.h

FORMS    += mainwindow.ui
