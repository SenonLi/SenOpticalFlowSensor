#include "SenBleDongleTalkFundamentals.h"

SenBleDongleTalkFundamentals::SenBleDongleTalkFundamentals(QObject *parent) : QObject(parent)
{
    SenPSoC_BleAddress.clear();
    SenPSoC_BleAddress[0] = (uchar)0x21;
    SenPSoC_BleAddress[1] = (uchar)0x1F;
    SenPSoC_BleAddress[2] = (uchar)0x04;
    SenPSoC_BleAddress[3] = (uchar)0x50;
    SenPSoC_BleAddress[4] = (uchar)0xA0;
    SenPSoC_BleAddress[5] = (uchar)0x00;// PSoC

//    SenPSoC_BleAddress[0] = (uchar)0x35;
//    SenPSoC_BleAddress[1] = (uchar)0x1E;
//    SenPSoC_BleAddress[2] = (uchar)0x0B;
//    SenPSoC_BleAddress[3] = (uchar)0x50;
//    SenPSoC_BleAddress[4] = (uchar)0xA0;
//    SenPSoC_BleAddress[5] = (uchar)0x00;// PRoC


    serialBleInfoGroupHardWrie();
    eventStatusResponseMapInitial();
    generalMapCmdPacketsInitial();
    gapMapCmdPacketsInitial();
    gattMapCmdPacketsInitial();
    L2CAP_MapCmdPacketsInitial();
}

void SenBleDongleTalkFundamentals::headerArrayInitial()
{
    headerArray[CMD_PACKET_TO_DONGLE_HEADER] = 0x5943;
    headerArray[DONGLE_EVENT_HEADER] = 0xA7BD;
}

void SenBleDongleTalkFundamentals::generalMapCmdPacketsInitial()
{
    // 00: Cmd_Get_Device_Id_Api
    cmdPacket_DeviceID[CMD_OPCODE_LOW]      = get_L8((uchar*)&generalCmdOpCodeArray[Cmd_Get_Device_Id_Api]);
    cmdPacket_DeviceID[CMD_OPCODE_HIGH]     = get_H8((uchar*)&generalCmdOpCodeArray[Cmd_Get_Device_Id_Api]);
    cmdPacket_DeviceID[CMD_PARA_LEN_LOW]    = 0x00;
    cmdPacket_DeviceID[CMD_PARA_LEN_HIGH]   = 0x00;

    // 07: Cmd_Init_Ble_Stack_Api
    cmdPacket_InitBleStack[CMD_OPCODE_LOW]      = get_L8((uchar*)&generalCmdOpCodeArray[Cmd_Init_Ble_Stack_Api]);
    cmdPacket_InitBleStack[CMD_OPCODE_HIGH]     = get_H8((uchar*)&generalCmdOpCodeArray[Cmd_Init_Ble_Stack_Api]);
    cmdPacket_InitBleStack[CMD_PARA_LEN_LOW]    = 0x00;
    cmdPacket_InitBleStack[CMD_PARA_LEN_HIGH]   = 0x00;

    // 12: Cmd_Get_Ble_Stack_Version_Api
    cmdPacket_GetBleStackVersion[CMD_OPCODE_LOW]      = get_L8((uchar*)&generalCmdOpCodeArray[Cmd_Get_Ble_Stack_Version_Api]);
    cmdPacket_GetBleStackVersion[CMD_OPCODE_HIGH]     = get_H8((uchar*)&generalCmdOpCodeArray[Cmd_Get_Ble_Stack_Version_Api]);
    cmdPacket_GetBleStackVersion[CMD_PARA_LEN_LOW]    = 0x00;
    cmdPacket_GetBleStackVersion[CMD_PARA_LEN_HIGH]   = 0x00;

    // 13: Cmd_Init_Ble_Stack_Api
    cmdPacket_GetRSSI[CMD_OPCODE_LOW]      = get_L8((uchar*)&generalCmdOpCodeArray[Cmd_Get_Rssi_Api]);
    cmdPacket_GetRSSI[CMD_OPCODE_HIGH]     = get_H8((uchar*)&generalCmdOpCodeArray[Cmd_Get_Rssi_Api]);
    cmdPacket_GetRSSI[CMD_PARA_LEN_LOW]    = 0x00;
    cmdPacket_GetRSSI[CMD_PARA_LEN_HIGH]   = 0x00;
}

void SenBleDongleTalkFundamentals::gapMapCmdPacketsInitial()
{
    // 00: Cmd_Set_Device_Io_Capabilities_Api ( Check Parameter; API Return; Trigger Complete)
    cmdPacket_SetDeviceIoCapabilities[CMD_OPCODE_LOW]      = get_L8((uchar*)&gapCmdOpCodeArray[Cmd_Set_Device_Io_Capabilities_Api]);
    cmdPacket_SetDeviceIoCapabilities[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gapCmdOpCodeArray[Cmd_Set_Device_Io_Capabilities_Api]);
    cmdPacket_SetDeviceIoCapabilities[CMD_PARA_LEN_LOW]    = 0x01; // sizeof(uint8)
    cmdPacket_SetDeviceIoCapabilities[CMD_PARA_LEN_HIGH]   = 0x00;
    cmdPacket_SetDeviceIoCapabilities[CMD_PARAMETERS_BEGINNING]   = 0x04;// CYBLE_GAP_IOCAP_KEYBOARD_DISPLAY

    // 01: Cmd_Get_Device_Io_Capabilities_Api ( Check Parameter; Immediate Response; Trigger Complete)
    cmdPacket_GetDeviceIoCapabilities[CMD_OPCODE_LOW]      = get_L8((uchar*)&gapCmdOpCodeArray[Cmd_Get_Device_Io_Capabilities_Api]);
    cmdPacket_GetDeviceIoCapabilities[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gapCmdOpCodeArray[Cmd_Get_Device_Io_Capabilities_Api]);
    cmdPacket_GetDeviceIoCapabilities[CMD_PARA_LEN_LOW]    = 0x00;
    cmdPacket_GetDeviceIoCapabilities[CMD_PARA_LEN_HIGH]   = 0x00;

    // 02_Public: Cmd_Get_Bluetooth_Device_Address_Api (Trigger Complete)
    cmdPacket_GetBleDevicePublicAddress[CMD_OPCODE_LOW]      = get_L8((uchar*)&gapCmdOpCodeArray[Cmd_Get_Bluetooth_Device_Address_Api]);
    cmdPacket_GetBleDevicePublicAddress[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gapCmdOpCodeArray[Cmd_Get_Bluetooth_Device_Address_Api]);
    cmdPacket_GetBleDevicePublicAddress[CMD_PARA_LEN_LOW]    = 0x01; // sizeof(uint8)
    cmdPacket_GetBleDevicePublicAddress[CMD_PARA_LEN_HIGH]   = 0x00;
    cmdPacket_GetBleDevicePublicAddress[CMD_PARAMETERS_BEGINNING]   = 0x00;// Public Address Type

    // 02_Random: Cmd_Get_Bluetooth_Device_Address_Api (Trigger Complete)
    cmdPacket_GetBleDeviceRandomAddress[CMD_OPCODE_LOW]      = get_L8((uchar*)&gapCmdOpCodeArray[Cmd_Get_Bluetooth_Device_Address_Api]);
    cmdPacket_GetBleDeviceRandomAddress[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gapCmdOpCodeArray[Cmd_Get_Bluetooth_Device_Address_Api]);
    cmdPacket_GetBleDeviceRandomAddress[CMD_PARA_LEN_LOW]    = 0x01; // sizeof(uint8)
    cmdPacket_GetBleDeviceRandomAddress[CMD_PARA_LEN_HIGH]   = 0x00;
    cmdPacket_GetBleDeviceRandomAddress[CMD_PARAMETERS_BEGINNING]   = 0x01;// Random Address Type

    // 03_Public: Cmd_Set_Bluetooth_Device_Address_Api ( API Return; Trigger Complete)
    cmdPacket_SetBleDevicePublicAddress[CMD_OPCODE_LOW]      = get_L8((uchar*)&gapCmdOpCodeArray[Cmd_Set_Bluetooth_Device_Address_Api]);
    cmdPacket_SetBleDevicePublicAddress[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gapCmdOpCodeArray[Cmd_Set_Bluetooth_Device_Address_Api]);
    cmdPacket_SetBleDevicePublicAddress[CMD_PARA_LEN_LOW]    = 0x07;
    cmdPacket_SetBleDevicePublicAddress[CMD_PARA_LEN_HIGH]   = 0x00;
    cmdPacket_SetBleDevicePublicAddress[CMD_PARAMETERS_BEGINNING + 0]   = 0x99;
    cmdPacket_SetBleDevicePublicAddress[CMD_PARAMETERS_BEGINNING + 1]   = 0x49;
    cmdPacket_SetBleDevicePublicAddress[CMD_PARAMETERS_BEGINNING + 2]   = 0x29;
    cmdPacket_SetBleDevicePublicAddress[CMD_PARAMETERS_BEGINNING + 3]   = 0x19;
    cmdPacket_SetBleDevicePublicAddress[CMD_PARAMETERS_BEGINNING + 4]   = 0x95;
    cmdPacket_SetBleDevicePublicAddress[CMD_PARAMETERS_BEGINNING + 5]   = 0x91;
    cmdPacket_SetBleDevicePublicAddress[CMD_PARAMETERS_BEGINNING + 6]   = 0x00;// Public Address Type

    // 03_Random: Cmd_Set_Bluetooth_Device_Address_Api ( API Return; Trigger Complete)
    cmdPacket_SetBleDeviceRandomAddress[CMD_OPCODE_LOW]      = get_L8((uchar*)&gapCmdOpCodeArray[Cmd_Set_Bluetooth_Device_Address_Api]);
    cmdPacket_SetBleDeviceRandomAddress[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gapCmdOpCodeArray[Cmd_Set_Bluetooth_Device_Address_Api]);
    cmdPacket_SetBleDeviceRandomAddress[CMD_PARA_LEN_LOW]    = 0x07;
    cmdPacket_SetBleDeviceRandomAddress[CMD_PARA_LEN_HIGH]   = 0x00;
    cmdPacket_SetBleDeviceRandomAddress[CMD_PARAMETERS_BEGINNING + 0]   = 0x00;
    cmdPacket_SetBleDeviceRandomAddress[CMD_PARAMETERS_BEGINNING + 1]   = 0xA0;
    cmdPacket_SetBleDeviceRandomAddress[CMD_PARAMETERS_BEGINNING + 2]   = 0x50;
    cmdPacket_SetBleDeviceRandomAddress[CMD_PARAMETERS_BEGINNING + 3]   = 0x0C;
    cmdPacket_SetBleDeviceRandomAddress[CMD_PARAMETERS_BEGINNING + 4]   = 0x10;
    cmdPacket_SetBleDeviceRandomAddress[CMD_PARAMETERS_BEGINNING + 5]   = 0x1F;
    cmdPacket_SetBleDeviceRandomAddress[CMD_PARAMETERS_BEGINNING + 6]   = 0x01;// Random Address Type

    // 08: Cmd_Get_Connection_Parameters_Api ( Check Parameter; Immediate Response; Trigger Complete)
    cmdPacket_GetConnectionParameters[CMD_OPCODE_LOW]      = get_L8((uchar*)&gapCmdOpCodeArray[Cmd_Get_Connection_Parameters_Api]);
    cmdPacket_GetConnectionParameters[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gapCmdOpCodeArray[Cmd_Get_Connection_Parameters_Api]);
    cmdPacket_GetConnectionParameters[CMD_PARA_LEN_LOW]    = 0x00;
    cmdPacket_GetConnectionParameters[CMD_PARA_LEN_HIGH]   = 0x00;

    // 09: Cmd_Set_Connection_Parameters_Api ( Immediate Response; Trigger Complete)
    cmdPacket_SetConnectionParameters[CMD_OPCODE_LOW]      = get_L8((uchar*)&gapCmdOpCodeArray[Cmd_Set_Connection_Parameters_Api]);
    cmdPacket_SetConnectionParameters[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gapCmdOpCodeArray[Cmd_Set_Connection_Parameters_Api]);
    cmdPacket_SetConnectionParameters[CMD_PARA_LEN_LOW]    = 0x19;
    cmdPacket_SetConnectionParameters[CMD_PARA_LEN_HIGH]   = 0x00;// 25 Bytes
    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 0]   = 0x10;
    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 1]   = 0x00;// scanIntv(16Bit):  try to be 10ms, not sure how to calculate yet
    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 2]   = 0x10;
    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 3]   = 0x00;// scanWindow(16Bit):  try to be 10ms, not sure how to calculate yet
    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 4]   = 0x00;// initiatorFilterPolicy: CYBLE_GAPC_CONN_ALL, (White list is not used to determine which advertiser to connect. Peer address is used)

    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 5]   = 0x1F;
    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 6]   = 0x10;
    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 7]   = 0x0C;
    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 8]   = 0x50;
    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 9]   = 0xA0;
    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 10]  = 0x00;// peerBdAddr[CYBLE_GAP_BD_ADDR_SIZE]: PUBLIC_ADDRESS (need to be changed for other dongles)

    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 11]  = 0x00;// peerAddrType: CYBLE_GAP_ADDR_TYPE_PUBLIC
    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 12]  = 0x00;// ownAddrType: CYBLE_GAP_ADDR_TYPE_PUBLIC (Not Sure Yet!)
    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 13]  = 0x06;
    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 14]  = 0x00;// connIntvMin(16Bit):  not sure how to calculate yet
    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 15]  = 0x08;
    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 16]  = 0x00;// connIntvMax(16Bit):  try to be 10ms, not sure how to calculate yet
    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 17]  = 0x00;
    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 18]  = 0x00;// connLatency(16Bit):  0
    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 19]  = 0x0A;
    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 20]  = 0x00;// supervisionTO(16Bit):  try to be 100ms
    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 21]  = 0x00;
    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 22]  = 0x00;// minCeLength(16Bit): try to be .00 ms
    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 23]  = 0x00;
    cmdPacket_SetConnectionParameters[CMD_PARAMETERS_BEGINNING + 24]  = 0x00;// maxCeLength(16Bit): try to be .00 ms

    // 10: Cmd_Get_Scan_Parameters_Api ( Check Parameter; Immediate Response; Trigger Complete)
    cmdPacket_GetScanParameters[CMD_OPCODE_LOW]      = get_L8((uchar*)&gapCmdOpCodeArray[Cmd_Get_Scan_Parameters_Api]);
    cmdPacket_GetScanParameters[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gapCmdOpCodeArray[Cmd_Get_Scan_Parameters_Api]);
    cmdPacket_GetScanParameters[CMD_PARA_LEN_LOW]    = 0x00;
    cmdPacket_GetScanParameters[CMD_PARA_LEN_HIGH]   = 0x00;

    // 11: Cmd_Set_Scan_Parameters_Api ( Immediate Response; Trigger Complete)
    cmdPacket_SetScanParameters[CMD_OPCODE_LOW]      = get_L8((uchar*)&gapCmdOpCodeArray[Cmd_Set_Scan_Parameters_Api]);
    cmdPacket_SetScanParameters[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gapCmdOpCodeArray[Cmd_Set_Scan_Parameters_Api]);
    cmdPacket_SetScanParameters[CMD_PARA_LEN_LOW]    = 0x0B;
    cmdPacket_SetScanParameters[CMD_PARA_LEN_HIGH]   = 0x00;
    cmdPacket_SetScanParameters[CMD_PARAMETERS_BEGINNING + 0]   = 0x00;// discProcedure: OBSERVATION_PROCEDURE
    cmdPacket_SetScanParameters[CMD_PARAMETERS_BEGINNING + 1]   = 0x01;// scanType: ACTIVE_SCAN
    cmdPacket_SetScanParameters[CMD_PARAMETERS_BEGINNING + 2]   = 0x10;
    cmdPacket_SetScanParameters[CMD_PARAMETERS_BEGINNING + 3]   = 0x00;// scanIntv: try to be 10ms, not sure how to calculate yet
    cmdPacket_SetScanParameters[CMD_PARAMETERS_BEGINNING + 4]   = 0x10;
    cmdPacket_SetScanParameters[CMD_PARAMETERS_BEGINNING + 5]   = 0x00;// scanWindow: try to be 10ms, not sure how to calculate yet
    cmdPacket_SetScanParameters[CMD_PARAMETERS_BEGINNING + 6]   = 0x00;// ownAddrType: CYBLE_GAP_ADDR_TYPE_PUBLIC
    cmdPacket_SetScanParameters[CMD_PARAMETERS_BEGINNING + 7]   = 0x00;// scanFilterPolicy: CYBLE_GAPC_ADV_ACCEPT_ALL_PKT
    cmdPacket_SetScanParameters[CMD_PARAMETERS_BEGINNING + 8]   = 0x00;
    cmdPacket_SetScanParameters[CMD_PARAMETERS_BEGINNING + 9]   = 0x00;// scanTo: If timeout is set as 0, then there will not be any timeout
    cmdPacket_SetScanParameters[CMD_PARAMETERS_BEGINNING + 10]  = 0x01;// filterDuplicates: CYBLE_GAPC_FILTER_DUP_ENABLE

    // 12: Cmd_Get_Local_Device_Security_Api ( Check Parameter; Immediate Response; Trigger Complete)
    cmdPacket_GetLocalDeviceSecurity[CMD_OPCODE_LOW]      = get_L8((uchar*)&gapCmdOpCodeArray[Cmd_Get_Local_Device_Security_Api]);
    cmdPacket_GetLocalDeviceSecurity[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gapCmdOpCodeArray[Cmd_Get_Local_Device_Security_Api]);
    cmdPacket_GetLocalDeviceSecurity[CMD_PARA_LEN_LOW]    = 0x00;
    cmdPacket_GetLocalDeviceSecurity[CMD_PARA_LEN_HIGH]   = 0x00;

    // 13: Cmd_Set_Local_Device_Security_Api ( Immediate Response; Trigger Complete)
    cmdPacket_SetLocalDeviceSecurity[CMD_OPCODE_LOW]      = get_L8((uchar*)&gapCmdOpCodeArray[Cmd_Set_Local_Device_Security_Api]);
    cmdPacket_SetLocalDeviceSecurity[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gapCmdOpCodeArray[Cmd_Set_Local_Device_Security_Api]);
    cmdPacket_SetLocalDeviceSecurity[CMD_PARA_LEN_LOW]    = 0x04;
    cmdPacket_SetLocalDeviceSecurity[CMD_PARA_LEN_HIGH]   = 0x00;// 4 Bytes
    cmdPacket_SetLocalDeviceSecurity[CMD_PARAMETERS_BEGINNING + 0]   = 0x11;// security
    cmdPacket_SetLocalDeviceSecurity[CMD_PARAMETERS_BEGINNING + 1]   = 0x00;// bonding: CYBLE_GAP_BONDING_NONE
    cmdPacket_SetLocalDeviceSecurity[CMD_PARAMETERS_BEGINNING + 2]   = 0x10;// ekeySize: CYBLE_GAP_ENCRYP_KEY_MAX
    cmdPacket_SetLocalDeviceSecurity[CMD_PARAMETERS_BEGINNING + 3]   = 0x00;// authErr: CYBLE_GAP_AUTH_ERROR_NONE

    // 15: Cmd_Get_White_List_Api (Trigger Complete; Check Parameter; Secondary Command)
    cmdPacket_GetWhiteList[CMD_OPCODE_LOW]      = get_L8((uchar*)&gapCmdOpCodeArray[Cmd_Get_White_List_Api]);
    cmdPacket_GetWhiteList[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gapCmdOpCodeArray[Cmd_Get_White_List_Api]);
    cmdPacket_GetWhiteList[CMD_PARA_LEN_LOW]    = 0x00;
    cmdPacket_GetWhiteList[CMD_PARA_LEN_HIGH]   = 0x00;

    // 19: Cmd_Start_Scan_Api (API Return; Check Parameter;)
    cmdPacket_StartScan[CMD_OPCODE_LOW]      = get_L8((uchar*)&gapCmdOpCodeArray[Cmd_Start_Scan_Api]);
    cmdPacket_StartScan[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gapCmdOpCodeArray[Cmd_Start_Scan_Api]);
    cmdPacket_StartScan[CMD_PARA_LEN_LOW]    = 0x00;
    cmdPacket_StartScan[CMD_PARA_LEN_HIGH]   = 0x00;

    // 20: Cmd_Stop_Scan_Api (API Return; Check Parameter; Secondary Command)
    cmdPacket_StopScan[CMD_OPCODE_LOW]      = get_L8((uchar*)&gapCmdOpCodeArray[Cmd_Stop_Scan_Api]);
    cmdPacket_StopScan[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gapCmdOpCodeArray[Cmd_Stop_Scan_Api]);
    cmdPacket_StopScan[CMD_PARA_LEN_LOW]    = 0x00;
    cmdPacket_StopScan[CMD_PARA_LEN_HIGH]   = 0x00;

    // 23: Cmd_Establish_Connection_Api (API Return)
    cmdPacket_EstablishConnection[CMD_OPCODE_LOW]      = get_L8((uchar*)&gapCmdOpCodeArray[Cmd_Establish_Connection_Api]);
    cmdPacket_EstablishConnection[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gapCmdOpCodeArray[Cmd_Establish_Connection_Api]);
    cmdPacket_EstablishConnection[CMD_PARA_LEN_LOW]    = 0x07;
    cmdPacket_EstablishConnection[CMD_PARA_LEN_HIGH]   = 0x00;
    cmdPacket_EstablishConnection[CMD_PARAMETERS_BEGINNING + 0]   = SenPSoC_BleAddress[0];
    cmdPacket_EstablishConnection[CMD_PARAMETERS_BEGINNING + 1]   = SenPSoC_BleAddress[1];
    cmdPacket_EstablishConnection[CMD_PARAMETERS_BEGINNING + 2]   = SenPSoC_BleAddress[2];
    cmdPacket_EstablishConnection[CMD_PARAMETERS_BEGINNING + 3]   = SenPSoC_BleAddress[3];
    cmdPacket_EstablishConnection[CMD_PARAMETERS_BEGINNING + 4]   = SenPSoC_BleAddress[4];
    cmdPacket_EstablishConnection[CMD_PARAMETERS_BEGINNING + 5]   = SenPSoC_BleAddress[5];
    cmdPacket_EstablishConnection[CMD_PARAMETERS_BEGINNING + 6]   = 0x00;//Type: Public

    // 24: Cmd_Terminate_Connection_Api ( Check Parameter; API Return; Secondary Command)
    cmdPacket_TerminateConnection[CMD_OPCODE_LOW]      = get_L8((uchar*)&gapCmdOpCodeArray[Cmd_Terminate_Connection_Api]);
    cmdPacket_TerminateConnection[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gapCmdOpCodeArray[Cmd_Terminate_Connection_Api]);
    cmdPacket_TerminateConnection[CMD_PARA_LEN_LOW]    = 0x02;
    cmdPacket_TerminateConnection[CMD_PARA_LEN_HIGH]   = 0x00;
    cmdPacket_TerminateConnection[CMD_PARAMETERS_BEGINNING + 0]   = 0x04;
    cmdPacket_TerminateConnection[CMD_PARAMETERS_BEGINNING + 1]   = 0x00;

    // 30: Cmd_Get_Bonded_Devices_List_Api ( Check Parameter; Trigger Complete; Secondary Command)
    cmdPacket_GetBondedDevicesList[CMD_OPCODE_LOW]      = get_L8((uchar*)&gapCmdOpCodeArray[Cmd_Get_Bonded_Devices_List_Api]);
    cmdPacket_GetBondedDevicesList[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gapCmdOpCodeArray[Cmd_Get_Bonded_Devices_List_Api]);
    cmdPacket_GetBondedDevicesList[CMD_PARA_LEN_LOW]    = 0x00;
    cmdPacket_GetBondedDevicesList[CMD_PARA_LEN_HIGH]   = 0x00;

    // 31: Cmd_UpdateConnectionParam_Resp_Api ( Check Parameter; API Return; Trigger Complete)
    cmdPacket_UpdateConnectionParamResp_Header[CMD_OPCODE_LOW]      = get_L8((uchar*)&gapCmdOpCodeArray[Cmd_UpdateConnectionParam_Resp_Api]);
    cmdPacket_UpdateConnectionParamResp_Header[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gapCmdOpCodeArray[Cmd_UpdateConnectionParam_Resp_Api]);
    cmdPacket_UpdateConnectionParamResp_Header[CMD_PARA_LEN_LOW]    = 0x04; // handle + response
    cmdPacket_UpdateConnectionParamResp_Header[CMD_PARA_LEN_HIGH]   = 0x00;

    // 33: Cmd_Resolve_Set_Peer_Addr_Api ( Check Parameter; Trigger Complete )
    cmdPacket_ResolveAndSetPeerAddr[CMD_OPCODE_LOW]      = get_L8((uchar*)&gapCmdOpCodeArray[Cmd_Resolve_Set_Peer_Addr_Api]);
    cmdPacket_ResolveAndSetPeerAddr[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gapCmdOpCodeArray[Cmd_Resolve_Set_Peer_Addr_Api]);
    cmdPacket_ResolveAndSetPeerAddr[CMD_PARA_LEN_LOW]    = 0x07;//(sizeof(CYBLE_GAP_BD_ADDR_T)
    cmdPacket_ResolveAndSetPeerAddr[CMD_PARA_LEN_HIGH]   = 0x00;
    cmdPacket_ResolveAndSetPeerAddr[CMD_PARAMETERS_BEGINNING + 0]   = SenPSoC_BleAddress[0];
    cmdPacket_ResolveAndSetPeerAddr[CMD_PARAMETERS_BEGINNING + 1]   = SenPSoC_BleAddress[1];
    cmdPacket_ResolveAndSetPeerAddr[CMD_PARAMETERS_BEGINNING + 2]   = SenPSoC_BleAddress[2];
    cmdPacket_ResolveAndSetPeerAddr[CMD_PARAMETERS_BEGINNING + 3]   = SenPSoC_BleAddress[3];
    cmdPacket_ResolveAndSetPeerAddr[CMD_PARAMETERS_BEGINNING + 4]   = SenPSoC_BleAddress[4];
    cmdPacket_ResolveAndSetPeerAddr[CMD_PARAMETERS_BEGINNING + 5]   = SenPSoC_BleAddress[5];
    cmdPacket_ResolveAndSetPeerAddr[CMD_PARAMETERS_BEGINNING + 6]   = 0x00;//Type: Public
}

void SenBleDongleTalkFundamentals::gattMapCmdPacketsInitial()
{
    // 00: Cmd_Discover_All_Primary_Services_Api ( Check Parameter; API Return )
    cmdPacket_DiscoverAllPrimaryServices[CMD_OPCODE_LOW]      = get_L8((uchar*)&gattCmdOpCodeArray[Cmd_Discover_All_Primary_Services_Api]);
    cmdPacket_DiscoverAllPrimaryServices[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gattCmdOpCodeArray[Cmd_Discover_All_Primary_Services_Api]);
    cmdPacket_DiscoverAllPrimaryServices[CMD_PARA_LEN_LOW]    = 0x02;// Needed: send last Connected Handle Info
    cmdPacket_DiscoverAllPrimaryServices[CMD_PARA_LEN_HIGH]   = 0x00;

    // 02: Cmd_Find_Included_Services_Api ( Check Parameter; API Return )
    cmdPacket_FindIncludedServices_Header[CMD_OPCODE_LOW]      = get_L8((uchar*)&gattCmdOpCodeArray[Cmd_Find_Included_Services_Api]);
    cmdPacket_FindIncludedServices_Header[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gattCmdOpCodeArray[Cmd_Find_Included_Services_Api]);
    cmdPacket_FindIncludedServices_Header[CMD_PARA_LEN_LOW]    = 0x06;// Needed: send last Connected Handle Info + handleRange
    cmdPacket_FindIncludedServices_Header[CMD_PARA_LEN_HIGH]   = 0x00;

    // 03: Cmd_Discover_All_Characteristics_Api ( Check Parameter; API Return )
    cmdPacket_DiscoverAllCharacteristics_Header[CMD_OPCODE_LOW]      = get_L8((uchar*)&gattCmdOpCodeArray[Cmd_Discover_All_Characteristics_Api]);
    cmdPacket_DiscoverAllCharacteristics_Header[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gattCmdOpCodeArray[Cmd_Discover_All_Characteristics_Api]);
    cmdPacket_DiscoverAllCharacteristics_Header[CMD_PARA_LEN_LOW]    = 0x06;// Needed: send last Connected Handle Info + handleRange
    cmdPacket_DiscoverAllCharacteristics_Header[CMD_PARA_LEN_HIGH]   = 0x00;

    // 05: Cmd_Discover_All_Characteristic_Descriptors_Api ( Check Parameter; API Return )
    cmdPacket_DiscoverAllCharacterDescriptors_Header[CMD_OPCODE_LOW]      = get_L8((uchar*)&gattCmdOpCodeArray[Cmd_Discover_All_Characteristic_Descriptors_Api]);
    cmdPacket_DiscoverAllCharacterDescriptors_Header[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gattCmdOpCodeArray[Cmd_Discover_All_Characteristic_Descriptors_Api]);
    cmdPacket_DiscoverAllCharacterDescriptors_Header[CMD_PARA_LEN_LOW]    = 0x06;// Needed: send last Connected Handle Info + handleRange
    cmdPacket_DiscoverAllCharacterDescriptors_Header[CMD_PARA_LEN_HIGH]   = 0x00;

    // 07: Cmd_Read_Using_Characteristic_Uuid_Api ( API Return )
    cmdPacket_ReadUsingCharacteristicUuid_Header[CMD_OPCODE_LOW]      = get_L8((uchar*)&gattCmdOpCodeArray[Cmd_Read_Using_Characteristic_Uuid_Api]);
    cmdPacket_ReadUsingCharacteristicUuid_Header[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gattCmdOpCodeArray[Cmd_Read_Using_Characteristic_Uuid_Api]);
    cmdPacket_ReadUsingCharacteristicUuid_Header[CMD_PARA_LEN_LOW]    = 0x09;
    cmdPacket_ReadUsingCharacteristicUuid_Header[CMD_PARA_LEN_HIGH]   = 0x00;

    cmdPacket_ReadUsingCharacteristicUuid_TypeValueHandleRange[0]      = 0x01;//CYBLE_GATT_16_BIT_UUID_FORMAT
    cmdPacket_ReadUsingCharacteristicUuid_TypeValueHandleRange[1]      = 0x00;
    cmdPacket_ReadUsingCharacteristicUuid_TypeValueHandleRange[2]      = 0x2A;//uuid16	0x2A00: Device Name
    cmdPacket_ReadUsingCharacteristicUuid_TypeValueHandleRange[3]      = 0x01;
    cmdPacket_ReadUsingCharacteristicUuid_TypeValueHandleRange[4]      = 0x00;
    cmdPacket_ReadUsingCharacteristicUuid_TypeValueHandleRange[5]      = 0xFF;
    cmdPacket_ReadUsingCharacteristicUuid_TypeValueHandleRange[6]      = 0xFF;

    // 11: Cmd_Write_Characteristic_Value_Api (       )
    cmdPacket_WriteCharacteristicValue_Header[CMD_OPCODE_LOW]      = get_L8((uchar*)&gattCmdOpCodeArray[Cmd_Write_Characteristic_Value_Api]);
    cmdPacket_WriteCharacteristicValue_Header[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gattCmdOpCodeArray[Cmd_Write_Characteristic_Value_Api]);
    cmdPacket_WriteCharacteristicValue_Header[CMD_PARA_LEN_LOW]    = 0x0A;
    cmdPacket_WriteCharacteristicValue_Header[CMD_PARA_LEN_HIGH]   = 0x00;
    cmdPacket_WriteCharacteristicValue_Header[CMD_PARAMETERS_BEGINNING + 0]   = 0x04;
    cmdPacket_WriteCharacteristicValue_Header[CMD_PARAMETERS_BEGINNING + 1]   = 0x00;// Device Handle

    cmdPacket_WriteCharacteristicValue_LedFrequencyServiceHandle[0]   = 0x0E;// LED Frequncy
    cmdPacket_WriteCharacteristicValue_LedFrequencyServiceHandle[1]   = 0x00;// Handle of Descriptor to write
    cmdPacket_WriteCharacteristicValue_LedFrequencyServiceHandle[2]   = 0x04;
    cmdPacket_WriteCharacteristicValue_LedFrequencyServiceHandle[3]   = 0x00;// Length of data to write

    cmdPacket_WriteCharacteristicValue_LED_Red[0]   = 0xFF;// channel red
    cmdPacket_WriteCharacteristicValue_LED_Red[1]   = 0x00;// channel green
    cmdPacket_WriteCharacteristicValue_LED_Red[2]   = 0x00;
    cmdPacket_WriteCharacteristicValue_LED_Red[3]   = 0xAF;// not 100% bright

    cmdPacket_WriteCharacteristicValue_LED_Green[0]   = 0x00;// channel red
    cmdPacket_WriteCharacteristicValue_LED_Green[1]   = 0xFF;// channel green
    cmdPacket_WriteCharacteristicValue_LED_Green[2]   = 0x00;
    cmdPacket_WriteCharacteristicValue_LED_Green[3]   = 0xAF;// not 100% bright

    cmdPacket_WriteCharacteristicValue_LED_Blue[0]   = 0x00;// channel red
    cmdPacket_WriteCharacteristicValue_LED_Blue[1]   = 0x00;// channel green
    cmdPacket_WriteCharacteristicValue_LED_Blue[2]   = 0xFF;
    cmdPacket_WriteCharacteristicValue_LED_Blue[3]   = 0xAF;// not 100% bright


    // 16: Cmd_Write_Characteristic_Descriptor_Api ( API Return )
    cmdPacket_WriteCharacteristicDescriptor_Header[CMD_OPCODE_LOW]      = get_L8((uchar*)&gattCmdOpCodeArray[Cmd_Write_Characteristic_Descriptor_Api]);
    cmdPacket_WriteCharacteristicDescriptor_Header[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gattCmdOpCodeArray[Cmd_Write_Characteristic_Descriptor_Api]);
    cmdPacket_WriteCharacteristicDescriptor_Header[CMD_PARA_LEN_LOW]    = 0x08;
    cmdPacket_WriteCharacteristicDescriptor_Header[CMD_PARA_LEN_HIGH]   = 0x00;
    cmdPacket_WriteCharacteristicDescriptor_Header[CMD_PARAMETERS_BEGINNING + 0]   = 0x04;
    cmdPacket_WriteCharacteristicDescriptor_Header[CMD_PARAMETERS_BEGINNING + 1]   = 0x00;// Device Handle

    cmdPacket_WriteCharacteristicDescriptor_MotionServiceHandle[0]   = 0x13;// Motion Burst
    cmdPacket_WriteCharacteristicDescriptor_MotionServiceHandle[1]   = 0x00;// Handle of Descriptor to write
    cmdPacket_WriteCharacteristicDescriptor_MotionServiceHandle[2]   = 0x02;
    cmdPacket_WriteCharacteristicDescriptor_MotionServiceHandle[3]   = 0x00;// Length of data to write

    cmdPacket_WriteCharacteristicDescriptor_ImageBurstServiceHandle[0]   = 0x17;// Image Burst
    cmdPacket_WriteCharacteristicDescriptor_ImageBurstServiceHandle[1]   = 0x00;// Handle of Descriptor to write
    cmdPacket_WriteCharacteristicDescriptor_ImageBurstServiceHandle[2]   = 0x02;
    cmdPacket_WriteCharacteristicDescriptor_ImageBurstServiceHandle[3]   = 0x00;// Length of data to write

    cmdPacket_WriteCharacteristicDescriptor_ServiceActive[0]   = 0x01;
    cmdPacket_WriteCharacteristicDescriptor_ServiceActive[1]   = 0x00;// Active Control

    cmdPacket_WriteCharacteristicDescriptor_ServiceStop[0]   = 0x00;
    cmdPacket_WriteCharacteristicDescriptor_ServiceStop[1]   = 0x00;// Stop Control



    // 18: Cmd_Exchange_GATT_MTU_Size_Api ( Check Parameter; API Return )
    cmdPacket_ExchangeGatt_MTU_Size_Header[CMD_OPCODE_LOW]      = get_L8((uchar*)&gattCmdOpCodeArray[Cmd_Exchange_GATT_MTU_Size_Api]);
    cmdPacket_ExchangeGatt_MTU_Size_Header[CMD_OPCODE_HIGH]     = get_H8((uchar*)&gattCmdOpCodeArray[Cmd_Exchange_GATT_MTU_Size_Api]);
    cmdPacket_ExchangeGatt_MTU_Size_Header[CMD_PARA_LEN_LOW]    = 0x04;
    cmdPacket_ExchangeGatt_MTU_Size_Header[CMD_PARA_LEN_HIGH]   = 0x00;

    cmdPacket_ExchangeGatt_MTU_Size_SetMTU[0]                   = 0x00;
    cmdPacket_ExchangeGatt_MTU_Size_SetMTU[1]                   = 0x02;//Central MTU: 0x0200



}

void SenBleDongleTalkFundamentals::L2CAP_MapCmdPacketsInitial()
{

}

void SenBleDongleTalkFundamentals::cmdOpCodeInitial()
{
    // General Command
    generalCmdOpCodeArray[Cmd_Get_Device_Id_Api] = 0x0000;
    generalCmdStringArray[Cmd_Get_Device_Id_Api] = "'Get Device Id'";

    generalCmdOpCodeArray[Cmd_Get_Supported_Tool_Ver_Api] = 0x0001;
    generalCmdStringArray[Cmd_Get_Supported_Tool_Ver_Api] = "'Get Supported Tool Ver'";

    generalCmdOpCodeArray[Cmd_Get_Firmware_Version_Api] = 0x0002;
    generalCmdStringArray[Cmd_Get_Firmware_Version_Api] = "'Get Firmware Version Api'";

    generalCmdOpCodeArray[Cmd_Get_Supported_Gap_Roles_Api] = 0x0003;
    generalCmdStringArray[Cmd_Get_Supported_Gap_Roles_Api] = "'Get Supported Gap Roles Api'";

    generalCmdOpCodeArray[Cmd_Get_Current_Gap_Role_Api] = 0x0004;
    generalCmdStringArray[Cmd_Get_Current_Gap_Role_Api] = "'Get_Current_Gap_Role Api'";

    generalCmdOpCodeArray[Cmd_Get_Supported_Gatt_Roles_Api] = 0x0005;
    generalCmdStringArray[Cmd_Get_Supported_Gatt_Roles_Api] = "'Get_Supported_Gatt_Roles_Api'";

    generalCmdOpCodeArray[Cmd_Get_Current_Gatt_Role_Api] = 0x0006;
    generalCmdStringArray[Cmd_Get_Current_Gatt_Role_Api] = "'Get_Current_Gatt_Role_Api'";

    generalCmdOpCodeArray[Cmd_Init_Ble_Stack_Api] = 0xFC07;//0x0007;
    generalCmdStringArray[Cmd_Init_Ble_Stack_Api] = "'Init Ble Stack'";

    generalCmdOpCodeArray[Cmd_Tool_Disconnected_Api] = 0x0008;
    generalCmdStringArray[Cmd_Tool_Disconnected_Api] = "'Cmd_Tool_Disconnected_Api'";

    generalCmdOpCodeArray[Cmd_Host_Timed_Out_Api] = 0x0009;
    generalCmdStringArray[Cmd_Host_Timed_Out_Api] = "'Cmd_Host_Timed_Out_Api'";

    generalCmdOpCodeArray[Cmd_Get_Device_Descriptor_Info] = 0x000A;
    generalCmdStringArray[Cmd_Get_Device_Descriptor_Info] = "'Cmd_Get_Device_Descriptor_Info'";

    generalCmdOpCodeArray[Cmd_Get_Hardware_Version_Api] = 0x000B;
    generalCmdStringArray[Cmd_Get_Hardware_Version_Api] = "'Cmd_Get_Hardware_Version_Api'";

    generalCmdOpCodeArray[Cmd_Get_Ble_Stack_Version_Api] = 0x000C;
    generalCmdStringArray[Cmd_Get_Ble_Stack_Version_Api] = "'Get Ble Stack Version'";

    generalCmdOpCodeArray[Cmd_Get_Rssi_Api] = 0x000D;
    generalCmdStringArray[Cmd_Get_Rssi_Api] = "'Get Rssi'";

    // Gap Command
    /**************************  Gap Command    00 - 04      *********************************/
    gapCmdOpCodeArray[Cmd_Set_Device_Io_Capabilities_Api] = 0x0280;
    gapCmdStringArray[Cmd_Set_Device_Io_Capabilities_Api] = "'Set Device Io Capabilities'";

    gapCmdOpCodeArray[Cmd_Get_Device_Io_Capabilities_Api] = 0x0281;
    gapCmdStringArray[Cmd_Get_Device_Io_Capabilities_Api] = "'Get Device Io Capabilities'";

    gapCmdOpCodeArray[Cmd_Get_Bluetooth_Device_Address_Api] = 0x0282;
    gapCmdStringArray[Cmd_Get_Bluetooth_Device_Address_Api] = "'Get Bluetooth Device Address'";

    gapCmdOpCodeArray[Cmd_Set_Bluetooth_Device_Address_Api] = 0x0283;
    gapCmdStringArray[Cmd_Set_Bluetooth_Device_Address_Api] = "'Set Bluetooth Device Address'";

    gapCmdOpCodeArray[Cmd_Get_Peer_Bluetooth_Device_Address_Api] = 0x0284;
    gapCmdStringArray[Cmd_Get_Peer_Bluetooth_Device_Address_Api] = "'Get Peer Bluetooth Device Address'";

    /**************************  Gap Command    05 - 09      *********************************/
    gapCmdOpCodeArray[Cmd_Get_Peer_Device_Handle_Api] = 0x0285;
    gapCmdStringArray[Cmd_Get_Peer_Device_Handle_Api] = "'Get Peer Device Handle'";

    gapCmdOpCodeArray[Cmd_GenerateBd_Addr_Api] = 0x0286;
    gapCmdStringArray[Cmd_GenerateBd_Addr_Api] = "'GenerateBd Addr'";

    gapCmdOpCodeArray[Cmd_Set_Oob_Data_Api] = 0x0287;
    gapCmdStringArray[Cmd_Set_Oob_Data_Api] = "'Set Oob Data'";

    gapCmdOpCodeArray[Cmd_Get_Connection_Parameters_Api] = 0x0288;
    gapCmdStringArray[Cmd_Get_Connection_Parameters_Api] = "'Get Connection Parameters'";

    gapCmdOpCodeArray[Cmd_Set_Connection_Parameters_Api] = 0x0289;
    gapCmdStringArray[Cmd_Set_Connection_Parameters_Api] = "'Set Connection Parameters'";

    /**************************  Gap Command    10 - 14      *********************************/
    gapCmdOpCodeArray[Cmd_Get_Scan_Parameters_Api] = 0x028A;
    gapCmdStringArray[Cmd_Get_Scan_Parameters_Api] = "'Get Scan Parameters'";

    gapCmdOpCodeArray[Cmd_Set_Scan_Parameters_Api] = 0x028B;
    gapCmdStringArray[Cmd_Set_Scan_Parameters_Api] = "'Set Scan Parameters'";

    gapCmdOpCodeArray[Cmd_Get_Local_Device_Security_Api] = 0x028C;
    gapCmdStringArray[Cmd_Get_Local_Device_Security_Api] = "'Get Local Device Security'";

    gapCmdOpCodeArray[Cmd_Set_Local_Device_Security_Api] = 0x028D;
    gapCmdStringArray[Cmd_Set_Local_Device_Security_Api] = "'Set Local Device Security'";

    gapCmdOpCodeArray[Cmd_Get_Peer_Device_Security_Api] = 0x028E;
    gapCmdStringArray[Cmd_Get_Peer_Device_Security_Api] = "'Get Peer Device Security'";

    /**************************   Gap Command   15 - 19      *********************************/
    gapCmdOpCodeArray[Cmd_Get_White_List_Api] = 0x028F;
    gapCmdStringArray[Cmd_Get_White_List_Api] = "'Get White List'";

    gapCmdOpCodeArray[Cmd_Add_Device_To_White_List_Api] = 0x0290;
    gapCmdStringArray[Cmd_Add_Device_To_White_List_Api] = "'Add Device To White List'";

    gapCmdOpCodeArray[Cmd_Remove_Device_From_White_List_Api] = 0x0291;
    gapCmdStringArray[Cmd_Remove_Device_From_White_List_Api] = "'Remove Device From White List'";

    gapCmdOpCodeArray[Cmd_Clear_White_List_Api] = 0x0292;
    gapCmdStringArray[Cmd_Clear_White_List_Api] = "'Clear White List'";

    gapCmdOpCodeArray[Cmd_Start_Scan_Api] = 0xFE93;//0x0293;
    gapCmdStringArray[Cmd_Start_Scan_Api] = "'Start Scan'";

    /**************************  Gap Command    20 - 24      *********************************/
    gapCmdOpCodeArray[Cmd_Stop_Scan_Api] = 0xFE94;
    gapCmdStringArray[Cmd_Stop_Scan_Api] = "'Stop Scan'";

    gapCmdOpCodeArray[Cmd_Generate_Keys_Api] = 0xFE95;
    gapCmdStringArray[Cmd_Generate_Keys_Api] = "'Generate Keys'";

    gapCmdOpCodeArray[Cmd_Set_Authentication_Keys_Api] = 0xFE96;
    gapCmdStringArray[Cmd_Set_Authentication_Keys_Api] = "'Set Authentication Keys'";

    gapCmdOpCodeArray[Cmd_Establish_Connection_Api] = 0xFE97;
    gapCmdStringArray[Cmd_Establish_Connection_Api] = "'Establish Connection'";

    gapCmdOpCodeArray[Cmd_Terminate_Connection_Api] = 0xFE98;
    gapCmdStringArray[Cmd_Terminate_Connection_Api] = "'Terminate Connection'";

    /**************************  Gap Command    25 - 29      *********************************/
    gapCmdOpCodeArray[Cmd_Initiate_Pairing_Request_Api] = 0xFE99;
    gapCmdStringArray[Cmd_Initiate_Pairing_Request_Api] = "'Initiate Pairing Request'";

    gapCmdOpCodeArray[Cmd_Delete_Pairing_Info_Api] = 0x029A;
    gapCmdStringArray[Cmd_Delete_Pairing_Info_Api] = "'Delete Pairing Info'";

    gapCmdOpCodeArray[Cmd_Pairing_PassKey_Api] = 0x029B;
    gapCmdStringArray[Cmd_Pairing_PassKey_Api] = "'Pairing PassKey'";

    gapCmdOpCodeArray[Cmd_Update_Connection_Params_Api] = 0x029C;
    gapCmdStringArray[Cmd_Update_Connection_Params_Api] = "'Update Connection Params'";

    gapCmdOpCodeArray[Cmd_Cancle_Connection_Api] = 0x029D;
    gapCmdStringArray[Cmd_Cancle_Connection_Api] = "'Cancle Connection'";

    /**************************  Gap Command    30 - 33      *********************************/
    gapCmdOpCodeArray[Cmd_Get_Bonded_Devices_List_Api] = 0x029E;
    gapCmdStringArray[Cmd_Get_Bonded_Devices_List_Api] = "'Get Bonded Devices List'";

    gapCmdOpCodeArray[Cmd_UpdateConnectionParam_Resp_Api] = 0x029F;
    gapCmdStringArray[Cmd_UpdateConnectionParam_Resp_Api] = "'Update Connection Parameter Response'";

    gapCmdOpCodeArray[Cmd_Get_PeerDevice_SecurityKeys_Api] = 0x02A0;
    gapCmdStringArray[Cmd_Get_PeerDevice_SecurityKeys_Api] = "'Get PeerDevice SecurityKeys'";

    gapCmdOpCodeArray[Cmd_Resolve_Set_Peer_Addr_Api] = 0x02A1;
    gapCmdStringArray[Cmd_Resolve_Set_Peer_Addr_Api] = "'Resolve Set Peer Addr'";

    // Gatt Command
    /**************************  Gatt Command    00 - 04      *********************************/
    gattCmdOpCodeArray[Cmd_Discover_All_Primary_Services_Api] = 0xFE00;
    gattCmdStringArray[Cmd_Discover_All_Primary_Services_Api] = "'Discover All Primary Services'";

    gattCmdOpCodeArray[Cmd_Discover_Primary_Services_By_Uuid_Api] = 0xFE01;
    gattCmdStringArray[Cmd_Discover_Primary_Services_By_Uuid_Api] = "'Discover Primary Services By Uuid'";

    gattCmdOpCodeArray[Cmd_Find_Included_Services_Api] = 0xFE02;
    gattCmdStringArray[Cmd_Find_Included_Services_Api] = "'Find Included Services'";

    gattCmdOpCodeArray[Cmd_Discover_All_Characteristics_Api] = 0xFE03;
    gattCmdStringArray[Cmd_Discover_All_Characteristics_Api] = "'Discover All Characteristics'";

    gattCmdOpCodeArray[Cmd_Discover_Characteristics_By_Uuid_Api] = 0xFE04;
    gattCmdStringArray[Cmd_Discover_Characteristics_By_Uuid_Api] = "'Discover Characteristics By Uuid'";

    /**************************  Gatt Command    05 - 09      *********************************/
    gattCmdOpCodeArray[Cmd_Discover_All_Characteristic_Descriptors_Api] = 0xFE05;
    gattCmdStringArray[Cmd_Discover_All_Characteristic_Descriptors_Api] = "'Discover All Characteristic Descriptors'";

    gattCmdOpCodeArray[Cmd_Read_Characteristic_Value_Api] = 0xFE06;
    gattCmdStringArray[Cmd_Read_Characteristic_Value_Api] = "'Read Characteristic Value'";

    gattCmdOpCodeArray[Cmd_Read_Using_Characteristic_Uuid_Api] = 0xFE07;
    gattCmdStringArray[Cmd_Read_Using_Characteristic_Uuid_Api] = "'Read Using Characteristic Uuid'";

    gattCmdOpCodeArray[Cmd_Read_Long_Characteristic_Values_Api] = 0xFE08;
    gattCmdStringArray[Cmd_Read_Long_Characteristic_Values_Api] = "'Read Long Characteristic Values'";

    gattCmdOpCodeArray[Cmd_Read_Multiple_Characteristic_Values_Api] = 0xFE09;
    gattCmdStringArray[Cmd_Read_Multiple_Characteristic_Values_Api] = "'Read Multiple Characteristic Values'";

    /**************************  Gatt Command    10 - 14      *********************************/
    gattCmdOpCodeArray[Cmd_Characteristic_Value_Write_Without_Response_Api] = 0xFE0A;
    gattCmdStringArray[Cmd_Characteristic_Value_Write_Without_Response_Api] = "'Characteristic Value Write Without Response'";

    gattCmdOpCodeArray[Cmd_Write_Characteristic_Value_Api] = 0xFE0B;
    gattCmdStringArray[Cmd_Write_Characteristic_Value_Api] = "'Write Characteristic Value'";

    gattCmdOpCodeArray[Cmd_Write_Long_Characteristic_Value_Api] = 0xFE0C;
    gattCmdStringArray[Cmd_Write_Long_Characteristic_Value_Api] = "'Write Long Characteristic Value'";

    gattCmdOpCodeArray[Cmd_Reliable_Characteristic_Value_Writes_Api] = 0xFE0D;
    gattCmdStringArray[Cmd_Reliable_Characteristic_Value_Writes_Api] = "'Reliable Characteristic Value Writes'";

    gattCmdOpCodeArray[Cmd_Read_Characteristic_Descriptor_Api] = 0xFE0E;
    gattCmdStringArray[Cmd_Read_Characteristic_Descriptor_Api] = "'Read Characteristic Descriptor'";

    /**************************  Gatt Command    15 - 20      *********************************/
    gattCmdOpCodeArray[Cmd_Read_Long_Characteristic_Descriptor_Api] = 0xFE0F;
    gattCmdStringArray[Cmd_Read_Long_Characteristic_Descriptor_Api] = "'Read Long Characteristic Descriptor'";

    gattCmdOpCodeArray[Cmd_Write_Characteristic_Descriptor_Api] = 0xFE10;
    gattCmdStringArray[Cmd_Write_Characteristic_Descriptor_Api] = "'Write Characteristic Descriptor'";

    gattCmdOpCodeArray[Cmd_Write_Long_Characteristic_Descriptor_Api] = 0xFE11;
    gattCmdStringArray[Cmd_Write_Long_Characteristic_Descriptor_Api] = "'Write Long Characteristic Descriptor'";

    gattCmdOpCodeArray[Cmd_Exchange_GATT_MTU_Size_Api] = 0xFE12;
    gattCmdStringArray[Cmd_Exchange_GATT_MTU_Size_Api] = "'Exchange GATT MTU Size'";

    gattCmdOpCodeArray[Cmd_GATT_Stop_Api] = 0xFE13;
    gattCmdStringArray[Cmd_GATT_Stop_Api] = "'GATT Stop'";

    gattCmdOpCodeArray[Cmd_Signed_Write_Without_Response_Api] = 0xFE14;
    gattCmdStringArray[Cmd_Signed_Write_Without_Response_Api] = "'Signed Write Without Response'";

      // L2CAP Command
      /**************************  L2CAP Command    00 - 04      *********************************/
    L2CAP_CmdOpCodeArray[Cmd_Register_PSM_Api] = 0xFD00;//0x0100;
    L2CAP_CmdStringArray[Cmd_Register_PSM_Api] = "'Register PSM'";

    L2CAP_CmdOpCodeArray[Cmd_Unregister_PSM_Api] = 0xFD01;//0x0101;
    L2CAP_CmdStringArray[Cmd_Unregister_PSM_Api] = "'Unregister PSM'";

    L2CAP_CmdOpCodeArray[Cmd_CBFC_SendConnectionReq_Api] = 0xFD02;//0x0102;
    L2CAP_CmdStringArray[Cmd_CBFC_SendConnectionReq_Api] = "'CBFC SendConnectionReq'";

    L2CAP_CmdOpCodeArray[Cmd_CBFC_SendConnectionResp_Api] = 0xFD03;//0x0103;
    L2CAP_CmdStringArray[Cmd_CBFC_SendConnectionResp_Api] = "'CBFC SendConnectionResp'";

    L2CAP_CmdOpCodeArray[Cmd_CBFC_SendFlowControlCredit_Api] = 0xFD04;//0x0104;
    L2CAP_CmdStringArray[Cmd_CBFC_SendFlowControlCredit_Api] = "'CBFC SendFlowControlCredit'";

    L2CAP_CmdOpCodeArray[Cmd_CBFC_SendData_Api] = 0xFD05;//0x0105;
    L2CAP_CmdStringArray[Cmd_CBFC_SendData_Api] = "'CBFC SendData'";

    L2CAP_CmdOpCodeArray[Cmd_CBFC_SendDisconnectReq_Api] = 0xFD06;//0x0106;
    L2CAP_CmdStringArray[Cmd_CBFC_SendDisconnectReq_Api] = "'CBFC SendDisconnectReq'";
}

void SenBleDongleTalkFundamentals::statusInitial()
{
    statusStringArray[CYBLE_ERROR_OK]                               = "CYBLE_ERROR_OK";
    statusStringArray[CYBLE_ERROR_INVALID_PARAMETER]                = "CYBLE_ERROR_INVALID_PARAMETER";
    statusStringArray[CYBLE_ERROR_INVALID_OPERATION]                = "CYBLE_ERROR_INVALID_OPERATION";
    statusStringArray[CYBLE_ERROR_MEMORY_ALLOCATION_FAILED]         = "CYBLE_ERROR_MEMORY_ALLOCATION_FAILED";
    statusStringArray[CYBLE_ERROR_INSUFFICIENT_RESOURCES]           = "CYBLE_ERROR_INSUFFICIENT_RESOURCES";
    statusStringArray[CYBLE_ERROR_OOB_NOT_AVAILABLE]                = "CYBLE_ERROR_OOB_NOT_AVAILABLE";
    statusStringArray[CYBLE_ERROR_NO_CONNECTION]                    = "CYBLE_ERROR_NO_CONNECTION";
    statusStringArray[CYBLE_ERROR_NO_DEVICE_ENTITY]                 = "CYBLE_ERROR_NO_DEVICE_ENTITY";
    statusStringArray[CYBLE_ERROR_REPEATED_ATTEMPTS]                = "CYBLE_ERROR_REPEATED_ATTEMPTS";
    statusStringArray[CYBLE_ERROR_GAP_ROLE]                         = "CYBLE_ERROR_GAP_ROLE";
    statusStringArray[CYBLE_ERROR_TX_POWER_READ]                    = "CYBLE_ERROR_TX_POWER_READ";
    statusStringArray[CYBLE_ERROR_BT_ON_NOT_COMPLETED]              = "CYBLE_ERROR_BT_ON_NOT_COMPLETED";
    statusStringArray[CYBLE_ERROR_SEC_FAILED]                       = "CYBLE_ERROR_SEC_FAILED";

    /******************************** L2CAP error codes ********************************/
    statusStringArray[CYBLE_ERROR_L2CAP_PSM_WRONG_ENCODING]         = "CYBLE_ERROR_L2CAP_PSM_WRONG_ENCODING";
    statusStringArray[CYBLE_ERROR_L2CAP_PSM_ALREADY_REGISTERED]     = "CYBLE_ERROR_L2CAP_PSM_ALREADY_REGISTERED";
    statusStringArray[CYBLE_ERROR_L2CAP_PSM_NOT_REGISTERED]         = "CYBLE_ERROR_L2CAP_PSM_NOT_REGISTERED";
    statusStringArray[CYBLE_ERROR_L2CAP_CONNECTION_ENTITY_NOT_FOUND] = "CYBLE_ERROR_L2CAP_CONNECTION_ENTITY_NOT_FOUND";
    statusStringArray[CYBLE_ERROR_L2CAP_CHANNEL_NOT_FOUND]          = "CYBLE_ERROR_L2CAP_CHANNEL_NOT_FOUND";
    statusStringArray[CYBLE_ERROR_L2CAP_PSM_NOT_IN_RANGE]           = "CYBLE_ERROR_L2CAP_PSM_NOT_IN_RANGE";
    statusStringArray[CYBLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE]      = "CYBLE_ERROR_GATT_DB_INVALID_ATTR_HANDLE";
    /****************************** Other Codes.  ***********************************/
    statusStringArray[CYBLE_ERROR_DEVICE_ALREADY_EXISTS]            = "CYBLE_ERROR_DEVICE_ALREADY_EXISTS";
    statusStringArray[CYBLE_ERROR_FLASH_WRITE_NOT_PERMITED]         = "CYBLE_ERROR_FLASH_WRITE_NOT_PERMITED";
    statusStringArray[CYBLE_ERROR_MIC_AUTH_FAILED]                  = "CYBLE_ERROR_MIC_AUTH_FAILED";
    statusStringArray[CYBLE_ERROR_MAX]                              = "CYBLE_ERROR_MAX";
}

void SenBleDongleTalkFundamentals::serialBleInfoGroupHardWrie()
{
    firstSerialInfo.name = "COM4";
    secondSerialInfo.name = "COM3";
    defaultSettingsInfo.baudRate = 115200;
    defaultSettingsInfo.dataBits = (QSerialPort::DataBits)8;
    defaultSettingsInfo.parity = (QSerialPort::Parity)0;
    defaultSettingsInfo.stopBits = (QSerialPort::StopBits)1;
    defaultSettingsInfo.flowControl = (QSerialPort::FlowControl)0;
}

QByteArray SenBleDongleTalkFundamentals::getReversedHexStringFromUnSignedRaw(QByteArray rawUnsignedData)
{
    QByteArray reversedHex;
    QByteArray rawHexChange = rawUnsignedData.toHex();

    for (int i=(rawUnsignedData.length() - 1); i>=0; i--)  {
        int j = i * 2;
        reversedHex += (uchar)rawHexChange[j];  // Low 4 bit Hex Info after .toHex
        reversedHex += (uchar)rawHexChange[j + 1];  // High 4 bit Hex Info after .toHex
    }

    return reversedHex;
}
uchar SenBleDongleTalkFundamentals::get_H8(uchar* dataPointer)
{
    return dataPointer[1];
}

uchar SenBleDongleTalkFundamentals::get_L8(uchar* dataPointer)
{
    return *dataPointer;
}

void SenBleDongleTalkFundamentals::eventStatusResponseMapInitial()
{
    headerArrayInitial();
    cmdOpCodeInitial();
    statusInitial();

    cmdPacketHeader += get_L8((uchar*)&headerArray[CMD_PACKET_TO_DONGLE_HEADER]);
    cmdPacketHeader += get_H8((uchar*)&headerArray[CMD_PACKET_TO_DONGLE_HEADER]);
}
