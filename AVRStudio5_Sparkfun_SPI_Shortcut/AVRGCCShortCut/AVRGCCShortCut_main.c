#include "ft245.h"
#include <avr/io.h>
#include "UComMaster.h"
#include "spi.h"
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdbool.h>
#include <util/delay.h>

volatile uint8_t current_phase = '1';
volatile uint8_t current_dorder = '0';
volatile uint8_t current_frequency = '4';

volatile uint16_t bits;
volatile uint8_t cnt;
volatile uint16_t fcnt=0;

uint8_t sendonly = 1;
volatile uint16_t send_string[256];

void SenReadCoodinataData(void);

void SenBurstMotionMenu(void);
char SenMotionBurstActive(void);
char SenRead_SPDR_Byte(void);
void SenContinueMotionBurst(void);

void firstCoordinateRetrieve(void);
void SenShowSetupData(void);

void SenBurstPixelMenu(void);
void SenImageDisplay(void);
void SenSerialFlush( void );
void SenContinuousImageDisplay(void);
void SenPixelsShowUp(void);

char getc245_blocking(void);
void send_ascii(void);
void toggle_mode(void);
void menu(void);
void actions_menu(void);
void settings_menu(void);
void send_single_ascii(void);
void send_single_hex(void);
void send_file(void);
void continuous_receive(void);
void single_receive(void);
void send_hex_string(void);
void send_ascii_string(void);
void send_command_string(void);
void set_polarity_phase(void);
void my_printf(uint8_t string_num);
void change_frequency(void);
void set_dorder(void);
void show_settings(void);

char welcom_menu[] PROGMEM = "\r*************************************************************\n\tWelcome to Sen Arduino Optical-Flow World \n*************************************************************\n";	
char arrow[] PROGMEM = "Please Input:\n";
char senMenu[] PROGMEM = "Please Input:\n 0, Continuous Frames Pixels-Burst;\r 1, Only One Frame Pixels-Burst;\r 2, Motion Tracking and Recording;\n 3, Sen Motion Burst.\n";
char invalid[] PROGMEM = "Invalid Character\n\r";
char send_single[] PROGMEM = "Enter characters to send, press enter to return to menu\n\r->";
char action_menu[] PROGMEM = "\n\rACTIONS MENU:\n\r(1) Send command string\n\r(2) Send ASCII characters\n\r(3) Continuous receive\n\r(4) Return to main menu\n\r\n\r";
char cont_receive[] PROGMEM = "Receiving, CTRL+C to stop\n\r";
char send_command[] PROGMEM = "Enter hex string of 256 values or less. Press return when finished.\n\rRR = Receive, CH = Chip Select High, CL = Chip Select Low, DY = 10ms Delay\n\r";
char cs_high [] PROGMEM = "CS High\n\r";
char cs_low[] PROGMEM = "CS Low\n\r";
char string_sent[] PROGMEM = "String sent!\n\r";
char settings[] PROGMEM = "\n\rSETTINGS MENU:\n\r(1) Set clock polarity and phase\n\r(2) Set frequency\n\r(3) Set data order\n\r(4) Show current settings\n\r(5) Return to main menu\n\r";
char set_polarity_1[] PROGMEM = "\n\rClock settings can be defined in the following ways:\n\r\n\rCPOL/CPHA___LEADING EDGE________TRAILING EDGE________MODE\n\r0/0         Sample (Rising)     Setup (Falling)       (1)\n\r";
char set_polarity_2[] PROGMEM = "0/1         Setup (Rising)      Sample (Falling)      (2)\n\r1/0         Sample (Falling)    Setup (Rising)        (3)\n\r1/1         Setup (Falling)     Sample(Rising)        (4)\n\r\n\r";
char set_polarity_3[] PROGMEM = "Change to mode:";
char mode_changed[] PROGMEM = "\n\rMode changed!\n\r\n\r";
char frequency_menu_1[] PROGMEM = "\n\rFrequency Options:\n\r\n\rMODE    EFFECTIVE FREQUENCY\n\r(1)          4MHz\n\r(2)          2MHz\n\r(3)          1MHz\n\r(4)          500kHz\n\r(5)          250kHz\n\r";
char frequency_menu_2[] PROGMEM = "(6)          125kHz\n\r(7)          62.5kHz\n\r\n\rNew frequency mode: ";
char frequency_changed[] PROGMEM = "Frequency changed!\n\r\n\r";
char dorder_menu[] PROGMEM = "\n\rData Order Modes:\n\r\n\r(0)   MSB transmitted first\n\r(1)   LSB transmitted first\n\rNew data mode: ";
char dorder_changed[] PROGMEM = "Data order changed!\n\r\n\r";
char cur_settings[] PROGMEM = "\n\rCurrent Settings:\n\r";
char data_order[] PROGMEM = "Data Order: ";
char msb[] PROGMEM = "MSB\n\r\n\r";
char lsb[] PROGMEM = "LSB\n\r\n\r";
char command_delay[] PROGMEM = "Delay 10ms\n\r";

PGM_P string_table[] PROGMEM = 
{
	welcom_menu,
	arrow,
	senMenu,
	invalid,
	send_single,
	action_menu,
	cont_receive,
	send_command,
	cs_high,
	cs_low,
	string_sent,
	settings,
	set_polarity_1,
	set_polarity_2,
	set_polarity_3,
	mode_changed,
	frequency_menu_1,
	frequency_menu_2,
	frequency_changed,
	dorder_menu,
	dorder_changed,
	cur_settings,
	data_order,
	msb,
	lsb,
	command_delay
};


char buffer[150];
int n;
int initialing = 0;    // 1: yes;  0: No
int initialNeeded = -1; // Re-Connection Initial is needed only at the point that "initialing" changed from 1 to 0


int main(void)
{
	DDRB |= (1<<LED);
	DDRC |= ((1<<SDA) | (1<<SCL));
	PORTB |= (1<<LED);
	for(int i = 0; i < 3; i++) // Test sequence
	{
		//PORTC |= (1<<SDA);
		//delay_ms(5);
		//PORTC &= ~(1<<SDA);
		//PORTC |= (1<<SCL);
		//delay_ms(5);
		//PORTC &= ~(1<<SCL);
	}
	ioinit();
	spi_init();// MOSI, SCK, CS pins all Output,  MISO pin Input | SPI enable  | master mode 
	//menu();
	
	/***** Sen Action *****/
	PORTC |= ((1<<SCL) | (1<<SDA));
	PORTC &= ~(1<<RESET);// SDA = RESET, Output, ==0
	
	SenSPI_Init();// SPI_Mode_3 |  SPI_CLOCK_DIV8, 2MB | MSB_First
	deselect();	
	//SenShowSetupData();
	select();
	char productID = SenReadSensorRegister(REG_PRODUCT_ID);
	my_printf(welcom_menu); // Print menu
	printf245("ProductID = 0x%x\n\r", productID);
	printf245("\r*************************************************************\n");
	printf245("\tWelcome to Sen Arduino Optical-Flow World \n");
	printf245("*************************************************************\n");	
	n=0;
	char input = 0;

		
	while(1)
	{		
		input = getc245_blocking();
		
	    if(input == 48)    {							// Continuous Frames Pixels      Num. 0 to start
			printf245("Ready to Burst:\n");
			select();
			SenContinuousImageDisplay();
		}else if (input == 49)    {						// One Frame Pixels             Num. 1 to start
			select();
			SenImageDisplay();
			_delay_ms(100);		
		}else if (input == 50)	{						// MotionTracking               Num. 2 to start
			printf245("\nSenMotionSprouting:\n");
			SenReadCoodinataData();
		}else if (input == 51)	{						// MotionBurst                  Num. 3 to start
			SenContinueMotionBurst();
		}else if (input == 113)	{						// Quit Request                 Letter q to quit, disconnected
			printf245("I'm Sen Stop Request!\n");
		}else	{
			printf245("now the input is: 0x%d\n", input);
		}
		
		printf245("\n");
		_delay_ms(100);//delay_ms(100);		
	}
}
/*********** Sen Read Coordinate Mode ************/
void SenReadCoodinataData(void){
//  char c = 0;
  int sycSample = 0;
  bool noStop = true;
  char myMotion;
  char myDelta_X;
  char myDelta_Y;

	setDataOutputs();
	
  do  {
    myMotion = SenReadSensorRegister(MOTION);
    myDelta_X = SenReadSensorRegister(DELTA_X);
    myDelta_Y = SenReadSensorRegister(DELTA_Y);
    
	SenPrint245Byte(myDelta_X);
	SenPrint245Byte(myDelta_Y);

    if (++sycSample >= SEN_SYNC_SAMPLE_NUM)  {
         if (SenSerialAvailableCheck())  {
		  SenPrint245Byte(0xAA);
          noStop = false;
        }else  {
		  SenPrint245Byte(0x55);
        }
        sycSample = 0;
    }
	//_delay_ms(8);
  }while (noStop);
  
  SenSerialFlush();
  printf245("\n");
}

/*********** Sen Motion-Burst Mode **************/
void SenBurstMotionMenu(void){
		SenContinueMotionBurst();
}

char SenMotionBurstActive(void){
  char value=0, fixedaddress = MOTION_BURST;
  fixedaddress &=~ (1 << 7);

  SenSPI_Transfer(fixedaddress);
  _delay_us(80);
  value = SenSPI_Transfer(0x00);
  _delay_us(80);
  
  return(value);
}
char SenRead_SPDR_Byte(void)
{
  SPDR = 0x00;
  while(!(SPSR & (1<<SPIF)))	{;}
  _delay_us(8);
  return SPDR;
}
void SenContinueMotionBurst(void){
  char c = 0;
  int burstCount = 0;
  printf245("Ready to Burst:\n\r");
  
  while(c != 49)
  {
	select();
	_delay_ms(275);
    char my_1_Motion = SenMotionBurstActive();
    char my_2_Delta_X = SenRead_SPDR_Byte();
    char my_3_Delta_Y = SenRead_SPDR_Byte();
   
    char my_4_SQUAL = SenRead_SPDR_Byte();
    char my_5_UpperShutter = SenRead_SPDR_Byte();
    char my_6_LowerShutter = SenRead_SPDR_Byte();
    char my_7_MaximumPixel = SenRead_SPDR_Byte();
	deselect();
	_delay_us(50);
	
    if(burstCount == 50)  { 
		burstCount=0;
	  	char productID = SenReadSensorRegister(REG_PRODUCT_ID);
		printf245("\nProductID = 0x%x\n", productID);
    }
    if(++burstCount == 1)   { 
		  printf245("Motion  Delta  Delta  Surface  Shutter  Shutter  MaxPixelNum\n");
		  printf245(" Y/N     _X     _Y    Quality   _Upper   _Lower   (0 ~ 63)\n");
    }

	printf245(" 0x%x     %d     %d       %d       %d       %d        %d\n", my_1_Motion,my_2_Delta_X,my_3_Delta_Y,my_4_SQUAL,my_5_UpperShutter,my_6_LowerShutter,my_7_MaximumPixel);
  
    c = getchar245();
  }
}

/*********** Sen Pixel-Burst Mode **************/
void SenBurstPixelMenu(void){
  char input = 0;
  while(1)
  {
	printf245("Input 2 for PixelBurst once; 0 for Continuous PixelBurst, 1 to stop.\n\r");
	my_printf(ARROW);
    input = getc245_blocking();
	
    if(input == 48)    { // Num. 0 to start
      char c = 0;
	  printf245("Ready to Burst:\n");
	  select();
	  SenContinuousImageDisplay();
    }	else if (input == 50)    {
	  select();
      SenImageDisplay();
      _delay_ms(100);//delay_ms(100);
    }
  }
}

void SenImageDisplay(void)
{
	printf245("image data\n");
    SenPixelsShowUp();
	printf245("----------\n");
}

void SenSerialFlush( void )
{
	unsigned char dummy = 0;
	setDataInputs();
	while(USBreadAllowed()){		//If RX Flag is low, lets get the data from the FIFO
		clearRD();			//Clock the next FIFO data onto the D0-D7 lines
		_delay_us(HOLD_TIME);//	delay_micro(HOLD_TIME);
		dummy = PIND;		//Get data from FT245		
		setRD();			//Relinquish control of the Data lines
	}
}

void SenContinuousImageDisplay(void)
{
    int i;
    SenSerialFlush();
		
    while( !SenSerialAvailableCheck() ) {
        SenImageDisplay();
        i=0;
        while( i<SEN_IMAGE_DISPLAY_INTERVAL)	{	// && !SenSerialAvailableCheck() ) {
			_delay_ms(50);//  delay_ms(50);          // give the viewer a bit of time to catchup
            i++;
        }
    }
	
    SenSerialFlush();
}
void SenPixelsShowUp(void)
{
    int i,j;
    bool isFirstPixel = true;
    unsigned char regValue;
    unsigned char pixelValue;

    // write to frame capture register to force capture of frame
    SenWriteReg(FRAME_CAPTURE,0x83);

    // wait 3 frame periods + 10 nanoseconds for frame to be captured
	_delay_us(1520);//delay_micro(1550); // min frame speed is 2000 frames/second so 1 frame = 500 nano seconds.  so 500 x 3 + 10 = 1510

    // display the pixel data
    for( i=0; i<30; i++ ) {
        for( j=0; j<30; j++ ) {
            regValue = SenReadSensorRegister(FRAME_CAPTURE);
            if( isFirstPixel && (regValue & 0x40) == 0 ) {
				printf245("failed to find first pixel\n");
            }
            isFirstPixel = false;
            pixelValue = ( regValue << 2);
			SenPrint245Byte(pixelValue);
            //if( j!= 30-1 )
				//printf245(",");
			_delay_us(75);//delay_micro(50); 
        }
        //printf245("\n");
    }

    // hardware reset to restore sensor to normal operation
    SenReset();
}

/****************** Sen Action *********************/
void SenShowSetupData(void){
	printf245("\n\rSetting up...\n\r");
	printf245("DDRB = 0x%x\n\r", DDRB);
	printf245("PORTB = 0x%x\n\r", PORTB);
	printf245("SPCR = 0x%x\n\r", SPCR);
	printf245("SPSR = 0x%x\n\r", SPSR);
  
	char productID = SenReadSensorRegister(REG_PRODUCT_ID);
	my_printf(welcom_menu); // Print menu
	printf245("ProductID = 0x%x\n", productID);
}

void firstCoordinateRetrieve(void){
	char myMotion = SenReadSensorRegister(MOTION);
	char myDeltaX = SenReadSensorRegister(DELTA_X);
	char myDeltaY = SenReadSensorRegister(DELTA_Y);
	
	printf245("Accumulation %d Times:   ", ++n);	
	printf245("  Delta_X = %d, ", myDeltaX);
	printf245("	 Delta_Y = %d\n\r", myDeltaY);
}


/***************************************************/
//void menu()
//{
	//char c = 0; 
	//deselect();	
	//
	//my_printf(welcom_menu); // Print menu
	//my_printf(ARROW);
	//
	//c = getc245_blocking();
	//printf245("%c\n\r",c);
	//
	//switch(c)
	//{
		//case '1':
			//actions_menu();
			//break;
		//case '2':
			//settings_menu();
			//break;
		//default:
		//{
			//my_printf(INVALID);
			//menu();
			//break;
		//}
	//}
//}
/*************************************************************************** 
	ACTIONS SECTION:       
		Actions include
			-Send command string
			-Send ASCII one at a time
			-Continuous Receive 
****************************************************************************/
//void actions_menu(void)
//{
	//char c = 0;
	//
	//my_printf(ACTIONS_MENU);
	//my_printf(ARROW);
	//c = getc245_blocking();
	//printf245("%c\n\r\n\r",c);
	//switch(c)
	//{
		//case '1':
			//send_command_string();
			//break;
		//case '2':
			//send_single_ascii();
			//break;
		//case '3':
			//continuous_receive();
			//break;
		//case '4':
			//menu();
			//break;
		//default:
		//{
			//my_printf(INVALID);
			//actions_menu();
			//break;
		//}
	//}
//}
//
//void send_single_ascii(void)
//{
	//char c = 0;
	//my_printf(SEND_SINGLE);
	//while(1)  
	//{
		//c = getc245_blocking();
		//if(c == 13){ printf245("\n\r\n\r"); actions_menu(); }
		//printf245("Sent %c\n\r",c);
		//select();
		//send_spi_byte(c);
		//deselect();
	//}
//}
//
//
//
//void continuous_receive(void)
//{
	//char c = 0;
	//uint8_t i = 0;
	//my_printf(CONT_RECEIVE);
	//while(c != 3)
	//{
		//send_spi_byte(0x00);
		//printf245("%x ",SPDR);
		//i++;
		//c = getchar245();
		//if(i == 15){ printf245("\n\r"); i = 0; }
	//}
	//printf245("\n\r\n\r");
	//actions_menu();
//}
//
//void send_command_string(void)
//{
	//char h = 0, l = 0;
	//uint16_t i = 0, j = 0, receive = 1;
	//my_printf(SEND_COMMAND);
	//while(i < 256)
	//{
		//h = getc245_blocking();
		//if(h == 13){ break; } 
		//printf245("%c",h);
		//l = getc245_blocking();
		//if(l == 13){ break; }
		//printf245("%c ",l);
		//
		//if(h == 'R') // Receive
		//{
			//send_string[i] = 0x0100;
		//}
		//else if(l == 'H') // CS High
		//{
			//send_string[i] = 0x0101;
		//}
		//
		//else if(l == 'L') // CS Low
		//{
			//send_string[i] = 0x0102;
		//}
		//else if(l == 'Y')
		//{
			//send_string[i] = 0x0103;
		//}
		//else
		//{
			//if(h >= 48 && h <= 57){ h -= 48; } 	  // h is a number
			//else if(h >= 65 && h <= 70){ h -= 55; } // h is a letter
			//else if(h >= 97 && h <= 102){ h -= 87; } // h is a letter
			//else{ my_printf(INVALID); send_command_string(); }
		//
			//if(l >= 48 && l <= 57){ l -= 48; } 	  // l is a number
			//else if(l >= 65 && l <= 70){ l -= 55; } // l is a letter
			//else if(l >= 97 && l <= 102){ l -= 87; } // l is a letter
			//else{ my_printf(INVALID); send_command_string(); }
		//
			//h = (h<<4) + l; 
			//send_string[i] = h;
		//}
		//
		//i++;
			//
	//}
	//printf245("\n\r");
	//if(i == 256){ i--; }
	//for(j = 0; j < i; j++)
	//{
	//
		//if(send_string[j] == 0x0100) // Receive
		//{
			//send_spi_byte(0x00);
			//send_string[j] = 0x0200 + SPDR;
		//}
		//else if(send_string[j] == 0x0101){ deselect(); }// CS HIGH
		//else if(send_string[j] == 0x0102){ select(); } // CS LOW
		//else if(send_string[j] == 0x0103){ delay_ms(10); } // DELAY
		//else{ send_spi_byte(send_string[j]); }
	//}
	//deselect();
	//// PRINT RESULTS
	//for(j = 0; j < i; j++)
	//{
	//
		//if(send_string[j] >= 0x0200){ printf245("R%d = 0x%x\n\r",receive,(send_string[j] & 0x00FF)); receive++; }
		//else if(send_string[j] == 0x0101){ my_printf(CS_HIGH); }
		//else if(send_string[j] == 0x0102){ my_printf(CS_LOW); }
		//else if(send_string[j] == 0x0103){ my_printf(DELAY); }
		//else{ printf245("Sent 0x%x\n\r", send_string[j]); }
	//}
	//my_printf(STRING_SENT);
	//
	//actions_menu();
//}
//
/*************************************************************************** 
	SETTINGS SECTION:       
		Settings include
			-Set clock polarity and phase
			-Change frequency
			-Change parity
****************************************************************************/

//void settings_menu(void)
//{
	//char c = 0;
	//
	//my_printf(SETTINGS);
	//my_printf(ARROW);
	//
	//c = getc245_blocking();
	//printf245("%c\n\r",c);
	//
	//switch(c)
	//{
		//case '1':
			//set_polarity_phase();
			//break;
		//case '2':
			//change_frequency();
			//break;
		//case '3':
			//set_dorder();
			//break;
		//case '4':
			//show_settings();
			//break;
		//case '5':
			//menu();
			//break;
		//default:
		//{
			//my_printf(INVALID);
			//menu();
			//break;
		//}
	//}
//}
//
//void set_polarity_phase(void)
//{
	//uint8_t c = 0;
	//my_printf(SET_POLARITY_1);
	//my_printf(SET_POLARITY_2);
	//my_printf(SET_POLARITY_3);
	//
	//c = getc245_blocking();
	//printf245(" %c\n\r",c);
	//current_phase = c;
	//switch(c)
	//{
		//case '1':
			//SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0) | (1<<SPIE);
			//break;
		//case '2':
			//SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0) | (1<<SPIE) | (1<<CPHA);
			//break;
		//case '3':
			//SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0) | (1<<SPIE) | (1<<CPOL);
			//break;
		//case '4':
			//SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0) | (1<<SPIE) | (1<<CPHA) | (1<<CPOL);
			//break;
		//default:
		//{
			//my_printf(INVALID);
			//set_polarity_phase();
			//break;
		//}
	//}
	//my_printf(MODE_CHANGED);
	//settings_menu();
//}
	//
//void change_frequency(void)
//{
	//uint8_t c;
	//my_printf(FREQUENCY_MENU_1);
	//my_printf(FREQUENCY_MENU_2);
	//c = getc245_blocking();
	//printf245("%c\n\r",c);
	//current_frequency = c;
	//
	//if(c == '1'|| c == '2'){ SPCR &= ~((1<<SPR0) | (1<<SPR1)); }	
	//else if(c == '3'|| c == '4'){ SPCR &= ~(1<<SPR1); SPCR |= (1<<SPR0); }
	//else if(c == '5'|| c == '6'){ SPCR &= ~(1<<SPR0); SPCR |= (1<<SPR1); }
	//else if(c == '7'){ SPCR |= ((1<<SPR0) | (1<<SPR1)); }
	//else{ my_printf(INVALID); }
//
	//if(c == '1' || c == '3' || c == '5'){ SPSR |= 0x01; } // Frequency doubler
	//else{ SPSR &= ~(1<<SPI2X); } 
	//
	//my_printf(FREQUENCY_CHANGED);
	//settings_menu();
		//
//}
//
//void set_dorder(void)
//{
	//uint8_t c;
	//my_printf(DORDER_MENU);
	//c = getc245_blocking();
	//printf245("%c\n\r",c);
	//current_dorder = c;
	//if(c){ SPCR |= (1<<DORD); }
	//else{ SPCR &= ~(1<<DORD); }
	//my_printf(DORDER_CHANGED);
	//settings_menu();
//}
//
//void show_settings(void)
//{	
	//my_printf(CURRENT_SETTINGS);
	//printf245("Polarity/Phase Mode: %c\n\r", current_phase);
	//printf245("Clock frequency: ");
	//switch(current_frequency)
	//{
		//case '1': printf245("4MHz\n\r"); break;
		//case '2': printf245("2MHz\n\r"); break;
		//case '3': printf245("1MHz\n\r"); break;
		//case '4': printf245("500kHz\n\r"); break;
		//case '5': printf245("250kHz\n\r"); break;
		//case '6': printf245("125kHz\n\r"); break;
		//case '7': printf245("62.5kHz\n\r"); break; 
	//}
	//my_printf(DATA_ORDER);
	//if(current_dorder == '1'){ my_printf(LSB); }
	//else{ my_printf(MSB); }
	//settings_menu();
//}
//
char getc245_blocking()
{
	char c = 0;
	while(c == 0){ c = getchar245(); }
	return c;
}

void my_printf(uint8_t string_num)
{
	strcpy_P(buffer, (PGM_P)pgm_read_word(&(string_table[string_num])));
	printf245(buffer);
}

