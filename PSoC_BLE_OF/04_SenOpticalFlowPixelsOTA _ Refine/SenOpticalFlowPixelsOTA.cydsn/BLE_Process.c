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
#include <project.h>
#include "BLE_Process.h"

/* Status flag for the Stack Busy state. This flag is used to notify the application 
* whether there is stack buffer free to push more data or not */
uint8 busyStatus = 0;

uint8 deviceConnected = FALSE;
uint8 motionBurstStartNotification = FALSE;//sendCapSenseSliderNotifications
uint8 imageBurstStartNotification = FALSE;

int8 motionReadTimes = 0;

void GeneralEventHandler(uint32 event, void * eventParam) { 
	/* Structure to store data written by Client */ 
	CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParam;
	
	/* 'RGBledData[]' is an array to store 4 bytes of RGB LED data*/ 
	uint8 RGBledData[RGB_CHAR_DATA_LEN]; 
	
	switch(event) { 
		case CYBLE_EVT_STACK_ON: 
			/* This event is generated when BLE stack is ON */
			/* BLE stack is on. Start BLE advertisement */ 
			CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
			break;
		
		case CYBLE_EVT_GAPP_ADVERTISEMENT_START_STOP:
			/* This event is generated whenever Advertisement starts or stops. 
			The exact state of advertisement is obtained by CyBle_State() */ 
			if(CyBle_GetState() == CYBLE_STATE_DISCONNECTED) { 
				CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST); 
			} break;
		
		case CYBLE_EVT_GAP_DEVICE_DISCONNECTED: 
			/* This event is generated at GAP disconnection. */ 
			/* Restart advertisement */ 
			CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
			break;

		/**********************************************************
        *                       GATT Events
        ***********************************************************/
        case CYBLE_EVT_GATT_CONNECT_IND:
			/* This event is received when device is connected over GATT level */
			
			/* Update attribute handle on GATT Connection*/
            cyBle_connHandle = *(CYBLE_CONN_HANDLE_T *)eventParam;
			
			/* This flag is used in application to check connection status */
			deviceConnected = TRUE;
			break;
		
		case CYBLE_EVT_GATT_DISCONNECT_IND: 
			/* This event is generated at GATT disconnection. */ 
			/* Update deviceConnected flag*/
			deviceConnected = FALSE;
			
			/* Reset the color values*/
			RGBledData[RED_INDEX] 		= FALSE;
			RGBledData[GREEN_INDEX] 	= FALSE;
			RGBledData[BLUE_INDEX] 		= FALSE;
			RGBledData[INTENSITY_INDEX] = FALSE;
			
			/* Switch off LEDs */
			UpdateRGBLED(RGBledData, sizeof(RGBledData));
			
			/* Register the new color in GATT DB*/
			UpdateRGBcharacteristic(RGBledData, sizeof(RGBledData), CYBLE_RGB_LED_RGB_LED_CONTROL_CHAR_HANDLE);
			
			/* close motionBurstService*/
					
			motionBurstStartNotification = FALSE;
			imageBurstStartNotification = FALSE;
			SenDeselectChip();	motionReadTimes = 0;
			endMotionBurstCharacteristic();
			endImageBurstCharacteristic();
			
			break;
		   	
			
			
		case CYBLE_EVT_GATTS_WRITE_REQ: 
			/* This event is generated when the connected Central device sends a Write request. */ 
			/* The parameter ‘eventParam’ contains the data written */ 
			
			/* Extract the Write data sent by Client */ 
			wrReqParam = (CYBLE_GATTS_WRITE_REQ_PARAM_T *) eventParam; 
			
			/* If the attribute handle of the characteristic written to 
			 * is equal to that of RGB_LED characteristic, then extract the RGB LED data */ 
			if(CYBLE_RGB_LED_RGB_LED_CONTROL_CHAR_HANDLE == wrReqParam->handleValPair.attrHandle) { 
				/* Store RGB LED data in local array */ 
				RGBledData[RED_INDEX] = wrReqParam->handleValPair.value.val[RED_INDEX];
				RGBledData[GREEN_INDEX] = wrReqParam->handleValPair.value.val[GREEN_INDEX];
				RGBledData[BLUE_INDEX] = wrReqParam->handleValPair.value.val[BLUE_INDEX];
				RGBledData[INTENSITY_INDEX] = wrReqParam->handleValPair.value.val[INTENSITY_INDEX];
				
				/* Update the PrISM component density value to represent color */ 
				UpdateRGBLED(RGBledData, sizeof(RGBledData)); 
				
				/* Update the GATT DB for RGB LED read characteristics*/ 
				UpdateRGBcharacteristic(RGBledData, sizeof(RGBledData), CYBLE_RGB_LED_RGB_LED_CONTROL_CHAR_HANDLE);
			}
			/* Check if the returned handle is matching to CCCD attribute */
			/* Sen Motion Burst Service */
			else if(CYBLE_SENMOTIONBURST_MOTIONBURST_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE == wrReqParam->handleValPair.attrHandle) {
				/* Only the first and second lowest significant bit can be 
				* set when writing on CCCD. If any other bit is set, then send error code */
				if(FALSE == (wrReqParam->handleValPair.value.val [CYBLE_SENMOTIONBURST_MOTIONBURST_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_INDEX]
								& (~CCCD_VALID_BIT_MASK))) {
					/* Set flag for application to know status of notifications. 
					* Only one byte is read as it contains the set value. */
					motionBurstStartNotification = wrReqParam->handleValPair.value.val [CYBLE_SENMOTIONBURST_MOTIONBURST_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_INDEX];
					if (TRUE == motionBurstStartNotification)	
							SenReset();
					else	{	SenDeselectChip(); motionReadTimes = 0;	}
					/* Update GATT DB with latest CCCD value */ 
					CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair, FALSE, &cyBle_connHandle, CYBLE_GATT_DB_LOCALLY_INITIATED);
				} else { 
					/* Send error response for Invalid PDU against Write * request */
					CYBLE_GATTS_ERR_PARAM_T err_param;
					err_param.opcode = CYBLE_GATT_WRITE_REQ;
					err_param.attrHandle = wrReqParam->handleValPair.attrHandle;
					err_param.errorCode = ERR_INVALID_PDU;
					
					/* Send Error Response */
					(void)CyBle_GattsErrorRsp(cyBle_connHandle, &err_param);
					/* Return to main loop */
					return;
				}
			}
			/* Sen Image Burst Service */
			else if(CYBLE_SENIMAGEBURST_IMAGEBURST_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE == wrReqParam->handleValPair.attrHandle) {
				/* Only the first and second lowest significant bit can be 
				* set when writing on CCCD. If any other bit is set, then send error code */
				if(FALSE == (wrReqParam->handleValPair.value.val [CYBLE_SENIMAGEBURST_IMAGEBURST_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_INDEX]
								& (~CCCD_VALID_BIT_MASK))) {
					/* Set flag for application to know status of notifications. 
					* Only one byte is read as it contains the set value. */
					imageBurstStartNotification = wrReqParam->handleValPair.value.val [CYBLE_SENIMAGEBURST_IMAGEBURST_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_INDEX];
					if (TRUE == imageBurstStartNotification)	
							SenReset();
					else	SenDeselectChip();
					/* Update GATT DB with latest CCCD value */ 
					CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair, FALSE, &cyBle_connHandle, CYBLE_GATT_DB_LOCALLY_INITIATED);
				} else { 
					/* Send error response for Invalid PDU against Write * request */
					CYBLE_GATTS_ERR_PARAM_T err_param;
					err_param.opcode = CYBLE_GATT_WRITE_REQ;
					err_param.attrHandle = wrReqParam->handleValPair.attrHandle;
					err_param.errorCode = ERR_INVALID_PDU;
					
					/* Send Error Response */
					(void)CyBle_GattsErrorRsp(cyBle_connHandle, &err_param);
					/* Return to main loop */
					return;
				}
			}
			
			
			
			/* Send the response to the write request received. */ 
			CyBle_GattsWriteRsp(cyBle_connHandle);
			
			break; 
			
		case CYBLE_EVT_STACK_BUSY_STATUS:
			/* This event is generated when the internal stack buffer is full and no more
			* data can be accepted or the stack has buffer available and can accept data.
			* This event is used by application to prevent pushing lot of data to stack. */
			
			/* Extract the present stack status */
            busyStatus = * (uint8*)eventParam;
            break;
			
		default:
			break; 
	}
}

void SendDataOverSenMotionBurstNotification(uint8 delta_X, uint8 delta_Y)
{
	/* 'motionBurstNotificationHandle' stores CapSense notification data parameters */
	CYBLE_GATTS_HANDLE_VALUE_NTF_T		motionBurstNotificationHandle;	
	uint8 motionDeltaArray[2];
	motionDeltaArray[0] = delta_X;
	motionDeltaArray[1] = delta_Y;
	/* If stack is not busy, then send the notification */
//	if(busyStatus == CYBLE_STACK_STATE_FREE)
//	{
		/* Update notification handle with CapSense slider data*/
		motionBurstNotificationHandle.attrHandle = CYBLE_SENMOTIONBURST_MOTIONBURST_CHAR_HANDLE;				
		motionBurstNotificationHandle.value.val = &(motionDeltaArray[0]);
		motionBurstNotificationHandle.value.len = Sen_MotionBurst_DATA_LEN;
		
		/* Send the updated handle as part of attribute for notifications */
		CyBle_GattsNotification(cyBle_connHandle,&motionBurstNotificationHandle);
//	}
}

void Send18ArrayDataOverSenImageBurstNotification(uint8 *Pixels18Array)
{
		/* 'motionBurstNotificationHandle' stores CapSense notification data parameters */
		CYBLE_GATTS_HANDLE_VALUE_NTF_T		imageBurstPixels301NotificationHandle;	

		/* Update notification handle with CapSense slider data*/
		imageBurstPixels301NotificationHandle.attrHandle = CYBLE_SENIMAGEBURST_IMAGEBURST_CHAR_HANDLE;				
		imageBurstPixels301NotificationHandle.value.val = Pixels18Array;
		imageBurstPixels301NotificationHandle.value.len = Sen_18_PixelsArray_LEN;
		
		/* Send the updated handle as part of attribute for notifications */
		CyBle_GattsNotification(cyBle_connHandle,&imageBurstPixels301NotificationHandle);
}

void endMotionBurstCharacteristic()	{
		/* update motionBurst dataBase*/
		/* Handle value to update the CCCD */
		CYBLE_GATT_HANDLE_VALUE_PAIR_T motionBurstNotificationCCCDhandle;
		uint8 motionBurstCCCDvalue[2];

		/* Write the present CapSense notification status to the local variable */
		motionBurstCCCDvalue[0] = motionBurstStartNotification;
		motionBurstCCCDvalue[1] = 0x00;
		
		/* Update CCCD handle with notification status data*/
		motionBurstNotificationCCCDhandle.attrHandle = CYBLE_SENMOTIONBURST_MOTIONBURST_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE;
		motionBurstNotificationCCCDhandle.value.val = motionBurstCCCDvalue;
		motionBurstNotificationCCCDhandle.value.len = CCCD_DATA_LEN;
		
		/* Report data to BLE component for sending data when read by Central device */
		CyBle_GattsWriteAttributeValue(&motionBurstNotificationCCCDhandle, FALSE, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
}

void endImageBurstCharacteristic()	{
		/* update motionBurst dataBase*/
		/* Handle value to update the CCCD */
		CYBLE_GATT_HANDLE_VALUE_PAIR_T imageBurstNotificationCCCDhandle;
		uint8 imageBurstCCCDvalue[2];

		/* Write the present CapSense notification status to the local variable */
		imageBurstCCCDvalue[0] = imageBurstStartNotification;
		imageBurstCCCDvalue[1] = 0x00;
		
		/* Update CCCD handle with notification status data*/
		imageBurstNotificationCCCDhandle.attrHandle = CYBLE_SENIMAGEBURST_IMAGEBURST_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE;
		imageBurstNotificationCCCDhandle.value.val = imageBurstCCCDvalue;
		imageBurstNotificationCCCDhandle.value.len = CCCD_DATA_LEN;
		
		/* Report data to BLE component for sending data when read by Central device */
		CyBle_GattsWriteAttributeValue(&imageBurstNotificationCCCDhandle, FALSE, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
}
void UpdateRGBcharacteristic(uint8* ledData, uint8 len, uint16 attrHandle) { 
	/* 'rgbHandle' stores RGB control data parameters */
	CYBLE_GATT_HANDLE_VALUE_PAIR_T rgbHandle;
	
	/* Update RGB control handle with new values */
	rgbHandle.attrHandle = attrHandle;
	rgbHandle.value.val = ledData;
	rgbHandle.value.len = len;
	
	/* Update the RGB LED attribute value. 
	This will allow Client device to read the existing color values over RGB LED characteristic */
	CyBle_GattsWriteAttributeValue(&rgbHandle, FALSE, &cyBle_connHandle, CYBLE_GATT_DB_LOCALLY_INITIATED); 
}

/* [] END OF FILE */
