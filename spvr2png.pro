#-------------------------------------------------
#
# Project created by QtCreator 2015-07-08T20:46:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = spvr2png
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    SegaPVRImage.c

HEADERS  += MainWindow.h \
    SegaPVRImage.h

FORMS    += MainWindow.ui
