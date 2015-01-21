/*
 * can.h
 *
 *  Created on: Nov 12, 2014
 *      Author: Administrator
 */

#ifndef CAN_H_
#define CAN_H_
#include "protocol/cpcu_protocol.h"



struct VCU_MASGDATA
 {
	UINT32 id;
    UINT8 datalen;
    UINT8 data[8];
 };


void ISR_CAN_A_TX0(void); 
void ISR_CAN_A_TX1(void);
void ISR_CAN_A_TX2(void); 
void ISR_CAN_A_TX3(void);
void ISR_CAN_A_TX(void);
void ISR_CAN_A_RX(void) ;
void initCAN_A(void);
void  CAN_A_RM(void) ;  
void CAN_A_TX( struct VCU_MASGDATA msg);
void ISR_CAN_C_RX(void);
void ISR_CAN_C_TX0(void);
void ISR_CAN_C_TX1(void);
void initCAN_C(void);
void CAN_C_TX(struct T_SMSGDATA  msg);
void CAN_test(void);
void initCAN();
 void CAN_C_RM() ;
#endif /* CAN_H_ */
