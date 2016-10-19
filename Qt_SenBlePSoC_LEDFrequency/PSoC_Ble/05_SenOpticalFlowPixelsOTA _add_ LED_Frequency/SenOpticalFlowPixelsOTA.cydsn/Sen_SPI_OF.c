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

#include "Sen_SPI_OF.h"


void SenDeselectChip(void){
  SS_Write(1);
//  SPI_Master_SpiSetActiveSlaveSelect(0x01u);
}

void SenChipSelect(void){
  	SS_Write(0);
//  SPI_Master_SpiSetActiveSlaveSelect(0x00u);
}
  
void SenReset(void){
    SenChipSelect();
    CyDelayUs(50);
    Reset_Write(1);                 // reset sensor
    CyDelayUs(75);
    Reset_Write(0);                     // return sensor to normal
    SenDeselectChip(); 
    SenChipSelect();
}

uint32 SenSPI_Transfer(uint8 fixedAddress){
  SPI_Master_SpiUartWriteTxData((uint32)fixedAddress);
  CyDelayUs(6);
  uint32 readBuffer = SPI_Master_SpiUartReadRxData();

  return readBuffer;
}  

void SenUartPutHexDecimalByte(uint8 dataByte) {
	uint8 temp = dataByte;
	temp = temp >> 4;
	if(temp>9)      temp+=('A'-10);
	else temp += '0';
        
	UART_UartPutChar(temp);
        
	dataByte = dataByte & 0x0f;
	if(dataByte > 9)        dataByte+=('A'-10);
	else    dataByte += '0';
        
	UART_UartPutChar(dataByte);
}

uint8 SenReadReg(uint8 fixedAddress){
	uint8 regValue = 0x00u;
	fixedAddress &= (uint8)~(1 << 7);
	SenChipSelect();

	SenSPI_Transfer(fixedAddress);
	CyDelayUs(50);
	regValue = SenSPI_Transfer(0x00);

        SenDeselectChip();

//        CyDelayUs(75);
	return (regValue);
}

void SenWriteReg(uint8 fixedAddress, uint8 dataValue){
  SenChipSelect();
  SenSPI_Transfer(fixedAddress | 0x80 );   // send register address
  CyDelayUs(50);                  // small delay
  SenSPI_Transfer(dataValue);             // send data
  SenDeselectChip(); 
}              