/* ========================================
 *
 * Copyright Sen Li, Dec 2015
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#ifndef BLE_PROCESS_H
#define BLE_PROCESS_H

#include <project.h>
#include "LED_Control.h"
#include "Sen_SPI_OF.h"


#define TRUE 	0x01 
#define FALSE 	0x00

/*****************GATT Error code*******************/
#define ERR_INVALID_PDU 0x04
#define CCCD_VALID_BIT_MASK 0x03
#define NOTIFY_BIT_MASK 0x01


/* Client Characteristic Configuration descriptor data length. This is defined as per BLE spec. */
#define Sen_18_PixelsArray_LEN		301u

/* Data length of CapSense Slider data sent over notification */
#define Sen_MotionBurst_DATA_LEN		2u

#define CCCD_DATA_LEN		2u


void GeneralEventHandler(uint32 event, void * eventParam);
void UpdateRGBcharacteristic(uint8* ledData, uint8 len, uint16 attrHandle);
void endMotionBurstCharacteristic();
void endImageBurstCharacteristic();

void SendDataOverSenMotionBurstNotification(uint8 delta_X, uint8 delta_Y);
void Send18ArrayDataOverSenImageBurstNotification(uint8 *Pixels18Array);


#endif /* BLE_PROCESS_H */
/* [] END OF FILE */
