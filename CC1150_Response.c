#include <avr/io.h>
#include <util/delay.h>

// To set the speed of the processor
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz       0x00
#define CPU_8MHz        0x01
#define CPU_4MHz        0x02
#define CPU_2MHz        0x03
#define CPU_1MHz        0x04
#define CPU_500kHz      0x05
#define CPU_250kHz      0x06
#define CPU_125kHz      0x07
#define CPU_62kHz       0x08

// CSen on the CC1150 is connected to PB0, pin 53
#define SELECT_CC1150 PORTB &= ~(1<<PORTB0)
#define DESELECT_CC1150 PORTB |= (1<<PORTB0)
#define SETUP_CS_CC1150 DDRB |= (1<<PORTB7)

#define SPI_SS_PIN PORTB0
#define SPI_SCK_PIN PORTB1
#define SPI_MOSI_PIN PORTB2
#define SPI_MISO_PIN PORTB3

#define SPI_MODE_0 0x00 
#define SPI_MODE_1 0x01 
#define SPI_MODE_2 0x02 
#define SPI_MODE_3 0x03

#define SPI_NO_INTERRUPT 0
#define SPI_INTERRUPT 1

#define SPI_MSTR_CLK4 0x00 // 4MHz
#define SPI_MSTR_CLK16 0x01 // 1MHz
#define SPI_MSTR_CLK64 0x02 // 250kHz 
#define SPI_MSTR_CLK128 0x03 // 125kHz
#define SPI_MSTR_CLK2 0x04 // 8MHz
#define SPI_MSTR_CLK8 0x05 // 2Mhz
#define SPI_MSTR_CLK32 0x06 // 500Mhz

// Control
#define CC1150_IOCFG2       0x00        // GDO2 output pin configuration
#define CC1150_IOCFG1       0x01        // GDO1 output pin configuration
#define CC1150_IOCFG0       0x02        // GDO0 output pin configuration
#define CC1150_FIFOTHR      0x03        // RX FIFO and TX FIFO thresholds
#define CC1150_SYNC1        0x04        // Sync word, high byte
#define CC1150_SYNC0        0x05        // Sync word, low byte
#define CC1150_PKTLEN       0x06        // Packet length
#define CC1150_PKTCTRL1     0x07        // Packet automation control
#define CC1150_PKTCTRL0     0x08        // Packet automation control
#define CC1150_ADDR         0x09        // Device address
#define CC1150_CHANNR       0x0A        // Channel number
#define CC1150_FSCTRL1      0x0B        // Frequency synthesizer control
#define CC1150_FSCTRL0      0x0C        // Frequency synthesizer control
#define CC1150_FREQ2        0x0D        // Frequency control word, high byte
#define CC1150_FREQ1        0x0E        // Frequency control word, middle byte
#define CC1150_FREQ0        0x0F        // Frequency control word, low byte
#define CC1150_MDMCFG4      0x10        // Modem configuration
#define CC1150_MDMCFG3      0x11        // Modem configuration
#define CC1150_MDMCFG2      0x12        // Modem configuration
#define CC1150_MDMCFG1      0x13        // Modem configuration
#define CC1150_MDMCFG0      0x14        // Modem configuration
#define CC1150_DEVIATN      0x15        // Modem deviation setting
#define CC1150_MCSM2        0x16        // Main Radio Control State Machine configuration
#define CC1150_MCSM1        0x17        // Main Radio Control State Machine configuration
#define CC1150_MCSM0        0x18        // Main Radio Control State Machine configuration
#define CC1150_FOCCFG       0x19        // Frequency Offset Compensation configuration
#define CC1150_BSCFG        0x1A        // Bit Synchronization configuration
#define CC1150_AGCCTRL2     0x1B        // AGC control
#define CC1150_AGCCTRL1     0x1C        // AGC control
#define CC1150_AGCCTRL0     0x1D        // AGC control
#define CC1150_WOREVT1      0x1E        // High byte Event 0 timeout
#define CC1150_WOREVT0      0x1F        // Low byte Event 0 timeout
#define CC1150_WORCTRL      0x20        // Wake On Radio control
#define CC1150_FREND1       0x21        // Front end RX configuration
#define CC1150_FREND0       0x22        // Front end TX configuration
#define CC1150_FSCAL3       0x23        // Frequency synthesizer calibration
#define CC1150_FSCAL2       0x24        // Frequency synthesizer calibration
#define CC1150_FSCAL1       0x25        // Frequency synthesizer calibration
#define CC1150_FSCAL0       0x26        // Frequency synthesizer calibration
#define CC1150_RCCTRL1      0x27        // RC oscillator configuration
#define CC1150_RCCTRL0      0x28        // RC oscillator configuration
#define CC1150_FSTEST       0x29        // Frequency synthesizer calibration control
#define CC1150_PTEST        0x2A        // Production test
#define CC1150_AGCTEST      0x2B        // AGC test
#define CC1150_TEST2        0x2C        // Various test settings
#define CC1150_TEST1        0x2D        // Various test settings
#define CC1150_TEST0        0x2E        // Various test settings

// Strobe commands
#define CC1150_SRES         0x30       // Reset chip.
#define CC1150_SFSTXON      0x31       // Enable and calibrate frequency synthesizer (if MCSM0.FS_AUTOCAL=1).
								// If in RX/TX: Go to a wait state where only the synthesizer is
								// running (for quick RX / TX turnaround).
#define CC1150_SXOFF        0x32       // Turn off crystal oscillator.
#define CC1150_SCAL         0x33       // Calibrate frequency synthesizer and turn it off
								// (enables quick start).
#define CC1150_SRX          0x34       // Enable RX. Perform calibration first if coming from IDLE and
                                // MCSM0.FS_AUTOCAL=1.
#define CC1150_STX          0x35       // In IDLE state: Enable TX. Perform calibration first if
                                // MCSM0.FS_AUTOCAL=1. If in RX state and CCA is enabled:
                                // Only go to TX if channel is clear.
#define CC1150_SIDLE        0x36       // Exit RX / TX, turn off frequency synthesizer and exit
                                // Wake-On-Radio mode if applicable.
#define CC1150_SAFC         0x37       // Perform AFC adjustment of the frequency synthesizer
#define CC1150_SWOR         0x38       // Start automatic RX polling sequence (Wake-on-Radio)
#define CC1150_SPWD         0x39       // Enter power down mode when CSn goes high.
#define CC1150_SFRX         0x3A       // Flush the RX FIFO buffer.
#define CC1150_SFTX         0x3B       // Flush the TX FIFO buffer.
#define CC1150_SWORRST      0x3C       // Reset real time clock.
#define CC1150_SNOP         0x3D       // No operation. May be used to pad strobe commands to two
                                // bytes for simpler software.
// Status									
#define CC1150_PARTNUM          0x30
#define CC1150_VERSION          0x31
#define CC1150_FREQEST          0x32
#define CC1150_LQI              0x33
#define CC1150_RSSI             0x34
#define CC1150_MARCSTATE        0x35
#define CC1150_WORTIME1         0x36
#define CC1150_WORTIME0         0x37
#define CC1150_PKTSTATUS        0x38
#define CC1150_VCO_VC_DAC       0x39
#define CC1150_TXBYTES          0x3A
#define CC1150_RXBYTES          0x3B
#define CC1150_RCCTRL1_STATUS   0x3C
#define CC1150_RCCTRL0_STATUS   0x3D
#define CC1150_PATABLE          0x3E

typedef struct {
uint8_t IOCFG1;
uint8_t IOCFG0; 
uint8_t FIFOTHR;
uint8_t SYNC1; 
uint8_t SYNC0; 
uint8_t PKTLEN; 
uint8_t PKTCTRL0;
uint8_t ADDR; 
uint8_t CHANNR;
uint8_t FREQ2; 
uint8_t FREQ1; 
uint8_t FREQ0; 
uint8_t MDMCFG4;
uint8_t MDMCFG3;
uint8_t MDMCFG2;
uint8_t MDMCFG1;
uint8_t MDMCFG0;
uint8_t DEVIATN;
uint8_t MCSM1;
uint8_t MCSM0; 
uint8_t FREND0;
uint8_t FSCAL3; 
uint8_t FSCAL2; 
uint8_t FSCAL1; 
uint8_t FSCAL0; 
uint8_t FSTEST; 
uint8_t PTEST; 
uint8_t TEST2; 
} REGSETTINGS;

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