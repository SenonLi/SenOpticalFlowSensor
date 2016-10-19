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

#include "main.h"

uint32 InterruptHpn;

int8 delta_X = 0;
int8 delta_Y = 0;
uint8 motionUpdateNeeded = FALSE;


//uint8 first300Pixels[301];
//uint8 second300Pixels[301];
//uint8 third300Pixels[301];
uint8 Pixels18Array[301];

int n=0, motionBurstCount=0;
int initialing = 0;    // 1: yes;  0: No
int initialNeeded = -1; // Re-Connection Initial is needed only at the point that "initialing" changed from 1 to 0

CY_ISR(InterruptHandler)
{
    /* Check interrupt source and clear Inerrupt */
 	InterruptHpn = SenTimer_GetInterruptSourceMasked();
   	if (InterruptHpn == SenTimer_INTR_MASK_TC)
    {
        SenTimer_ClearInterrupt(SenTimer_INTR_MASK_TC);
		
			/* Send Motion Burst data when respective notification is enabled */
			if(motionBurstStartNotification & NOTIFY_BIT_MASK)
			{
//				SenReset();
				SenReadCoodinataData();
				
//				if(TRUE == oneSecondPassed && TRUE == motionUpdateNeeded)
				if(motionReadTimes >= 20 && TRUE == motionUpdateNeeded)
				{
					motionUpdateNeeded = FALSE;
					/* Check for CapSense slider swipe and send data accordingly */
					SenMotionBurstHandler();
					motionReadTimes = 0;
				}
			}else {	delta_X = 0; delta_Y = 0;	}
    }
}

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */

	/* Start the components */
	Initialize_BLE_ISR_RGBLED_System();
	SPI_Master_Enable();
	initialNeeded = 1;
    SPI_Master_Start();

	Reset_Write(0);
//	Reset2_Write(0);
//	SenChipSelect();
//	SenShowSetupData();
       
	for(;;) {
		/* Process BLE Events. This generates events in the callback function */
		CyBle_ProcessEvents(); //***********************************************************

		if(TRUE == deviceConnected)
		{			
			/* Send Image Burst data when respective notification is enabled */
			if(imageBurstStartNotification & NOTIFY_BIT_MASK)
			{
//				Reset_Write(1);
				SenPixelsShowUp();// Read and save 900 Pixels
//				imageBurstStartNotification = FALSE;
			}			
		}
	}
}

void SenPixelsShowUp(void)
{
    int i,j,k;
    int isFirstPixel = TRUE;
    unsigned char regValue;
    unsigned char pixelValue;

	SenReset();
//	Reset_Write(0);
    // write to frame capture register to force capture of frame
    SenWriteReg(FRAME_CAPTURE,0x83);

    // wait 3 frame periods + 10 nanoseconds for frame to be captured
    CyDelayUs(1550);      // min frame speed is 2000 frames/second so 1 frame = 500 nano seconds.  so 500 x 3 + 10 = 1510
		
		for(i=1; i<=3; i++)	{
			Pixels18Array[0] = i;
			for(j=1; j<=300; j++)	{
				regValue = SenReadReg(FRAME_CAPTURE);
//	            if( isFirstPixel && (regValue & 0x40) == 0 ) {
//	                j = 18;	i = 50;
//	            }
	            isFirstPixel = FALSE;
				
	            pixelValue = ( regValue << 2);
				Pixels18Array[j] = pixelValue;
	            CyDelayUs(20);
				CyBle_ProcessEvents();
			}
			Send18ArrayDataOverSenImageBurstNotification((uint8 *) Pixels18Array);
			CyBle_ProcessEvents();
		}
		
    // hardware reset to restore sensor to normal operation
    SenReset();
}

/*********** Sen Motion-Tracking Mode **************/
void SenReadCoodinataData(void){

    uint8 newMotion = SenReadReg(MOTION);
    int8 newDelta_X = SenReadReg(DELTA_X);
    int8 newDelta_Y = SenReadReg(DELTA_Y);
    
	if (newDelta_X != 0 || newDelta_Y !=0)	{
		delta_X = delta_X + newDelta_X;
		delta_Y = delta_Y + newDelta_Y;
		motionUpdateNeeded = TRUE;
	}
	motionReadTimes = motionReadTimes + 1;
}


void SenMotionBurstHandler() {
		SendDataOverSenMotionBurstNotification((uint8)delta_X, (uint8)delta_Y);
		delta_X = 0;
		delta_Y = 0;
}

void Initialize_BLE_ISR_RGBLED_System(void) { 
	/* Enable Global Interrupt Mask */ 
	CyGlobalIntEnable; 
	
	/* Enable the Interrupt component connected to interrupt */
    SenTimerISR_StartEx(InterruptHandler);

	/* Start the components */
    SenTimer_Start();
	
	/* Start BLE stack and register the event callback function.*/ 
	CyBle_Start(GeneralEventHandler); 
	
	/* Start PrISM modules for LED control */ 
	PrISM_1_Start(); 
	PrISM_2_Start(); 
	
	/* Switch off the RGB LEDs through PrISM modules */ 
	PrISM_1_WritePulse0(RGB_LED_OFF); 
	PrISM_1_WritePulse1(RGB_LED_OFF); 
	PrISM_2_WritePulse0(RGB_LED_OFF); 
	
	/* Set Drive modes of the output pins to Strong drive */ 
	RED_SetDriveMode(RED_DM_STRONG); 
	GREEN_SetDriveMode(GREEN_DM_STRONG); 
	BLUE_SetDriveMode(BLUE_DM_STRONG); 
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
               	UART_UartPutString("Now the input is: ");
				UART_UartPutChar(input);
            }
            UART_UartPutString("\n");
            CyDelay(500);

   }
}



uint8 SenSerialReadBlocking(void){
    uint8 input = (uint8)-1;
    int initialConfirmCount = 0;
    
    if (initialing == 0)  {     
      //  while(input == -1 || input < 0)  { 
        while(input == (uint8)-1)  { 
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


uint8 UART_ReadAvailableCheck(void)
{
        return (0u != UART_SpiUartGetRxBufferSize()) ;
}

/* [] END OF FILE */
