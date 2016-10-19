#ifndef SENOPTICALFLOWSENSORSMAINWINDOW_H
#define SENOPTICALFLOWSENSORSMAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QMessageBox>

#include "SenConsole.h"
#include "SenSerialSettingDialog.h"

////
#include <QThread>
#include <QApplication>
#include <QDebug>
#include <QMetaType>
#include <QLayout>
#include <QGridLayout>
#include <QDesktopWidget>
#include <QTimer>

#include "SenSerialObject.h"

#include "SenImageBurstWidget.h"
#include "SenMotionTrackingWidget.h"
#include "SenBleDongleTalkFundamentals.h"
#include "SenDecodeCySmtProtocolPackageObject.h"
////

#define SEND_DONGLE_PACKET  (metaSerialObject->mySerialPort->write)
#define Sen_Ascii_Zero                  48
#define FAKE_KEY_IMAGE_BUTST_START      "0"
#define FAKE_KEY_ASK_DEVICE_ID          "~"
#define FAKE_KEY_INITIAL_BLE_STACK      "*"

namespace Ui {
class SenOpticalFlowSensorsMainWindow;
}

class SenOpticalFlowSensorsMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SenOpticalFlowSensorsMainWindow(QWidget *parent = 0, QString OF_SensorID = "");
    ~SenOpticalFlowSensorsMainWindow();

    void sendCmd_Header()       { SEND_DONGLE_PACKET(dongleProtocol->getCmdPacket_Header());   }
    void sendCmd_GetDeviceId()    { SEND_DONGLE_PACKET(dongleProtocol->getCmdPacket_DeviceID());
//                                    lastCommandNotComplete = true;
                                    mySenConsole->putData((QString("'Get Device ID' request sent\n")).toLatin1(), 1);}
    void sendCmd_InitBleStack()    { SEND_DONGLE_PACKET(dongleProtocol->getCmdPacket_InitBleStack());
                                    mySenConsole->putData((QString("'Initial Ble Stack' request sent\n")).toLatin1(), 1);}
    void sendCmd_GetBleStackVersion()      {   SEND_DONGLE_PACKET(dongleProtocol->getCmdPacket_GetBleStackVersion());
                                   // lastCommandNotComplete = true;// Need to add complete reset in decoding function
                                    mySenConsole->putData((QString("'Get Ble Stack Version' request sent\n")).toLatin1(), 1);}
    void sendCmd_GetRSSI()      {   SEND_DONGLE_PACKET(dongleProtocol->getCmdPacket_GetRSSI());
                                   // lastCommandNotComplete = true;// Need to add complete reset in decoding function
                                    mySenConsole->putData((QString("'Get RSSI' request sent\n")).toLatin1(), 1);}
    // Gap Cmd
    //**************************** Gap:  Scan Section *******************************************************
    void sendGapCmd_SetDeviceIoCapabilities()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_SetDeviceIoCapabilities());
                mySenConsole->putData((QString("'GAP: Set Device Io Capabilities' request (KEYBOARD + DISPLAY) sent\n")).toLatin1(), 1);}

    void sendGapCmd_GetDeviceIoCapabilities()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_GetDeviceIoCapabilities());
                mySenConsole->putData((QString("'GAP: Get Device Io Capabilities' request sent\n")).toLatin1(), 1);}

    void sendGapCmd_SetScanParameters()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_SetScanParameters());
                mySenConsole->putData((QString("'GAP: Set Scan Parameters' request sent\n")).toLatin1(), 1);}

    void sendGapCmd_GetScanParameters()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_GetScanParameters());
                mySenConsole->putData((QString("'GAP: Get Scan Parameters' request sent\n")).toLatin1(), 1);}

    void sendGapCmd_SetConnectionParameters()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_SetConnectionParameters());
                mySenConsole->putData((QString("'GAP: Set Connection Parameters' request sent\n")).toLatin1(), 1);}

    void sendGapCmd_GetConnectionParameters()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_GetConnectionParameters());
                mySenConsole->putData((QString("'GAP: Get Connection Parameters' request sent\n")).toLatin1(), 1);}

    void sendGapCmd_SetLocalDeviceSecurity()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_SetLocalDeviceSecurity());
                mySenConsole->putData((QString("'GAP: Set Local Device Security' request sent\n")).toLatin1(), 1);}

    void sendGapCmd_GetLocalDeviceSecurity()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_GetLocalDeviceSecurity());
                mySenConsole->putData((QString("'GAP: Get Local Device Security' request sent\n")).toLatin1(), 1);}

    void sendGapCmd_GetDevicePublicAddress()
                                {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_GetBleDevicePublicAddress());
                                    mySenConsole->putData((QString("'GAP: Get Device Public Address' request sent\n")).toLatin1(), 1);}
    void sendGapCmd_GetDeviceRandomAddress()
                                {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_GetBleDeviceRandomAddress());
                                    mySenConsole->putData((QString("'GAP: Get Device Random Address' request sent\n")).toLatin1(), 1);}

    void sendGapCmd_SetDevicePublicAddress()
                                {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_SetBleDevicePublicAddress());
                                    mySenConsole->putData((QString("'GAP: Set Device Public Address' request sent\n")).toLatin1(), 1);}
    void sendGapCmd_SetDeviceRandomAddress()
                                {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_SetBleDeviceRandomAddress());
                                    mySenConsole->putData((QString("'GAP: Set Device Random Address' request sent\n")).toLatin1(), 1);}

    void sendGapCmd_GetWhiteList()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_GetWhiteList());
                mySenConsole->putData((QString("'GAP: Get White List' request sent\n")).toLatin1(), 1);}
    void sendGapCmd_GetBondedDevicesList()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_GetBondedDevicesList());
                mySenConsole->putData((QString("'GAP: Get Bonded Devices List' request sent\n")).toLatin1(), 1);}

    void sendGapCmd_StartScan()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_StartScan());
                mySenConsole->putData((QString("'GAP: Start Scan' request sent\n")).toLatin1(), 1);}

    void sendGapCmd_StopScan()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_StopScan());
                mySenConsole->putData((QString("'GAP: Stop Scan' request sent\n")).toLatin1(), 1);}

    //**************************** Gap:  Connect Section *******************************************************
    void sendGapCmd_ResolveAndSetPeerAddr()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_ResolveAndSetPeerAddr());
                mySenConsole->putData((QString("'GAP: Resolve & Set Peer BD Address' request sent\n")).toLatin1(), 1);}
    void sendGapCmd_EstablishConnection()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_EstablishConnection());
                mySenConsole->putData((QString("'GAP: Establish Connection' request sent\n")).toLatin1(), 1);}

    void SL_GapCMD_UpdateConnectionParamResp(QByteArray updateHandle, QByteArray responseByteArray)    {   sendCmd_Header();   sendGapCmd_UpdateConnectionParamResp_Header();   sendCmd_Handle(updateHandle);  sendGattCmd_UpdateConnectionParamResp_Response(responseByteArray);    }
    void SL_GapCMD_TerminateConnection()    {    sendCmd_Header();   sendGapCmd_TerminateConnection();    }

    // Gatt Cmd
    //*********************************** Gatt *******************************************************
    void SL_GattCMD_ExchangeGatt_MTU_Size()    {   sendCmd_Header();   sendGattCmd_ExchangeGatt_MTU_Size_Header();   sendGattCmd_LastConnected_ConnectionHandle();  sendGattCmd_ExchangeGatt_MTU_Size_SetMTU();    }
    void SL_GattCMD_DiscoverAllPrimaryServices()    {   sendCmd_Header();   sendGattCmd_DiscoverAllPrimaryServices();   sendGattCmd_LastConnected_ConnectionHandle();   }
    void SL_GattCMD_ReadUsingCharacteristicUuid() {  sendCmd_Header();   sendGattCmd_ReadUsingCharacteristicUuid_Header();   sendGattCmd_LastConnected_ConnectionHandle();   sendGattCmd_ReadUsingCharacteristicUuid_TypeValueHandleRange();    }
    void SL_GattCMD_FindIncludedServices()    {    sendCmd_Header();   sendGattCmd_FindIncludedServices_Header();    sendGattCmd_LastConnected_ConnectionHandle();  sendGattCmd_InputServiceRange(1, 1); }//Need further coding
    void SL_GattCMD_DiscoverAllCharacteristics_ByChoosenService(int inputServiceIndex, int sensorID) {
                    mySenConsole->putData((QString("'GATT: Discover All Characteristics' for Service ")).toLatin1(), 1);
                    mySenConsole->putData(QString::number(inputServiceIndex).toLatin1(), 1);
                    mySenConsole->putData((QString(" request sent\n")).toLatin1(), 1);
                    sendCmd_Header();   sendGattCmd_DiscoverAllCharacteristics_Header();   sendGattCmd_LastConnected_ConnectionHandle();   sendGattCmd_InputServiceRange(inputServiceIndex, sensorID);
                }
    void SL_GattCMD_DiscoverAllCharacterDescriptors_ByChoosenService(int inputServiceIndex, int sensorID) {
                    mySenConsole->putData((QString("'GATT: Discover All Characteristics Descriptors' for Service ")).toLatin1(), 1);
                    mySenConsole->putData(QString::number(inputServiceIndex).toLatin1(), 1);
                    mySenConsole->putData((QString(" request sent\n")).toLatin1(), 1);
                    sendCmd_Header();   sendGattCmd_DiscoverAllCharacterDescriptors_Header();   sendGattCmd_LastConnected_ConnectionHandle();   sendGattCmd_InputServiceRange(inputServiceIndex, sensorID);
                }
    void SL_GattCMD_MotionServiceActive()    {   sendCmd_Header();   sendGattCmd_WriteCharacteristicDescriptor_Header();   sendGattCmd_WriteCharacteristicDescriptor_MotionServiceHandle();  sendGattCmd_WriteCharacteristicDescriptor_ServiceActive();   }
    void SL_GattCMD_MotionServiceStop()    {   sendCmd_Header();   sendGattCmd_WriteCharacteristicDescriptor_Header();   sendGattCmd_WriteCharacteristicDescriptor_MotionServiceHandle();  sendGattCmd_WriteCharacteristicDescriptor_ServiceStop();   }

    void SL_GattCMD_ImageBurstServiceActive()    {   sendCmd_Header();   sendGattCmd_WriteCharacteristicDescriptor_Header();   sendGattCmd_WriteCharacteristicDescriptor_ImageBurstServiceHandle();  sendGattCmd_WriteCharacteristicDescriptor_ServiceActive();   }
    void SL_GattCMD_ImageBurstServiceStop()    {   sendCmd_Header();   sendGattCmd_WriteCharacteristicDescriptor_Header();   sendGattCmd_WriteCharacteristicDescriptor_ImageBurstServiceHandle();  sendGattCmd_WriteCharacteristicDescriptor_ServiceStop();   }

    void SenDelay(int msec);

private slots:
    void about();
////
    void slotSerialBandPassModeBigin(uchar low, uchar high, uchar judge, int mySensorID) { bandPassInputMode = true; bandPassLow = low; bandPassHigh = high; bandPassJudge = judge; bandPassSensorID = mySensorID;}

    void slotSL_GattCMD_MotionServiceActive() { SL_GattCMD_MotionServiceActive(); }
    void slotSendGapCmd_UpdateConnectionParamResp(QByteArray updateHandle, QByteArray responseByteArray)  {   SL_GapCMD_UpdateConnectionParamResp(updateHandle, responseByteArray);   }
    void slotSendGattCmd_DiscoverAllCharacterDescriptors_ByChoosenService(int inputServiceIndex, int sensorID)  {   SL_GattCMD_DiscoverAllCharacterDescriptors_ByChoosenService(inputServiceIndex, sensorID);   }
    void slotSendGattCmd_DiscoverAllPrimaryServices(){   sendCmd_Header();   sendGattCmd_DiscoverAllPrimaryServices(); sendGattCmd_LastConnected_ConnectionHandle();}
    void slotSendGattCmd_ExchangeGatt_MTU_Size()     {   sendCmd_Header();   sendGattCmd_ExchangeGatt_MTU_Size_Header(); sendGattCmd_LastConnected_ConnectionHandle(); sendGattCmd_ExchangeGatt_MTU_Size_SetMTU(); }
    void slotSendGapCmd_EstablishConnection()       {   sendCmd_Header();   sendGapCmd_EstablishConnection();   }
    void slotSendGapCmd_ResolveAndSetPeerAddr()     {   sendCmd_Header();   sendGapCmd_ResolveAndSetPeerAddr();   }
    void slotSendGapCmd_SetConnectionParameters()   {   sendCmd_Header();   sendGapCmd_SetConnectionParameters();   }
    void slotSendGapCmd_SetLocalDeviceSecurity()    {   sendCmd_Header();   sendGapCmd_SetLocalDeviceSecurity();    }
    void slotSendGapCmd_StartScan()                 {   sendCmd_Header();   sendGapCmd_StartScan(); }
    void slotSendGapCmd_StopScan()                  {   sendCmd_Header();   sendGapCmd_StopScan(); }
    void slotBleSendCmdInitBleStack()                  {   sendCmd_Header();  sendCmd_InitBleStack(); }



    void SenBleSendCmdInitBleStack();
    void processCmdCompleteSignal(uchar event_ID);

    void writeDataToMultiSerialPorts(const QByteArray &data);
    void setSerialCloseStatus();
    void setSerialOpenedStatus();
    void setSerialOpenErrorStatus(QString serialOpenErrorString);
    void setSerialCriticalErrorStatus(QString serialCriticalErrorString);
    void openAssignedSerialPortIfChanged();
    void showDataInConsole(QByteArray data, int colorNumber);

    void initialMotionFileRecordObjectFeature(int serialObject_ID);
    void reportMotionFileRecordObjectFeature(qint64 elapsedTime, int serialObject_ID);

    void openMultiSerialObjectsPorts();
    void closeMultiSerialObjectsPorts();

    void startAllSerialImageBurstCommand();

signals:
    void emitOpenSerialOrder(SerialInfoGroup);
////

private:

    void sendCmd_Handle(QByteArray handleByteArray)     {   SEND_DONGLE_PACKET(handleByteArray);    }
    /********** Gap Part Command ***************************/
    void sendGapCmd_UpdateConnectionParamResp_Header()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_UpdateConnectionParamResp_Header()); }

    void sendGattCmd_UpdateConnectionParamResp_Response(QByteArray responseByteArray)   {
        SEND_DONGLE_PACKET(responseByteArray);
        mySenConsole->putData((QString("'GAP: Update Connection Parameter Response'  ")).toLatin1(), 1);
        mySenConsole->putData(responseByteArray.toHex(), 1);
        mySenConsole->putData((QString("  request sent\n")).toLatin1(), 1);
    }
    void sendGapCmd_TerminateConnection()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGapCmdPacket_TerminateConnection());
                mySenConsole->putData((QString("'GAP: Terminate Connection' request sent\n")).toLatin1(), 1);}

    /********** Gatt Part Command ***************************/
    void sendGattCmd_LastConnected_ConnectionHandle()
            {   SEND_DONGLE_PACKET(getMiddlePacket_LastReceivedHandleInfo());}
    void sendGattCmd_InputServiceRange(int inputServiceIndex, int sensorID)
            {
                SEND_DONGLE_PACKET(getFinalChosenPacket_InputServiceRange(inputServiceIndex, sensorID));             
//                SEND_DONGLE_PACKET(getTestPacket_InputServiceRange());
            }

    void sendGattCmd_ExchangeGatt_MTU_Size_Header()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_ExchangeGatt_MTU_Size_Header()); }

    void sendGattCmd_ExchangeGatt_MTU_Size_SetMTU()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_ExchangeGatt_MTU_Size_SetMTU());
                mySenConsole->putData((QString("'GATT: Exchange Gatt MTU Size' request sent\n")).toLatin1(), 1);}

    void sendGattCmd_ReadUsingCharacteristicUuid_Header()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_ReadUsingCharacteristicUuid_Header()); }
    void sendGattCmd_ReadUsingCharacteristicUuid_TypeValueHandleRange()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_ReadUsingCharacteristicUuid_TypeValueHandleRange());
                mySenConsole->putData((QString("'GATT: Read Using Characteristic Uuid' request sent\n")).toLatin1(), 1);}

    void sendGattCmd_DiscoverAllPrimaryServices()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_DiscoverAllPrimaryServices());
                mySenConsole->putData((QString("'GATT: Discover All Primary Services' request sent\n")).toLatin1(), 1);}

    void sendGattCmd_FindIncludedServices_Header()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_FindIncludedServices_Header());
                mySenConsole->putData((QString("'GATT: Find Included Services' No. # request sent\n")).toLatin1(), 1);}

    void sendGattCmd_DiscoverAllCharacteristics_Header()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_DiscoverAllCharacteristics_Header());      }
    void sendGattCmd_DiscoverAllCharacterDescriptors_Header()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_DiscoverAllCharacterDescriptors_Header());  }

    void sendGattCmd_WriteCharacteristicDescriptor_Header()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_WriteCharacteristicDescriptor_Header());  }
    void sendGattCmd_WriteCharacteristicDescriptor_MotionServiceHandle()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_WriteCharacteristicDescriptor_MotionServiceHandle());  }
    void sendGattCmd_WriteCharacteristicDescriptor_ImageBurstServiceHandle()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_WriteCharacteristicDescriptor_ImageBurstServiceHandle());  }

    void sendGattCmd_WriteCharacteristicDescriptor_ServiceActive()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_WriteCharacteristicDescriptor_ServiceActive());  }
    void sendGattCmd_WriteCharacteristicDescriptor_ServiceStop()
            {   SEND_DONGLE_PACKET(dongleProtocol->getGattCmdPacket_WriteCharacteristicDescriptor_ServiceStop());  }

    /*************************************************************************************************************************/
    int senStopPreparationTime;

    void multiSerialsInitialAndOpen();
    void initUI_Connections();
    void initMetaSerialConnections();
    void openMetaSerialPort(QString metaSerialInfoName);
    void writeDataToMetaSerialPort(const QByteArray &data);

    QByteArray getTestPacket_InputServiceRange();
    QByteArray getFinalChosenPacket_InputServiceRange(int inputServiceIndex, int sensorID);
    QByteArray getMiddlePacket_LastReceivedHandleInfo();
    void autoTimerAskDeviceID();

    uchar bandPassLow, bandPassHigh, bandPassJudge;
    bool lastCommandNotComplete, bandPassInputMode;
    int bandPassSensorID;

    Ui::SenOpticalFlowSensorsMainWindow *myMainWindow_ui;
    SenConsole *mySenConsole;

    SenSerialSettingDialog *mySenSettingsDialog;
    SenBleDongleTalkFundamentals *dongleProtocol;

    QString mySensorID;
    QThread *donglePackageDecodingThread;
    SerialInfoGroup mySettingInfo;

    SenSerialObject *metaSerialObject, *myFirstSerialObject, *mySecondSerialObject;
    SenDecodeCySmtProtocolPackageObject *metaDongleDecodingObject, *firstDglDecObject, *secondDglDecObject;


    SenImageBurstWidget *metaImageWidget, *firstImageWidegt, *secondImageWidget;

    SenMotionTrackingWidget *myMotionTrackingWidget;
////
};

#endif // SENOPTICALFLOWSENSORSMAINWINDOW_H
