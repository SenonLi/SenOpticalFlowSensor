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

//#if !defined(LED_CONTROL_H)
//#define LED_CONTROL_H
	
/* RGB LED Characteristic data length*/ 
#define RGB_CHAR_DATA_LEN 4

/* LED Color and status related Macros */ 
#define RGB_LED_MAX_VAL 	0xFF
#define RGB_LED_OFF 		0xFF
#define RGB_LED_ON 			0x00 

/* Index values in array where respective color coordinates * are saved */
#define RED_INDEX 			0x00 
#define GREEN_INDEX 		0x01
#define BLUE_INDEX 			0x02 
#define INTENSITY_INDEX 	0x03



void UpdateRGBLED(uint8* ledData, uint8 len);

/* [] END OF FILE */
