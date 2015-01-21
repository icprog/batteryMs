/*
 * ParameterManager.c
 *
 *  Created on: Dec 18, 2014
 *      Author: mgc
 */
#include "public/ParameterManager.h"
#include "public/types.h"
#include "public/drivers.h"
Cparameter gParameters =
{
	/****************************gloable varables**************************/
	{
		{
			VALUE_TH_UV_CELL, //UINT16 vol_low;
			VALUE_TH_UV_CELL_LEVEL1, //UINT16 vol_low_level1;
			VALUE_TH_UV_CELL_LEVEL2, //UINT16 vol_low_level2;
			VALUE_TH_OV_CELL, //UINT16 vol_high;
			VALUE_TH_OV_CELL_LEVEL1, //UINT16 vol_high_level1;
			VALUE_TH_OV_CELL_LEVEL2, //UINT16 vol_high_level2;
		},
		{
			VALUE_TH_UV_SUM, //UINT16 vol_low;
			VALUE_TH_UV_SUM_LEVEL1, //UINT16 vol_low_level1;
			VALUE_TH_UV_SUM_LEVEL2, //UINT16 vol_low_level2;
			VALUE_TH_OV_SUM, //UINT16 vol_high;
			VALUE_TH_OV_SUM_LEVEL1, //UINT16 vol_high_level1;
			VALUE_TH_OV_SUM_LEVEL2, //UINT16 vol_high_level2;	
		},
		{

			VALUE_TH_DIFF_VOL_CELL_LEVEL1,
			VALUE_TH_DIFF_VOL_CELL_LEVEL2 } },
	/***************************struct temperature_const_vars**********************/
	{
		{
			VALUE_TH_UT, //UINT16 vol_low;
			VALUE_TH_UT_LEVEL1, //UINT16 vol_low_level1;
			VALUE_TH_UT_LEVEL2, //UINT16 vol_low_level2;
			VALUE_TH_OT, //UINT16 vol_high;
			VALUE_TH_OT_LEVEL1, //UINT16 vol_high_level1;
			VALUE_TH_OT_LEVEL2, //UINT16 vol_high_level2;
		},
		{
			VALUE_TH_DIFF_TEMP_CELL_LEVEL1,
			VALUE_TH_DIFF_TEMP_CELL_LEVEL2 } },
	/***************************struct temperature_const_vars**********************/
	{
		{
			{
				THREATHOLD_VALUE_CAHRGE_OC_LEVEL1,
				THREATHOLD_VALUE_CAHRGE_OC_LEVEL2,
				ADC_PORT_CURRENT_CHARGE },
			{
				THREATHOLD_VALUE_DISCAHRGE_OC_LEVEL1,
				THREATHOLD_VALUE_DISCAHRGE_OC_LEVEL2,
				ADC_PORT_CURRENT_DISCHARGE },
			{
				280,
				275,
				0 },
			{
				THREATHOLD_VALUE_FEEDBACK_OC_LEVEL1,
				THREATHOLD_VALUE_FEEDBACK_OC_LEVEL2,
				ADC_PORT_CURRENT_DISCHARGE }, } },
	/*************************struct dev_info*************************************/
	{
		DEFUALT_COMMBYTE, //ͨ�Ű汾     V1.0					
		DEFUALT_BARTYPE,      //�������   NCM					
		DEFUALT_CHARGE_RATECURRENT,     //�����  ���45A���ŵ�65A��			
		DEFUALT_DISCHARGE_RATECURRENT, //
		DEFUALT_CHARGI, // = ,//������������                180A				
		DEFUALT_RATEVOLTE, // = ,      //���ѹ  288V					
		DEFUALT_SCHGV, // = , //���嶯����������������ѹ  4.2V					
		DEFUALT_ALVOLT,  //�������������ܵ�ѹ  ����������ǲ�õ��ܵ�ѹ					
		DEFUALT_VREQUEST, //,  //��ѹ����        ������鲻֪��				
		DEFUALT_ACHATEV, // = ,//����������ܵ�ѹ          4.2x80=336v				
		DEFUALT_AWEIGHT, // = ,//�������ر��������   Ŀǰ��֪��        								
		DEFUALT_ALTEMP, // = ,//��������¶�         һ�������¶�					
		DEFUALT_ELCSTATE, // = ; //�����������غɵ�״̬  �������SOC		
		DEFUALT_BATTERY_Q, //��ض����			90Ah		
		{
			DEFUALT_TIME_POWEROFF_TO_SLEEP,  //�µ絽�������ߵ�ʱ�� ��λs
			6000,//DEFUALT_TIME_SLEEP_TO_SOBER,  //������ʱ�� ��λs
			DEFUALT_TIME_SOBER_TO_SLEEP, 	//�Ի���ʱ�� ��λs
		},
		{
			DEFUALT_NCLEES_EACH_SLAVE,
			DEFUALT_NTEMPS_EACH_SLAVE,
			DEFUALT_N_SLAVES,
			DEFUALT_MAX_CELLS, },
		{
			10.1000f,
			20.2000f,
			30.3000f,
			40.4000f,
			50.5000f,
			60.6000f,
			70.7000f,
			80.8000f,
			90.9000f } }

};

int setParamVoltage(UINT8 id, TYPE_VOLTAGE parameter);
int setParamTemperature(UINT8 id, TYPE_VOLTAGE parameter);
/*
 * in: 
 * 	@addrOffset  start from zero
 * 	@parameters  the parameters which should be store to eeprom
 * 	@len the length of parameters
 * 	
 *out:
 *	>=0 success
 *	<0 failed
 * */
static int storeParameterByAddr(UINT8 addrOffset, UINT8 *paramters, int len)
{
	(void)addrOffset;
	(void)paramters;
	(void)len;
	return 0; //TODO
}

/*
 * out:
 * 	>=0 success
 * 	<0 failed
 * */
int storeAllParameters(void)
{
	WriteEprom(0, sizeof(gParameters), (UINT8 *) &gParameters);
	return 0; //TODO
}

int storeAllParametersByTimer(UINT32 timemsNow, UINT32 timeTH)
{
	static UINT32 timems = 0;
	if (TIME_TRHESHOLD_CAL(timemsNow, timems) > timeTH)
	{
		timemsNow = timems;
		storeAllParameters();
		return 1;
	}
	return 0;
}

int eepromParametersInit(void)
{
	UINT8 blank = 0;
	static UINT8 err_count = 0;
	start: ReadEprom(0, sizeof(blank), &blank);
	if (blank != 0xa5)
	{
		mprintf("empty eeprom found, writting default parameters!!!\r\n");
		blank = 0xa5;
		WriteEprom(1, sizeof(gParameters), (UINT8 *) &gParameters);
		WriteEprom(0, sizeof(blank), &blank);
		if (err_count++ > 10)
		{
			mprintf("sorry,eeprom error!!\r\n");
			return -1; //eeprom err!!!
		}
		goto start;
	}
	ReadEprom(1, sizeof(gParameters), (UINT8 *) &gParameters);
	return 0; 
}

static int readAlleepromParameters(void)
{
	ReadEprom(1, sizeof(gParameters), (UINT8 *) &gParameters);
	return 0;
}

/*
 * in:
 * 	@ids: the id of parameter that defined in ParameterManager.h
 * 	@parameter: the value of the parameter.
 * */
int setParamById(UINT8 id, DF64 parameter)
{
	if (id <= PARAM_ID_DIFF_VOL_CELL_LEVEL2 && id >= PARAM_ID_OV_CELL)
	{
		return setParamVoltage(id, (TYPE_VOLTAGE) parameter);
	}

	else if (id >= PARAM_ID_UT && id <= PARAM_ID_TEMP_HEAT_CLOSE)
	{
		return setParamTemperature(id, (TYPE_TEMP) parameter);
	}

	return 0;
}

/*
 * type: 0 read, 1 write
 * */
static INT64 opParamVoltage(UINT8 id, TYPE_VOLTAGE parameter, UINT8 type)
{
	struct voltage_const_vars *pvoltage = &gParameters.e_vol;
	INT64 ret = 0;
	if (!type)
	{
		readAlleepromParameters();
	}
	switch (id)
	{
		case PARAM_ID_OV_CELL:
			if (type)
				pvoltage->cell_vol_value.vol_high = (TYPE_VOLTAGE) parameter;
			ret = pvoltage->cell_vol_value.vol_high;
			break;
		case PARAM_ID_OV_CELL_LEVEL1: //, //= 4300; //the warning lever 1 threshold value of cell over voltage 
			if (type)
				pvoltage->cell_vol_value.vol_high_level1 =
						(TYPE_VOLTAGE) parameter;
			ret = pvoltage->cell_vol_value.vol_high_level1;
			break;
		case PARAM_ID_OV_CELL_LEVEL2: // , //= 4250; //the  warning lever 2threshold value of cell over voltage 
			if (type)
				pvoltage->cell_vol_value.vol_high_level2 =
						(TYPE_VOLTAGE) parameter;
			ret = pvoltage->cell_vol_value.vol_high_level2;
			break;
		case PARAM_ID_UV_CELL: //, // = 2800, //; //the warning threshold value of cell under voltage
			if (type)
				pvoltage->cell_vol_value.vol_low = (TYPE_VOLTAGE) parameter;
			ret = pvoltage->cell_vol_value.vol_low;
			break;
		case PARAM_ID_UV_CELL_LEVEL1: //, // = 2700; //the warning lever 1 threshold value of cell under voltage
			if (type)
				pvoltage->cell_vol_value.vol_low_level1 =
						(TYPE_VOLTAGE) parameter;
			ret = pvoltage->cell_vol_value.vol_low_level1;
			break;
		case PARAM_ID_UV_CELL_LEVEL2: //, //= 2750; //the warning lever 2 threshold value of cell under voltage
			if (type)
				pvoltage->cell_vol_value.vol_low_level2 =
						(TYPE_VOLTAGE) parameter;
			ret = pvoltage->cell_vol_value.vol_low_level2;
			break;

		case PARAM_ID_OV_SUM: // , //= 336000; //UINT16 vol_low;
			if (type)
				pvoltage->sum_vol_value.vol_high = (TYPE_VOLTAGE) parameter;
			ret = pvoltage->sum_vol_value.vol_high;
			break;
		case PARAM_ID_OV_SUM_LEVEL1: //, // = 344000; //UINT16 vol_low_level1;
			if (type)
				pvoltage->sum_vol_value.vol_high_level1 =
						(TYPE_VOLTAGE) parameter;
			ret = pvoltage->sum_vol_value.vol_high_level1;
			break;
		case PARAM_ID_OV_SUM_LEVEL2: //, // = 340000; //UINT16 vol_low_level2;
			if (type)
				pvoltage->sum_vol_value.vol_high_level2 =
						(TYPE_VOLTAGE) parameter;
			ret = pvoltage->sum_vol_value.vol_high_level2;
			break;
		case PARAM_ID_UV_SUM: //, // = 224000;//87000; //UINT16 vol_high;
			if (type)
				pvoltage->sum_vol_value.vol_low = (TYPE_VOLTAGE) parameter;
			ret = pvoltage->sum_vol_value.vol_low;
			break;
		case PARAM_ID_UV_SUM_LEVEL1: //, // =216000;// 84000; //UINT16 vol_high_level1;
			if (type)
				pvoltage->sum_vol_value.vol_low_level1 =
						(TYPE_VOLTAGE) parameter;
			ret = pvoltage->sum_vol_value.vol_low_level1;
			break;
		case PARAM_ID_UV_SUM_LEVEL2: //, // = 220000;//86000; //UINT16 vol_high_level2;
			if (type)
				pvoltage->sum_vol_value.vol_low_level2 =
						(TYPE_VOLTAGE) parameter;
			ret = pvoltage->sum_vol_value.vol_low_level2;
			break;

		case PARAM_ID_DIFF_VOL_CELL_LEVEL1: // , //= 380;
			if (type)
				pvoltage->cell_diff_value.cell_diff_danger1 =
						(TYPE_VOLTAGE) parameter;
			ret = pvoltage->cell_diff_value.cell_diff_danger1;
			break;
		case PARAM_ID_DIFF_VOL_CELL_LEVEL2: //, //= 300;
			if (type)
				pvoltage->cell_diff_value.cell_diff_danger2 =
						(TYPE_VOLTAGE) parameter;
			ret = pvoltage->cell_diff_value.cell_diff_danger2;
			break;
		default:
			break;
	}
	if (type)
	{
		storeAllParameters();
	}
	return ret; //
}
/*
 * in:
 * 	@ids: the id of parameter that defined in ParameterManager.h
 * 	@parameter: the value of the parameter.
 * */
DF64 getParamById(UINT8 ids)
{
	(void)ids;
//	return opParamVoltage(ids, parameter, 0);
	return 0;
}

int setParamVoltage(UINT8 id, TYPE_VOLTAGE parameter)
{
	opParamVoltage(id, parameter, 1);
	return 0;
}

int setParamTemperature(UINT8 id, TYPE_VOLTAGE parameter)
{
	(void)id;
	(void)parameter;
	return 0;
}

