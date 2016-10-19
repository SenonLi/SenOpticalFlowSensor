/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

/* [] END OF FILE */
#include <project.h>

#define REG_INVERSE_PRODUCT_ID 0x3Fu

#define REG_PRODUCT_ID 0x00u
#define MOTION 0x02
#define DELTA_X 0x03
#define DELTA_Y 0x04                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
#define FRAME_CAPTURE 0x13
#define PIXEL_BURST 0x40
#define MOTION_BURST 0x50

#define SEN_IMAGE_DISPLAY_INTERVAL 1
#define SEN_SYNC_SAMPLE_NUM 100
#define SEN_RECONNECTION_CONFIRM_NUM 1500


void SenDeselectChip(void);
void SenChipSelect(void);
void SenReset(void);

void SenShowSetupData(void);
uint32 SenSPI_Transfer(uint8 fixedAddress);
uint8 SenReadReg(uint8 fixedAddress);
void SenWriteReg(uint8 fixedAddress, uint8 dataValue);

void SenUartPutHexDecimalByte(uint8 dataByte);

