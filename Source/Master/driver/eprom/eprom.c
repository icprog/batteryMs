/*
 * eprom.c
 *
 *  Created on: Dec 18, 2014
 *      Author: Administrator
 */

/*********************************************************
 *
 * 晟芯源微电子有限公司(UCT)
 *
 * File Description
 *
 *   $Revision: 2057 $
 *   $Date: 2014-06-05 14:57:35 
 *
 *********************************************************/
#include "MPC5634M.h"
//#include <hidef.h>
//#include "derivative.h" /* include peripheral declarations */
#include "driver/eprom/eprom.h"
#include "public/types.h"
//#include "spi/spi.h"
#include "driver/spi/spi_simulator.h"

//=========================================================================
//函数名称：WriteWREN
//参数说明： 无
//函数返回： 无
//功能概要：铁电写使能
//=========================================================================
UINT8 WriteWREN(void)
{
	UINT32 temp;
	spicss_enable(1);
	//TransmitDataDSPI_C(WREN);
	spic_write_byte(WREN);
	// spic_write_byte(RDSR);
//   // (void)SPI2_Byte(WREN);
//    for ( temp = 0; temp < 50000; temp++ );
//    
//    CS_SPI2 = 1; //拉高CS引脚
	spicss_enable(0);
	return TRUE;
}

//=========================================================================
//函数名称：WriteEprom
//参数说明：Adr     写入EPROM的地址
//          *DAT    写入的数据
//          LETH    所写数据的长度
//函数返回： 无
//功能概要：写EPROM
//=========================================================================
UINT8 WriteEprom(UINT16 Adr, UINT16 LETH, UINT8 *DAT)
{
	UINT32 temp1;
	UINT16 length;
	if ((Adr > DRESS_MAX) | (Adr < DRESS_MIN))
		return FALSE;
	WriteWREN();
	spicss_enable(1);
	spic_write_byte(WRITE);
	spic_write_byte((UINT8) (Adr >> 8));
	spic_write_byte((UINT8) (Adr));
//    TransmitDataDSPI_C(WRITE);
//    TransmitDataDSPI_C((UINT8)(Adr>>8));
//    TransmitDataDSPI_C((UINT8)(Adr));
	for (length = 0; length < LETH; length++)
	{
		// TransmitDataDSPI_C(DAT[length]);
		spic_write_byte(DAT[length]);
	}
	// for ( temp1 = 0; temp1 < 50000; temp1++ )
	spicss_enable(0);
	return TRUE;
}

//=========================================================================
//函数名称：ReadEprom
//参数说明：Adr     读EPROM的地址
//          *DAT    读入的数据
//          LETH    所读数据的长度
//函数返回： 无
//功能概要：读EPROM
//=========================================================================
UINT8 ReadEprom(UINT16 Adr, UINT16 LETH, UINT8 *DAT)
{
	UINT32 temp1;
	UINT16 Length = 0;
	if ((Adr > DRESS_MAX) | (Adr < DRESS_MIN))
		return FALSE;
	WriteWREN();
	spicss_enable(1);
	spic_write_byte(READ);
	spic_write_byte((UINT8) (Adr >> 8));
	spic_write_byte((UINT8) (Adr));
//    TransmitDataDSPI_C(READ);
//    TransmitDataDSPI_C((UINT8)(Adr>>8));
//    TransmitDataDSPI_C((UINT8)(Adr));
	for (Length = 0; Length < LETH; Length++)
	{
		//DAT[ Length ] = ReadDataDSPI_C();
		DAT[Length] = spic_read_byte();
	}
	//  for ( temp1 = 0; temp1 < 50000; temp1++ );
	spicss_enable(0);
	return TRUE;
}

int eepromInit(void)
{
	return spic_init();
}


void testrom()
{
	UINT32 temp;
	UINT16 len = 10, rdst = 0;
	static UINT8 dat[10];
	UINT16 addr = 0X7FFF;
	spic_init();
	for (temp = 0; temp < 10; temp++)
	{
		dat[temp] = temp;
	}
	//while(1) {

	//WriteWREN();
	while (1)
	{
		WriteWREN();
		spicss_enable(1);
		spic_write_byte(RDSR);
		rdst = spic_read_byte();
		spicss_enable(0);
		
		WriteEprom(addr, len, dat);
		for (temp = 0; temp < 10; temp++)
		{
			dat[temp] = 0;
		}
		//(void)memset(dat,0,sizeof(dat[10]));
		ReadEprom(addr, len, dat);
		// for ( temp = 0; temp < 50000; temp++ );
	}

}
