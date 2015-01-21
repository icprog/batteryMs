/* main.c:  performs a single transfer from DSPI_C to DSPI_B on MPC555x or MPC563x */
/* Rev 1.0 Jun 2 2008 SM - Ported from AN2865 example Rev 2.2 for DSPI C, DSPI B */
/*                         and used POPR[RXDATA] for RecDataMaster, RecDataSlave */
/* Rev 1.1 Aug 15 2008 SM - Modified SIU.DISR line for internal DSPI connections */
/* Rev 1.2 Aug 18 2008 D McKenna- Kept DSPI_MCR[HALT] set during  initialization*/
/* Copyright Freescale Semiconductor, Inc. 2007 All rights reserved. */
/* Notes:  */
/*  1. MMU not initialized; must be done by debug scripts or BAM */
/*  2. SRAM not initialized; must be done by debug scripts or in a crt0 type file */

#include "MPC5634M.h" /* Use proper include file such as mpc5510.h or mpc5554.h */
#include "public/public.h"
//  vuint32_t i = 0;                      /* Dummy idle counter */
//  vuint32_t RecDataMaster = 0;          /* Data recieved on master SPI */
//  vuint32_t RecDataSlave  = 0;          /* Data received on slave SPI */
//
//void initSysclk (void) {
///* MPC563x: Use the next line        */
//  FMPLL.ESYNCR1.B.CLKCFG = 0X7;    /* MPC563x: Change clk to PLL normal from crystal*/  
//  FMPLL.SYNCR.R = 0x16080000;         /* 8 MHz xtal: 0x16080000; 40MHz: 0x46100000 */
//  while (FMPLL.SYNSR.B.LOCK != 1) {}; /* Wait for FMPLL to LOCK  */
//  FMPLL.SYNCR.R = 0x16000000;         /* 8 MHz xtal: 0x16000000; 40MHz: 0x46080000 */
//}
UINT32 RecData = 0;
void initDSPI_C(void) {
  DSPI_C.MCR.R = 0x80010001;     /* Configure DSPI_C as master */
  DSPI_C.CTAR[0].R = 0x780A7727; /* Configure CTAR0  */
  DSPI_C.MCR.B.HALT = 0x0;	     /* Exit HALT mode: go from STOPPED to RUNNING state*/
  SIU.PCR[107].R = 0x0A00;       /* MPC555x: Config pad as DSPI_C SOUT output */
  SIU.PCR[108].R = 0x0900;       /* MPC555x: Config pad as DSPI_C SIN input */
  SIU.PCR[109].R = 0x0A00;       /* MPC555x: Config pad as DSPI_C SCK output */
  SIU.PCR[110].R = 0x0A00;       /* MPC555x: Config pad as DSPI_C PCS0 output */
}

UINT32 ReadDataDSPI_C(void) {
  while (DSPI_C.SR.B.RFDF != 1){}  /* Wait for Receive FIFO Drain Flag = 1 */
  RecData = DSPI_C.POPR.B.RXDATA;   /* Read data received by master SPI */
  DSPI_C.SR.R = 0x90020000;        /* Clear TCF, RDRF, EOQ flags by writing 1 */
  return RecData;
}

//void initDSPI_B(void) {
//  DSPI_B.MCR.R = 0x80010001;     /* Configure DSPI_B as slave */
//  DSPI_B.CTAR[0].R = 0x780A7727; /* Configure CTAR0  */
//  DSPI_B.MCR.B.HALT = 0x0;	     /* Exit HALT mode: go from STOPPED to RUNNING state*/
//  SIU.PCR[102].R = 0x0500;       /* MPC555x: Config pad as DSPI_B SCK input */
//  SIU.PCR[103].R = 0x0500;       /* MPC555x: Config pad as DSPI_B SIN input */
//  SIU.PCR[104].R = 0x0600;       /* MPC555x: Config pad as DSPI_B SOUT output*/
//  SIU.PCR[105].R = 0x0500;       /* MPC555x: Config pad as DSPI_B PCS0/SS input */
//}
//
//UINT32 ReadDataDSPI_B(void) {
//  while (DSPI_B.SR.B.RFDF != 1){}      /* Wait for Receive FIFO Drain Flag = 1 */
//  RecDataSlave = DSPI_B.POPR.B.RXDATA; /* Read data received by slave SPI */
//  DSPI_B.SR.R = 0x80020000;    
//  /* Clear TCF, RDRF flags by writing 1 to them */
//  return RecDataSlave;
//}

void TransmitDataDSPI_C(UINT8 data )
{
	DSPI_B.PUSHR.R = data;
}


void SPIC_Write(UINT8 *buff, UINT16 len)
{

  UINT16 i = 0;
                
 // delayus(50);
  
  for( i=0; i<len ; i++) {
	  TransmitDataDSPI_C( buff[i]);
	 // delayus( 1925); 
  }
  
  //while(!SPI1SR_SPTEF);
  //delayus(200);
}

void SPIC_Read(UINT8 *dstbuf, UINT16 n) 
{
   // delayus(50);
    while(n-- && dstbuf){
      //SPI1_send(0xff);
    //	delayus(1925);
      *dstbuf++ = ReadDataDSPI_C();     
    }
   // delayus(50);  
}


//int test_spi_B_C(void) {
///* Optional:  Use one of the next two lines for internal DSPI connections: */
///* SIU.DISR.R = 0x0000C0FC; */ /* MPC55xx except MPC563x: Connect DSPI_C, DSPI_D*/
//  SIU.DISR.R = 0x00A8A000;     /* MPC563x only: Connect DSPI_C, DSPI_B */
////  initSysclk();                /* Set sysclk = 64MHz running from PLL */
//  initDSPI_C();                /* Initialize DSPI_C as master SPI and init CTAR0 */
//  initDSPI_B();                /* Initialize DSPI_B as Slave SPI and init CTAR0 */
//  DSPI_B.PUSHR.R = 0x00001234; /* Initialize slave DSPI_B's response to master */
//  DSPI_C.PUSHR.R = 0x08015678; /* Transmit data from master to slave SPI with EOQ */
//  ReadDataDSPI_B();            /* Read data on slave DSPI */
//  ReadDataDSPI_C();            /* Read data on master DSPI */
//  while (1) {i++; }            /* Wait forever */
//}
