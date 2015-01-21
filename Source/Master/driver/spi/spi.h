#ifndef _SPI_H_
#define _SPI_H_
#include "Gpio/gpio.h"
//int spi_init(void);
//int spi_read( UINT8 *rd_buff, UINT8 len);
//int spi_write( const UINT8 *wr_buff, UINT8 len);
//int spi_read_write(UINT8 *rd_buff, const UINT8 *wr_buff, UINT8 len);
//int test_spi(void);

#endif


#define SCLK        SPIC_PORT_SCK                    //定义SCLK所对应的GPIO接口编号  
#define MOSI        SPIC_PORT_MOSI                     //定义SCLK所对应的GPIO接口编号  
#define MISO        SPIC_PORT_MISO                    //定义MISO所对应的GPIO接口编号  
#define CS_SPI2     SPIC_PORT_CS                     //定义SS所对应的GPIO接口编号  
#define OUTP        GPIO_OUTPUT                       //表示GPIO接口方向为输出  
#define INP GPIO_INPUT                           //表示GPIO接口方向为输入  


void TransmitDataDSPI_C(UINT8 data );
UINT32 ReadDataDSPI_C(void);
