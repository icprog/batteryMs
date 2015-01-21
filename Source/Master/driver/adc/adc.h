#ifndef _ADC_H_
#define _ADC_H_
#include "public/public.h"
#include "public/types.h"

void initADC0(void);
//UINT32 ReadResult(void) ;
void Adc_InitADC0(void);
//UINT32 Adc_AdcToMv( UINT32 adc_result );
//UINT32 Adc_ReadResultMvByChannel(UINT8 channel);
//UINT32 Adc_ReadResultByChannel( UINT8 channel);
int test_Adc_init(void);
int test_Adc(void);
int adc_update(void);
int adc_currentUpdate(void);
UINT16 getMvByAdc(UINT8 channel);
int testAdc2(void);

#endif

