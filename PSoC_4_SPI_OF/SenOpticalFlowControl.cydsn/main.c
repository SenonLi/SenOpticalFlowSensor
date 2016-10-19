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
#include <project.h>
#include "Sen_SPI_OF.h"
#define False 0
#define True !False

uint8 UART_ReadAvailableCheck(void);
void loop(void);
void SenImageDisplay();
void SenContinuousImageDisplay();
void SenPixelsShowUp(void);

void SenReadCoodinataData(void);

void SenContinueMotionBurst(void)   {;}

int n=0, motionBurstCount=0;
int initialing = 0;    // 1: yes;  0: No
int initialNeeded = -1; // Re-Connection Initial is needed only at the point that "initialing" changed from 1 to 0

uint8 SenSerialReadBlocking(void){
    uint8 input = 0u;
    int initialConfirmCount = 0;
    
    if (initialing == 0)  {     
      //  while(input == -1 || input < 0)  { 
        while(input == 0u)  { 
          if (initialNeeded == 1)  {
              if (++initialConfirmCount >= SEN_RECONNECTION_CONFIRM_NUM)
                  loop();             
          }else   {
              initialConfirmCount = 0;
          }
          input = UART_UartGetByte();
          if (input == (uint8)-2)  {
              initialing = 1;      
          }
        }
    }
    
    /*****************  For Re-Connection detection  **************************/
    if (initialing == 1)  {
          initialConfirmCount = 0;
          if (input == (uint8)-1)  {  // Nothing been read, initialing finished
              initialing = 0;
              initialNeeded = 1;
          }
          SenSerialReadBlocking();    
    }        
    /*****************  For Re-Connection detection  **************************/
    
    return input;
}

void SenEcho(void)      {
       uint8 input = SenSerialReadBlocking();
       UART_UartPutChar(input);
       UART_UartPutString("\n");
       CyDelayUs(50);
}

void SenShowSetupData(void){
       uint8 productID = SenReadReg(REG_PRODUCT_ID);
       UART_UartPutString("ProductID = 0x");
        if (productID)  {
                SenUartPutHexDecimalByte(productID);
        }else   UART_UartPutString("00");
       UART_UartPutString(".\n");
}

int main()
{
       SPI_Master_Enable();
       SPI_Master_Start();
       UART_Start();
       SenChipSelect();

//       UART_UartPutString("********************************************************\n");
//       UART_UartPutString("                  Sen PSoC 4 Optical-Flow World USB-UART\n");
//       UART_UartPutString("********************************************************\n");

       SenShowSetupData();
       
        initialNeeded = 1;
        loop();

}

void loop(void) {
   for(;;)
   {
//                SenEcho();
        
            if (initialNeeded == 0)  {
              UART_UartPutString("Please Input:\r 0, Continuous Frames Pixels-Burst;\r 1, Only One Frame Pixels-Burst;\r 2, Motion Tracking and Recording;\n 3, Sen Motion Burst.");
            }
              //****************************************************************************************************************************//
            if (initialNeeded == 1)  {
                UART_UartPutString("\r*************************************************************\n");
                UART_UartPutString("                  Welcome to Sen PSoC Optical-Flow World\n"); 
                UART_UartPutString("*************************************************************\n"); 
                SenShowSetupData();
                UART_UartPutString("Please Input:\r 0, Continuous Frames Pixels-Burst;\r 1, Only One Frame Pixels-Burst;\r 2, Motion Tracking and Recording;\n 3, Sen Motion Burst.\n");
                initialNeeded = 0;
            }
            //****************************************************************************************************************************//
          
            uint8 input = 0;
            input = SenSerialReadBlocking();
            
            if(input == 48)    {      // Continuous Frames Pixels      Num. 0 to start
                UART_UartPutString("Ready to Burst:\n");
                SenContinuousImageDisplay();
            }else if (input == 49)    {// One Frame Pixels             Num. 1 to start
                SenChipSelect();
                SenImageDisplay();
                CyDelay(100);
            }else if (input == 50)    {// MotionTracking               Num. 2 to start
                UART_UartPutString("\n");
                UART_UartPutString("SenMotionSprouting:\n");
                SenReset();
                SenReadCoodinataData();
            }else if (input == 51)    {// MotionBurst                  Num. 3 to start
                SenContinueMotionBurst();
            }else if (input == 113)    {// Quit Request                 Letter q to quit, disconnected
               UART_UartPutString("I'm Sen Stop Request!\n");
               UART_UartPutString("Now the input is: ");
               UART_UartPutChar(input);
               UART_UartPutString("\n");
            }else  {
        //       Serial.print("Now the input is: ");
        //       Serial.println(input, DEC);
            }
            UART_UartPutString("\n");
            CyDelay(100);

   }
}



/*********** Sen Pixel-Burst Mode **************/
void SenImageDisplay()
{
    UART_UartPutString("image data\n");
    SenPixelsShowUp();
    UART_UartPutString("----------\n");
}

void SenContinuousImageDisplay()
{
    int i, burstCount = 0;
    SPI_Master_SpiUartClearTxBuffer();// Serial.flush();

    while( !UART_ReadAvailableCheck() ) {
        SenImageDisplay();
    }
    CyDelay(50);
    SPI_Master_SpiUartClearTxBuffer();// Serial.flush();
}
void SenPixelsShowUp(void)
{
    int i,j;
    int isFirstPixel = True;
    unsigned char regValue;
    unsigned char pixelValue;

    // write to frame capture register to force capture of frame
    SenWriteReg(FRAME_CAPTURE,0x83);

    // wait 3 frame periods + 10 nanoseconds for frame to be captured
    CyDelayUs(1550);      // min frame speed is 2000 frames/second so 1 frame = 500 nano seconds.  so 500 x 3 + 10 = 1510

    // display the pixel data
    for( i=0; i<30; i++ ) {
        for( j=0; j<30; j++ ) {
            regValue = SenReadReg(FRAME_CAPTURE);
            if( isFirstPixel && (regValue & 0x40) == 0 ) {
                UART_UartPutString("failed to find first pixel\n");
            }
            isFirstPixel = False;
            pixelValue = ( regValue << 2);
            UART_UartPutChar(pixelValue);
            CyDelayUs(25);
        }
    }
    // hardware reset to restore sensor to normal operation
    SenReset();
}

/*********** Sen Motion-Tracking Mode **************/
void SenReadCoodinataData(void){
  int sycSample = 0;
  int noStop = True;
//  unsigned int timeInterval = 0, oldtime, newtime;
//  oldtime = micros();
//  newtime = micros();
  
  do  {
    uint8 myMotion = SenReadReg(MOTION);
    uint8 myDelta_X = SenReadReg(DELTA_X);
    uint8 myDelta_Y = SenReadReg(DELTA_Y);
    
    UART_UartPutChar(myDelta_X);
    UART_UartPutChar(myDelta_Y);    
//    Serial.write(1);  // for test
    
    sycSample = sycSample + 1;
    if (sycSample >= SEN_SYNC_SAMPLE_NUM)  {
//         if (UART_ReadAvailableCheck())  {  
         if (UART_ReadAvailableCheck())  {  
          UART_UartPutChar(0xAA);  
//          Serial.write(0x58);   // for test
          noStop = False;
        }else  {
          UART_UartPutChar(0x55);
        }
        
        sycSample = 0;
    }
  }while (noStop);

  CyDelay(100);
  UART_UartPutString("\n");// Serial.flush();
  SPI_Master_SpiUartClearTxBuffer();
        

  n=0;
}

uint8 UART_ReadAvailableCheck(void)
{
//	uint8 character=0;
//        CyDelay(26);
//        if(SPI_Master_SpiUartReadRxData())      character = 1u;
//        else character = 0u;
//        
//	return character;
        return (0u != UART_SpiUartGetRxBufferSize()) ;
}

/* [] END OF FILE */
