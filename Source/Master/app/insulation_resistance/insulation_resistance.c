#include "insulation_resistance/insulation_resistance.h"
#include "voltage/voltage.h"
#include "driver/timer/timer.h"
#include "driver/Gpio/gpio.h"
/*defination*/
#define open_s1() set_CONTROL_S0(1)
#define close_s1() set_CONTROL_S0(0)
#define close_s2() set_CONTROL_S1(0)
#define open_s2() set_CONTROL_S1(1)
#define READ_V1()     read_res_vp() //  voltage v
#define READ_V2()     read_res_vn() //  voltage v
#define  R0   50000
/*types*/
enum state_insulation
{
    step1,
    step2,
    step3,
    step4
};

/*private value*/
static INT16 v1_0,  v2_0,  v1_1,  v2_1;
static UINT32 R_N = 0;
static UINT32 R_P = 0;
//static  const UINT16 R0 = 49900; //50ko

static UINT16 cal_average_v ( const UINT16 *data, UINT16 len )
{
    UINT32 sum = 0;
    UINT8 i = 0;

    if ( len == 0 ) return 0;

    for ( i = 0; i < len; i++ )
    {
        sum += data[i];
    }

    return sum / len;
}

static  UINT16 v1[20] = {0};
static  UINT16 v2[20] = {0};
static int get_average_v1_v2 ( UINT16 * vv1, UINT16 * vv2 )
{
    UINT8 flag_counter = 0;

    for ( flag_counter = 0; flag_counter < 20; flag_counter++ )
    {
        //adc_capture();
        v1[flag_counter] = READ_V1();
        v2[flag_counter] = READ_V2();
    }

    * vv1 =  cal_average_v ( v1, (sizeof ( v1 ))/sizeof(UINT16 ) ) ;
    * vv2 =  cal_average_v ( v2, (sizeof ( v2 ))/sizeof(UINT16 )) ;
    return 0;
}

static void insulation_check_R_N ( UINT32 rn )
{
	(void)rn;
    //todo
}
static void insulation_check_R_P ( UINT32 rp )
{
	(void)rp;
    //todo
}
/*
 * insulation resistance calculat
 * if < 0 err
 * if >0 success
 * by mgc 20140827
 */
// #define   100UV_TO_V(uv)   ( (uv)/10000)
//int insulation_resistance_calculate ( void )
//{
//    //step 1
//    //v1_0 = get_average_v1_20();
//    //v2_0 = get_average_v2_20();
//    UINT32 temp1 = 0, temp2 = 0;
//     
//    close_s1();
//    close_s2();
//    delay_us(20000);
//    
//    get_average_v1_v2 ( (UINT16 *)&v1_0, (UINT16 *)&v2_0 );
//    mprintf("\r\nv1_0:%lu(0.1mv),v2_0:%lu(0.1mv)\r\n", v1_0&0x0ffff, v2_0&0x0ffff);
//    /*
//    if ( v1_0 <= 0 && v2_0 <= 0 )
//    {
//        return -1;
//    }
//      */
//    if ( v1_0 > v2_0 )
//    {
//        open_s1();
//        close_s2();
//    }
//    else
//    {
//        open_s2();
//        close_s1();
//    }
//                                 
//    delay_us(20000);
//
//    get_average_v1_v2 ( (UINT16 *)&v1_1, (UINT16 *)&v2_1 );
//    mprintf("v1_1:%u(0.1mv),v2_1:%u(0.1mv)\r\n", v1_1, v2_1);
//        /*
//    if ( v1_1 <= 0 || v2_1 <= 0 )
//    {
//        return -2;
//    }
//          
//    v1_1 = 100UV_TO_V(v1_1);
//    v2_1 = 100UV_TO_V(v2_1);
//    v1_0 = 100UV_TO_V(v1_0);
//    v2_0 = 100UV_TO_V(v2_0);
//             */
//    if ( v1_1 > v2_1 )
//    {
//       temp1 = v1_0 - v1_1;
//       if( temp1 < 0)
//            temp1 = 1 - temp1;
//       if( v1_0 == 0 )
//       {
//                 mprintf("danger v1_0 == 0 !!!\r\n");
//                  temp2 = 65535;
//                //err process
//       }
//       else
//       {
//                temp2 = v2_1/v1_0;         //v
//                temp2 = v1_0*R0*temp2;    //100uv
//       }
//       
//       if( temp2 == 0 )
//       {
//            R_N = 0xFFFF;
//       }
//       else
//            R_N = temp1/temp2;
//       // R_N = ( ( v1_0 - v1_1 ) / ( v1_0 * R0 * ( 1 + v2_1 / v1_0 ) ) );
//        //insulation_check_R_N ( R_N );
//    }
//    else
//    {
//       temp1 = v2_0 - v2_1;
//       if( temp1<0 )
//            temp1 = 1 - temp1;
//       if( v2_0 == 0 )
//       {
//                 mprintf("danger v2_0 == 0 !!!\r\n");
//                //err process
//                temp2 = 0xffff;
//       }
//       else
//       {
//            temp2 = v1_0/v2_0;         //v
//            temp2 = v2_0*R0*temp2;    //100uv
//       }
//       
//       if( temp2 == 0 )
//       {
//            R_P = 0xffff;
//       }
//       else
//            R_P = temp1/temp2;
//       // R_P = ( ( v2_0 -v2_1 ) / ( v2_0 * R0 * ( 1 + v1_0 / v2_0 ) ) );
//        //insulation_check_R_P ( R_P );
//    }
//      mprintf("RP:%lu, PN:%lu\r\n", R_P, R_N);
//    return 0;
//}
int insulation_resistance_calculate ( void )
{
    UINT32 temp=0,temp1 = 0, temp2 = 0;
     
    close_s1();
    close_s2();
    
    delay_us(20000);
    
    v1_0 = READ_V1();
    v2_0 = READ_V2();

    if ( v1_0 > v2_0 )
    {
        open_s1();
        close_s2();
    }
    else
    {
        open_s2();
        close_s1();
    }
                                 
    delay_us(20000);
   
    adc_update();
   
    v1_1 = READ_V1();
    v2_1 = READ_V2();

    if ( v1_0 > v2_0 )
    {
       if(v1_1==0||v1_0==0)
       {
    	   R_N = R_P = 0;   
       }
       else
       {
           temp1 = v2_1/v1_1;
           temp2 = v2_0/v1_0;
           temp = temp1-temp2;
           R_N = R0*temp;
           R_P = 0xFFFF;
       }
    }
    else
    {
       if(v2_1==0||v2_0==0)
       {
    	   R_N = R_P = 0;
       }
       else
       {
           temp1 = v1_1/v2_1;
           temp2 = v1_0/v2_0;
           temp = temp1-temp2;
           R_P = R0*temp;
           R_N = 0xFFFF;
       }
    }
      mprintf("RP:%lu, PN:%lu\r\n", R_P, R_N);
    return 0;
}


UINT16 get_insulation_res_p ( void )
{
    return (UINT16)R_P;
}

UINT16 get_insulation_res_n ( void )
{
    return (UINT16)R_N;
}

void set_CONTROL_S0( UINT8 state)
{
    switch(state)
    {
     case 0:   
    	 set_gpio_value(CONTROL_S0, 1);
         break;
     case 1:
    	 set_gpio_value(CONTROL_S0, 0);
         break;
     default:
         break;
    }
}

void set_CONTROL_S1(UINT8 state)
{
    switch(state)
    {
     case 0:   
    	 set_gpio_value(CONTROL_S1, 1);
         break;
     case 1:
    	 set_gpio_value(CONTROL_S1, 0);
         break;
     default:
         break;
    }
}


int BMS_GetErrStatusResInsulationByType( UINT8 types) //1:err, 0 OK
{
	return 0; //TODO
}
TYPE_INSRES GetInsulationResValuePositivePole(void)  //ko
{
	return get_insulation_res_p();
}
TYPE_INSRES GetInsulationResValueNegativePole(void) //ko
{
	return get_insulation_res_n();
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
