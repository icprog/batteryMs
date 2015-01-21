#ifndef _SPI_H_
#define _SPI_H_
#include "Gpio/gpio.h"
//int spi_init(void);
//int spi_read( UINT8 *rd_buff, UINT8 len);
//int spi_write( const UINT8 *wr_buff, UINT8 len);
//int spi_read_write(UINT8 *rd_buff, const UINT8 *wr_buff, UINT8 len);
//int test_spi(void);

#endif


#define SCLK        SPIC_PORT_SCK                    //����SCLK����Ӧ��GPIO�ӿڱ��  
#define MOSI        SPIC_PORT_MOSI                     //����SCLK����Ӧ��GPIO�ӿڱ��  
#define MISO        SPIC_PORT_MISO                    //����MISO����Ӧ��GPIO�ӿڱ��  
#define CS_SPI2     SPIC_PORT_CS                     //����SS����Ӧ��GPIO�ӿڱ��  
#define OUTP        GPIO_OUTPUT                       //��ʾGPIO�ӿڷ���Ϊ���  
#define INP GPIO_INPUT                           //��ʾGPIO�ӿڷ���Ϊ����  


void TransmitDataDSPI_C(UINT8 data );
UINT32 ReadDataDSPI_C(void);
