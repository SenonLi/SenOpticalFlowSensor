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
    SenSerialObject.cpp \
    SenImageBurstWidget.cpp \
    SenMotionTrackingWidget.cpp \
    SenBleDongleTalkFundamentals.cpp \
    SenDecodeCySmtProtocolPackageObject.cpp \
    SenSerialSettingDialog.cpp

HEADERS  += SenOpticalFlowSensorsMainWindow.h \
    SenConsole.h \
    SenSerialObject.h \
    SenImageBurstWidget.h \
    SenMotionTrackingWidget.h \
    SenBleDongleTalkFundamentals.h \
    SenDecodeCySmtProtocolPackageObject.h \
    SenSerialSettingDialog.h

FORMS    += SenOpticalFlowSensorsMainWindow.ui

RESOURCES += \
    SenSerialTerminalResource.qrc
