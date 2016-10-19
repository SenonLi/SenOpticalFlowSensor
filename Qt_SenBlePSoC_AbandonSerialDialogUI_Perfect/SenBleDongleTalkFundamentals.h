#ifndef SENBLEDONGLETALKFUNDAMENTALS_H
#define SENBLEDONGLETALKFUNDAMENTALS_H

#include <QObject>

#include <QMetaType>
#include <QSerialPort>
#include <QByteArray>

#define BLE_DONGLE_ID               0x00000001
#define Sen_EVT_TYPE_LOW_8BIT_MASK  0x80
#define Sen_EVT_GAP_OUTOF_GAPGATT   0x80
#define Sen_CMD_ID_LOW_8BIT_MASK    0x7F
#define Sen_CMD_ID_HIGH_8BIT_MASK   0x03
#define Sen_CMD_GAP_OR_GATT_MASK    0x80
#define Sen_CMD_GAP_GROUP           0X80
#define Sen_Max_Services_Number             10
#define Sen_Max_Characteristics_Number      10
#define Sen_Max_Descriptors_Number          10

#define CYBLE_L2CAP_CONN_PARAM_ACCEPTED   			0x0000
#define CYBLE_L2CAP_CONN_PARAM_REJECTED   			0x0001
#define BandPassJudge_AfterGotAllServices   1
#define CYBLE_GATT_16_BIT_UUID_FORMAT       0x01
#define CYBLE_GATT_128_BIT_UUID_FORMAT      0x02
#define HEADERS_TOTAL_NUM                   5
#define EVT_OPCODE_TOTAL_NUM                50
#define GENERAL_CMD_OPCODE_TOTAL_NUM        256// upto one byte for now
#define GAP_CMD_OPCODE_TOTAL_NUM            256// upto one byte for now
#define GATT_CMD_OPCODE_TOTAL_NUM           256// upto one byte for now
#define L2CAP_CMD_OPCODE_TOTAL_NUM          256// upto one byte for now
#define STATUS_TOTAL_NUM                    256

#define CYBLE_GAP_BD_ADDR_SIZE              6
#define GAPC_ADV_REPORT_T_STRUCKTURE_SIZE   10
#define CONNECTION_PARAMETERS_NOTIFICATION_Parameter_Length     9

#define Sen_BLE_ADV_ATT_Type                0x09
#define TimeIntv_Calculation_Base_Byte      0x0006
#define TimeIntv_Calculation_Base           750 //7.5ms
#define TimeIntv_Calculation_PerIncrease    125 //1.25ms

typedef enum{
    CMD_OPCODE_LOW               =       0,
    CMD_OPCODE_HIGH              =       1,
    CMD_PARA_LEN_LOW             =       2,
    CMD_PARA_LEN_HIGH            =       3,
    CMD_PARAMETERS_BEGINNING     =       4
}CMD_Packet_Index;

#define CMD_PACKET_TO_DONGLE_HEADER         0
#define DONGLE_EVENT_HEADER                 1

struct SerialInfoGroup {
    QString name;
    qint32 baudRate;
    QString stringBaudRate;
    QSerialPort::DataBits dataBits;
    QString stringDataBits;
    QSerialPort::Parity parity;
    QString stringParity;
    QSerialPort::StopBits stopBits;
    QString stringStopBits;
    QSerialPort::FlowControl flowControl;
    QString stringFlowControl;
    bool localEchoEnabled;
};
Q_DECLARE_METATYPE(SerialInfoGroup);

class SenBleDongleTalkFundamentals : public QObject
{
    Q_OBJECT
public:
    explicit SenBleDongleTalkFundamentals(QObject *parent = 0);
    QByteArray SenPSoC_BleAddress;

    SerialInfoGroup getDefaultSerialInfo()  { return defaultSettingsInfo;   }
    SerialInfoGroup getFirstSerialInfo()  { return firstSerialInfo;   }
    SerialInfoGroup getSecondSerialInfo()  { return secondSerialInfo;   }

    uchar get_L8(uchar* dataPointer);
    uchar get_H8(uchar* dataPointer);

    uchar getHeaderFromArray_Low(int headerIndex)    { return get_L8((uchar*)&headerArray[headerIndex]);}
    uchar getHeaderFromArray_High(int headerIndex)   { return get_H8((uchar*)&headerArray[headerIndex]);}

    QString getGeneralCmdStringFromArray(int generalCmdOpCodeIndex)   {   return  generalCmdStringArray[generalCmdOpCodeIndex]; }
    QString getGapCmdStringFromArray(int gapCmdOpCodeIndex)   {   return gapCmdStringArray[gapCmdOpCodeIndex]; }
    QString getGattCmdStringFromArray(int gattCmdOpCodeIndex)   {   return gattCmdStringArray[gattCmdOpCodeIndex]; }
    QString getL2CAP_CmdStringFromArray(int L2CAP_CmdOpCodeIndex)   {   return  L2CAP_CmdStringArray[L2CAP_CmdOpCodeIndex]; }
    QString getStatusStringFromArray(int evtStatusIndex)    {   return  statusStringArray[evtStatusIndex];  }

    QByteArray getCmdPacket_Header()    { return cmdPacketHeader; }
    // Get General Command Packet
    QByteArray getCmdPacket_DeviceID()    { return cmdPacket_DeviceID;    }
    QByteArray getCmdPacket_InitBleStack()    { return cmdPacket_InitBleStack;    }
    QByteArray getCmdPacket_GetRSSI()    { return cmdPacket_GetRSSI;    }
    QByteArray getCmdPacket_GetBleStackVersion()    { return cmdPacket_GetBleStackVersion;    }

    // Get Gap Command Packet
    //**************************** Gap: Scan Section *******************************************************
    QByteArray getGapCmdPacket_SetDeviceIoCapabilities()   { return cmdPacket_SetDeviceIoCapabilities; }
    QByteArray getGapCmdPacket_GetDeviceIoCapabilities()   { return cmdPacket_GetDeviceIoCapabilities; }

    QByteArray getGapCmdPacket_SetScanParameters()   { return cmdPacket_SetScanParameters; }
    QByteArray getGapCmdPacket_GetScanParameters()   { return cmdPacket_GetScanParameters; }

    QByteArray getGapCmdPacket_SetConnectionParameters()   { return cmdPacket_SetConnectionParameters; }
    QByteArray getGapCmdPacket_GetConnectionParameters()   { return cmdPacket_GetConnectionParameters; }

    QByteArray getGapCmdPacket_SetLocalDeviceSecurity()   { return cmdPacket_SetLocalDeviceSecurity; }
    QByteArray getGapCmdPacket_GetLocalDeviceSecurity()   { return cmdPacket_GetLocalDeviceSecurity; }

    QByteArray getGapCmdPacket_GetBleDevicePublicAddress()   { return cmdPacket_GetBleDevicePublicAddress; }
    QByteArray getGapCmdPacket_GetBleDeviceRandomAddress()   { return cmdPacket_GetBleDeviceRandomAddress; }

    QByteArray getGapCmdPacket_SetBleDevicePublicAddress()   { return cmdPacket_SetBleDevicePublicAddress; }
    QByteArray getGapCmdPacket_SetBleDeviceRandomAddress()   { return cmdPacket_SetBleDeviceRandomAddress; }

    QByteArray getGapCmdPacket_GetBondedDevicesList()   { return cmdPacket_GetBondedDevicesList; }
    QByteArray getGapCmdPacket_GetWhiteList()   { return cmdPacket_GetWhiteList; }

    QByteArray getGapCmdPacket_StartScan()   { return cmdPacket_StartScan; }
    QByteArray getGapCmdPacket_StopScan()   { return cmdPacket_StopScan; }
    //**************************** Gap:  Connect Section *******************************************************
    QByteArray getGapCmdPacket_ResolveAndSetPeerAddr()   { return cmdPacket_ResolveAndSetPeerAddr; }
    QByteArray getGapCmdPacket_EstablishConnection()   { return cmdPacket_EstablishConnection; }
    QByteArray getGapCmdPacket_UpdateConnectionParamResp_Header()   { return cmdPacket_UpdateConnectionParamResp_Header; }
    QByteArray getGapCmdPacket_TerminateConnection()   { return cmdPacket_TerminateConnection; }

    // Get Gatt Command Packet
    //*********************************** Gatt *******************************************************
    QByteArray getGattCmdPacket_ExchangeGatt_MTU_Size_Header()   { return cmdPacket_ExchangeGatt_MTU_Size_Header; }
    QByteArray getGattCmdPacket_ExchangeGatt_MTU_Size_SetMTU()   { return cmdPacket_ExchangeGatt_MTU_Size_SetMTU; }

    QByteArray getGattCmdPacket_ReadUsingCharacteristicUuid_Header()   { return cmdPacket_ReadUsingCharacteristicUuid_Header; }
    QByteArray getGattCmdPacket_ReadUsingCharacteristicUuid_TypeValueHandleRange()   { return cmdPacket_ReadUsingCharacteristicUuid_TypeValueHandleRange; }

    QByteArray getGattCmdPacket_DiscoverAllPrimaryServices()   { return cmdPacket_DiscoverAllPrimaryServices; }
    QByteArray getGattCmdPacket_FindIncludedServices_Header()   { return cmdPacket_FindIncludedServices_Header; }

    QByteArray getGattCmdPacket_DiscoverAllCharacteristics_Header()   { return cmdPacket_DiscoverAllCharacteristics_Header; }
    QByteArray getGattCmdPacket_DiscoverAllCharacterDescriptors_Header()   { return cmdPacket_DiscoverAllCharacterDescriptors_Header; }

    QByteArray getGattCmdPacket_WriteCharacteristicDescriptor_Header()   { return cmdPacket_WriteCharacteristicDescriptor_Header; }
    QByteArray getGattCmdPacket_WriteCharacteristicDescriptor_MotionServiceHandle()   { return cmdPacket_WriteCharacteristicDescriptor_MotionServiceHandle; }
    QByteArray getGattCmdPacket_WriteCharacteristicDescriptor_ImageBurstServiceHandle()   { return cmdPacket_WriteCharacteristicDescriptor_ImageBurstServiceHandle; }

    QByteArray getGattCmdPacket_WriteCharacteristicDescriptor_ServiceActive()   { return cmdPacket_WriteCharacteristicDescriptor_ServiceActive; }
    QByteArray getGattCmdPacket_WriteCharacteristicDescriptor_ServiceStop()   { return cmdPacket_WriteCharacteristicDescriptor_ServiceStop; }


    QByteArray getReversedHexStringFromUnSignedRaw(QByteArray rawUnsignedData);

private:
    QByteArray cmdPacketHeader;
    // General Command Packet
    QByteArray cmdPacket_DeviceID, cmdPacket_InitBleStack, cmdPacket_GetBleStackVersion, cmdPacket_GetRSSI;
    // Gap Command Packet
    //******************************** Gap: Scan Section *******************************************************
    QByteArray cmdPacket_SetDeviceIoCapabilities, cmdPacket_GetDeviceIoCapabilities;
    QByteArray cmdPacket_GetScanParameters, cmdPacket_SetScanParameters;
    QByteArray cmdPacket_SetConnectionParameters, cmdPacket_GetConnectionParameters;
    QByteArray cmdPacket_SetLocalDeviceSecurity, cmdPacket_GetLocalDeviceSecurity;
    QByteArray cmdPacket_GetBleDevicePublicAddress, cmdPacket_GetBleDeviceRandomAddress;
    QByteArray cmdPacket_SetBleDevicePublicAddress, cmdPacket_SetBleDeviceRandomAddress;
    QByteArray cmdPacket_GetWhiteList, cmdPacket_GetBondedDevicesList;
    QByteArray cmdPacket_UpdateConnectionParamResp_Header, cmdPacket_TerminateConnection;

    QByteArray cmdPacket_StartScan, cmdPacket_StopScan;
    //******************************** Gap: Connect Section *******************************************************
    QByteArray cmdPacket_ResolveAndSetPeerAddr, cmdPacket_EstablishConnection;

    // Gatt Command Packet
    QByteArray cmdPacket_ExchangeGatt_MTU_Size_Header, cmdPacket_ExchangeGatt_MTU_Size_SetMTU;
    QByteArray cmdPacket_ReadUsingCharacteristicUuid_Header, cmdPacket_ReadUsingCharacteristicUuid_TypeValueHandleRange;
    QByteArray cmdPacket_DiscoverAllPrimaryServices, cmdPacket_FindIncludedServices_Header;
    QByteArray cmdPacket_DiscoverAllCharacteristics_Header, cmdPacket_DiscoverAllCharacterDescriptors_Header;
    QByteArray cmdPacket_WriteCharacteristicDescriptor_Header, cmdPacket_WriteCharacteristicDescriptor_MotionServiceHandle;
    QByteArray cmdPacket_WriteCharacteristicDescriptor_ImageBurstServiceHandle;
    QByteArray cmdPacket_WriteCharacteristicDescriptor_ServiceActive, cmdPacket_WriteCharacteristicDescriptor_ServiceStop;
//    QByteArray

    ushort headerArray[HEADERS_TOTAL_NUM];

    ushort generalCmdOpCodeArray[GENERAL_CMD_OPCODE_TOTAL_NUM];
    QString generalCmdStringArray[GENERAL_CMD_OPCODE_TOTAL_NUM];

    ushort gapCmdOpCodeArray[GAP_CMD_OPCODE_TOTAL_NUM];
    QString gapCmdStringArray[GAP_CMD_OPCODE_TOTAL_NUM];

    ushort gattCmdOpCodeArray[GATT_CMD_OPCODE_TOTAL_NUM];
    QString gattCmdStringArray[GATT_CMD_OPCODE_TOTAL_NUM];

    ushort L2CAP_CmdOpCodeArray[L2CAP_CMD_OPCODE_TOTAL_NUM];
    QString L2CAP_CmdStringArray[L2CAP_CMD_OPCODE_TOTAL_NUM];


    QString statusStringArray[STATUS_TOTAL_NUM];//CYBLE_API_RESULT_T, Protocol.h

    void serialBleInfoGroupHardWrie();

    void eventStatusResponseMapInitial();
    void headerArrayInitial();
    void cmdOpCodeInitial();
    void statusInitial();

    void generalMapCmdPacketsInitial();
    void gapMapCmdPacketsInitial();
    void gattMapCmdPacketsInitial();
    void L2CAP_MapCmdPacketsInitial();

    SerialInfoGroup defaultSettingsInfo, firstSerialInfo, secondSerialInfo;

signals:

public slots:
};

typedef enum
{
    /** Connectable undirected advertising */
    CYBLE_GAPC_CONN_UNDIRECTED_ADV          =   0x00,

    /** Connectable directed advertising */
    CYBLE_GAPC_CONN_DIRECTED_ADV            =   0x01,

    /** Scannable undirected advertising */
    CYBLE_GAPC_SCAN_UNDIRECTED_ADV          =   0x02,

    /** Non connectable undirected advertising */
    CYBLE_GAPC_NON_CONN_UNDIRECTED_ADV      =   0x03,

    /** Scan Response*/
    CYBLE_GAPC_SCAN_RSP                     =   0x04,
}CYBLE_GAPC_ADV_EVENT_T;

/** Advertisement report received by GAP Central */
typedef struct
{
    uchar		eventType;

    /** bd address type of the device advertising.
        - CYBLE_GAP_ADDR_TYPE_PUBLIC (Public device address)
        - CYBLE_GAP_ADDR_TYPE_RANDOM (Random device address)
     */
    uchar  	    				peerAddrType;

    /** Public Device Address or Random Device Address for
       each device which responded to scanning. */
    uchar	    				peerBdAddr[CYBLE_GAP_BD_ADDR_SIZE];

    /** length of the data for each device that responded to scanning */
    uchar       				dataLen;

    /** Pointer to advertising or scan response data */
    uchar* 	    				data;

    /** Rssi of the responding device.
                 * Range: -85 <= N <= 0
                 * Units: dBm */
    char	    				rssi;
}CYBLE_GAPC_ADV_REPORT_T;

/** GATT Attribute Handle Range type */
typedef struct
{
    /** Start Handle */
    ushort    startHandle;

    /** End Handle */
    ushort    endHandle;

}CYBLE_GATT_ATTR_HANDLE_RANGE_T;

typedef struct
{
    ushort                              descriptorHandle;
    ushort                              descriptorUuid;
    QByteArray                          descriptorValue;
    bool                                descriptorUuid_128;
    uint                                descriptorUuid128_first32;
    uint                                descriptorUuid128_32To64;
    uint                                descriptorUuid128_64To96;
    uint                                descriptorUuid128_96To128;
}SenBLE_valueDescriptor_INFO_T;
typedef struct
{
    ushort                              attHandle;//(characteristic declaration ID)only one attribute in one service
    ushort                              characterValueHandle;
    ushort                              characterValueUuid;
    QByteArray                          characterValue;
    uchar                               characterProperties;
    SenBLE_valueDescriptor_INFO_T       valueDescriptorsList[Sen_Max_Descriptors_Number];//Add initial Array num if exception error occurs
    int                                 valueDescriptorsCount;
    bool                                characterValueUuid_128;
    uint                                charaValueUuid128_first32;
    uint                                charaValueUuid128_32To64;
    uint                                charaValueUuid128_64To96;
    uint                                charaValueUuid128_96To128;
}SenBLE_DISC_Characteristic_INFO_T;
/** Service data received with read by group type response during discovery process */
typedef struct
{
    CYBLE_GATT_ATTR_HANDLE_RANGE_T      range;
    ushort                              uuid;//(Service Declaration)
    SenBLE_DISC_Characteristic_INFO_T   charactersList[Sen_Max_Characteristics_Number];//Add initial Array num if exception error occurs
    int                                 charactersCount;
    int                                 descriptorsTotalCount;
    bool                                uuid128;
    uint                                uuid128_first32;
    uint                                uuid128_32To64;
    uint                                uuid128_64To96;
    uint                                uuid128_96To128;
}CYBLE_DISC_SRVC_INFO_T;

/** Data Element for Group Response */
typedef struct
{
    /** atribute handle value pair */
    uchar              * attrValue;

    /** Length of each Attribute Data Element including the Handle Range */
    ushort             length;

    /** Total Length of Attribute Data */
    ushort             attrLen;
}CYBLE_GATTC_GRP_ATTR_DATA_LIST_T;

typedef struct
{
    /** Identifies the peer device(s) bonded or in current connection. Stack supports CYBLE_GAP_MAX_BONDED_DEVICE+1 devices.
       first device connected is assinged value CYBLE_GAP_MAX_BONDED_DEVICE. If previous
       device is bonded then current device will be assigned value CYBLE_GAP_MAX_BONDED_DEVICE-1,
       else CYBLE_GAP_MAX_BONDED_DEVICE.
    */
    uchar     	bdHandle;

    /** Identifies the ATT Instance. Current implementation supports only one att instance (0) due to availability
       of only on fixed channel for att. This parameter is introduced as part of connection handle to keep the
       interface unchanged event if new Bluetooth spect defines more fixed channels for ATT payload.
    */
    uchar       attId;
    uchar       MTU_Low;
    uchar       MTU_High;
}CYBLE_CONN_HANDLE_T;

// Cmd Num need to be cosistant with Cmd & Mask for decoding Cmd from cmdArray
typedef enum
{
        Cmd_Get_Device_Id_Api                                       =   0,
        Cmd_Get_Supported_Tool_Ver_Api                              =   1,
        Cmd_Get_Firmware_Version_Api                                =   2,
        Cmd_Get_Supported_Gap_Roles_Api                             =   3,
        Cmd_Get_Current_Gap_Role_Api                                =   4,
        Cmd_Get_Supported_Gatt_Roles_Api                            =   5,
        Cmd_Get_Current_Gatt_Role_Api                               =   6,
        Cmd_Init_Ble_Stack_Api                                      =   7,
        Cmd_Tool_Disconnected_Api                                   =   8,
        Cmd_Host_Timed_Out_Api                                      =   9,
        Cmd_Get_Device_Descriptor_Info                              =   10,
        Cmd_Get_Hardware_Version_Api                                =   11,
        Cmd_Get_Ble_Stack_Version_Api                               =   12,
        Cmd_Get_Rssi_Api                                            =   13,
}GeneralCommands_API;

typedef enum
{
         Cmd_Set_Device_Io_Capabilities_Api                     =   0,
         Cmd_Get_Device_Io_Capabilities_Api                     =   1,
         Cmd_Get_Bluetooth_Device_Address_Api                   =   2,
         Cmd_Set_Bluetooth_Device_Address_Api                   =   3,
         Cmd_Get_Peer_Bluetooth_Device_Address_Api              =   4,
         Cmd_Get_Peer_Device_Handle_Api                         =   5,
         Cmd_GenerateBd_Addr_Api                                =   6,
         Cmd_Set_Oob_Data_Api                                   =   7,
         Cmd_Get_Connection_Parameters_Api                      =   8,
         Cmd_Set_Connection_Parameters_Api                      =   9,
         Cmd_Get_Scan_Parameters_Api                            =   10,
         Cmd_Set_Scan_Parameters_Api                            =   11,
         Cmd_Get_Local_Device_Security_Api                      =   12,
         Cmd_Set_Local_Device_Security_Api                      =   13,
         Cmd_Get_Peer_Device_Security_Api                       =   14,
         Cmd_Get_White_List_Api                                 =   15,
         Cmd_Add_Device_To_White_List_Api                       =   16,
         Cmd_Remove_Device_From_White_List_Api                  =   17,
         Cmd_Clear_White_List_Api                               =   18,
         Cmd_Start_Scan_Api                                     =   19,
         Cmd_Stop_Scan_Api                                      =   20,
         Cmd_Generate_Keys_Api                                  =   21,
         Cmd_Set_Authentication_Keys_Api                        =   22,
         Cmd_Establish_Connection_Api                           =   23,
         Cmd_Terminate_Connection_Api                           =   24,
         Cmd_Initiate_Pairing_Request_Api                       =   25,
         Cmd_Delete_Pairing_Info_Api                            =   26,
         Cmd_Pairing_PassKey_Api                                =   27,
         Cmd_Update_Connection_Params_Api                       =   28,
         Cmd_Cancle_Connection_Api                              =   29,
         Cmd_Get_Bonded_Devices_List_Api                        =   30,
         Cmd_UpdateConnectionParam_Resp_Api                     =   31,
         Cmd_Get_PeerDevice_SecurityKeys_Api                    =   32,
         Cmd_Resolve_Set_Peer_Addr_Api                          =   33
}GapCommands_API;

typedef enum
{
     Cmd_Discover_All_Primary_Services_Api                      =   0,
     Cmd_Discover_Primary_Services_By_Uuid_Api                  =   1,
     Cmd_Find_Included_Services_Api                             =   2,
     Cmd_Discover_All_Characteristics_Api                       =   3,
     Cmd_Discover_Characteristics_By_Uuid_Api                   =   4,
     Cmd_Discover_All_Characteristic_Descriptors_Api            =   5,
     Cmd_Read_Characteristic_Value_Api                          =   6,
     Cmd_Read_Using_Characteristic_Uuid_Api                     =   7,
     Cmd_Read_Long_Characteristic_Values_Api                    =   8,
     Cmd_Read_Multiple_Characteristic_Values_Api                =   9,
     Cmd_Characteristic_Value_Write_Without_Response_Api        =   10,
     Cmd_Write_Characteristic_Value_Api                         =   11,
     Cmd_Write_Long_Characteristic_Value_Api                    =   12,
     Cmd_Reliable_Characteristic_Value_Writes_Api               =   13,
     Cmd_Read_Characteristic_Descriptor_Api                     =   14,
     Cmd_Read_Long_Characteristic_Descriptor_Api                =   15,
     Cmd_Write_Characteristic_Descriptor_Api                    =   16,
     Cmd_Write_Long_Characteristic_Descriptor_Api               =   17,
     Cmd_Exchange_GATT_MTU_Size_Api                             =   18,
     Cmd_GATT_Stop_Api                                          =   19,
     Cmd_Signed_Write_Without_Response_Api                      =   20
}GattCommands_API;

typedef enum
{
    Cmd_Register_PSM_Api                                        =   0,
    Cmd_Unregister_PSM_Api                                      =   1,
    Cmd_CBFC_SendConnectionReq_Api                              =   2,
    Cmd_CBFC_SendConnectionResp_Api                             =   3,
    Cmd_CBFC_SendFlowControlCredit_Api                          =   4,
    Cmd_CBFC_SendData_Api                                       =   5,
    Cmd_CBFC_SendDisconnectReq_Api                              =   6
}L2CAP_Commands_API;

/** Common error codes received as API result */
typedef enum
{
    /** No Error occurred */
    CYBLE_ERROR_OK                                      =           0,

    /** At least one of the input parameters is invalid */
    CYBLE_ERROR_INVALID_PARAMETER                       =           1,

    /** Operation is not permitted */
    CYBLE_ERROR_INVALID_OPERATION                       =           2,

    /** An internal error occurred in the stack */
    CYBLE_ERROR_MEMORY_ALLOCATION_FAILED                =           3,

    /** Insufficient resources to perform requested operation */
    CYBLE_ERROR_INSUFFICIENT_RESOURCES                  =           4,

    /** OOB data not available */
    CYBLE_ERROR_OOB_NOT_AVAILABLE                       =           5,

    /** Connection is required to perform requested operation. Connection not present */
    CYBLE_ERROR_NO_CONNECTION                           =           6,

    /** No device entity to perform requested operation */
    CYBLE_ERROR_NO_DEVICE_ENTITY                        =           7,

    /** Attempted repeat operation is not allowed */
    CYBLE_ERROR_REPEATED_ATTEMPTS                       =           8,

    /** GAP role is incorrect */
    CYBLE_ERROR_GAP_ROLE                                =           9,

    /** Error reading TC power */
    CYBLE_ERROR_TX_POWER_READ                           =           10,

    /** BLE Initialization failed */
    CYBLE_ERROR_BT_ON_NOT_COMPLETED                     =           11,

    /** Security operation failed */
    CYBLE_ERROR_SEC_FAILED                              =           12,


    /******************************** L2CAP error codes ********************************/

    /** L2CAP PSM encoding is incorrect */
    CYBLE_ERROR_L2CAP_PSM_WRONG_ENCODING                =           13,

    /** L2CAP PSM has already been registered */
    CYBLE_ERROR_L2CAP_PSM_ALREADY_REGISTERED            =           14,

    /** L2CAP PSM has not been registered */
    CYBLE_ERROR_L2CAP_PSM_NOT_REGISTERED                =           15,

    /** L2CAPconnection entity not found */
    CYBLE_ERROR_L2CAP_CONNECTION_ENTITY_NOT_FOUND       =           16,

    /** L2CAP channel not found */
    CYBLE_ERROR_L2CAP_CHANNEL_NOT_FOUND                 =           17,

    /** Specified PSM is out of range */
    CYBLE_ERROR_L2CAP_PSM_NOT_IN_RANGE                  =           18,

    /**GATT DB error codes*/

    /** Invalid attribute handle */
    CYBLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE             =           19,

    /****************************** Other Codes.  ***********************************/

    /** Device cannot be added to whitelist as it has already been added */
    CYBLE_ERROR_DEVICE_ALREADY_EXISTS                   =           39,     //0x27u,
    /** Write to flash is not permitted */
    CYBLE_ERROR_FLASH_WRITE_NOT_PERMITED                =           40,     //0x28u,
    /** MIC Authentication failure */
    CYBLE_ERROR_MIC_AUTH_FAILED                         =           41,     //0x29u,
    /** All other errors not covered in the above list map to this error code */
    CYBLE_ERROR_MAX                                     =           255,    //0xFFu,

}CYBLE_API_RESULT_T;


/* CySmart Event opcodes as per protocol */

typedef enum
{
    General_Events      = 0x04,
    L2CAP_Events        = 0x05,
    GATT_GAP_Events     = 0x06
}Event_Types;

typedef enum
{
    General_Commands      = 0x00,
    L2CAP_Commands        = 0x01,
    GATT_GAP_Commands     = 0x02
}Command_Group_Types;

typedef enum
{
        EVT_COMMAND_STATUS                                          = 0x047E,
        EVT_COMMAND_COMPLETE                                        = 0x047F,
        EVT_GET_DEVICE_ID_RESPONSE                                  = 0x0400,
        EVT_GET_SUPPORTED_TOOL_VERSION_RESPONSE                     = 0x0401,
        EVT_GET_FIRMWARE_VERSION_RESPONSE                           = 0x0402,
        EVT_GET_BLE_STACK_VERSION_RESPONSE                          = 0x0403,
        EVT_REPORT_STACK_MISC_STATUS                                = 0x0404,
        EVT_GET_SUPPORTED_GAP_ROLES_RESPONSE                        = 0x0405,
        EVT_GET_CURRENT_GAP_ROLE_RESPONSE                           = 0x0406,
        EVT_GET_SUPPORTED_GATT_ROLES_RESPONSE                       = 0x0407,
        EVT_GET_CURRENT_GATT_ROLE_RESPONSE                          = 0x0408,
        EVT_GET_RSSI_RESPONSE                                       = 0x0409,

        EVT_GET_DEVICE_DESCRIPTION_RESPONSE                         = 0x040A,
        EVT_GET_HARDWARE_VERSION_RESPONSE                           = 0x040B,
    Total_Other_GeneralEvents_Num     = 0x0C
}GeneralEventsTypes;

typedef enum
{       /* GAP Events */
        EVT_GET_DEVICE_IO_CAPABILITIES_RESPONSE                     = 0x0680,
        EVT_GET_BLUETOOTH_DEVICE_ADDRESS_RESPONSE                   = 0x0681,
        EVT_GET_PEER_BLUETOOTH_DEVICE_ADDRESS_RESPONSE              = 0x0682,
        EVT_GET_PEER_DEVICE_HANDLE_RESPONSE                         = 0x0683,
        EVT_CURRENT_CONNECTION_PARAMETERS_NOTIFICATION              = 0x0684,
        EVT_GET_CONNECTION_PARAMETERS_RESPONSE                      = 0x0685,
        EVT_GET_SCAN_PARAMETERS_RESPONSE                            = 0x0686,
        EVT_GET_LOCAL_DEVICE_SECURITY_RESPONSE                      = 0x0687,
        EVT_GET_PEER_DEVICE_SECURITY_RESPONSE                       = 0x0688,
        EVT_GET_WHITE_LIST_RESPONSE                                 = 0x0689,
        EVT_SCAN_PROGRESS_RESULT                                    = 0x068A,
        EVT_GENERATE_BD_ADDR_RESPONSE                               = 0x068B,
        EVT_GENERATE_KEYS_RESPONSE                                  = 0x068C,
        EVT_PASSKEY_ENTRY_REQUEST                                   = 0x068D,
        EVT_PASSKEY_DISPLAY_REQUEST                                 = 0x068E,
        EVT_ESTABLISH_CONNECTION_RESPONSE                           = 0x068F,
        EVT_CONNECTION_TERMINATED_NOTIFICATION                      = 0x0690,
        EVT_SCAN_STOPPED_NOTIFICATION                               = 0x0691,
        EVT_PAIRING_REQUEST_RECEIVED_NOTIFICATION                   = 0x0692,
        EVT_AUTHENTICATION_ERROR_NOTIFICATION                       = 0x0693,
        EVT_CONNECTION_CANCELLED_NOTIFICATION                       = 0x0694,
        EVT_GET_BONDED_DEVICES_LIST_RESPONSE                        = 0x0695,
        EVT_UPDATE_CONNECTION_PARAMETERS_NOTIFICATION               = 0x0696,
        EVT_GET_PEER_DEVICE_SECURITY_KEYS_RESPONSE                  = 0x0697,
        EVT_RESOLVE_AND_SET_PEER_BD_ADDRESS_RESPONSE                = 0x0698,
}GapEventsTypes;

typedef enum
{
        /* GATT Events */
        EVT_DISCOVER_ALL_PRIMARY_SERVICES_RESULT_PROGRESS           = 0x0600,
        EVT_DISCOVER_PRIMARY_SERVICES_BY_UUID_RESULT_PROGRESS       = 0x0601,
        EVT_FIND_INDLUDED_SERVICES_RESULT_PROGRESS                  = 0x0602,
        EVT_DISCOVER_ALL_CHARACTERISTICS_RESULT_PROGRESS            = 0x0603,
        EVT_DISCOVER_CHARACTERISTICS_BY_UUID_RESULT_PROGRESS        = 0x0604,
        EVT_DISCOVER_ALL_CHARACTERISTIC_DESCRIPTORS_RESULT_PROGRESS = 0x0605,
        EVT_READ_CHARACTERISTIC_VALUE_RESPONSE                      = 0x0606,
        EVT_READ_USING_CHARACTERISTIC_UUID_RESPONSE                 = 0x0607,
        EVT_READ_LONG_CHARACTERISTIC_VALUE_RESPONSE                 = 0x0608,
        EVT_READ_MULTIPLE_CHARACTERISTIC_VALUES_RESPONSE            = 0x0609,
        EVT_READ_CHARACTERISTIC_DESCRIPTOR_RESPONSE                 = 0x060A,
        EVT_READ_LONG_CHARACTERISTIC_DESCRIPTOR_RESPONSE            = 0x060B,
        EVT_CHARACTERISTIC_VALUE_NOTIFICATION                       = 0x060C,
        EVT_CHARACTERISTIC_VALUE_INDICATION                         = 0x060D,
        EVT_GATT_ERROR_NOTIFICATION                                 = 0x060E,
        EVT_EXCHANGE_GATT_MTU_SIZE_RESPONSE                         = 0x060F,
        EVT_GATT_STOP_NOTIFICATION                                  = 0x0610,
        EVT_GATT_TIMEOUT_NOTIFICATION                               = 0x0611,
}GattEventsTypes;

typedef enum
{
    /* L2CAP Events */
    EVT_CBFC_CONNECTION_INDICATION                              = 0x0500,
    EVT_CBFC_CONNECTION_CONFIRMATION                            = 0x0501,
    EVT_CBFC_DISCONNECT_INDICATION                              = 0x0502,
    EVT_CBFC_DISCONNECT_CONFIRMATION                            = 0x0503,
    EVT_CBFC_DATA_RECEIVIED_NOTIFICATION                        = 0x0504,
    EVT_CBFC_RX_CREDIT_INDICATION                               = 0x0505,
    EVT_CBFC_TX_CREDIT_INDICATION                               = 0x0506,
    EVT_CBFC_DATA_WRITE_INDICATION                              = 0x0507
}L2CAP_EventsTypes;

#endif // SENBLEDONGLETALKFUNDAMENTALS_H
