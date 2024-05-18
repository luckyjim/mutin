TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
LIBS += -lrt
CONFIG += -g
CONFIG += -pg

SOURCES += main.cpp \
    MQenC.c \
    detector.cpp \
    messagemq.cpp \
    sortermessage.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    MQenC.h \
    detector.h \
    messagemq.h \
    sortermessage.h

