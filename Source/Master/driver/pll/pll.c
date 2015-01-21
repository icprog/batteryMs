
/************************************************************/
/*             锁相环程序:晶振频率8M, 系统频率64M           */

/************************************************************/

#include "MPC5634M.h"
#include "driver/pll/pll.h"
void Config_PLL(void)
{
//	  SIU.ECCR.B.EBDF = 3;                /* Divide sysclk by 3+1 for CLKOUT */
//	  FMPLL.ESYNCR1.B.CLKCFG = 0X7;       /* Change clk to PLL normal mode from crystal */  
//	  FMPLL.SYNCR.R = 0x16080000;         /* Initial values: PREDIV=1, MFD=12, RFD=1 */
//	  while (FMPLL.SYNSR.B.LOCK != 1) {}; /* Wait for FMPLL to LOCK  */
//	  FMPLL.SYNCR.R = 0x16000000;         /* Final value for 64 MHz: RFD=0 */
	   FMPLL.ESYNCR1.B.CLKCFG=7;//Fsys = (Fref×(MFD+4))／((PREDIV+1)×2^RFD)
	   FMPLL.SYNCR.B.MFD=4;
	   FMPLL.SYNCR.B.RFD=1;
	   FMPLL.SYNCR.B.PREDIV=0;         //上面三句话配置系统频率为32M, 即目的频率(64M)的一半.
	   while(FMPLL.SYNSR.B.LOCK==0){;} //等待锁相环上锁,上锁表示配置开始生效了.
	   FMPLL.SYNCR.B.RFD=0;            //更改了RFD的值以后, 系统频率变为了目的频率64M.

}
