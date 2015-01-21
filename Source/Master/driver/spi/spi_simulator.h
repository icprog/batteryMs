#ifndef _SPI_SIMULATOR_H_
#define _SPI_SIMULATOR_H_
#include "driver/Gpio/gpio.h"
#include "driver/Gpio/port_defs.h"

#define SCLK    SPIB_PORT_SCK                    //定义SCLK所对应的GPIO接口编号  
#define MOSI    SPIB_PORT_MOSI                     //定义SCLK所对应的GPIO接口编号  
#define MISO    SPIB_PORT_MISO                    //定义MISO所对应的GPIO接口编号  
#define SS      SPIB_PORT_CS                     //定义SS所对应的GPIO接口编号  
#define OUTP    GPIO_OUTPUT                       //表示GPIO接口方向为输出  
#define INP GPIO_INPUT                           //表示GPIO接口方向为输入  


#define SPIC_SCLK    SPIC_PORT_SCK                    //定义SCLK所对应的GPIO接口编号  
#define SPIC_MOSI    SPIC_PORT_MOSI                     //定义SCLK所对应的GPIO接口编号  
#define SPIC_MISO    SPIC_PORT_MISO                    //定义MISO所对应的GPIO接口编号  
#define SPIC_SS      SPIC_PORT_CS                     //定义SS所对应的GPIO接口编号  
#define SPIC_WP      IO_FRAM_WP


#define LTC6804_CMD_MD_FAST        01      //fast Mode, 27kHz/14kHz Mode
#define LTC6804_CMD_MD_NORMAL        02    //Normal Mode, 7kHz/3kHz Mode
#define LTC6804_CMD_MD_FILTER        03    //filter Mode, 26Hz/2kHz Mode
#define LTC6804_CMD_CH        0x0       //All Cells
#define LTC6804_CMD_CHG       0x0       //All GPIOs
#define LTC6804_CMD_DCP       0x0       //DCP Disabled
#define LTC6804_CMD_PUP       0x0
#define LTC6804_CMD_ST        0x0
#define LTC6804_CMD_CHST      0x0

#define LTC6804_CMD_WRCFG     0x0001
#define LTC6804_CMD_RDCFG     0x0002
#define LTC6804_CMD_RDCVA     0x0004
#define LTC6804_CMD_RDCVB     0x0006
#define LTC6804_CMD_RDCVC     0x0008
#define LTC6804_CMD_RDCVD     0x000A
#define LTC6804_CMD_RDAUXA    0x000C
#define LTC6804_CMD_RDAUXB    0x000E
#define LTC6804_CMD_RDSTATA   0x0010
#define LTC6804_CMD_RDSTATB   0x0012
#define LTC6804_CMD_ADCV      (0x0260 | (LTC6804_CMD_MD << 7) | (LTC6804_CMD_DCP << 4) | LTC6804_CMD_CH)                    
//#define LTC6804_CMD_ADCV      (0x0220 | (LTC6804_CMD_MD << 7) | (LTC6804_CMD_DCP << 4) | LTC6804_CMD_CH)
#define LTC6804_CMD_ADOW      (0x0228 | (LTC6804_CMD_MD << 7) | (LTC6804_CMD_PUP << 6) | (LTC6804_CMD_DCP << 4) | LTC6804_CMD_CH)
#define LTC6804_CMD_CVST      (0x0207 | (LTC6804_CMD_MD << 7) | (LTC6804_CMD_ST << 5))
#define LTC6804_CMD_ADAX      (0x0460 | (LTC6804_CMD_MD << 7) | LTC6804_CMD_CHG)
#define LTC6804_CMD_ADSTAT    (0x0468 | (LTC6804_CMD_MD << 7) | LTC6804_CMD_CHST)
#define LTC6804_CMD_STATST    (0x040F | (LTC6804_CMD_MD << 7) | (LTC6804_CMD_ST << 5))
#define LTC6804_CMD_CLRCELL   0x0711
#define LTC6804_CMD_CLRAUX    0x0712
#define LTC6804_CMD_CLRSTAT   0x0713
#define LTC6804_CMD_PLADC     0x0714
#define LTC6804_CMD_DIAGN     0x0715
#define LTC6804_CMD_WRCOMM    0x0721
#define LTC6804_CMD_RDCOMM    0x0722
#define LTC6804_CMD_STCOMM    0x0723

#define NUM_RX_BYT            8
#define BYT_IN_REG            6
#define CELL_IN_REG           3 
#define GPIO_IN_REG           3
#define LTC6804_IC_NUM        2
#define LTC6804_MAX_TEMP      5                                 
#define LTC6804_MAX_VOL       12

#define set_gpio_direction(port, direction) gpio_init( port, direction)
#define	set_gpio_value(port, value) gpio_set(port, value)
#define get_gpio_value(port) gpio_read(port)
INT16 ltc6804_init();
int spi_init(void);
void spi_write_byte(UINT8 b);
UINT8 spi_read_byte(void);
void SPI_Write(UINT8 *buff, UINT16 len);
void SPI_Read(UINT8 *dstbuf, UINT16 n) ;

int spic_init(void);
void spic_write_byte(UINT8 b);
UINT8 spic_read_byte(void);

enum UCT_ERR_CODE ltc6804_voltage_monitor () ;
enum UCT_ERR_CODE ltc6804_temp_monitor ();
#endif




