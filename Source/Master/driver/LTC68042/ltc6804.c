/*
 * ltc6804.c
 *
 *  Created on: Nov 4, 2014
 *      Author: Administrator
 */
#include "driver/LTC68042/ltc6804.h"
#include "driver/spi/spi_simulator.h"
//#include "public.h"
#include <string.h>
#include "public/public.h"
#include "driver/timer/timer.h"


#define TOTAL_TEMP_POINT_NUM (sizeof(BQ536_TEMP)/sizeof(UINT16))
void set_adc (UINT8 MD, UINT8 DCP, UINT8 CH,UINT8 CHG);

UINT8 ADCV[2];
UINT8 ADAX[2];
static UINT8 IC_state;
static UINT8 counter[MAX_ALLOWED_N_SLAVES];
static UINT16 RD6804failed_note;

static UINT16 v[MAX_ALLOWED_N_SLAVES][ LTC6804_MAX_VOL ];

static INT16 t[MAX_ALLOWED_N_SLAVES][ LTC6804_MAX_TEMP];

static const UINT16 BQ536_TEMP[] =
{ 270, 288, 307, 328, 349, 372, 396, 422, 449, 477, 507, 539, 572, 607, 644,
		683, 724, 767, 812, 860, 910, 962, 1016, 1074, 1134, 1197, 1262, 1331,
		1402, 1477, 1555, 1637, 1722, 1810, 1902, 1997, 2097, 2200, 2307, 2419,
		2534, 2653, 2777, 2905, 3037, 3174, 3315, 3461, 3611, 3765, 3925, 4088,
		4257, 4430, 4607, 4789, 4976, 5167, 5362, 5562, 5766, 5974, 6186, 6403,
		6623, 6847, 7075, 7306, 7541, 7779, 8020, 8265, 8511, 8761, 9013, 9267,
		9523, 9781, 10041, 10302, 10564, 10828, 11092, 11357, 11622, 11887,
		12153, 12418, 12683, 12947, 13210, 13472, 13734, 13993, 14252, 14508,
		14763, 15016, 15267, 15515, 15761, 16004, 16245, 16483, 16718, 16950,
		17179, 17405, 17628, 17848, 18064, 18277, 18486, 18692, 18895, 19094,
		19289, 19481, 19670, 19855, 20036, 20214, 20388, 20559, 20726, 20890,
		21051, 21208, 21361, 21512, 21658, 21802, 21943, 22080, 22214, 22345,
		22473, 22598, 22720, 22840, 22956, 23070, 23180, 23289, 23394, 23497,
		23597, 23695, 23791, 23884, 23975, 24063, 24149, 24234, 24316, 24396,
		24473, 24549, 24623, 24696, 24766, 24834, 24901, 24966, 25030, 25091 };


//static const UINT16 r_to_temp[TOTAL_TEMP_POINT_NUM] ={25561, 25343, 25119, 24888, 24650, 24406, 24156, 23898,
//	                                                  23636, 23367, 23092, 22811, 22524, 22239, 21944, 21644,
//	                                                  21340, 21031, 20718, 20401, 20082, 19758, 19432, 19103,
//	                                                  18773, 18440, 18106, 17771, 17434, 17098, 16761, 16425,
//	                                                  16089, 15755, 15421, 15100, 14759, 14431, 14105, 13782,
//	                                                  13463, 13147, 12834, 12525, 12220, 11918, 11621, 11329,
//	                                                  11040, 10758, 10478, 10205, 9937, 9673, 9414, 9162,
//	                                                  8911, 8670, 8433, 8201, 7975, 7753, 7538, 7326,
//	                                                  7120, 6921, 6725, 6534, 6349, 6125, 5994, 5822,
//	                                                  5655, 5495, 5336, 5183, 5035, 4890, 4749, 4613,
//	                                                  4480, 4353, 4228, 4105, 3989, 3875, 3762, 3655,
//	                                                  3552, 3451, 3351};  //-10°C ~ 80°C
typedef  struct _CFGR_T
{
    UINT8 buff[6];
} CFGR_T;
static CFGR_T  g_cfgr[MAX_ALLOWED_TEMPSENSORS];

static const UINT16 crc15Table[256] = {0x0, 0xc599, 0xceab, 0xb32, 0xd8cf, 0x1d56, 0x1664, 0xd3fd, 0xf407, 0x319e, 0x3aac, //!<precomputed CRC15 Table
                                       0xff35, 0x2cc8, 0xe951, 0xe263, 0x27fa, 0xad97, 0x680e, 0x633c, 0xa6a5, 0x7558, 0xb0c1,
                                       0xbbf3, 0x7e6a, 0x5990, 0x9c09, 0x973b, 0x52a2, 0x815f, 0x44c6, 0x4ff4, 0x8a6d, 0x5b2e,
                                       0x9eb7, 0x9585, 0x501c, 0x83e1, 0x4678, 0x4d4a, 0x88d3, 0xaf29, 0x6ab0, 0x6182, 0xa41b,
                                       0x77e6, 0xb27f, 0xb94d, 0x7cd4, 0xf6b9, 0x3320, 0x3812, 0xfd8b, 0x2e76, 0xebef, 0xe0dd,
                                       0x2544, 0x2be, 0xc727, 0xcc15, 0x98c, 0xda71, 0x1fe8, 0x14da, 0xd143, 0xf3c5, 0x365c,
                                       0x3d6e, 0xf8f7, 0x2b0a, 0xee93, 0xe5a1, 0x2038, 0x7c2, 0xc25b, 0xc969, 0xcf0, 0xdf0d,
                                       0x1a94, 0x11a6, 0xd43f, 0x5e52, 0x9bcb, 0x90f9, 0x5560, 0x869d, 0x4304, 0x4836, 0x8daf,
                                       0xaa55, 0x6fcc, 0x64fe, 0xa167, 0x729a, 0xb703, 0xbc31, 0x79a8, 0xa8eb, 0x6d72, 0x6640,
                                       0xa3d9, 0x7024, 0xb5bd, 0xbe8f, 0x7b16, 0x5cec, 0x9975, 0x9247, 0x57de, 0x8423, 0x41ba,
                                       0x4a88, 0x8f11, 0x57c, 0xc0e5, 0xcbd7, 0xe4e, 0xddb3, 0x182a, 0x1318, 0xd681, 0xf17b,
                                       0x34e2, 0x3fd0, 0xfa49, 0x29b4, 0xec2d, 0xe71f, 0x2286, 0xa213, 0x678a, 0x6cb8, 0xa921,
                                       0x7adc, 0xbf45, 0xb477, 0x71ee, 0x5614, 0x938d, 0x98bf, 0x5d26, 0x8edb, 0x4b42, 0x4070,
                                       0x85e9, 0xf84, 0xca1d, 0xc12f, 0x4b6, 0xd74b, 0x12d2, 0x19e0, 0xdc79, 0xfb83, 0x3e1a, 0x3528,
                                       0xf0b1, 0x234c, 0xe6d5, 0xede7, 0x287e, 0xf93d, 0x3ca4, 0x3796, 0xf20f, 0x21f2, 0xe46b, 0xef59,
                                       0x2ac0, 0xd3a, 0xc8a3, 0xc391, 0x608, 0xd5f5, 0x106c, 0x1b5e, 0xdec7, 0x54aa, 0x9133, 0x9a01,
                                       0x5f98, 0x8c65, 0x49fc, 0x42ce, 0x8757, 0xa0ad, 0x6534, 0x6e06, 0xab9f, 0x7862, 0xbdfb, 0xb6c9,
                                       0x7350, 0x51d6, 0x944f, 0x9f7d, 0x5ae4, 0x8919, 0x4c80, 0x47b2, 0x822b, 0xa5d1, 0x6048, 0x6b7a,
                                       0xaee3, 0x7d1e, 0xb887, 0xb3b5, 0x762c, 0xfc41, 0x39d8, 0x32ea, 0xf773, 0x248e, 0xe117, 0xea25,
                                       0x2fbc, 0x846, 0xcddf, 0xc6ed, 0x374, 0xd089, 0x1510, 0x1e22, 0xdbbb, 0xaf8, 0xcf61, 0xc453,
                                       0x1ca, 0xd237, 0x17ae, 0x1c9c, 0xd905, 0xfeff, 0x3b66, 0x3054, 0xf5cd, 0x2630, 0xe3a9, 0xe89b,
                                       0x2d02, 0xa76f, 0x62f6, 0x69c4, 0xac5d, 0x7fa0, 0xba39, 0xb10b, 0x7492, 0x5368, 0x96f1, 0x9dc3,
                                       0x585a, 0x8ba7, 0x4e3e, 0x450c, 0x8095
                                      };

//DECLARE_INIT_PROCESS( ltc6804_init ) 
//void ltc6804_init()
//{
//  set_adc(2,0,0,0);
//}
static void delayus(UINT16 n)
{
	while(n--)
		;
	//UINT16 j = 0, i = 0;
	//for( j=0x0f;j;j--)
		//for(i=0xffff;i;i--)
			//;
}
static UINT16 pec15_calc ( UINT8 *data , UINT16 len )
{
    UINT16 remainder, address, i;
    remainder = 16;//PEC seed

    for ( i = 0; i < len; i++ )
    {
        address = ( ( remainder >> 7 ) ^ data[i] ) & 0xff; //calculate PEC table address
        remainder = ( remainder << 8 ) ^ crc15Table[address];
    }

    return ( remainder * 2 ); //The CRC15 has a0 in the LSB so the final value must be multiplied by2
}


int reverse_index( UINT8 index, UINT8 SizeOfArray)
{
	index=SizeOfArray-index-1;
	return index;
}

static INT8 bq536_check_temp(UINT16 temp)
{
	UINT8 mid, low = 0, high = 0, j = 0;
	UINT16 ad_dived = 0;
	INT16 value;
	high = sizeof(BQ536_TEMP) / sizeof(UINT16);
//	mprintf("sizeof BQ536_TEMP:%d\r\n", sizeof(BQ536_TEMP));
	while (low <= high)
	{
		mid = (low + high) / 2;
//		mprintf("low:%d high:%d mid:%d real:%u, now:%u\r\n", low,high,mid,temp,BQ536_TEMP[mid]);
		if (mid == 0)
		{
			value = -40;
			goto end;
		}

		if (mid >= high - 1)
		{
			mid = high - 1;
			value = mid - 40;
			goto end;
		}

		if ((temp < BQ536_TEMP[mid]) && (temp > BQ536_TEMP[mid - 1]))
		{
			ad_dived = BQ536_TEMP[mid] - BQ536_TEMP[mid - 1];
			value = mid - 40;
			goto end;
		}
		else if (temp < BQ536_TEMP[mid])
		{
			high = mid - 1;
		}
		else if (temp > BQ536_TEMP[mid])
		{
			low = mid + 1;
		}
		else
		{
			value = mid - 40;
			goto end;
		}
	}
	end:
	value = reverse_index( value+40, (sizeof(BQ536_TEMP)/sizeof(UINT16)));
	value -= 40;
//	mprintf("get temp %d\r\n",value);
	return ((INT8)value);
}
static void wakeup ()
{
	    set_gpio_value(SS, 0);
        delayus ( 1000 );
        set_gpio_value(SS, 1);
        delayus( 3000 );
}
static void wakeup_idle()
{
	set_gpio_value(SS, 0);
    delayus ( 10 );
    set_gpio_value(SS, 1);
}

static void wakeup_sleep()
{
	set_gpio_value(SS, 0);
    delayus ( 5000 );
    set_gpio_value(SS, 1);
}

static void ltc6804_spi_write ( UINT8 *buff, UINT16 len )
{
    SPI_Write ( buff , len );   
}

static void ltc6804_spi_write_read ( UINT8 *inbuff, UINT16 inlen, UINT8 *outbuff, UINT16 outlen )
{
    SPI_Write ( inbuff , inlen );
    SPI_Read (  outbuff, outlen );
}
/***********************************************
 * 设置ADC转化模式 1/0
 ************************************************/
static void set_adc (UINT8 MD, UINT8 DCP, UINT8 CH,UINT8 CHG)
{
  
   UINT8 md_bits;
  
   md_bits = (MD & 0x02) >> 1;
   ADCV[0] = md_bits + 0x82;
   md_bits = (MD & 0x01) << 7;
   ADCV[1] =  md_bits + 0x60 + (DCP<<4) + CH;
 
   md_bits = (MD & 0x02) >> 1;
   ADAX[0] = md_bits + 0x04;
   md_bits = (MD & 0x01) << 7;
   ADAX[1] = md_bits + 0x60 + CHG ;
  
}

static void copydata(void *outbuff,const void *inbuff,UINT8 datalen)
{		
	memcpy(outbuff,inbuff,datalen);
}
/***********************************************
 * 设置基准电压 1/0
 ************************************************/
static void set_v ( UINT8 ic_num, UINT8 state )
{
	UINT8 cmd[4];
	UINT8 ltcwTemp[20];
    UINT16 temp_pec = 0;

    if ( state == 1 )
        state = 0xff;
    else
        state = 0xfb;

    /*写配置*/
    cmd[0] = 0x80 + (ic_num<<3);
    cmd[1] = 0x01;
    temp_pec = (UINT16)pec15_calc(cmd,2);
    cmd[2]  = (UINT8)(temp_pec>>8);
    cmd[3]  = (UINT8)(temp_pec&0xff);
    /*操作电压     */
    //V = ~V;
    copydata ( ltcwTemp, cmd, 4 );
    ltcwTemp[4] = state;
    ltcwTemp[5] = 0;
    ltcwTemp[6] = 0x80;
    ltcwTemp[7] = 0x13;
    ltcwTemp[8] =  g_cfgr[ic_num].buff[4];
    ltcwTemp[9] =  g_cfgr[ic_num].buff[5];

    temp_pec = ( UINT16 ) pec15_calc ( &ltcwTemp[4], 6 ); // calculating the PEC for each board

    ltcwTemp[10] = ( UINT8 ) ( temp_pec >> 8 );
    ltcwTemp[10 + 1] = ( UINT8 ) temp_pec;
     wakeup_idle ();
     set_gpio_value(SS, 0);
    ltc6804_spi_write ( ltcwTemp, 12 );
     set_gpio_value(SS, 1);
}




/*************************************************************
 *写配置寄存器到
 *  
 * by mgc 20140728
 **************************************************************/

static void WR_CFGR6804 ( UINT8 ic_num )
{
    UINT8 cmd[4];
    UINT8 ltcwTemp[15]={0};
    UINT8 *pbuff = 0;
    UINT16 temp_pec = 0;  
    //g_cfgr[ic_num].buff[0] = 0;
   // g_cfgr[ic_num].buff[1] = 0;
    //g_cfgr[ic_num].buff[2] = 0x80;
    //g_cfgr[ic_num].buff[3] = 0x13;
    /*写配置*/
    //WRCFG
    cmd[0] = 0x80 + (ic_num<<3);
    cmd[1] = 0x01;
    //PEC
    temp_pec = (UINT16)pec15_calc(cmd,2);
    cmd[2]  = (UINT8)(temp_pec>>8);
    cmd[3]  = (UINT8)(temp_pec&0xff); 
    copydata ( ltcwTemp, cmd, 4 );
    /*写寄存器*/
    copydata ( ltcwTemp + 4, g_cfgr[ic_num].buff, sizeof ( g_cfgr[ic_num].buff ) );
    temp_pec = ( UINT16 ) pec15_calc ( &ltcwTemp[4], 6 ); // calculating the PEC for each board

    ltcwTemp[10] = ( UINT8 ) ( temp_pec >> 8 );
    ltcwTemp[10 + 1] = ( UINT8 ) temp_pec;
    wakeup_idle ();
    set_gpio_value(SS, 0);
     ltc6804_spi_write (ltcwTemp, 12 );
     set_gpio_value(SS, 1);
}

/*************************************************************
 * 读配置寄存器到
 * 输入, 
 *  
 *      buff: 读出的配置寄存器值存储的地方,注意buff长度需要超过6
 * return:
 *  >0成功 <0 失败
 * by mgc 20140814
 **************************************************************/
static void RD_CFGR6804( UINT8 ic_num, UINT8 *buff ) 
{
 		UINT8 cmd[4];
 		UINT8 ltcrTemp[15]={0};
 		UINT8 *pbuff = 0;
 		UINT16 temp_pec = 0;
    
    /*读配置*/
    //WRCFG
    cmd[0] = 0x80 + (ic_num<<3);
    cmd[1] = 0x02;
    //PEC
    temp_pec = (UINT16)pec15_calc(cmd,2);
    cmd[2]  = (UINT8)(temp_pec>>8);// 0x2B;
    cmd[3]  = (UINT8)(temp_pec&0xff);//0x0A; 
      
//    copydata(ltcrTemp, cmd, 4);
     wakeup_idle ();
     set_gpio_value(SS, 0);
    ltc6804_spi_write_read(  cmd, 4, ltcrTemp, 10 );  // 6 cfg + 2 pec
    copydata(buff,ltcrTemp, 6);
    set_gpio_value(SS, 1);   
}

static enum UCT_ERR_CODE ltc6804_rdcv_reg ( UINT8 ic_num,
                                     UINT8 reg,
                                     UINT8 *data )
{
    enum UCT_ERR_CODE rv = UCT_ERR_INVALID_PARAMETER;
    static UINT8 cmd[4] = {0,0,0,0};
    static UINT8 buff[10] = {0};
    UINT8 temp_8 = 0;
    UINT16 temp_pec = 0;
    UINT16 temp_16 = 0;

    if ( reg == 1 )
    {
        cmd[1] = 0x04;
        cmd[0] = 0x80 + (ic_num<<3);
    }
    else if ( reg == 2 )
    {
        cmd[1] = 0x06;
        cmd[0] = 0x80 + (ic_num<<3);
    }
    else if ( reg == 3 )
    {
        cmd[1] = 0x08;
        cmd[0] = 0x80 + (ic_num<<3);
    }
    else if ( reg == 4 )
    {
          cmd[1] = 0x0A;
          cmd[0] = 0x80 + (ic_num<<3);
    }
    else
    {
        return rv;
    }
    temp_pec = pec15_calc ( cmd, 2 );
    cmd[2] = ( UINT8 ) ( temp_pec >> 8 );
    cmd[3] = ( UINT8 ) ( temp_pec );
    wakeup_idle ();
    set_gpio_value(SS, 0);
    ltc6804_spi_write_read ( cmd, 4, buff, 10 );
    set_gpio_value(SS, 1);
    temp_pec = ( ( buff[6] << 8 ) & 0xFF00 ) | ( buff[7] & 0xff );

    if ( temp_pec == pec15_calc ( &buff[0], 6 ) )
    {
        copydata ( data, &buff[0], 6 );
        rv = UCT_SUCCESS;
    }

    return rv;
}



static enum UCT_ERR_CODE ltc6804_rdcv ( UINT8 ic_num,
                                 UINT8 reg,
                                 UINT16 *cell_codes )
{
    enum UCT_ERR_CODE rv = UCT_SUCCESS;
    UINT8 cell_reg = 0;
    UINT8 current_ic = 0;
    UINT8 current_cell = 0;
    UINT8 cell_data[NUM_RX_BYT] = {0};
    UINT8 pec_error = 0;
    UINT16 parsed_cell;
    UINT8 data_counter = 0; //data counter

    //1.a
    if ( reg == 0 )
    {
        //a.i
        for ( cell_reg = 1; cell_reg < 5; cell_reg++ )               //executes once for each of the LTC6804 cell voltage registers
        {
            data_counter = 0;
            rv = ltc6804_rdcv_reg ( ic_num, cell_reg, cell_data );

            if ( rv != UCT_SUCCESS )
            {
                return rv;
            }

            //a.ii
            for ( current_cell = 0; current_cell < CELL_IN_REG; current_cell++ )                                // This loop parses the read back data. Loops
            {
                // once for each cell voltages in the register
                parsed_cell = ( ( cell_data[data_counter + 1] << 8 ) & 0xFF00 ) | cell_data[data_counter];
                cell_codes[current_cell  + ( ( cell_reg - 1 ) * CELL_IN_REG )] = parsed_cell/10;
                data_counter = data_counter + 2;
            }
        }
    }
//1.b
    else
    {
        //b.i
        rv = ltc6804_rdcv_reg ( ic_num, reg, cell_data );

        if ( rv != UCT_SUCCESS )
        {
            return rv;
        }

        //b.ii
        for ( current_cell = 0; current_cell < CELL_IN_REG; current_cell++ )                                    // This loop parses the read back data. Loops
        {
            // once for each cell voltage in the register
            parsed_cell = ( ( cell_data[data_counter + 1] << 8 ) & 0xFF00 ) | cell_data[data_counter];
            cell_codes[current_cell + ( ( reg - 1 ) * CELL_IN_REG )] = parsed_cell/10;
            data_counter = data_counter + 2;
        }
    }
    return rv;
}


static enum UCT_ERR_CODE ltc6804_rdaux_reg ( UINT8 ic_num,
                                      UINT8 reg,
                                      UINT8 *data )
{
    enum UCT_ERR_CODE rv = UCT_ERR_INVALID_PARAMETER;
    static UINT8 cmd[4];
    static UINT8 buff[10] = {0};
    UINT16 temp_pec;

    //1
    if ( reg == 1 )
    {
        cmd[1] = 0x0C;
        cmd[0] = 0x80 + (ic_num<<3);
    }
    else if ( reg == 2 )
    {
        cmd[1] = 0x0e;
        cmd[0] = 0x80 + (ic_num<<3);
    }
    else
    {
        cmd[1] = 0x0C;
        cmd[0] = 0x80 + (ic_num<<3);
    }
    temp_pec = pec15_calc ( cmd, 2 );
    cmd[2] = ( UINT8 ) ( temp_pec >> 8 );
    cmd[3] = ( UINT8 ) ( temp_pec );
    set_gpio_value(SS, 0);
    ltc6804_spi_write_read (cmd, 4, buff, 10 );
    set_gpio_value(SS, 1);
    temp_pec = ( ( buff[6] << 8 ) & 0xFF00 ) | ( buff[7] & 0xff );

    if ( temp_pec == pec15_calc ( &buff[0], 6 ) )
    {
        copydata ( data, &buff[0], 6 );
        rv = UCT_SUCCESS;
    }
    return rv;
}

static enum UCT_ERR_CODE LTC6804_rdaux ( UINT8 ic_num,
                                  UINT8 reg,
                                  UINT16 *aux_codes )
{
    enum UCT_ERR_CODE rv = UCT_SUCCESS;
    UINT8 data[NUM_RX_BYT * LTC6804_IC_NUM] = {0};
    UINT8 data_counter = 0;
    UINT8 pec_error = 0;
    UINT8 gpio_reg;
    UINT8 current_gpio;

    //1.a
    if ( reg == 0 )
    {
        //a.i
        for ( gpio_reg = 1; gpio_reg < 3; gpio_reg++ )               //executes once for each of the LTC6804 aux voltage registers
        {
            data_counter = 0;
            rv = ltc6804_rdaux_reg ( ic_num, gpio_reg, data );

            if ( rv != UCT_SUCCESS )
            {
                return rv;
            }

            for ( current_gpio = 0; current_gpio < GPIO_IN_REG; current_gpio++ ) // This loop parses GPIO voltages stored in the register
            {
                aux_codes[current_gpio + ( ( gpio_reg - 1 ) *GPIO_IN_REG )] = data[data_counter] + ( data[data_counter + 1] << 8 );
                data_counter = data_counter + 2;
            }

            delayus ( 1000 );
        }
    }
    else
    {
        //b.i
        rv = ltc6804_rdaux_reg ( ic_num, reg, data );

        if ( rv != UCT_SUCCESS )
        {
            return rv;
        }

        //b.ii
        for ( current_gpio = 0; current_gpio < GPIO_IN_REG; current_gpio++ ) // This loop parses the read back data. Loops
        {
            // once for each aux voltage in the register
            aux_codes[current_gpio + ( ( reg - 1 ) *GPIO_IN_REG )] = data[data_counter] + ( data[data_counter + 1] << 8 );
            data_counter = data_counter + 2;
        }
    }

    return rv;
}

static void wr_balance ( UINT8 ic_num, UINT8 index, UINT8 state )
{
	set_v(ic_num,1);

    if ( index < 8 )
    {
        if ( state == 0 )
           MGC_RESET_BIT ( g_cfgr[ic_num].buff[4], index );
        else
            MGC_SET_BIT ( g_cfgr[ic_num].buff[4], index );
    }
    else
    {
        index = index - 8;

        if ( state == 0 )
            MGC_RESET_BIT ( g_cfgr[ic_num].buff[5], index );
        else
            MGC_SET_BIT ( g_cfgr[ic_num].buff[5], index );
    }
//    WR_CFGR6804 ( ic_num );
}


static void ltc6804_adcv()
{
    UINT8 cmd[4];
    UINT16 temp_pec;

    cmd[0] =0x03; //ADCV[0]; //MD[1:0]=10;  
    cmd[1] =0x60; //ADCV[1];   //   
    temp_pec = pec15_calc ( cmd, 2 );
    cmd[2] = ( UINT8 ) ( temp_pec >> 8 );
    cmd[3] = ( UINT8 ) ( temp_pec );
    wakeup_idle (); //This will guarantee that the LTC6804 isoSPI port is awake. 
    set_gpio_value(SS, 0);   
    ltc6804_spi_write ( cmd, 4 );
    set_gpio_value(SS, 1); 
}


static void ltc6804_adax ()
{
    UINT8 cmd[4];
    UINT16 temp_pec;
    cmd[0] =0x05;    //ADAX[0];
    cmd[1] =0x60;    //ADAX[1];
    temp_pec = pec15_calc ( cmd, 2 );
    cmd[2] = ( UINT8 ) ( temp_pec >> 8 );
    cmd[3] = ( UINT8 ) ( temp_pec );
    set_gpio_value(SS, 0);
    ltc6804_spi_write ( cmd, 4 );
    set_gpio_value(SS, 1);
}

enum UCT_ERR_CODE ltc6804_checkself ()
{
    enum UCT_ERR_CODE rv = UCT_SUCCESS;
    rv =ltc6804_voltage_monitor ();
    if ( rv != UCT_SUCCESS )
    {
         return rv;
    }
    rv =ltc6804_temp_monitor ();
    return rv;
}

struct ltc6804_update_flag
{
	UINT8 temperatureUpdate;
	UINT8 voltageUpdate;
};

static struct ltc6804_update_flag g6804Update = {0,0};

int getLtc6804VoltageUpdate(void)
{
	return g6804Update.voltageUpdate;
}

int getLtc6804TempUpdate(void)
{
	return g6804Update.temperatureUpdate;
}

enum UCT_ERR_CODE ltc6804_readV_check ()
{
	UINT8 i;
	if(IC_state!=0)
	{
		for(i=0;i<N_SLAVES;i++)
		{
			if(IC_state&(1<<i))
			    counter[i]++;
			if(counter[i]==10)
				MGC_SET_BIT ( RD6804failed_note, i );
			//todo
		}
	}
	else
	{
		RD6804failed_note=0;
		for(i=0;i<N_SLAVES;i++)
		{
			counter[i]=0;
		}
	}
	return RD6804failed_note;
}

int ReadLtc6804stateByID (UINT8 index)  //0：正常；1：通信失败
{
	UINT8 state;
	state=(RD6804failed_note&(1<<index))>>index;
	return state;
}
enum UCT_ERR_CODE ltc6804_voltage_monitor ()
{
    enum UCT_ERR_CODE rv = UCT_SUCCESS;
    UINT16 data[12] = {0};
    UINT16 errdata[12]={0};
    UINT8 current_ic;
    UINT8 count = 0;
    static UINT8 counter[2]={0};
	g6804Update.voltageUpdate = 0;
    wakeup();
    delayus ( 1000 );                
    ltc6804_adcv(); 
    delayus (1000 );
    for( current_ic = 0; current_ic<N_SLAVES; current_ic++)
    {
    	set_v(current_ic,1);
         rv = ltc6804_rdcv ( current_ic, 0, data );
         if ( rv != UCT_SUCCESS )
         {
        	  copydata ( &v[current_ic][0], errdata, 24 );
               continue;
         }
 
         if ( rv == UCT_SUCCESS )
         {
        	 copydata ( &v[current_ic][0], data, 24 );
        	 MGC_RESET_BIT ( IC_state, current_ic );
         }
         delayus ( 3000 );
    }
//    if(rv == UCT_SUCCESS)
    	g6804Update.voltageUpdate = 1;    
   return rv;
}
enum UCT_ERR_CODE ltc6804_temp_monitor ()
{   
    enum UCT_ERR_CODE rv = UCT_SUCCESS;
    UINT16 data[6] = {0};
    UINT16 errdata[6]={0xff,0xff,0xff,0xff,0xff,0xff};
    UINT8 current_ic;
    UINT8 temp_num = 0;
    UINT8 sensor_num = 0;
    INT8 count = 0;
    UINT8 i;
    UINT8 ic_num = 0;
    UINT16 temp_16 = 0;
    UINT32 temp = 0;
    UINT32 temp_32 = 0;
    static UCHAR stable = 0, stable1 = 0;
    UINT16 buff[LTC6804_MAX_TEMP] = {0};
    g6804Update.temperatureUpdate = 0;
    wakeup ();
    delayus ( 1000 );                
    ltc6804_adax(); 
    delayus (1000 );
    for( current_ic = 0; current_ic<N_SLAVES; current_ic++)
    {
    	set_v(current_ic,1);
         rv = LTC6804_rdaux ( current_ic, 0, buff );
         if ( rv != UCT_SUCCESS )
         {
               copydata ( &t[current_ic][0], errdata, 10 );
               continue;
         }
         for ( sensor_num = 0; sensor_num < LTC6804_MAX_TEMP; sensor_num ++ )
         {
               t[current_ic][sensor_num]=bq536_check_temp(buff[sensor_num]);
      
//            if ( buff[sensor_num] >= r_to_temp[0] )
//            {
////                 stable = 0;
////
////                 if ( stable1++ >= 1 )
////                 {
////                      stable1 = 0;
//                      t[current_ic][sensor_num] = -10;
////                 }
//                  continue;
//            }
//            else if ( buff[sensor_num] <= r_to_temp[TOTAL_TEMP_POINT_NUM - 1] )
//            {
////                 stable1 = 0;
////                 if ( stable++ >= 1 )
////                 {
////                      stable = 0;
//                      t[current_ic][sensor_num] = 80;
////                 }
//                 // */
//                 //g_UCT_temperature[ic_num * LTC6804_MAX_TEMP + sensor_num] = TOTAL_TEMP_POINT_NUM - 1 - 10;
//                  continue;
//             }
//             else
//             {
//                  stable = 0;
//                  stable1 = 0;
//             }
//             for ( count = 0; count < ( TOTAL_TEMP_POINT_NUM - 1 ); count ++ )
//             {
//                  if ( ( buff[sensor_num] < r_to_temp[count] )&& ( buff[sensor_num] > r_to_temp[count + 1] ) )
//                  {
//                	  t[current_ic][sensor_num] = count - 10;
//                  }
//             }

          }
         delayus ( 3000 );
    }        
//    if(rv == UCT_SUCCESS)
    	g6804Update.temperatureUpdate = 1;
    return rv;
}
enum UCT_ERR_CODE ltc6804_set_balance(UINT8 index,UINT8 state)
{
	  enum UCT_ERR_CODE rv = UCT_ERR_INVALID_PARAMETER;
    UINT8 current_ic=0,real_index=0;
    if((index<0)||(index>MAX_CELLS))
    {
    	 return rv;
    }
    current_ic=(index)/NCLEES_EACH_SLAVE;
    real_index=(index)%NCLEES_EACH_SLAVE;
    wr_balance ( current_ic, real_index, state );
    rv = UCT_SUCCESS ;
    return rv;
}
#define _SIMULATE_VOLTAGE 0
UINT16 ltc6804_read_v(UINT8 index)
{
    enum UCT_ERR_CODE rv = UCT_ERR_INVALID_PARAMETER;
    UINT8 current_ic=0,real_index=0;
    static UINT8 ID;
    static UINT16 count;
    UINT16 value=0;
    if((index<0)||(index>(MAX_CELLS)) )
    {
    	 return rv;
    }
    current_ic=(index)/NCLEES_EACH_SLAVE;
    real_index=(index)%NCLEES_EACH_SLAVE;
#if _SIMULATE_VOLTAGE
    value = 2500 + index*10*(1+ID)+ID*1000;
    count++;
    if(count==2700)
    {
   	count=0;
   	ID=1-ID;    	
    }
    
#else
    value=v[current_ic][real_index];
#endif
    return value;
}
#define _SIMULATE_TEMPERATURE 0
INT16 ltc6804_read_t(UINT8 index)
{
    static int count,ID;
	enum UCT_ERR_CODE rv = UCT_ERR_INVALID_PARAMETER;
    UINT8 current_ic=0,real_index=0;
    INT16 value=0;
    if((index<0)||(index>(NTEMPS_EACH_SLAVE*N_SLAVES)))
    {
    	 return rv;
    }
    current_ic=(index)/NTEMPS_EACH_SLAVE;
    real_index=(index)%NTEMPS_EACH_SLAVE;
#if SIMULATE_TEMPERATURE
        value=index*(1+ID)+10*ID-10;
        count++;
       if(count == 900)
       {
      	  ID=1-ID;
      	  count = 0;
       }
#else
    value=t[current_ic][real_index];
#endif
//    if((real_index==4)&&(current_ic==0));
//    else if((real_index==2)&&(current_ic==1));
//    else
//    	value=15;
    return value;
}

UINT8 ltc6804_read_balance(UINT8 index)
{
    UINT8 data_H = 0,  data_L = 0;    
    UINT8 current_ic=0,real_index=0;
    UINT8 state=0;
    static UINT8 rdcfg[6];
    current_ic=(index)/NCLEES_EACH_SLAVE;
    real_index=(index)%NCLEES_EACH_SLAVE;
    RD_CFGR6804(  current_ic,rdcfg); 
     if(real_index<8)
    {
    	 state= (rdcfg[4]&(1<<real_index))>>real_index;
    }
    else
    {
    	 real_index=real_index-8;
    	 state= (rdcfg[5]&(1<<real_index))>>real_index;
    }
    return state;
}

int testrdltc6804_V()
{
    enum UCT_ERR_CODE rv = UCT_SUCCESS;
    UINT16 data[12] = {0};
    UINT8 current_ic;
    UINT8 count = 0;
    static UINT8 counter[2]={0};
    static UINT16 T_readV=0;
    static UINT16 F_readV=0;
	g6804Update.voltageUpdate = 0;
    wakeup();
    delayus ( 1000 );                
    ltc6804_adcv(); 
    delayus (1000 );
    for( current_ic = 0; current_ic<N_SLAVES; current_ic++)
    {
         rv = ltc6804_rdcv ( current_ic, 0, data );
         if( rv == UCT_SUCCESS )
         {
        	 T_readV++;
         }
         else
        	 F_readV++;
         if ( rv == UCT_SUCCESS )
         {
        	 copydata ( &v[current_ic][0], data, 24 );
        	 MGC_RESET_BIT ( IC_state, current_ic );
         }
         else
         {
        	 MGC_SET_BIT(IC_state, current_ic);
         }
         delayus ( 3000 );
    }
//    if(rv == UCT_SUCCESS)
    	g6804Update.voltageUpdate = 1;
//  return IC_state;
    
   return rv;
}


