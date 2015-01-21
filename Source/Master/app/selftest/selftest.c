/*
 * selftest.c
 *
 *  Created on: Jan 4, 2015
 *      Author: mgc
 */
#include "app/public/drivers.h"
#include "app/selftest/selftest.h"
#include "app/middleLayer/appToDriver.h"
#include "app/public/types.h"
#include <string.h>

struct s_selftest
{
	int (*func)(void);
	const char *name;
};

struct s_selfDetec
{
	UINT8 reversed :7;
	UINT8 err_flag :1;
};

struct s_selfDetectionResult
{
	struct s_selfDetec current;
	struct s_selfDetec voltage;
	struct s_selfDetec balance;
	struct s_selfDetec temprature;
	struct s_selfDetec slaves[MAX_ALLOWED_SLAVES];
};

#define REGIST_TEST_FUNC( func, name ) {func, name}

int self_test_eeprom(void);
int self_test_adc(void);
int self_test_ltc68042(void);
int self_test_CAN(void);
int self_detection_current(void);
int self_detection_voltage(void);
int self_detection_temperature(void);
int self_detection_slaves(void);

static const UINT16 EEPROM_ADDR_SELF_TEST = 0x7000;
static const struct s_selftest testfuncs[] =
{
		REGIST_TEST_FUNC(self_test_eeprom, "eeprom"),
		REGIST_TEST_FUNC(self_test_adc, "adc"),
		REGIST_TEST_FUNC(self_test_ltc68042, "ltc6804"),
		REGIST_TEST_FUNC(self_test_CAN, "CAN"),
		REGIST_TEST_FUNC(self_detection_current, "currents"),
		REGIST_TEST_FUNC(self_detection_voltage, "voltage"),
		REGIST_TEST_FUNC(self_detection_temperature, "temprature"),
		REGIST_TEST_FUNC(self_detection_slaves, "slaves")
};
static const int ntestfuncs = sizeof(testfuncs) / sizeof(struct s_selftest);

static UINT8 self_detection_results[SELF_DETECTION_TOTAL] =
{
	0 };

static int bms_setSlefDetResByType(int type, int value)
{
	assert(type < SELF_DETECTION_TOTAL);
	if (value == 0)
		self_detection_results[type] = CHECK_ERR;
	else
		self_detection_results[type] = CHECK_ERR_NONE;
}

/*
 * ret: 0 none err, 1 err!!!
 * */
int bms_getSlefDetResByType(int type)
{
	assert(type < SELF_DETECTION_TOTAL);
	return self_detection_results[type];
}

int bms_getselfdetection_sum(void)
{
	UINT8 i = 0 ;
	for(i=0; i<SELF_DETECTION_TOTAL;i++)
	{
		if(self_detection_results[i] == CHECK_ERR )
			return CHECK_ERR;
	}
	return CHECK_ERR_NONE;
}

int self_test_eeprom(void)
{
	UINT8 data_ori[] =
	{
		0, 1, 2, 3, 4, 5, 6, 7, 8 };
	UINT8 i = 0, data_eeprom = 0;
	int err = CHECK_ERR_NONE;
	WriteEprom(EEPROM_ADDR_SELF_TEST, sizeof(data_ori), data_ori);
	for (i = 0; i < sizeof(data_ori); i++)
	{
		ReadEprom((UINT16) (EEPROM_ADDR_SELF_TEST + i), (UINT16) 1,
				&data_eeprom);
		if (i != data_eeprom)
		{
			mprintf("err eeprom self test!\r\n");
			err = CHECK_ERR;
		}
	}
	mprintf("eeprom self test success!!!");
	bms_setSlefDetResByType(SELF_DETECTION_EEPROM, err);
	return err;
}

int self_test_adc(void)
{
	UINT16 mv = 0, i = 0;
	int ret = 0;
	ret = DriverInitByFd(FD_AD_BY_INDEX);
	ret = DriverUpdate(FD_AD_BY_INDEX);
	for (i = 0; i < ID_ADC_TOTAL; i++)
	{
		DriverRead(FD_AD_BY_INDEX, (char *) &mv, sizeof(UINT16), i);
		if (mv >= 5100)
		{
			return -1;
		}
	}
	return 0;
}

int self_test_ltc68042(void)
{

}

int self_test_CAN(void)
{

}

int self_detection_current(void)
{
	TYPE_CURRENT cur = BMS_GetCurrentValueBUS();
	int err = CHECK_ERR_NONE;
	if (cur > 500 || cur < -500)
	{
		err = CHECK_ERR;
	}
	else
	{
		err = CHECK_ERR;
	}

	bms_setSlefDetResByType(SELF_DETECTION_CURRENT, CHECK_ERR_NONE);
	return err;
}

int self_detection_voltage(void)
{
	TYPE_VOLTAGE vol_min = BMS_GetVoltageValueMin();
	TYPE_VOLTAGE vol_max = BMS_GetVoltageValueMax();
	if (vol_min < 2000 || vol_max > 5000)
	{
		bms_setSlefDetResByType(SELF_DETECTION_VOLTAGE, CHECK_ERR);
	}
	else
	{
		bms_setSlefDetResByType(SELF_DETECTION_VOLTAGE, CHECK_ERR_NONE);

	}

	return bms_getSlefDetResByType(SELF_DETECTION_VOLTAGE);
}

int self_detection_temperature(void)
{
	TYPE_TEMP tmax = BMS_GetTemperaturValueMaxValue();
	TYPE_TEMP tmin = BMS_GetTemperatureValueMinValue();
	int err = CHECK_ERR_NONE;
	if (tmin < -30 )
	{
		err = CHECK_ERR;
	}
	if (tmax > 80)
	{
		err = CHECK_ERR;
	}
	bms_setSlefDetResByType(SELF_DETECTION_TEMPRATURE, err);
	return err;
}

int self_detection_slaves(void)
{

}

int bms_self_detection_update(void)
{
	int ret = 0;
	int i;
	for (i = 0; i < ntestfuncs; i++)
	{
		if (testfuncs[i].func)
		{
			ret = testfuncs[i].func();
			mprintf("test %s", testfuncs[i].name);
			if (ret < 0)
				mprintf("failed!\r\n");
			else
				mprintf("success!!!\r\n");
		}
	}
	return ret;
}

int bms_self_detection_init(void)
{
	memset(self_detection_results, 0, sizeof(self_detection_results));
	return 0;
}

int bms_self_test(void)
{
	int ret = 0;
	int i;
	for (i = 0; i < ntestfuncs; i++)
	{
		if (testfuncs[i].func)
		{
			ret = testfuncs[i].func();
			mprintf("test %s", testfuncs[i].name);
			if (ret < 0)
				mprintf("failed!\r\n");
			else
				mprintf("success!!!\r\n");
		}
	}
	return ret;
}
