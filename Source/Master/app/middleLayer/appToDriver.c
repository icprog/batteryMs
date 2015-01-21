/*
 * appToDriver.c
 *
 *  Created on: Dec 27, 2014
 *      Author: mgc
 */
#include "public/public.h"
#include "public/drivers.h"
#include "string.h"
#include "middleLayer/appToDriver.h"
#include "middleLayer/simulateDrivers.h"

//enum dirver_fds
//{
//	FD_DEV_6804,
//	FD_DEV_AD,
//	FD_DEV_GPIO,
//	FD_DEV_USART,
//	FD_DEV_CAN_A,
//	FD_DEV_CAN_C
//};

typedef int (*type_driver_updates)(void);
typedef int (*type_driver_init)(void);
typedef int (*type_driver_ctl)(void);
typedef int (*type_driver_read)(char *buff, int len, int param);
typedef int (*type_driver_write)(const char *buff, int len, int param);
//typedef int (*type_driver_updates)(void);

struct driver_apis
{
	int fd;
	type_driver_init init;
	type_driver_updates update;
	type_driver_ctl ctl;
	type_driver_read read;
	type_driver_write write;
};

#define REGISTER_DRIVER_API( fd,  init_func , update_func,  ctl_func, read_func, write_func )\
		{ fd,  (type_driver_init)init_func , update_func,  ctl_func, read_func, write_func }

static struct driver_apis gDrivers[] =
{
//	FD_AD_BY_INDEX,(type_driver_init)Adc_InitADC0,adc_update,0,0,0
		REGISTER_DRIVER_API(FD_AD_BY_INDEX, Adc_InitADC0, adc_update, 0,0,0),
		REGISTER_DRIVER_API(FD_GPIO_BY_INDEX, gpioFuncsInit, gpioControl, 0,0,0)
};

static const int nDrivers = sizeof(gDrivers) / sizeof(struct driver_apis);

static int findDriverIdByFd(int fd)
{
	int i = 0;
	for (i = 0; i < nDrivers; i++)
	{
		if (gDrivers[i].fd == fd)
		{
			return i;
		}
	}
	return -1;
}
int DriverInitByFd(int fd)
{
	int id = 0;
	assert(fd>=0);
	id = findDriverIdByFd(fd);
	if (id >= 0 && gDrivers[id].init != 0)
	{
		return gDrivers[id].init();
	}
	return -1;
}

int DriverUpdate(int fd)
{
	int id = 0;
	assert(fd>=0);
	id = findDriverIdByFd(fd);
	if (id >= 0 && gDrivers[id].update != 0)
	{
		return gDrivers[id].update();
	}
	return -1;
}

int DriverRead(int fd, void *buff, int len, int param)
{
	UINT8 i = 0;
	assert(buff);
	(void) len;
	switch (fd)
	{
		case FD_VOL_BY_INDEX:
		{
			TYPE_VOLTAGE V = 0;
			if (getSimulateVolEnable() == ENABLE)
			{
				V = getSimulateVolById((UINT8) param & 0xff);
			}
			else
			{
				ltc6804_read_v((UINT8) param & 0xff);

			}
			memcpy((char *) buff, &V, sizeof(TYPE_VOLTAGE));
			return sizeof(TYPE_VOLTAGE);
		}
		case FD_TEMP_BY_INDEX:
		{
			TYPE_TEMP T = 0;
			if (getSimulateTempEnable() == ENABLE)
			{
				T = getSimulateTempById((UINT8) param & 0xff);
			}
			else
			{
				T = ltc6804_read_t((UINT8) (param & 0xff));
			}
			memcpy((char *) buff, &T, sizeof(TYPE_TEMP));
			return sizeof(TYPE_TEMP);
		}
		case FD_AD_BY_INDEX:
		{
			UINT16 mv = 0;
			if (getSimulateAdState() == ENABLE)
			{
				mv = GetSimulateAdmvById((UINT8) (param & 0xff));
			}
			else
			{
				mv = getMvByAdc((UINT8) (param & 0xff));
			}
			memcpy((char *) buff, &mv, sizeof(UINT16));
			return sizeof(UINT16);
		}
		case FD_GPIO_BY_INDEX:
		{
			((char *) buff)[0] = BMS_IO_GET((UINT8) (param & 0xff));
			return 1;
		}
		case FD_USART:
			break;
		default:
			break;
	}
	return -1;
}

int DriverWrite(int fd, const char *buff, int len, int param)
{
	int i = 0;
	assert(buff);
	switch (fd)
	{
		case FD_GPIO_BY_INDEX:
			BMS_IO_SET((UINT8) ((*(UINT8 *) buff) & 0xff),
					(UINT8) (param & 0xff)); // buff:ioid,param: pinstate
			return 0;
		case FD_USART:
			for (i = 0; i < len; i++)
			{
				SCIA_putChar(((const char *) buff)[i]);
			}
			return len;
		case FD_BANLANCE_BY_INDEX:
		{
			ltc6804_set_balance((UINT8) buff[0], (UINT8) buff[1]);
			return 0;
		}
		default:
			break;
	}
	return -1;
}

