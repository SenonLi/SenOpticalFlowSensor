#-------------------------------------------------
#
# Project created by QtCreator 2015-06-05T23:02:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET = SenOpticalFlowSensors
TEMPLATE = app


SOURCES += main.cpp\
        SenOpticalFlowSensorsMainWindow.cpp \
    SenConsole.cpp \
    SenSettingsDialog.cpp \
    SenSerialControlAlgorithmObject.cpp

HEADERS  += SenOpticalFlowSensorsMainWindow.h \
    SenConsole.h \
    SenSettingsDialog.h \
    SenSerialControlAlgorithmObject.h

FORMS    += SenOpticalFlowSensorsMainWindow.ui \
    SenSettingsDialog.ui

RESOURCES += \
    SenSerialTerminalResource.qrc
