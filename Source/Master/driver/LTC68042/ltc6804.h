/*
 * ltc6804.h
 *
 *  Created on: Nov 4, 2014
 *      Author: Administrator
 */
/*********************************************************
*
* 晟芯源微电子有限公司(UCT)
*
* File Description
*
*   $Revision: 2484 $
*   $Author: huhai $
*   $Date: 2014-08-01 11:05:59 +0800 (鍛ㄤ簲, 01 鍏湀 2014) $
*
*********************************************************/
#ifndef _LTC6804_H_
#define _LTC6804_H_


#include "public/public.h"

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

//INT16 ltc6804_init();
//void ltc6804_init() ;
//void wr_balance(  UINT8 index, UINT8 state );
enum UCT_ERR_CODE ltc6804_checkself ();
enum UCT_ERR_CODE ltc6804_readV_check ();
enum UCT_ERR_CODE ltc6804_temp_monitor ();
enum UCT_ERR_CODE ltc6804_set_balance(UINT8 index,UINT8 state);
enum UCT_ERR_CODE ltc6804_voltage_monitor ();
int ReadLtc6804stateByID (UINT8 index);
int getLtc6804VoltageUpdate(void);
int getLtc6804TempUpdate(void);
UINT16 ltc6804_read_v(UINT8 index);
INT16 ltc6804_read_t(UINT8 index);
UINT8 ltc6804_read_balance(UINT8 index);

#endif


