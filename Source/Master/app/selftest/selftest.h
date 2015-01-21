/*
 * selftest.h
 *
 *  Created on: Jan 4, 2015
 *      Author: Administrator
 */

#ifndef SELFTEST_H_
#define SELFTEST_H_


enum selfDetectionType
{
	SELF_DETECTION_CURRENT = 1,
	SELF_DETECTION_VOLTAGE,
	SELF_DETECTION_TEMPRATURE,
	SELF_DETECTION_BALANCE,
	SELF_DETECTION_SLAVE0 = 5,
	SELF_DETECTION_SLAVE1,
	SELF_DETECTION_SLAVE2,
	SELF_DETECTION_SLAVE3,
	SELF_DETECTION_SLAVE4,
	SELF_DETECTION_SLAVE5,
	SELF_DETECTION_SLAVE6,
	SELF_DETECTION_SLAVE7,
	SELF_DETECTION_EEPROM,
	SELF_DETECTION_CAN_VCU,
	SELF_DETECTION_TOTAL
};

int bms_self_test(void);
int bms_self_detection_init(void);
int bms_getSlefDetResByType(int type);
int bms_self_detection_update(void);
int bms_getselfdetection_sum(void);

#endif /* SELFTEST_H_ */
