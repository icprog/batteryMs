
/************************************************************/
/*             ���໷����:����Ƶ��8M, ϵͳƵ��64M           */

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
	   FMPLL.ESYNCR1.B.CLKCFG=7;//Fsys = (Fref��(MFD+4))��((PREDIV+1)��2^RFD)
	   FMPLL.SYNCR.B.MFD=4;
	   FMPLL.SYNCR.B.RFD=1;
	   FMPLL.SYNCR.B.PREDIV=0;         //�������仰����ϵͳƵ��Ϊ32M, ��Ŀ��Ƶ��(64M)��һ��.
	   while(FMPLL.SYNSR.B.LOCK==0){;} //�ȴ����໷����,������ʾ���ÿ�ʼ��Ч��.
	   FMPLL.SYNCR.B.RFD=0;            //������RFD��ֵ�Ժ�, ϵͳƵ�ʱ�Ϊ��Ŀ��Ƶ��64M.

}
