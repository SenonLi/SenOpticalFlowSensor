#include <SPI.h>

#define NCS 10
#define MOSI 11
#define MISO 12
#define SCLK 13
#define RESET 7

#define REG_PRODUCT_ID 0x00
#define REG_REVISION_ID 0x01
#define MOTION 0x02
#define DELTA_X 0x03
#define DELTA_Y 0x04                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
#define SQUAL 0x05
#define PIXEL_SUM 0x06
#define MAXIMUM_PIXEL 0x07
#define CONFIGURATION_BITS 0x0a
#define EXTENDED_CONFIG 0x0b
#define DATA_OUT_LOWER 0x0c
#define DATA_OUT_UPPER 0x0d
#define SHUTTER_LOWER 0x0e
#define SHUTTER_UPPER 0x0f
#define FRAME_PERIOD_LOWER 0x10
#define FRAME_PERIOD_UPPER 0x11
#define MOTION_CLEAR 0x12
#define FRAME_CAPTURE 0x13
#define SROM_ENABLE 0x14
#define FRAME_PERIOD_MAX_BOUND_LOWER 0x19
#define FRAME_PERIOD_MAX_BOUND_UPPER 0x1a
#define FRAME_PERIOD_MIN_BOUND_LOWER 0x1b
#define FRAME_PERIOD_MIN_BOUND_UPPER 0x1c
#define SHUTTER_MAX_BOUND_LOWER 0x1d
#define SHUTTER_MAX_BOUND_UPPER 0x1e
#define SROM_ID 0x1f
#define OBSERVATION 0x3d
#define INVERSE_PRODUCT_ID 0x3f
#define PIXEL_BURST 0x40
#define MOTION_BURST 0x50
#define SROM_LOAD 0x60

#define SEN_IMAGE_DISPLAY_INTERVAL 1
#define SEN_MOTION_SCAN_INTERVAL 300
#define SEN_SYNC_SAMPLE_NUM 100

#define SEN_RECONNECTION_CONFIRM_NUM 1500

int n=0, motionBurstCount=0;
int initialing = 0;    // 1: yes;  0: No
int initialNeeded = -1; // Re-Connection Initial is needed only at the point that "initialing" changed from 1 to 0

  
void SenReset(void){
    digitalWrite(RESET,HIGH);                 // reset sensor
    delayMicroseconds(75);
    digitalWrite(RESET,LOW);                      // return sensor to normal
    SenDeselectChip(); 
    SenChipSelect();
}
void SenShowSetupData(void){
  Serial.print("SPCR = 0x");
  Serial.println(SPCR,HEX);
  Serial.print("SPSR = 0x");
  Serial.println(SPSR,HEX);
  Serial.print("DDRB = 0x");
  Serial.println(DDRB,HEX);
  Serial.print("PORTB = 0x");
  Serial.println(PORTB,HEX);
  
  byte productID = SenReadReg(REG_PRODUCT_ID);
  Serial.print("ProductID = 0x");
  Serial.println(productID,HEX);
  Serial.println("");
}

void setup() {
  // put your setup code here, to run once:
  pinMode(NCS,OUTPUT);
  pinMode(MOSI,OUTPUT);
  pinMode(MISO,INPUT);
  pinMode(SCLK,OUTPUT);
  pinMode(RESET,OUTPUT);

  digitalWrite(NCS,HIGH);
 
  Serial.begin(43200);
//  Serial.begin(38400);
  SPI.begin();  
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE3);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
    
//****************************************************************************************************************************//
  initialNeeded = 1;
//****************************************************************************************************************************//
}
               
char SenSerialReadBlocking(void){
    char input = -1;
    int initialConfirmCount = 0;
    
    if (initialing == 0)  {     
      //  while(input == -1 || input < 0)  { 
        while(input == -1)  { 
          if (initialNeeded == 1)  {
              if (++initialConfirmCount >= SEN_RECONNECTION_CONFIRM_NUM)
                  loop();             
          }else   {
              initialConfirmCount = 0;
          }
          input = Serial.read();
          if (input == -2)  {
              initialing = 1;      
          }
        }
    }
    
    /*****************  For Re-Connection detection  **************************/
    if (initialing == 1)  {
          initialConfirmCount = 0;
          if (input == -1)  {  // Nothing been read, initialing finished
              initialing = 0;
              initialNeeded = 1;
          }
          SenSerialReadBlocking();    
    }        
    /*****************  For Re-Connection detection  **************************/
    
    return input;
}

void loop() {
    
  // put your main code here, to run repeatedly:
    if (initialNeeded == 0)  {
      Serial.println("Please Input:\r 0, Continuous Frames Pixels-Burst;\r 1, Only One Frame Pixels-Burst;\r 2, Motion Tracking and Recording;\n 3, Sen Motion Burst.");
    }
      //****************************************************************************************************************************//
    if (initialNeeded == 1)  {
        Serial.println("\r*************************************************************");
        Serial.println("\tWelcome to Sen Arduino Optical-Flow World "); 
        Serial.println("*************************************************************"); 
        byte productID = SenReadReg(REG_PRODUCT_ID);
        Serial.print("ProductID = 0x");
        Serial.println(productID,HEX);
        Serial.println("Please Input:\r 0, Continuous Frames Pixels-Burst;\r 1, Only One Frame Pixels-Burst;\r 2, Motion Tracking and Recording;\n 3, Sen Motion Burst.");
        initialNeeded = 0;
        //  SenShowSetupData();
        //  Serial.println("Get into Menu:");
    }
    //****************************************************************************************************************************//
  
    char input = 0;
    input = SenSerialReadBlocking();

//    Serial.print("input = ");
//    Serial.println(input, DEC);
    
    if(input == 48)    {      // Continuous Frames Pixels      Num. 0 to start
        Serial.println("Ready to Burst:");
        SenContinuousImageDisplay();
    }else if (input == 49)    {// One Frame Pixels             Num. 1 to start
        SenChipSelect();
        SenImageDisplay();
        delay(100);
    }else if (input == 50)    {// MotionTracking               Num. 2 to start
        Serial.println("");
        Serial.println("SenMotionSprouting:");
        SenReadCoodinataData();
    }else if (input == 51)    {// MotionBurst                  Num. 3 to start
        SenContinueMotionBurst();
    }else if (input == 113)    {// Quit Request                 Letter q to quit, disconnected
       Serial.println("I'm Sen Stop Request!");
       Serial.print("Now the input is: ");
       Serial.println(input, DEC);
    }else  {
//       Serial.print("Now the input is: ");
//       Serial.println(input, DEC);
    }
    Serial.println("");
    delay(100);
  
//  Serial.println("End of the loop:");
//  while(1);
}
/*********** Sen Pixel-Burst Mode **************/
void SenBurstPixelMenu(void){

  char input = 0;

    Serial.println("Input 2 for PixelBurst once; 0 for Continuous PixelBurst, 1 to stop.");
    Serial.print("Please input: ");
    input = SenSerialReadBlocking();
//    Serial.print("input = ");
//    Serial.println(input, DEC);
    
    if(input == 48)    {// Num. 0 to start
      char c = 0;
//      int burstCount = 0;
      Serial.println("Ready to Burst:");
      SenContinuousImageDisplay();
    }else if (input == 50)    {
      SenChipSelect();
      SenImageDisplay();
      delay(100);
    }

}

void SenImageDisplay()
{
    Serial.println("image data");
    SenPixelsShowUp();
    Serial.println("----------");
}

void SenContinuousImageDisplay()
{
    int i, burstCount = 0;
    Serial.flush();

    while( !Serial.available() ) {
        SenImageDisplay();
        if(++burstCount == 5)   {
          Serial.flush();
          SenReset();
          burstCount = 0;
        }
        i=0;
        while( i<SEN_IMAGE_DISPLAY_INTERVAL && !Serial.available() ) {
            delay(100);          // give the viewer a bit of time to catchup
            i++;
        }
    }
    Serial.flush();
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
    delayMicroseconds(1600);      // min frame speed is 2000 frames/second so 1 frame = 500 nano seconds.  so 500 x 3 + 10 = 1510

    // display the pixel data
    for( i=0; i<30; i++ ) {
        for( j=0; j<30; j++ ) {
            regValue = SenReadReg(FRAME_CAPTURE);
            if( isFirstPixel && (regValue & 0x40) == 0 ) {
                Serial.println("failed to find first pixel");
            }
            isFirstPixel = false;
            pixelValue = ( regValue << 2);
//            Serial.print(pixelValue,HEX);
            Serial.write(pixelValue);
//            if( j!= 30-1 )
//                Serial.print(",");
            delayMicroseconds(75);
        }
//        Serial.println("");
    }
    // hardware reset to restore sensor to normal operation
    SenReset();
}

/*********** Sen Motion-Tracking Mode **************/
void SenReadCoodinataData(void){
  int sycSample = 0;
  bool noStop = true;
  unsigned int timeInterval = 0, oldtime, newtime;
  oldtime = micros();
  newtime = micros();
  
  do  {
    byte myMotion = SenReadReg(MOTION);
    byte myDelta_X = SenReadReg(DELTA_X);
    byte myDelta_Y = SenReadReg(DELTA_Y);
    
    Serial.write(myDelta_X);
    Serial.write(myDelta_Y);    
//    Serial.write(1);  // for test
    
    sycSample = sycSample + 1;
    if (sycSample >= SEN_SYNC_SAMPLE_NUM)  {
         if (Serial.available())  {  
          Serial.write(0xAA);  
//          Serial.write(0x58);   // for test
          noStop = false;
        }else  {
          Serial.write(0x55);
        }
        
        sycSample = 0;
    }
  }while (noStop);
  
  Serial.flush();
  Serial.println("");
  n=0;
}

/*********** Sen Motion-Burst Mode **************/
void SenBurstMotionMenu(void){
      SenContinueMotionBurst();
}

byte SenMotionBurstActive(void){
  byte value=0, fixedaddress = MOTION_BURST;
  fixedaddress &=~ (1 << 7);

  SPI.transfer(fixedaddress);
  delayMicroseconds(80);
  value = SPI.transfer(0x00);
  delayMicroseconds(80); 
  
  return(value);
}
byte SenRead_SPDR_Byte(void)
{
  SPDR = 0x00;
  while(!(SPSR & (1<<SPIF)));
  delayMicroseconds(8);
  return SPDR;
}
void SenContinueMotionBurst(void){
  char c = 0;
  int burstCount = 0;
  Serial.println("Ready to Burst:");
  Serial.println("");
  
  while(c != 49)
  {
    SenChipSelect();
    delay(275);
    byte my_1_Motion = SenMotionBurstActive();
    byte my_2_Delta_X = SenRead_SPDR_Byte();
    byte my_3_Delta_Y = SenRead_SPDR_Byte();
   
    byte my_4_SQUAL = SenRead_SPDR_Byte();
    byte my_5_UpperShutter = SenRead_SPDR_Byte();
    byte my_6_LowerShutter = SenRead_SPDR_Byte();
    byte my_7_MaximumPixel = SenRead_SPDR_Byte();
    SenDeselectChip();
    delayMicroseconds(50);

    if(burstCount == 50)  { 
      burstCount=0;    Serial.println(""); 
      byte productID = SenReadReg(REG_PRODUCT_ID);
      Serial.print("ProductID = 0x");
      Serial.println(productID,HEX);
    }
    if(++burstCount == 1)   { 
      Serial.println("Motion  Delta  Delta  Surface  Shutter  Shutter  MaxPixelNum"); 
      Serial.println(" Y/N     _X     _Y    Quality   _Upper   _Lower   (0 ~ 63)"); 
    }

    Serial.print(" 0x");
    Serial.print(my_1_Motion,HEX);
    Serial.print("     ");
    Serial.print(my_2_Delta_X,DEC); 
    Serial.print("     ");
    Serial.print(my_3_Delta_Y,DEC); 
    Serial.print("       ");
    Serial.print(my_4_SQUAL,DEC); 
    Serial.print("       ");
    Serial.print(my_5_UpperShutter,DEC); 
    Serial.print("      ");
    Serial.print(my_6_LowerShutter,DEC); 
    Serial.print("        ");
    Serial.println(my_7_MaximumPixel,DEC); 
  
    c = Serial.read();
  }
}
/*********** Sen Basic I/O Fuction *********/
void SenDeselectChip(void){
  digitalWrite(NCS,HIGH);
}

void SenChipSelect(void){
  digitalWrite(NCS,LOW);
}

byte SenReadReg(byte fixedaddress){
  byte value=0;
  fixedaddress &=~ (1 << 7);
  
  SenChipSelect();
  SPI.transfer(fixedaddress);
  delayMicroseconds(50);

  SPDR = 0x00;
  while (!(SPSR & _BV(SPIF)))
    ;
  value = SPDR;
  
  SenDeselectChip();
  delayMicroseconds(25); 
  
  return(value);
}

void SenWriteReg(byte fixedAddress, byte dataValue){
  SenChipSelect();
  SPI.transfer(fixedAddress | 0x80 );   // send register address
  delayMicroseconds(75);                  // small delay
  SPI.transfer(dataValue);             // send data
  SenDeselectChip(); 
}




//void SenReadCoodinataData(void){
//  char c = 0;
//  unsigned int timeInterval = 0, oldtime, newtime;
//  oldtime = micros();
//  newtime = micros();
////  oldtime = millis();
////  newtime = millis();
////  delay(SEN_MOTION_SCAN_INTERVAL);
//  while(c != 49 && ++n <= 10)
//  {  
//    byte myMotion = SenReadReg(MOTION);
//    byte myDelta_X = SenReadReg(DELTA_X);
//    byte myDelta_Y = SenReadReg(DELTA_Y);
//    
////    Serial.print(" ");
////    Serial.print(++n,DEC); 
////    Serial.print(":   Delta_X = ");
////    Serial.print(myDelta_X,DEC);
////    Serial.print(",    Delta_Y = ");
////    Serial.print(myDelta_Y,DEC);
//    
//    Serial.write(myDelta_X);
//    Serial.write(myDelta_Y);                
//                
////    oldtime = newtime;
//////    newtime = millis();
////    newtime = micros();
////    timeInterval = newtime - oldtime;    
////    Serial.print(",    d_T = ");
////    Serial.println(timeInterval,DEC);
//    
////    Serial.print(",    FlowV_X = ");
////    Serial.print(1000*myDelta_X/timeInterval,DEC);    
////    Serial.print(" /s,    FlowV_Y = ");
////    Serial.print(1000*myDelta_Y/timeInterval,DEC); 
////    Serial.println(" /s");
////    delay(SEN_MOTION_SCAN_INTERVAL);
////    delayMicroseconds(SEN_MOTION_SCAN_INTERVAL);
//    c = Serial.read(); 
//  }
//  n=0;
//}
