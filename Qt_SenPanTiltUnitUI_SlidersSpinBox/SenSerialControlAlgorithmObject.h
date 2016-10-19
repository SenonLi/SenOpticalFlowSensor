#ifndef SENSERIALCONTROLALGORITHMOBJECT_H
#define SENSERIALCONTROLALGORITHMOBJECT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>

#include "SenSettingsDialog.h"

class SenSerialControlAlgorithmObject : public QObject
{
    Q_OBJECT
public:
    explicit SenSerialControlAlgorithmObject(QObject *parent = 0);
    QSerialPort *mySerialPort;

private:

signals:
    void emitSerialCloseSetting();
    void emitSerialOpenedSetting();
    void emitSerialOpenErrorSetting(QString);
    void emitSerialCriticalErrorSetting(QString);
    void emitSerialDataToConsole(QByteArray);

public slots:

    void openSerialPort(SenSettingsDialog::Settings mySettingInfo);
    void closeSerialPort();
    void readData();
    void handleError(QSerialPort::SerialPortError error);

};

#endif // SENSERIALCONTROLALGORITHMOBJECT_H
