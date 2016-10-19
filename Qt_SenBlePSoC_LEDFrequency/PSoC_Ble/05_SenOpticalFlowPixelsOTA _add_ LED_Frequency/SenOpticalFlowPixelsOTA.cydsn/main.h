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
#include "LED_Control.h"
#include "BLE_Process.h"
#include "Sen_SPI_OF.h"


extern uint8 deviceConnected;
extern uint8 motionBurstStartNotification;
extern uint8 imageBurstStartNotification;
extern int8 motionReadTimes;

void Initialize_BLE_ISR_RGBLED_System(void);
void SenMotionBurstHandler();

/* motionBurst Functions*/
uint8 motionAccumulation = 0;



uint8 SenSerialReadBlocking(void);
uint8 UART_ReadAvailableCheck(void);
void loop(void);
void SenImageDisplay();
void SenContinuousImageDisplay();
void SenPixelsShowUp(void);

void SenReadCoodinataData(void);

void SenContinueMotionBurst(void)   {;}

//
//int main()
//{
//
//
//}
//












/* [] END OF FILE */
