// General defines and helper macros for CC1150
// Combination of several sources.

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

// SPI defines
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
#define SPI_MSTR_CLK8 0x05 // 2MHz
#define SPI_MSTR_CLK32 0x06 // 500kHz

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

// Structure defining all of the settings
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
