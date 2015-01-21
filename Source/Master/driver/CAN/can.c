
#include "MPC5634M.h"
#include "IntcInterrupts.h"
#include "public/types.h"
#include "driver/can/can.h"
#include "protocol/cpcu_protocol.h"
#include "protocol/vcu_protocol.h"
//#include "protocol/vcu_protocol.c"
UINT8 CANCBufferNumber = 0;
UINT8 CANABufferNumber = 0;
UINT32 SAE_ID;
INT32 j;
UINT8 dummyA,dummyC;
#define RECVSAVENUM 2    //接收堆栈存储数据量大小

UINT32 RxCODE_A,RxCODE_C;               /* Received message buffer code */
UINT32 RxID_A,RxID_C;                 /* Received message ID */
UINT32 RxLENGTH_A,RxLENGTH_C;             /* Recieved message number of data bytes */
UINT8  RxDATA_A[8],RxDATA_C[8];            /* Received message data string*/
UINT32 RxTIMESTAMP_A,RxTIMESTAMP_C;          /* Received message time */    

struct T_RMSGDATA g_pERevMsg[RECVSAVENUM];//充电桩信息存储
struct VCU_MASGDATA g_pCRevMsg[RECVSAVENUM];//电池管理系统信息存储
UINT8 g_iERp = 0; //读充电桩信息内存地址
UINT8 g_iEWp = 0; //写充电桩信息内存地址
UINT8 g_iCRp = 0; //读管理系统信息内存地址
UINT8 g_iCWp = 0; //写管理系统信息内存地址

 
 void CAN_A_RM(void)        //CAN0 的接收
 {  
	  	 //UINT32 dummy;
       struct VCU_MASGDATA msg;
	   struct canPack gCP1;
//	   extern  void vcu_can_isr(struct canPack ppack);
//	   for(i=0;i<4;j++)
//	   {
//	    msg.id[i] = CAN_C.BUF[4].ID.C[i];
//	   } 
	  msg.id =  CAN_A.BUF[5].ID.R;
	  for (j=0; j<CAN_A.BUF[5].CS.B.LENGTH; j++)
	   {	 
		 msg.data[j] = CAN_A.BUF[5].DATA.B[j];
	   }
	   CAN_A.BUF[5].CS.B.CODE = 4;
	   gCP1.id =  msg.id ;
	   gCP1.data = msg.data;
	   vcu_can_isr(&gCP1);
}

 
void ISR_CAN_A_TX0(void)  
{
	CAN_A.IFRL.R = 0x00000001; 
}
void ISR_CAN_A_TX1(void)  
{
	CAN_A.IFRL.R = 0x00000002; 
}
void ISR_CAN_A_TX2(void)  
{
	CAN_A.IFRL.R = 0x00000004; 
}
void ISR_CAN_A_TX3(void)  
{
	CAN_A.IFRL.R = 0x00000008; 
}
void ISR_CAN_A_RX(void)     
{
	//if(CAN_A_RM())
	CAN_A_RM();
    CAN_A.IFRL.R = 0x00000020;
}	
void initCAN_A()
{
	UINT8 i;
    CAN_A.MCR.R=0x5000003F;         //使能FlexCAN，启用所有64个消息缓冲区
    //CAN_A.CR.R = 0x04DB0006;        //Configure for 8MHz OSC, 100KHz bit time  
    CAN_A.CR.R = 0x01DB0006;          //250KHZ;
    for(i=0;i<32;i++)
    {
        CAN_A.BUF[i].CS.B.CODE = 0; //所有消息缓冲区为非活动态	
    }
  /* for(i=0;i<4;i++)
    {
    CAN_A.BUF[i].CS.B.CODE= 8;	
    }
    *
    //CAN_A.BUF[i].CS.B.IDE = 0;
    CAN_A.BUF[0].CS.B.CODE= 8;
    //CAN_A.BUF[4].CS.B.IDE = 0;      /* MB 4 will look for a standard ID */
    CAN_A.BUF[5].CS.B.IDE = 1;      /* MB 4 will look for a standard ID */
    CAN_A.BUF[5].CS.B.CODE= 4;       //设置缓冲通道0发送脚为非活动态
    SIU.PCR[83].R = 0x062C;         //配置CAN_A为发送脚， open drain
    SIU.PCR[84].R = 0x0500;         //配置CAN_A为接收
    CAN_A.MCR.R = 0x0000003F;       /* Negate FlexCAN A halt state for 64 MB */   
    //CAN_A.RXGMASK.R=0x1FFFFFFF;
    CAN_A.RXGMASK.R=0x00000000;
    INTC_InstallINTCInterruptHandler(ISR_CAN_A_RX, 160, 1);
    INTC_InstallINTCInterruptHandler(ISR_CAN_A_TX0, 155, 2);
    INTC_InstallINTCInterruptHandler(ISR_CAN_A_TX1, 156, 2);
    INTC_InstallINTCInterruptHandler(ISR_CAN_A_TX2, 157, 2);
    INTC_InstallINTCInterruptHandler(ISR_CAN_A_TX3, 158, 2);
    INTC.CPR.B.PRI = 0;
   // CAN_A.IMRL.B.BUF04M = 1;
    
     CAN_A.IMRL.B.BUF05M = 1;        // Enable MB5 interrupt; 
     CAN_A.IMRL.B.BUF00M = 1; 
     CAN_A.IMRL.B.BUF01M = 1;  
     CAN_A.IMRL.B.BUF02M = 1;  
     CAN_A.IMRL.B.BUF03M = 1;  
    while(CAN_A.MCR.B.NOTRDY==1){}    //等待CAN_A准备好
}




void CAN_A_TX(struct VCU_MASGDATA msg)
{
	UINT8  j;
	if (CANABufferNumber == 4)
	// CAN_A.IFRL.R = 0x00000001; 
	CANABufferNumber = 0; 
	// send_buf = 0;
	//     //用MB0-MB3发送数据!
     if(!(CAN_A.BUF[CANABufferNumber].CS.B.CODE==8))
    CAN_A.BUF[CANABufferNumber].CS.B.CODE = 8;
     
	CAN_A.BUF[CANABufferNumber].CS.B.IDE = 1; /* Use standard ID length */
	//PDU_ID2CAN0_ID(ID); //传送ID  	CAN_A.BUF[CAN0BufferNumber].ID.B.EXT_ID =ID;
	//CAN_A.BUF[CANABufferNumber].ID.B.PRIO = 6;
//	for(i=0;i<4;i++){
//		// mprintf("msgdat :%d\r\n",msg.id[i]);
//		
//	CAN_A.BUF[CANABufferNumber].ID.C[i] = msg.id[i];
//	}
	CAN_A.BUF[CANABufferNumber].ID.R = msg.id;
	//CAN_A.BUF[CANABufferNumber].ID.R =ID;
	CAN_A.BUF[CANABufferNumber].CS.B.RTR =	0;
	/* Data frame, not remote Tx request frame */
	CAN_A.BUF[CANABufferNumber].CS.B.LENGTH = msg.datalen;
	/* # bytes to transmit w/o null */
	for (j = 0; j < msg.datalen ; j++)
	{
		CAN_A.BUF[CANABufferNumber].DATA.B[j] =msg.data[j]; /* Data to be transmitted */
		//j = j+4;
	}
	CAN_A.BUF[CANABufferNumber].CS.B.SRR = 1; /* Tx frame (not req'd for standard frame)*/
	CAN_A.BUF[CANABufferNumber].CS.B.CODE = 0xC; /* Activate msg. buf. to transmit data frame */
	CANABufferNumber++;
}





void CAN_C_RM()      //CAN0 的接收
{
	 UINT8	 i, j = 0;
		 //UINT32 dummy;
		 struct T_RMSGDATA msg;
		//  while (CAN_A.IFRL.B.BUF04I == 0) {};    /* MPC555x: Wait for CAN C MB 4 flag */
		 // RxCODE_A   = CAN_A.BUF[5].CS.B.CODE;      /* Read CODE, ID, LENGTH, DATA, TIMESTAMP */
	     for(i=0;i<4;i++){
	      msg.id[i] = CAN_C.BUF[4].ID.C[i];}
	     
	     for (j=0; j<CAN_C.BUF[4].CS.B.LENGTH; j++)
	     {	 
	      msg.data[j] = CAN_C.BUF[4].DATA.B[j];
	     }
		  CAN_C.BUF[4].CS.B.CODE = 4;
		 // return TRUE;
		  //判断数据是否已写到末尾
		 if(g_iEWp>=RECVSAVENUM)
	       g_iEWp = 0;
		    /*for(sp2=0;sp2<4;sp2++)
		    {
		       if(g_pCRevMsg[g_iCWp].id[sp2]!=0)
		           return(FALSE); 
		    }*/
		  g_pERevMsg[g_iEWp] = msg;
		  g_iEWp++; 
		  
}


void ISR_CAN_C_TX0(){
	 CAN_C.IFRL.R = 0x00000002; 
}

void ISR_CAN_C_TX1(){
	 CAN_C.IFRL.R = 0x00000001; 
}
void ISR_CAN_C_TX2(){
	 CAN_C.IFRL.R = 0x00000004; 
}
void ISR_CAN_C_RX(void)     
{
	//if(CAN_A_RM())
	CAN_C_RM();
    CAN_C.IFRL.R = 0x00000010;
}	
/************************************************************/
/*                    初始化CAN_C：接收                     */
/************************************************************/
void initCAN_C(void)
{
	UINT8 i;
	CAN_C.MCR.R=0x5000001F;         //使能FlexCAN，启用所有64个消息缓冲区
    //CAN_C.CR.R=0x04DB0006;          //Configure for 8MHz OSC, 100KHz bit time
	CAN_C.CR.R=0x01DB0006;             //250khz;
    for(i=0;i<32;i++)
    {
    	CAN_C.BUF[i].CS.B.CODE=0;   //所有消息缓冲区为非活动态

    }
//    for(i=0;i<4;i++)
//    CAN_C.BUF[i].CS.B.CODE= 8;  
    CAN_C.BUF[4].CS.B.IDE=1;        //缓冲通道4将寻找一个标准的ID
    CAN_C.BUF[4].CS.B.CODE=4;
    //CAN_C.RXGMASK.R=0x1FFFFFFF;     //Global acceptance mask
    SIU.PCR[87].R=0x0E2C;           //配置CAN_C为发送脚， open drain
    SIU.PCR[88].R=0x0D00;  
    //配置CAN_C为接收脚
    CAN_C.MCR.R=0x0000003F;         //Negate FlexCAN C halt state for  64 MB
    CAN_C.RXGMASK.R=0x00000000;
    INTC_InstallINTCInterruptHandler(ISR_CAN_C_RX, 180, 2);
    INTC_InstallINTCInterruptHandler(ISR_CAN_C_TX0, 177, 1);
    INTC_InstallINTCInterruptHandler(ISR_CAN_C_TX1, 176, 1);
    INTC_InstallINTCInterruptHandler(ISR_CAN_C_TX2, 178, 1);
    INTC.CPR.B.PRI = 0;
    CAN_C.IMRL.B.BUF04M = 1;
    CAN_C.IMRL.B.BUF00M = 1;
    CAN_C.IMRL.B.BUF01M = 1;
    CAN_C.IMRL.B.BUF02M = 1;
    while(CAN_C.MCR.B.NOTRDY==1){}    //等待CAN_A准备好
}

void CAN_C_TX(struct T_SMSGDATA msg)
{
	UINT8 i;
	if (CANCBufferNumber == 3)	
		//while( CAN_C.IFRL.B.BUF00I)
	CANCBufferNumber = 0;
	// CANCBufferNumber = 1 ;     //用MB0-MB3发送数据
	//if((CAN_C.BUF[CANCBufferNumber].CS.B.CODE == 12))
	//CAN_C.BUF[CANCBufferNumber].CS.B.CODE = 8;
	CAN_C.BUF[CANCBufferNumber].CS.B.IDE = 1; /* Use standard ID length */
	/*******************************这个PDU_ID2CAN0_ID()到底是干什么用的**0****************************/
	//PDU_ID2CAN0_ID(ID); //传送ID  	CAN_A.BUF[CAN0BufferNumber].ID.B.EXT_ID =ID;
	for(i=0;i<4;i++){
	CAN_C.BUF[CANCBufferNumber].ID.C[i] = msg.id[i];}
	//CAN_C.BUF[CANCBufferNumber].ID.R = msg.id;
	/*******************************这个PDU_ID2CAN0_ID()到底是干什么用的******************************/
	CAN_C.BUF[CANCBufferNumber].CS.B.RTR =	0;
	/* Data frame, not remote Tx request frame */
	CAN_C.BUF[CANCBufferNumber].CS.B.LENGTH = msg.len;
	/* # bytes to transmit w/o null */
		
	for (i = 0; i < msg.len; i++)
	{
		CAN_C.BUF[CANCBufferNumber].DATA.B[i] = msg.data[i]; /* Data to be transmitted */
	}
	CAN_C.BUF[CANCBufferNumber].CS.B.SRR = 1; /* Tx frame (not req'd for standard frame)*/
	CAN_C.BUF[CANCBufferNumber].CS.B.CODE = 0xC; /* Activate msg. buf. to transmit data frame */
	CANCBufferNumber++;
}


UINT8  MSCAN0GetMsg(struct T_ID* msgid,UINT8* data)    //BOOL
{
  static struct T_RMSGDATA receivemsg ;
  //UINT8 pa = &msgdata;
  UINT16 i = 0;
    //判断当前的读取位置是否已处于最大位置
  if(g_iERp>=RECVSAVENUM)
      g_iERp = 0;
  //判断当前要读取位置保存的消息id是否为0，为0则说明没有消息  
  if((g_pERevMsg[g_iERp].id[0]==0x00)&&(g_pERevMsg[g_iERp].id[1]==0x00)&&(g_pERevMsg[g_iERp].id[2]==0x00)&&(g_pERevMsg[g_iERp].id[3]==0x00))
        return FALSE;   
  //将要解析的消息放到传入参数中  
  receivemsg = g_pERevMsg[g_iERp];
  //清消息id
  for(i=0;i<4;i++) {
     g_pERevMsg[g_iERp].id[i] = 0;
  }
  (void)UnpackMsgid(receivemsg,msgid);
 // SelectUnpackFun(*msgid,msg,msgdata);
  for(i=0;i<8;i++) {
	 // mprintf("msgdata :%d\r\n",msgdata[i]);
      data[i] = g_pERevMsg[g_iERp].data[i]; 
    //  mprintf("msgdat :%d\r\n",msgdata[i]);
    }
  g_iERp++;
  return TRUE;
}
/*************************************************************/
/*                      获取控制系统的消息内容                          */
///*************************************************************/
//UINT8 MSCAN1GetMsg(struct T_ID* msgid,char* msgdata)    //BOOL
//{
//  struct VCU_MASGDATA msg;
//  extern struct canPack gCp1;
//  UINT16 i = 0;
// // struct T_SINGV *ag = (struct T_SINGV *)msgdata;
//    //判断当前的读取位置是否已处于最大位置
//  if(g_iCRp>=RECVSAVENUM)
//      g_iCRp = 0;
//  //判断当前要读取位置保存的消息id是否为0，为0则说明没有消息  
//  //if((g_pCRevMsg[g_iCRp].id[0]==0x00)&&(g_pCRevMsg[g_iCRp].id[1]==0x00)&&(g_pCRevMsg[g_iCRp].id[2]==0x00)&&(g_pCRevMsg[g_iCRp].id[3]==0x00))
//  if((g_pCRevMsg[g_iCRp].id==0x00000000))
//        return FALSE;   
//  //将要解析的消息放到传入参数中  
//  msg = g_pCRevMsg[g_iCRp];
//  //清消息id
////  for(i=0;i<4;i++) {
////     g_pCRevMsg[g_iERp].id[i] = 0;
////  }
//   g_pCRevMsg[g_iCRp].id = 0;
//    g_iCRp++;
////  (void)UnpackMsgid(msg,msgid);
//     for(i=0;i<8;i++) {
//     msgdata[i] = g_pCRevMsg[g_iERp].data[i]; 
// }
////  //SelectUnpackFun(*msgid,msg,msgdata); 
////  return TRUE;
//}

void initCAN()
{
	 initCAN_C();
	 initCAN_A();
}
void CAN_test()
{
	struct T_SMSGDATA msg	;
	struct VCU_MASGDATA msg1;
	UINT8 i;
	UINT8 id[4] = { 1,2,3,4};
	UINT8 data[8] = {0,1,2,3,4,5,6,7};
	msg1.id =  0x1000ffff;
	msg1.datalen = 8;
	for(i=0;i<4;i++)
	msg.id[i] = i;
	msg.prty = 6;
	for(i=0;i<8;i++)
	{
		msg1.data[i] = i;	
	}
//	 initCAN_C();
	// initCAN_A();
	while(1)
	{
		 CAN_A_TX(msg1);
		 // CAN_C_TX(msg1);  
	    // CAN_C_TX(msg); 
	     
	}
}
//
//void test1()
//{
//	  SIU.PCR[87].R=0x0203; 
//	  SIU.PCR[88].R=0x0203; 
//	 // gpio_set(87,1);
//	  SIU .GPDO[88].R = 1;
//	  //gpio_set(81,1);
//	  //delay_ms(5);
//	 // gpio_set(87,0);
//	  SIU .GPDO[88].R = 0;
//	 // gpio_set(81,0);
//	 // delay_ms(5);
//}
