TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    MQenC.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    MQenC.h

LIBS += -lrt
