#include "ft245.h"
#include <avr/io.h>
#include "UComMaster.h"
#include "spi.h"
#include <util/delay.h>

void spi_init()
{
	// Setup SPI master for programming	
	DDRB = (1<<MOSI) | (1<<SCK);
    DDRB |= (1<<CS);
	DDRC |= (1<<4);
	DDRB &= ~(1<<MISO);
	/** Sen fixed as Serial.Begin() **/
	PORTB |= ((1<<CS));// | (1<<MISO));
	PORTB &= ~((1<<MOSI) | (1<<SCK));
	
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<CPHA);// | (1<<SPIE) | (1<<SPR1)
}

void select(void){ cbi(PORTB, CS); } //cbi(PORTB, PB2); }

void reselect(void) { deselect(); _delay_ms(10); select(); }

void deselect(void) { sbi(PORTB, CS); } //sbi(PORTB, PB2); }

void send_spi_byte(char c)
{
	SPDR = c;
	while(!(SPSR & (1<<SPIF)));
}


char read_spi_byte(void)
{
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}



/****************** Sen Shortcut Sensor Control *********************/


char SenReadSensorRegister(char fixedAddress){
	char regValue;
			
	//printf245("\nBefore, fixedAddress = 0x%x\n\r", fixedAddress);
	fixedAddress &= (uint8_t)~(1 << 7);
	
	select();
	SenSPI_Transfer(fixedAddress);
	_delay_us(50);//	delay_micro(75);
	regValue = SenSPI_Transfer(0x00);
	deselect();
	//_delay_us(50);//	delay_micro(75);
	
	return (regValue);
}

void SenWriteReg(char fixedAddress, char dataValue){
  select();
  SenSPI_Transfer(fixedAddress | 0x80 );   // send register address
  _delay_us(50);// delay_micro(75);                  // small delay
  SenSPI_Transfer(dataValue);             // send data
  deselect();
}



char SenSPI_Transfer(char fixedAddress){
  SPDR = fixedAddress;
  while (!(SPSR & _BV(SPIF)))
    ;
  return SPDR;
}  

void SenReset(void){
    sbi(PORTC, RESET);                 // reset sensor
	_delay_us(50);//   delay_micro(50);
    cbi(PORTC, RESET);                      // return sensor to normal
	
	deselect();
	_delay_us(10);//	delay_micro(10);
	select();
}



void SenSPI_Init(void){
               SPCR = (1<<SPE) | (1<<MSTR) | (1<<CPHA) | (1<<SPR0) | (1<<CPOL);// 1101 1101, master mode, SPI_Mode_3,  | (1<<SPIE)

               //SPCR &= ~((1<<SPR1) | (1<<SPR0));
               //SPSR |= (0x01);// SPI_CLOCK_DIV2, not 2MB, as fast as possible

               SPCR &= ~((1<<SPR1) | (1<<SPR0));
               SPSR &= ~(1<<SPI2X); // SPI_CLOCK_DIV4, 2MB

               SPCR &= ~((1<<DORD));// MSB_First
}
