#include <avr/io.h>
#include <util/delay.h>
#include <CC1150_Response.h>

REGSETTINGS regSettings = {
	0x0B,
	0x0C,
	0x07,
	0xD3,
	0x91,
	0xFF,
	0x12,
	0x00,
	0x00,
	0x21,
	0x65,
	0x6A,
	0x87,
	0xF8,
	0x00,
	0x22,
	0xF8,
	0x50,
	0x30,
	0x18,
	0x15,
	0xE9,
	0x2A,
	0x00,
	0x1F,
	0x59,
	0x35,
	0x09
};


uint8_t paTable[] = {
	0x01,
	0x0B,
	0x25,
	0x68,
	0x60,
	0x86,
	0xCC,
	0xC3
};


	
void setup_spi(uint8_t clock) {
	
	// configure the DDR for the pins.
	DDRB |= (1<<SPI_MOSI_PIN); // output
    DDRB &= ~(1<<SPI_MISO_PIN); // input
    DDRB |= (1<<SPI_SCK_PIN);// output
    DDRB |= (1<<SPI_SS_PIN);//output
	
	// configure SPI control register
	SPCR = (0 << SPIE) // no interrupt please
		| (1 << SPE) // enable SPI
		| (0 << DORD) // MSB first
		| (1 << MSTR) // Master of the universe
		| (0 << CPOL) // default
		| (0 << CPHA) // default
		| (((clock & 0x02) == 2) << SPR1) // top clock bit
		| ((clock & 0x01) << SPR0); // bottom clock bit;
		
	SPSR = (((clock & 0x04) == 4) << SPI2X);
}

void enable_spi(void) {
	SPCR |= (1 << SPE);
}

void disable_spi(void) {
	SPCR &= ~(1 << SPE);
}

uint8_t send_spi(uint8_t byte) {
	SPDR = byte;
	while (!(SPSR & (1<<SPIF)));
	return SPDR;
}

void select(void) {
	PORTB &= ~(1 << PORTB0);
}

void deselect(void) {
	PORTB |= (1 << PORTB0);
}

void send_command(uint8_t command) {
	select();
	send_spi(command);
	deselect();
	_delay_us(10);
}

void set_register(uint8_t address, uint8_t data) {
	select();
	send_spi(address);
	send_spi(data);
	deselect();
	_delay_us(10);
}

void set_register_burst(uint8_t address, uint8_t *data, uint8_t length) {
	uint8_t i;
	
	select();
	send_spi(address);
	
	for (i = 0; i < length; i++) {
		send_spi(data[i]);
		_delay_us(10);
	}
	
	deselect();
	_delay_us(10);
}
void write_settings(REGSETTINGS *pRegSettings) {
	set_register(CC1150_IOCFG1, pRegSettings->IOCFG1);
	set_register(CC1150_IOCFG0, pRegSettings->IOCFG0);
	set_register(CC1150_FIFOTHR, pRegSettings->FIFOTHR);
	set_register(CC1150_SYNC1, pRegSettings->SYNC1);
	set_register(CC1150_SYNC0, pRegSettings->SYNC0);
	set_register(CC1150_PKTLEN, pRegSettings->PKTLEN);
	set_register(CC1150_PKTCTRL0, pRegSettings->PKTCTRL0);
	set_register(CC1150_ADDR, pRegSettings->ADDR);
	set_register(CC1150_CHANNR, pRegSettings->CHANNR);
	set_register(CC1150_FREQ2, pRegSettings->FREQ2);
	set_register(CC1150_FREQ1, pRegSettings->FREQ1);
	set_register(CC1150_FREQ0, pRegSettings->FREQ0);
	set_register(CC1150_MDMCFG4, pRegSettings->MDMCFG4);
	set_register(CC1150_MDMCFG3, pRegSettings->MDMCFG3);
	set_register(CC1150_MDMCFG2, pRegSettings->MDMCFG2);
	set_register(CC1150_MDMCFG1, pRegSettings->MDMCFG1);
	set_register(CC1150_MDMCFG0, pRegSettings->MDMCFG0);
	set_register(CC1150_DEVIATN, pRegSettings->DEVIATN);
	set_register(CC1150_MCSM1, pRegSettings->MCSM1);
	set_register(CC1150_MCSM0, pRegSettings->MCSM0);
	set_register(CC1150_FREND0, pRegSettings->FREND0);
	set_register(CC1150_FSCAL3, pRegSettings->FSCAL3);
	set_register(CC1150_FSCAL2, pRegSettings->FSCAL2); 
	set_register(CC1150_FSCAL1, pRegSettings->FSCAL1); 
	set_register(CC1150_FSCAL0, pRegSettings->FSCAL0); 
	set_register(CC1150_FSTEST, pRegSettings->FSTEST); 
	set_register(CC1150_PTEST, pRegSettings->PTEST); 
	set_register(CC1150_TEST2, pRegSettings->TEST2); 
}

int main(void) {
	CPU_PRESCALE(0);
	
	DDRL |= (1<<PORTL0);
	
	setup_spi(SPI_MSTR_CLK64);
	enable_spi();
	_delay_us(10);
	
	while(1) {
	send_command(CC1150_SRES);
	_delay_us(100);
	
	write_settings(&regSettings);
	set_register_burst(CC1150_PATABLE + 0x40, paTable, sizeof(paTable));
	send_command(CC1150_STX);
	set_register(CC1150_IOCFG0,0x0C);
	set_register(CC1150_IOCFG1,0x0B);
	
		while(1) {
			PORTL |= (1<<PORTL0);
			_delay_us(160);
			PORTL &= ~(1<<PORTL0);
			_delay_us(160);
		}
	}
	
	
	

}