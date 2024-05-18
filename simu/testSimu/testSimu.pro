TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
LIBS += -lrt

SOURCES += main.cpp \
    ../vers1/detector.cpp \
    ../vers1/simugerbe.cpp \
    ../vers1/MQenC.c \
    test_detector.cpp \
    test_simugerbe.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    ../vers1/detector.h \
    ../vers1/MQenC.h \
    ../vers1/simugerbe.h \
    test_detector.h \
    test_simugerbe.h

