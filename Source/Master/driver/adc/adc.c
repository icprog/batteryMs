/************************************************************/
/*                     初始化ADC转换                        */
/************************************************************/
#include "MPC5634M.h"
#include "public/types.h"
#include "driver/Gpio/gpio.h"
#include "driver/Gpio/port_defs.h"
#include "driver/adc/adc.h"
#include "driver/timer/timer.h"

struct s_adcValues
{
	UINT16 adcmv;
	UINT16 adcValue;
};
static struct s_adcValues gAdcValues[ID_ADC_TOTAL];

static void initADC0(void)
{
    EQADC.CFPR[0].R=0x80801001;            /* Send CFIFO 0 a ADC0 configuration command 
                                            enable ADC0 & sets prescaler= divide by 2 */    
    EQADC.CFCR[0].R=0x0410;                // Trigger CFIFO 0 using Single Scan SW
 	while(EQADC.FISR[0].B.EOQF!=1) {}      // Wait for End Of Queue flag
    EQADC.FISR[0].B.EOQF=1;        	       // Clear End Of Queue flag
}

void Adc_InitADC0(void)
{	
	UINT32 command = 0x80801001;
//	assert( channel<39);s
	//command |= (channel<<16);
    EQADC.CFPR[0].R=command;            /* Send CFIFO 0 a ADC0 configuration command 
                                            enable ADC0 & sets prescaler= divide by 2 */    
    EQADC.CFCR[0].R=0x0410;                // Trigger CFIFO 0 using Single Scan SW
 	while(EQADC.FISR[0].B.EOQF!=1) {}      // Wait for End Of Queue flag
    EQADC.FISR[0].B.EOQF=1;        	       // Clear End Of Queue flag
}

/************************************************************/
/*                       ADC转换命令        转换 0~39               */
/************************************************************/
static void Adc_ConvertByChannel(UINT8 channel)
{
	UINT32 command = 0x80000000;
	assert( channel<39);
	command |= (channel<<8);
    EQADC.CFPR[0].R = command;  /* Conversion command: convert channel 0 */
                                   /* with ADC0, set EOQ, and send result to RFIFO 0 */                           
    EQADC.CFCR[0].R = 0x0410;     /* Trigger CFIFO 0 using Single Scan SW mode */  	
}

static UINT16 Adc_ReadResult(void)
{
	UINT32 Result = 0;
//	UINT32 ResultInmv = 0;
	while (EQADC.FISR[0].B.RFDF != 1){}      /* Wait for RFIFO 0's Drain Flag to set */
	  Result = EQADC.RFPR[0].R;                /* ADC result */ 
//	  ResultInmv = (uint32_t)((5000*Result)/0x3FFC);  /* ADC result in millivolts */        
	  EQADC.FISR[0].B.RFDF = 1;                /* Clear RFIFO 0's Drain Flag */
	  EQADC.FISR[0].B.EOQF = 1;                /* Clear CFIFO's End of Queue flag */
	  return Result;
}

static UINT16 Adc_AdcToMv( UINT16 adc_result )
{

//	static const UINT32 a0 = 100;
	UINT16 result_mv = 0;
//	static UINT16 const adcFull = 4096;
//	static UINT16 const mvfull = 5000;
//	static UINT16 const adcZero = 0;
//	static UINT16 const mvZero = 0;
//	const float k = ((float)(mvfull-mvZero))/((float)(adcFull-adcZero));
////	const float k = 1.0;
//	result_mv = (UINT16)(((float)(adc_result)) * k + a0);
	result_mv = (UINT16)((((float)adc_result)*0.00031-0.065)*1000);
//	result_mv =  (uint32_t)((5000*adc_result)/0x3FFC);
	return result_mv;
}


static UINT16 Adc_ReadResultByChannel( UINT8 channel)
{
	Adc_ConvertByChannel(channel);
	return Adc_ReadResult();
}

UINT16 getMvByAdc(UINT8 channel)
{
	assert(channel<ID_ADC_TOTAL);
	return gAdcValues[channel].adcmv;
}





static UINT32 Adc_ReadResultMvByChannel(UINT8 channel)
{
	UINT16 result_mv = 0;
//	Adc_ConvertByChannel(channel);
	result_mv = Adc_ReadResultByChannel(channel);
	gAdcValues[channel].adcValue = result_mv;
	result_mv = Adc_AdcToMv(result_mv);// (uint32_t)((4950*result_mv)/0x3FFC);
	return result_mv;
}

/************************************************************/
/*                       ADC转换命令                        */
/************************************************************/
static void SendConvCmd(void)
{
    
    EQADC.CFPR[0].R = 0x80000000;  /* Conversion command: convert channel 0 */
                                   /* with ADC0, set EOQ, and send result to RFIFO 0 */                           
    EQADC.CFCR[0].R = 0x0410;     /* Trigger CFIFO 0 using Single Scan SW mode */  	
}


/************************************************************/
/*                       ADC读取结果                       */
/************************************************************/
//uint32_t ReadResult(void) 
//{  
//  uint32_t Result = 0;
//  uint32_t ResultInmv = 0;
//  SendConvCmd();                /* Send one conversion command */
//  while (EQADC.FISR[0].B.RFDF != 1){}      /* Wait for RFIFO 0's Drain Flag to set */
//  Result = EQADC.RFPR[0].R;                /* ADC result */ 
//  ResultInmv = (uint32_t)((5000*Result)/0x3FFC);  /* ADC result in millivolts */        
//  EQADC.FISR[0].B.RFDF = 1;                /* Clear RFIFO 0's Drain Flag */
//  EQADC.FISR[0].B.EOQF = 1;                /* Clear CFIFO's End of Queue flag */
//  return Result;
//}

int adc_currentUpdate(void)
{
	return 0;
}
#define  AAADC_FILTER_TIMES 100
static UINT16 adct[ID_ADC_TOTAL][AAADC_FILTER_TIMES];
static UINT16 adcv[ID_ADC_TOTAL][AAADC_FILTER_TIMES];
int adc_update(void)
{
	UINT16 i = 0,j=0,count=0;
	UINT32 sum = 0;
	UINT32 sum2 = 0;
	UINT16 result_mv = 0;
	for(i=0;i<ID_ADC_TOTAL;i++)
	{
		for(j=0;j<AAADC_FILTER_TIMES;j++)
		{
				result_mv = Adc_ReadResultByChannel(i);
				adcv[i][j] = result_mv;
				adct[i][j]  = Adc_AdcToMv(result_mv);// (uint32_t)((4950*result_mv)/0x3FFC);
				
			  //Adc_ReadResultMvByChannel(i);
		}
		sum = 0;
		sum2 = 0;
		for(j=0;j<AAADC_FILTER_TIMES;j++)
		{
				sum+=adct[i][j];
				sum2+=adcv[i][j];
		}
		gAdcValues[i].adcValue = 		 (UINT16)(sum2/(AAADC_FILTER_TIMES));
		gAdcValues[i].adcmv = (UINT16)(sum/(AAADC_FILTER_TIMES));//Adc_ReadResultMvByChannel(i);
	}
	return 0; //TODO
}

int test_Adc_init(void)
{
	Adc_InitADC0();
}

static int printfAdcs(void)
		{
			UINT8 i = 0;
			for(i=0; i<ID_ADC_TOTAL;i++)
			{
				mprintf("[ad%d:%d]", i, getMvByAdc(i));
			}
			mprintf("\r\n");
			return 0;
		}

#include "driver/timer/timer.h"
int testAdc2(void)
{
	UINT8 i =0;
	UINT32 t,t2 = getSystemTimeMs();
	test_Adc_init();
	while(1)
	{
		t = getSystemTimeMs();
		adc_update();
		t2 = getSystemTimeMs();
		t = TIME_TRHESHOLD_CAL(t,t2);
		mprintf("adc update take %d ms\r\n", t);
		printfAdcs();
	}
}

int test_Adc(void)
{
	static const UINT8 MAX_ADS = 6;
	static UINT8 i = 0;
	UINT32 t,t2 = getSystemTimeMs();
//	static UINT32 adcresult[MAX_ADS];
//	static UINT32 adcresult_mv[MAX_ADS];
	Adc_InitADC0();
	while(1)
	{
		t = getSystemTimeMs();
		adc_update();
		t2 = getSystemTimeMs();
		t = TIME_TRHESHOLD_CAL(t,t2);
		for( i=0; i<ID_ADC_TOTAL;i++)
		{
			mprintf("ad[%d:%05umv(%d)] ",i, gAdcValues[i].adcmv, gAdcValues[i].adcValue);
		}
		mprintf("\r\nadc update take %d ms\r\n", t);
	}
	return 0;
}

