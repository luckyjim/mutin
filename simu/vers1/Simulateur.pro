TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

SOURCES += main.cpp \
    detector.cpp \
    simugerbe.cpp \
    MQenC.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    detector.h \
    simugerbe.h \
    MQenC.h

LIBS += -lrt
