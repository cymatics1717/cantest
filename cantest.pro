#-------------------------------------------------
#
# Project created by QtCreator 2017-12-02T19:05:12
#
#-------------------------------------------------

QT       += core gui network
CONFIG += c++11 warn_on
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cantest
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    dbc/dbc_classes.cpp \
    dbc/dbchandler.cpp \
    dbc/utility.cpp

HEADERS += \
        mainwindow.h \
    dbc/dbc_classes.h \
    dbc/dbchandler.h \
    dbc/utility.h

FORMS += \
        mainwindow.ui
