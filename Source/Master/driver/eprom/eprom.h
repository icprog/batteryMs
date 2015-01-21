/*
 * eprom.h
 *
 *  Created on: Dec 18, 2014
 *      Author: Administrator
 */

#ifndef EPROM_H_
#define EPROM_H_


/*********************************************************
*
* 晟芯源微电子有限公司(UCT)
*
* File Description
*
*   $Revision: 2252 $
*   $Author: $
*   $Date: 2014-12-18 14:57:35 
*
*********************************************************/
#include "public/types.h"
//#include "public.h"
//UINT8 WREN,WRDI,RDSR,WRSR,READ,WRITE;
#define WREN 0x06
#define WRDI 0x04
#define RDSR 0x05
#define WRSR 0x01
#define READ 0x03
#define WRITE 0x02
#define DRESS_MAX 0x7fff
#define DRESS_MIN 0x0000
void testrom(void) ;
int eeprom_self_test(void);
UINT8 WriteWREN( void );
UINT8 WriteEprom( UINT16 Adr, UINT16 LETH, UINT8 *DAT );
UINT8   ReadEprom( UINT16 Adr, UINT16 LETH, UINT8 *DAT );
int eepromInit(void);
extern  void spicss_enable(int enable);

#endif /* EPROM_H_ */
