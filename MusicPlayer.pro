#-------------------------------------------------
#
# Project created by QtCreator 2018-10-13T10:47:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MusicPlayer
TEMPLATE = app


SOURCES += main.cpp\
        window.cpp \
    musicplay.cpp \
    mplayercall.cpp \
    skindialog.cpp

HEADERS  += window.h \
    musicplay.h \
    mplayercall.h \
    sliderclick.h \
    skindialog.h

RESOURCES += \
    pic.qrc
