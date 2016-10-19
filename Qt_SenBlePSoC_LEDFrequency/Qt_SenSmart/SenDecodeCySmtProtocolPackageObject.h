#ifndef SENDECODECYSMTPROTOCOLPACKAGEOBJECT_H
#define SENDECODECYSMTPROTOCOLPACKAGEOBJECT_H

#include <QObject>
#include <QDebug>

#include "SenBleDongleTalkFundamentals.h"

#define DATA_LOW_8BIT                           0
#define DATA_HIGH_8BIT                          1
#define DONGLE_READ_PACKAGE_MAX_TIMES           5
#define Sen_BluetoothDevice_Maximum_Num         4
#define Sen_First_Bluetooth_Device_ID           0x04
#define Sen_Second_Bluetooth_Device_ID          0x03
#define Sen_Third_Bluetooth_Device_ID           0x02
#define Sen_Fourth_Bluetooth_Device_ID          0x01

typedef enum
{
    PRIORITY_HEADER                     =       0,
    PRIORITY_LENGTH_INFO                =       1,
    PRIORITY_EVENT_WITH_PARAMETERS      =       2,

}Raw_Data_Handler_Priorities;

typedef enum
{
    LED_Color_Red                   =       0,
    LED_Color_Green                 =       1,
    LED_Color_Blue                  =       2,
    LED_Color_Off                   =       3,

}Led_Frequency_Color;

typedef enum
{
    EVT_HEADER_BYTE_SIZE        =       2,
    EVT_LENGTH_INFO_OCCUPATION  =       2,
}Basic_Size;


class SenDecodeCySmtProtocolPackageObject : public QObject
{
    Q_OBJECT
public:
//    (QObject *parent = 0, int OF_SensorID = 9);
    explicit SenDecodeCySmtProtocolPackageObject(QObject *parent = 0, int OF_SensorID = 9);
    ~SenDecodeCySmtProtocolPackageObject();

    CYBLE_CONN_HANDLE_T peerBD_Handle[Sen_BluetoothDevice_Maximum_Num];
    int establishedConnectionNum;
    void showHexDataToConsole(QByteArray anyUnsignedData, int arrayLenth);
    CYBLE_DISC_SRVC_INFO_T firstDeviceServicesList[Sen_Max_Services_Number], secondDeviceServicesList[Sen_Max_Services_Number], thirdDeviceServicesList[Sen_Max_Services_Number], fourthDeviceServicesList[Sen_Max_Services_Number];
    CYBLE_DISC_SRVC_INFO_T *metaDeviceServicesList;

signals:
    void emitDataToConsole(QByteArray, int colorNumber);
    void emitSendCmdInitBleStack();
    void emitCmdComplete(uchar cmdOpCodeID);

    void emitSL_GattCMD_LED_WriteLedRed();
    void emitSL_GattCMD_LED_WriteLedGreen();
    void emitSL_GattCMD_LED_WriteLedBlue();
    void emitSL_GattCMD_LED_FrequencyServiceActive();
    void emitGattCMD_MotionServiceActive();
    void emitGapCmdSetConnectionParameters();
    void emitGapCmdSetLocalDeviceSecurity();
    void emitGapCmdStartScan();
    void emitGapCmdStopScan();
    void emitGapCmd_ResolveAndSetPeerAddr();
    void emitGapCmd_EstablishConnection();
    void emitGattCmd_ExchangeGatt_MTU_Size();
    void emitGattCmd_DiscoverAllPrimaryServices();
    void emitGattCmd_DiscoverAllCharacterDescriptors_ByChoosenService(int chosenServiceIndex, int sensorID);
    void emitGapCmd_UpdateConnectionParamResp(QByteArray updateHandle, QByteArray responseByteArray);

    void emitJumpMotionUpdateSignal(QByteArray jumpMotionArray,int arrayLength, int serialObject_ID);
    void emit300PixelsShowUp(QByteArray pixels300Array, int pixels300SectionIndex);

    void emitBandPassModeBegin(uchar bandPassLow, uchar bandPassHigh, uchar bandPassJudge, int mySensorID);

public slots:
    void showBasicResponseFormat(QByteArray rawDonglePackage);
    void decodeDongle(QByteArray rawDonglePackage);
    void slotShowHexDataToConsole(QByteArray anyUnsignedData);

private:    
    int ledColor;
    int mySensorID;
    int evtParamterLenSize;
    bool resolveSetPeerAddAutoAfterScanStop, resolve_SetPeerAddGetGoodResponse;
    bool establishConnectionSuccess;
    bool updateConnctionParaRequestSent, updateConnctionParaRequestSuccess, connectionParameterSuccessfullyUpdated;
    QByteArray bandPassModeInputHint;
    int chosenServiceIndex;

    int firstDeviceServicesCount, secondDeviceServicesCount, thirdDeviceServicesCount, fourthDeviceServicesCount;
    int *metaDeviceServicesCount;
    uchar dongleEvtHeader_Low, dongleEvtHeader_High;

    // decodeBufferPriority System
    QByteArray decodeRawBuffer;

    QByteArray check_rawHeaderPackage, check_rawLengthInfoPackage;
    QByteArray check_after_rawLengthInfoPackage;
    QByteArray dumpCheck;
    QByteArray check_rawEvtWithParamPackage, check_evtPackageArray;
    QByteArray check_after_rawEvtWithParamPackage;
    QByteArray check_wholeEvtWithParamStartPackage;

    int nextRawNeededSize, nextRawDataPriority;

    SenBleDongleTalkFundamentals *dongleProtocol;

    void initialDeviceServicesList();
    void initialMetaDevaceServicesList();


    void priorityHeaderHandler(QByteArray rawHeaderPackage);
    void priorityLengthInfoHandler(QByteArray rawLengthInfoPackage);
    void priorityEvtWithParametersHandler(QByteArray rawEvtWithParamPackage);

    void generalEventHandler(ushort evtType, QByteArray evtPackageArray, int packageLength);
    void gapEventHandler(ushort evtType, uchar cmdOpCodeID, QByteArray evtPackageArray, int packageLength);
    void gattEventHandler(ushort evtType, uchar cmdOpCodeID, QByteArray evtPackageArray, int packageLength);
    void L2CAP_EventHandler(ushort evtType, uchar cmdOpCodeID, QByteArray evtPackageArray, int packageLength);



    QByteArray extract_Y_Bytes_Since_X_From(QByteArray anyUsignedData, int length, int startPoint);
    QByteArray getHexFeedBackStringFromDataStartPointer(uchar* peerBdAddr, int dataLen);
    QByteArray getAddFeedBackStringFromAddStartPointer(uchar* peerBdAddr, int dataLen);

};

#endif // SENDECODECYSMTPROTOCOLPACKAGEOBJECT_H
