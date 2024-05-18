#-------------------------------------------------
#
# Project created by QtCreator 2015-06-15T14:29:55
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11
LIBS += -lrt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Gate
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialogadddetector.cpp \
    detector.cpp \
    dialogmodifynetwork.cpp \
    infowindow.cpp \
    valuedslider.cpp \
    tensionwidget.cpp \
    MQenC.c \
    messagereader.cpp \
    messagemq.cpp

HEADERS  += mainwindow.h \
    dialogadddetector.h \
    detector.h \
    dialogmodifynetwork.h \
    infowindow.h \
    valuedslider.h \
    tensionwidget.h \
    MQenC.h \
    messagereader.h \
    messagemq.h

FORMS    += mainwindow.ui \
    dialogadddetector.ui \
    dialogmodifynetwork.ui \
    infowindow.ui
