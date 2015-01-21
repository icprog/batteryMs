/*
 * appToDriver.h
 *
 *  Created on: Dec 27, 2014
 *      Author: Administrator
 */

#ifndef APPTODRIVER_H_
#define APPTODRIVER_H_

/*
 *read:
 *    devid  read_vol->cell->id
 *dev->6804->paramid1->paramid2->paramid3...? 
*/


enum dirver_fds
{
	FD_VOL_BY_INDEX,
	FD_TEMP_BY_INDEX,
	FD_AD_BY_INDEX,
	FD_GPIO_BY_INDEX,
	FD_BANLANCE_BY_INDEX,
	FD_USART,
	FD_CAN_A,
	FD_CAN_C
};


int DriverInitByFd(int fd);
int DriverUpdate(int fd);
int DriverRead(int fd, void *buff, int len, int param);
int DriverWrite(int fd, const char *buff, int len, int param);
//int DriverCtl(int fd, int cmd, ...);
#endif /* APPTODRIVER_H_ */
