#include "SenSerialControlAlgorithmObject.h"

SenSerialControlAlgorithmObject::SenSerialControlAlgorithmObject(QObject *parent) : QObject(parent)
{
    mySerialPort = new QSerialPort();


    connect(mySerialPort, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));
    connect(mySerialPort, SIGNAL(readyRead()), this, SLOT(readData()));
}

void SenSerialControlAlgorithmObject::openSerialPort(SenSettingsDialog::Settings mySettingInfo)
{
    mySerialPort->setPortName(mySettingInfo.name);
    mySerialPort->setBaudRate(mySettingInfo.baudRate);
    mySerialPort->setDataBits(mySettingInfo.dataBits);
    mySerialPort->setParity(mySettingInfo.parity);
    mySerialPort->setStopBits(mySettingInfo.stopBits);
    mySerialPort->setFlowControl(mySettingInfo.flowControl);
    if (mySerialPort->open(QIODevice::ReadWrite)) {
        emit emitSerialOpenedSetting();
    } else {
        emit emitSerialOpenErrorSetting(mySerialPort->errorString());
    }
}

void SenSerialControlAlgorithmObject::closeSerialPort()
{
    if (mySerialPort->isOpen())
        mySerialPort->close();

    emit emitSerialCloseSetting();
}

void SenSerialControlAlgorithmObject::readData()
{
    QByteArray data = mySerialPort->readAll();
    emit emitSerialDataToConsole(data);
}

void SenSerialControlAlgorithmObject::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        emit emitSerialCriticalErrorSetting(mySerialPort->errorString());
        closeSerialPort();
    }
}

