#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <CC1150_Response.h>

// Copied from logic trace of CC1150 from door contact
// Currently not stored in PROGMEM as may modify
REGSETTINGS regSettings = {
	0x0B, // IOCFG1
	0x0C, // IOCFG0	
	0x07, // FIFOTHR
	0xD3, // SYNC1
	0x91, // SYNC0
	0xFF, // PKTLEN
	0x12, // PKTCTRL0
	0x00, // ADDR
	0x00, // CHANNR
	0x21, // FREQ2
	0x65, // FREQ1
	0x6A, // FREQ0
	0x87, // MDMCFG4
	0xF8, // MDMCFG3
	0x00, // MDMCFG2
	0x22, // MDMCFG1
	0xF8, // MDMCFG0
	0x50, // DEVIATN
	0x30, // MCSM1
	0x18, // MCSM0
	0x15, // FREND0
	0xE9, // FSCAL3
	0x2A, // FSCAL2
	0x00, // FSCAL1
	0x1F, // FSCAL0
	0x59, // FSTEST
	0x35, // PTEST
	0x09  // TEST2
};

// Copied from logic trace of CC1150 from door contact
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

void setup_pcint(void) {
	// SPI_MISO_PIN receives a synchronous clock
	// Setup PCINT3 interrupt
	PCICR |= (1 << PCIE0);
	PCMSK0 |= (1 << PCINT3);
	
	sei();
}

ISR(PCINT0_vect) {
	// CC1150 samples on falling edge
	// So we need to setup on rising edge
	if (!(PINB & (1 << SPI_MISO_PIN))) {
		PORTL ^= (1 << PORTL0);
	}
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
	PORTB &= ~(1 << SPI_SS_PIN);
}

void deselect(void) {
	PORTB |= (1 << SPI_SS_PIN);
}

void send_command(uint8_t command) {
	select();
	send_spi(command);
	deselect();
}

void send_command_sres() {
	// Keeps CC1150 selected and waits for MISO to go low
	// as per p21 of datasheet
	select();
	send_spi(CC1150_SRES);
	while(PINB & (1 << SPI_MISO_PIN));
	deselect();
}

void set_register(uint8_t address, uint8_t data) {
	select();
	send_spi(address);
	send_spi(data);
	deselect();
}

void set_register_burst(uint8_t address, uint8_t *data, uint8_t length) {
	uint8_t i;
	
	select();
	send_spi(address);
	
	for (i = 0; i < length; i++) {
		send_spi(data[i]);
	}
	
	deselect();
}
void write_settings(REGSETTINGS *pRegSettings) {
	set_register(CC1150_IOCFG1, 	pRegSettings->IOCFG1);
	set_register(CC1150_IOCFG0, 	pRegSettings->IOCFG0);
	set_register(CC1150_FIFOTHR, 	pRegSettings->FIFOTHR);
	set_register(CC1150_SYNC1, 		pRegSettings->SYNC1);
	set_register(CC1150_SYNC0, 		pRegSettings->SYNC0);
	set_register(CC1150_PKTLEN, 	pRegSettings->PKTLEN);
	set_register(CC1150_PKTCTRL0, 	pRegSettings->PKTCTRL0);
	set_register(CC1150_ADDR, 		pRegSettings->ADDR);
	set_register(CC1150_CHANNR,		pRegSettings->CHANNR);
	set_register(CC1150_FREQ2, 		pRegSettings->FREQ2);
	set_register(CC1150_FREQ1, 		pRegSettings->FREQ1);
	set_register(CC1150_FREQ0, 		pRegSettings->FREQ0);
	set_register(CC1150_MDMCFG4, 	pRegSettings->MDMCFG4);
	set_register(CC1150_MDMCFG3, 	pRegSettings->MDMCFG3);
	set_register(CC1150_MDMCFG2, 	pRegSettings->MDMCFG2);
	set_register(CC1150_MDMCFG1, 	pRegSettings->MDMCFG1);
	set_register(CC1150_MDMCFG0, 	pRegSettings->MDMCFG0);
	set_register(CC1150_DEVIATN, 	pRegSettings->DEVIATN);
	set_register(CC1150_MCSM1, 		pRegSettings->MCSM1);
	set_register(CC1150_MCSM0, 		pRegSettings->MCSM0);
	set_register(CC1150_FREND0, 	pRegSettings->FREND0);
	set_register(CC1150_FSCAL3, 	pRegSettings->FSCAL3);
	set_register(CC1150_FSCAL2, 	pRegSettings->FSCAL2); 
	set_register(CC1150_FSCAL1, 	pRegSettings->FSCAL1); 
	set_register(CC1150_FSCAL0, 	pRegSettings->FSCAL0); 
	set_register(CC1150_FSTEST, 	pRegSettings->FSTEST); 
	set_register(CC1150_PTEST, 		pRegSettings->PTEST); 
	set_register(CC1150_TEST2, 		pRegSettings->TEST2); 
}

int main(void) {
	CPU_PRESCALE(0);
	
	DDRL |= (1<<PORTL0);
	
	// Door contact runs very slowly
	// But CC1150 supports 4MHz
	setup_spi(SPI_MSTR_CLK4);
	enable_spi();
	
	setup_pcint();

	while(1) {
		send_command_sres();
		
		write_settings(&regSettings);
		set_register_burst(CC1150_PATABLE + 0x40, paTable, sizeof(paTable));
		send_command(CC1150_STX);
		// Why do I need to set these again?
		set_register(CC1150_IOCFG0,0x0C);
		set_register(CC1150_IOCFG1,0x0B);

		// Currently do nothing 
		while(1);
	}
	
	
	

}
