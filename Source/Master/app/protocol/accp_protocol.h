/*
 * accp_protocol.h
 *
 *  Created on: Nov 11, 2014
 *      Author: mgc
 */

#ifndef ACCP_PROTOCOL_H_
#define ACCP_PROTOCOL_H_
#include "public/types.h"
/*
 * types
 * */
struct charger_ac_info
{
	UINT32 chargeOutVoltage;
	//TODO
};

struct Accp_state
{
	UINT16 outputvol;
	UINT16 outputcur;
	UINT8 accpstate;
	UINT8 canconnect;
	UINT8 inputvolprotect;
	UINT8 tempprotect;
	UINT8 hardwareerro;
	UINT8 Directovervolprotect;
	UINT8 Directundervolprotect;
	UINT8 Directovercurprotect;
	UINT8 Directovervolwarn;
	UINT8 Directundervolwarn;
	UINT8 Directovercurwarn;
	UINT8 Directshort;
	UINT8 Directreverse;
	UINT8 Overvoldataerro;
	UINT8 Undervoldataerro;
	UINT8 Overcurdataerro;	
} ;

static struct Accp_state ACCP_STATE;
/*
 * out interfaces
 * */
int getChargerAcInfor( struct charger_ac_info *info);
int OpenAcCharge(void); // ask charger to charge


/*
 * in interfaces
 * */
UINT32 getThOVProtectValue(void);
UINT32 getThUVProtectValue(void);
UINT32 getThOVWarningValue(void);
UINT32 getThUVWarningValue(void);
UINT32 getThOCWarningValue(void);
UINT32 getThUCWarningValue(void);

UINT32 getMaxAllowedChargeVoltage(void);
UINT32 getMaxAllowedChargeCurrent(void);

int ACCP_communication(void);

#endif /* ACCP_PROTOCOL_H_ */


