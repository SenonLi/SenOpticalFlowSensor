#ifndef SENSERIALOBJECT_H
#define SENSERIALOBJECT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QTime>
#include <QCoreApplication>
#include <QDebug>
#include <QElapsedTimer>

#include <QByteArray>

#include <QTimer>

#include "SenSerialSettingDialog.h"
#include "SenBleDongleTalkFundamentals.h"

#define DATA_LOW_8BIT                   0
#define DATA_HIGH_8BIT                  1
#define DONGLE_READ_PACKAGE_MAX_TIMES   5

class SenSerialObject : public QObject
{
    Q_OBJECT
public:
    explicit SenSerialObject(QObject *parent = 0, int givenSerialNum = 9);
    ~SenSerialObject();

    void stopPrepare()  { pixelsBurstBegun = false; motionReadBegun = false;
                          syncNumMotionsCount = 0; emitCount = 0; debugError = false;
                          motionReadBytes = 2 * motionReadSyncNum + 1;
                          motionReadPerTime = 2 * motionReadSyncNum + 1;
                          SenMotionByteArray.clear();
                          SenPixelsBurstByteArray.clear();//
                          SenLineByteArray.clear();//
                          SenLineString = "";
                          initialPixelsBurst = false;
                        }

    QSerialPort *mySerialPort;
    QByteArray SenMotionByteArray;
    bool motionReadBegun;
    QElapsedTimer *myElapsedTimer;

signals:
    void emitSerialCloseSetting();
    void emitSerialOpenedSetting();
    void emitSerialOpenErrorSetting(QString);
    void emitSerialCriticalErrorSetting(QString);

    void emitSerialDataToConsole(QByteArray, int colorNumber);
    // int: 1(Green) for the first, 2(Yellow) for the second;
    // int: 0(Red) for the Error status
    void emitMotionTrackingStart(int);
    void emitArduinoInitialization();
    void emitMotionTrackingStop(qint64, int);

    void emitBurstDataQByteArray(QByteArray SenPixelsBurstByteArray, int emitCount);
    void emitMotionUpdateSignal(QByteArray SenMotionByteArray, int byteArrayLength, int serialObject_ID);
    void emitMotionUpdateSignal(QByteArray SenMotionByteArray, int, bool, int);
    void emitAllSerialImageBurstCommand();

    // Dongle signals
    void emitToShowHexDataFromDongleOnly(QByteArray donglePackage);
    void emitToShowDongleBasicResponseFormat(QByteArray donglePackage);
    void emitToDecodeDonglePackage(QByteArray donglePackage);

public slots:

    void openSerialPort(SerialInfoGroup mySettingInfo);
    void closeSerialPort();
    void readData();
    void readBleDongle();
    void handleError(QSerialPort::SerialPortError error);


    void setBurstBegunFalse()   { pixelsBurstBegun = false;   }
    void stopForGoodNextActive()   { stopForGoodNext = true;   }

private:
    // Dongle Package Decoding functions
    void dongleDecode();
    void generalEventHandler(ushort evtType, QByteArray evtPackageArray);
    void showHexDataToConsole(QByteArray anyUnsignedData);
    void showBasicResponseFormat();
    void hexDataImmeResponseFromDongleToConsole(QByteArray rawData);

    SenBleDongleTalkFundamentals *dongleProtocol;
    QByteArray dongleByteArrary;
    uchar dongleEvtHeader_Low, dongleEvtHeader_High;
    int evtHeaderByteSize, evtLenInfoOccupation;

    void SenDelay(int msec);
    void SenDeadDelay(int msec);

    QString linePixelsDataQString;
    QByteArray SenPixelsBurstByteArray;//
    QByteArray SenLineByteArray;//
    QString SenLineString;

    bool pixelsBurstBegun, stopForGoodNext;
    int motionReadBytes, delta_X, delta_Y;

    int emitCount;
    int serialReadPixelsBurstMax, pixelsBurstSum;
    int pixelsUpdatePerTime, motionReadPerTime, syncNumMotionsCount, motionReadSyncNum = 100;
    int serialObject_ID;

    bool debugError;
    bool initialPixelsBurst;
};

#endif // SENSERIALOBJECT_H
