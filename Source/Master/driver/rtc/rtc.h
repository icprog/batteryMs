/*
 * rtc.h
 *
 *  Created on: Dec 25, 2014
 *      Author: Administrator
 */

#ifndef RTC_H_
#define RTC_H_



#endif /* RTC_H_ */
#include "MPC5634M.h"
#include "driver/Gpio/gpio.h"
#define SCL_PORT		118        
#define SDA_PORT		119
//#define SCL   SIU .GPDO[SCL_PORT].R
//#define SDA   SIU .GPDO[SDA_PORT].R
#define	PCF8563_SLA		0xA2
//unsigned char init_time[7]={0,0,10,11,3,4,7};

struct DATE{
	UINT8  second;
	UINT8  minute;
	UINT8  hour;
	UINT8  day;
	UINT8  week;
	UINT8  month;
	UINT8  year;
};


void writebyte(UINT8 wdata);
UINT8 Readbyte();
void writeData(UINT8 address,UINT8 mdata);
UINT8 ReadData(UINT8 address) ;
void ReadData1(UINT8 address,UINT8 count,UINT8 * buff);
void P8563_Read();
void P8563_gettime();
void P8563_settime();
void P8563_init();
void RTC_test();
void set_date(struct DATE date);
struct DATE read_date(void);
struct DATE calculate_date(struct DATE b_date,struct DATE a_date);
UINT8  bcd_convert_hex(UINT8  bcd_data);
UINT8   hex_convert_bcd(UINT8  hex_data);
