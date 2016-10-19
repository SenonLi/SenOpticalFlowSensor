#ifndef SENPOSITIONTRACKINGWIDGET_H
#define SENPOSITIONTRACKINGWIDGET_H


#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPointF>
#include <QPen>
#include <QDebug>
#include <QElapsedTimer>
#include <QTimer>
#include <QLineF>

#include <QFile>
#include <QTextStream>
#include <iostream>

/****  For Transfer Start  *********************/
#include <QThread>
#include <QApplication>
#include <QMetaType>
#include <QLayout>
#include <QDesktopWidget>
#include <QMenu>
#include <QMouseEvent>

#include "SenSerialObject.h"
#include "SenSerialSettingDialog.h"
#include "SenBleDongleTalkFundamentals.h"
#include "SenDecodeCySmtProtocolPackageObject.h"

//#include "SenImageBurstWidget.h"

#define SEND_DONGLE_PACKET  (metaSerialObject->mySerialPort->write)
#define Sen_Ascii_Zero                  48
#define FAKE_KEY_IMAGE_BUTST_START      "0"
#define FAKE_KEY_ASK_DEVICE_ID          "~"
#define FAKE_KEY_INITIAL_BLE_STACK      "*"

/****  For Transfer End  ************************/

class SenMotionTrackingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SenMotionTrackingWidget(QWidget *parent = 0);
    ~SenMotionTrackingWidget();

//    /****  For Transfer Start  *****************************************************************************************************************************************************************************************************************************************************************/

//    void sendCmd_Header()       { SEND_DONGLE_PACKET(dongleProtocol->getCmdPacket_Header());   }
//    void sendCmd_GetDeviceId()    { SEND_DONGLE_PACKET(dongleProtocol->getCmdPacket_DeviceID());  }
//    void sendCmd_InitBleStack()    { SEND_DONGLE_PACKET(dongleProtocol->getCmdPacket_InitBleStack());}
//    void sendCmd_GetBleStackVersion()      {   SEND_DONGLE_PACKET(dongleProtocol->getCmdPacket_GetBleStackVersion());  }
//    void sendCmd_GetRSSI()      {   SEND_DONGLE_PACKET(dongleProtocol->getCmdPacket_GetRSSI()); }
//    // Gap Cmd
//    //**************************** Gap:  Scan Section *******************************************************
//    void sendGapCmd_SetDeviceIoCapabilities()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_SetDeviceIoCapabilities()); }

//    void sendGapCmd_GetDeviceIoCapabilities()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_GetDeviceIoCapabilities()); }

//    void sendGapCmd_SetScanParameters()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_SetScanParameters()); }

//    void sendGapCmd_GetScanParameters()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_GetScanParameters()); }

//    void sendGapCmd_SetConnectionParameters()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_SetConnectionParameters()); }

//    void sendGapCmd_GetConnectionParameters()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_GetConnectionParameters()); }

//    void sendGapCmd_SetLocalDeviceSecurity()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_SetLocalDeviceSecurity()); }

//    void sendGapCmd_GetLocalDeviceSecurity()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_GetLocalDeviceSecurity()); }

//    void sendGapCmd_GetDevicePublicAddress()
//                                {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_GetBleDevicePublicAddress()); }
//    void sendGapCmd_GetDeviceRandomAddress()
//                                {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_GetBleDeviceRandomAddress()); }

//    void sendGapCmd_SetDevicePublicAddress()
//                                {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_SetBleDevicePublicAddress()); }
//    void sendGapCmd_SetDeviceRandomAddress()
//                                {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_SetBleDeviceRandomAddress()); }

//    void sendGapCmd_GetWhiteList()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_GetWhiteList()); }
//    void sendGapCmd_GetBondedDevicesList()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_GetBondedDevicesList()); }

//    void sendGapCmd_StartScan()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_StartScan()); }

//    void sendGapCmd_StopScan()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_StopScan()); }

//    //**************************** Gap:  Connect Section *******************************************************
//    void sendGapCmd_ResolveAndSetPeerAddr()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_ResolveAndSetPeerAddr()); }
//    void sendGapCmd_EstablishConnection()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_EstablishConnection()); }

//    void SL_GapCMD_UpdateConnectionParamResp(QByteArray updateHandle, QByteArray responseByteArray)    {   sendCmd_Header();   sendGapCmd_UpdateConnectionParamResp_Header();   sendCmd_Handle(updateHandle);  sendGattCmd_UpdateConnectionParamResp_Response(responseByteArray);    }
//    void SL_GapCMD_TerminateConnection()    {    sendCmd_Header();   sendGapCmd_TerminateConnection();    }

//    // Gatt Cmd
//    //*********************************** Gatt *******************************************************
//    void SL_GattCMD_ExchangeGatt_MTU_Size()    {   sendCmd_Header();   sendGattCmd_ExchangeGatt_MTU_Size_Header();   sendGattCmd_LastConnected_ConnectionHandle();  sendGattCmd_ExchangeGatt_MTU_Size_SetMTU();    }
//    void SL_GattCMD_DiscoverAllPrimaryServices()    {   sendCmd_Header();   sendGattCmd_DiscoverAllPrimaryServices();   sendGattCmd_LastConnected_ConnectionHandle();   }
//    void SL_GattCMD_ReadUsingCharacteristicUuid() {  sendCmd_Header();   sendGattCmd_ReadUsingCharacteristicUuid_Header();   sendGattCmd_LastConnected_ConnectionHandle();   sendGattCmd_ReadUsingCharacteristicUuid_TypeValueHandleRange();    }
//    void SL_GattCMD_FindIncludedServices()    {    sendCmd_Header();   sendGattCmd_FindIncludedServices_Header();    sendGattCmd_LastConnected_ConnectionHandle();  sendGattCmd_InputServiceRange(1, 1); }//Need further coding
//    void SL_GattCMD_DiscoverAllCharacteristics_ByChoosenService(int inputServiceIndex, int sensorID) {
//                    sendCmd_Header();   sendGattCmd_DiscoverAllCharacteristics_Header();   sendGattCmd_LastConnected_ConnectionHandle();   sendGattCmd_InputServiceRange(inputServiceIndex, sensorID);
//                }
//    void SL_GattCMD_DiscoverAllCharacterDescriptors_ByChoosenService(int inputServiceIndex, int sensorID) {
//                    sendCmd_Header();   sendGattCmd_DiscoverAllCharacterDescriptors_Header();   sendGattCmd_LastConnected_ConnectionHandle();   sendGattCmd_InputServiceRange(inputServiceIndex, sensorID);
//                }
//    void SL_GattCMD_MotionServiceActive()    {   sendCmd_Header();   sendGattCmd_WriteCharacteristicDescriptor_Header();   sendGattCmd_WriteCharacteristicDescriptor_MotionServiceHandle();  sendGattCmd_WriteCharacteristicDescriptor_ServiceActive();   }
//    void SL_GattCMD_MotionServiceStop()    {   sendCmd_Header();   sendGattCmd_WriteCharacteristicDescriptor_Header();   sendGattCmd_WriteCharacteristicDescriptor_MotionServiceHandle();  sendGattCmd_WriteCharacteristicDescriptor_ServiceStop();   }

//    void SL_GattCMD_ImageBurstServiceActive()    {   sendCmd_Header();   sendGattCmd_WriteCharacteristicDescriptor_Header();   sendGattCmd_WriteCharacteristicDescriptor_ImageBurstServiceHandle();  sendGattCmd_WriteCharacteristicDescriptor_ServiceActive();   }
//    void SL_GattCMD_ImageBurstServiceStop()    {   sendCmd_Header();   sendGattCmd_WriteCharacteristicDescriptor_Header();   sendGattCmd_WriteCharacteristicDescriptor_ImageBurstServiceHandle();  sendGattCmd_WriteCharacteristicDescriptor_ServiceStop();   }

//    void SenDelay(int msec);
//    /****  For Transfer End  **************************************************************************************************************************************************************************************************************************************************************/

    void initialization()   { firstSensorPointF.setX(0); firstSensorPointF.setY(0);
                             secondSensorPointF.setX(-10); secondSensorPointF.setY(-10);
                             thirdSensorPointF.setX(-10); thirdSensorPointF.setY(10);
                             fourthSensorPointF.setX(10); fourthSensorPointF.setY(10);
                             update(); }

public slots:
    void SenMotionUpdate(QByteArray syncSamplesByteArray, int byteArrayLength, int serialObject_ID);
    void initializationSlot()   { firstSensorPointF.setX(0); firstSensorPointF.setY(0);
                             secondSensorPointF.setX(-10); secondSensorPointF.setY(-10);
                             thirdSensorPointF.setX(-10); thirdSensorPointF.setY(10);
                             fourthSensorPointF.setX(10); fourthSensorPointF.setY(10);
                             update(); }

protected:
    void paintEvent(QPaintEvent *event);


//    /****  For Transfer Start  *********************/
//    void mousePressEvent(QMouseEvent *event) {
//        if (event->button() == Qt::RightButton){
//            if (serialDialogMenu){
//                serialDialogMenu->popup(event->globalPos());
//            }
//        }
//    }


//    /****  For Transfer End  ************************/

signals:

private:
    QPointF firstSensorPointF, secondSensorPointF, thirdSensorPointF, fourthSensorPointF;
    QPen firstSensorPointPen, secondSensorPointPen, thirdSensorPointPen, fourthSensorPointPen;
    QPen linePen;

    QElapsedTimer *railUpdateElapsedTimer;
    QLineF horizontalLine, verticalLine;


//    /****  For Transfer Start  *****************************************************************************************************************************************************************************************************************************************************************/

//    void sendCmd_Handle(QByteArray handleByteArray)     {   SEND_DONGLE_PACKET(handleByteArray);    }
//    /********** Gap Part Command ***************************/
//    void sendGapCmd_UpdateConnectionParamResp_Header()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_UpdateConnectionParamResp_Header()); }

//    void sendGattCmd_UpdateConnectionParamResp_Response(QByteArray responseByteArray)   {
//        SEND_DONGLE_PACKET(responseByteArray);
//    }
//    void sendGapCmd_TerminateConnection()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_TerminateConnection()); }

//    /********** Gatt Part Command ***************************/
//    void sendGattCmd_LastConnected_ConnectionHandle()
//            {   SEND_DONGLE_PACKET(getMiddlePacket_LastReceivedHandleInfo());}
//    void sendGattCmd_InputServiceRange(int inputServiceIndex, int sensorID)
//            {
//                SEND_DONGLE_PACKET(getFinalChosenPacket_InputServiceRange(inputServiceIndex, sensorID));
////                SEND_DONGLE_PACKET(getTestPacket_InputServiceRange());
//            }

//    void sendGattCmd_ExchangeGatt_MTU_Size_Header()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_ExchangeGatt_MTU_Size_Header()); }

//    void sendGattCmd_ExchangeGatt_MTU_Size_SetMTU()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_ExchangeGatt_MTU_Size_SetMTU()); }

//    void sendGattCmd_ReadUsingCharacteristicUuid_Header()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_ReadUsingCharacteristicUuid_Header()); }
//    void sendGattCmd_ReadUsingCharacteristicUuid_TypeValueHandleRange()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_ReadUsingCharacteristicUuid_TypeValueHandleRange()); }

//    void sendGattCmd_DiscoverAllPrimaryServices()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_DiscoverAllPrimaryServices()); }

//    void sendGattCmd_FindIncludedServices_Header()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_FindIncludedServices_Header()); }

//    void sendGattCmd_DiscoverAllCharacteristics_Header()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_DiscoverAllCharacteristics_Header());      }
//    void sendGattCmd_DiscoverAllCharacterDescriptors_Header()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_DiscoverAllCharacterDescriptors_Header());  }

//    void sendGattCmd_WriteCharacteristicDescriptor_Header()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_WriteCharacteristicDescriptor_Header());  }
//    void sendGattCmd_WriteCharacteristicDescriptor_MotionServiceHandle()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_WriteCharacteristicDescriptor_MotionServiceHandle());  }
//    void sendGattCmd_WriteCharacteristicDescriptor_ImageBurstServiceHandle()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_WriteCharacteristicDescriptor_ImageBurstServiceHandle());  }

//    void sendGattCmd_WriteCharacteristicDescriptor_ServiceActive()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_WriteCharacteristicDescriptor_ServiceActive());  }
//    void sendGattCmd_WriteCharacteristicDescriptor_ServiceStop()
//            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_WriteCharacteristicDescriptor_ServiceStop());  }


//    QByteArray getTestPacket_InputServiceRange();
//    QByteArray getFinalChosenPacket_InputServiceRange(int inputServiceIndex, int sensorID);
//    QByteArray getMiddlePacket_LastReceivedHandleInfo();
//    void autoTimerAskDeviceID();

//    void multiSerialsInitialAndOpen();
//    void initMetaSerialConnections();

//    void openMetaSerialPort(QString metaSerialInfoName);
//    void writeDataToMetaSerialPort(const QByteArray &data);

//    QMenu *serialDialogMenu;

//    uchar bandPassLow, bandPassHigh, bandPassJudge;
//    bool bandPassInputMode;//, lastCommandNotComplete
//    int bandPassSensorID;

//    QString mySensorID;
//    QThread *donglePackageDecodingThread;
//    SerialInfoGroup mySettingInfo;

//    SenSerialSettingDialog *mySenSettingsDialog;
//    SenBleDongleTalkFundamentals *dongleProtocol;
//    SenSerialObject *metaSerialObject, *myFirstSerialObject, *mySecondSerialObject;
//    SenDecodeCySmtProtocolPackageObject *metaDongleDecodingObject, *firstDglDecObject, *secondDglDecObject;

//private slots:
//    void onSerialConfigureTrigered();

//    void openMultiSerialObjectsPorts();
//    void writeDataToMultiSerialPorts(const QByteArray &data);
//    void closeMultiSerialObjectsPorts();
//    void setSerialCloseStatus();

//    void setSerialOpenErrorStatus(QString serialOpenErrorString);

//    void initialMotionFileRecordObjectFeature(int serialObject_ID);
//    void reportMotionFileRecordObjectFeature(qint64 elapsedTime, int serialObject_ID);

//    void openAssignedSerialPortIfChanged();



////    void setSerialCriticalErrorStatus(QString serialCriticalErrorString);
////    void showDataInConsole(QByteArray data, int colorNumber);
////    void startAllSerialImageBurstCommand();


//    void slotSerialBandPassModeBigin(uchar low, uchar high, uchar judge, int mySensorID) { bandPassInputMode = true; bandPassLow = low; bandPassHigh = high; bandPassJudge = judge; bandPassSensorID = mySensorID;}
//    void slotSL_GattCMD_MotionServiceActive() { SL_GattCMD_MotionServiceActive(); }

//    void slotSendGapCmd_UpdateConnectionParamResp(QByteArray updateHandle, QByteArray responseByteArray)  {   SL_GapCMD_UpdateConnectionParamResp(updateHandle, responseByteArray);   }
//    void slotSendGattCmd_DiscoverAllCharacterDescriptors_ByChoosenService(int inputServiceIndex, int sensorID)  {   SL_GattCMD_DiscoverAllCharacterDescriptors_ByChoosenService(inputServiceIndex, sensorID);   }
//    void slotSendGattCmd_DiscoverAllPrimaryServices(){   sendCmd_Header();   sendGattCmd_DiscoverAllPrimaryServices(); sendGattCmd_LastConnected_ConnectionHandle();}
//    void slotSendGattCmd_ExchangeGatt_MTU_Size()     {   sendCmd_Header();   sendGattCmd_ExchangeGatt_MTU_Size_Header(); sendGattCmd_LastConnected_ConnectionHandle(); sendGattCmd_ExchangeGatt_MTU_Size_SetMTU(); }
//    void slotSendGapCmd_EstablishConnection()       {   sendCmd_Header();   sendGapCmd_EstablishConnection();   }
//    void slotSendGapCmd_ResolveAndSetPeerAddr()     {   sendCmd_Header();   sendGapCmd_ResolveAndSetPeerAddr();   }
//    void slotSendGapCmd_SetConnectionParameters()   {   sendCmd_Header();   sendGapCmd_SetConnectionParameters();   }
//    void slotSendGapCmd_SetLocalDeviceSecurity()    {   sendCmd_Header();   sendGapCmd_SetLocalDeviceSecurity();    }
//    void slotSendGapCmd_StartScan()                 {   sendCmd_Header();   sendGapCmd_StartScan(); }
//    void slotSendGapCmd_StopScan()                  {   sendCmd_Header();   sendGapCmd_StopScan(); }
//    void slotBleSendCmdInitBleStack()                  {   sendCmd_Header();  sendCmd_InitBleStack(); }

//    /****  For Transfer End  ************************/

};















//#include <QWidget>
//#include <QPainter>
//#include <QPaintEvent>
//#include <QPointF>
//#include <QPen>
//#include <QDebug>
//#include <QElapsedTimer>
//#include <QTimer>

//#include <QFile>
//#include <QTextStream>
//#include <iostream>

//class SenMotionTrackingWidget : public QWidget
//{
//    Q_OBJECT
//public:
//    explicit SenMotionTrackingWidget(QWidget *parent = 0);
//    ~SenMotionTrackingWidget();

//    void initialization()   { firstSensorPointF.setX(10); firstSensorPointF.setY(-10);
//                             secondSensorPointF.setX(-10); secondSensorPointF.setY(-10);
//                             thirdSensorPointF.setX(-10); thirdSensorPointF.setY(10);
//                             fourthSensorPointF.setX(10); fourthSensorPointF.setY(10);
//                             update(); }

//protected:
//    void paintEvent(QPaintEvent *event);

//private:
//    QPointF firstSensorPointF, secondSensorPointF, thirdSensorPointF, fourthSensorPointF;
//    QPen firstSensorPointPen, secondSensorPointPen, thirdSensorPointPen, fourthSensorPointPen;

//    QElapsedTimer *railUpdateElapsedTimer;

//signals:

//public slots:
//    void SenMotionUpdate(QByteArray syncSamplesByteArray, int byteArrayLength, int serialObject_ID);
//    void initializationSlot()   { firstSensorPointF.setX(10); firstSensorPointF.setY(-10);
//                             secondSensorPointF.setX(-10); secondSensorPointF.setY(-10);
//                             thirdSensorPointF.setX(-10); thirdSensorPointF.setY(10);
//                             fourthSensorPointF.setX(10); fourthSensorPointF.setY(10);
//                             update(); }
//};

#endif // SENPOSITIONTRACKINGWIDGET_H
