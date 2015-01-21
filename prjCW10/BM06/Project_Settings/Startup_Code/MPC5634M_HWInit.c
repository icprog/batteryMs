/*
 * FILE : MPC563xM_HWInit.c
 *    
 * COPYRIGHT (c) 2007-2013 Freescale Semiconductor, Inc. All rights reserved.
 *     
 * DESCRIPTION: This file contains all MPC563xM derivative basic initializations,
 *  and initializations for the MPC563xM boards which are supported.
 *  This includes setting up the MMU, SRAM and WatchDog.
 *  
 *  For more on Qorivva MCUs module initializations please consult the Qorivva cookbook AN2865.  
 *
 * VERSION: 1.1
 */

/*----------------------------------------------------------------------------*/
/* Includes                                                                   */
/*----------------------------------------------------------------------------*/

#include "MPC5634M.h"       /* MCU platform development header            */
#include "MPC5634M_HWInit.h"

#ifdef __cplusplus
extern "C" {
#endif


/*------------------------------------------------------------------*/
/* Symbol L2SRAM_LOCATION is defined in the application linker      */
/* command file (.lcf). Its value should reflect SRAM start address.*/ 
/*------------------------------------------------------------------*/
/*lint -esym(752, L2SRAM_LOCATION) */
extern unsigned long L2SRAM_LOCATION;  

/*------------------------------------------------------------------*/
/* Symbol L2SRAM_CNT is defined in the application linker command   */
/* file (.lcf). It represents the how many writes with stmw         */
/* (128 bytes each) are needed to cover the whole L2SRAM.           */
/*------------------------------------------------------------------*/
extern unsigned long L2SRAM_CNT;

/*----------------------------------------------------------------*/
/* Function implementations                                       */
/*----------------------------------------------------------------*/

MAKE_HLI_COMPATIBLE(SR_WSC_1, 0xc520) /*50464*/
MAKE_HLI_COMPATIBLE(SR_WSC_2, 0xd928) /*55592*/
MAKE_HLI_COMPATIBLE(MCR_VALUE, 0x8000010A)
MAKE_HLI_COMPATIBLE(SWT_SR, &SWT.SR.R) /* Address of the SWT SR */
MAKE_HLI_COMPATIBLE(SWT_MCR, &SWT.MCR.R) /* Address of the SWT MCR */

/*------------------------------------------------------------------*/
/* FUNCTION     : INIT_Derivative                                   */
/* PURPOSE      : This function initializes the derivative.         */
/* SEQUENCE:                                                        */
/* - initializes the MMU                                            */
/* - disables system WDT and Core WDT                               */
/* - initializes the internal SRAM for ROM targets                  */
/*------------------------------------------------------------------*/
__asm void INIT_Derivative(void) 
{
	nofralloc

    /* By default the BAM initializes the MMU (please se below table)*/
    /* Don't have a stack yet, save the return address in a register */
#ifdef L2SRAM_64K
	/*
	   Initialize the device MMU table
    */
    mflr r26
    bl __initMMU
    mtlr r26
#endif

    /* Clear the soft lock bit SWT_CR.SLKSWT_CR: */
    /* SR --> 0x0000c520 */
    lis r4, 0
    ori r4, r4, SR_WSC_1@l
    lis r3, SWT_SR@ha
    stw r4, SWT_SR@l(r3)
    /* SR --> 0x0000d928 */
    lis r4, 0
    ori r4, r4, SR_WSC_2@l
    stw r4, SWT_SR@l(r3)

    /* Disable watchdog, SWT.CR.WEN = 0*/
    lis r4, MCR_VALUE@h
    ori r4, r4, MCR_VALUE@l
    lis r3, SWT_MCR@ha
    stw r4, SWT_MCR@l(r3)

#if defined(ROM_VERSION) && (ROM_VERSION == 1)
    /* enable SPE if the compiler isn't called with -spe_vector */

    /* SRAM initialization code*/
    lis r11,L2SRAM_LOCATION@h
    ori r11,r11,L2SRAM_LOCATION@l

    /* Loops to cover L2SRAM, stmw allows 128 bytes (32 GPRS x 4 bytes) writes */
    lis r12,L2SRAM_CNT@h
    ori r12,r12,L2SRAM_CNT@l
    mtctr r12

    init_l2sram_loop:
        stmw r0, 0(r11)        /* Write 32 GPRs to SRAM*/
        addi r11,r11,128       /* Inc the ram ptr; 32 GPRs * 4 bytes = 128B */
        bdnz init_l2sram_loop  /* Loop for L2SRAM_CNT times */
#endif

    blr
}

/*----------------------------------------------------------------------*/
/* FUNCTION:WriteMMUTableEntryS                                         */
/* PURPOSE: Creates a new TLB entry with synchronization.It ensures that*/
/*          the TLB context change doesn't affect the core by execution */
/*          before and after tlbwe a CSI. This should be called when    */
/*          creating SRAM or FLASH TLB entries.                         */
/* SEQUENCE: write GPR to MAS, execute CSI, tlbwe, CSI                  */
/*----------------------------------------------------------------------*/
__asm void WriteMMUTableEntryS( void )
{ 
nofralloc
    /* Write MMU Assist Register 0 (MAS0); SPR 624 */
    mtspr   624, r3
    /* Write MMU Assist Register 1 (MAS1); SPR 625 */
    mtspr   625, r4
    /* Write MMU Assist Register 2 (MAS2); SPR 626 */
    mtspr   626, r5
    /* Write MMU Assist Register 3 (MAS3); SPR 627 */
    mtspr   627, r6
    /* Write the table entry */

    /* All instruction will complete here in current context. */
    msync
    tlbwe
    /* synchronize instruction fetches and data accesses in respect
     with the new created TLB entry. */
    msync
    isync
    blr
}

/*--------------------------------------------------*/
/* FUNCTION:WriteMMUTableEntry                      */
/* PURPOSE: Creates a new TLB entry with            */
/* SEQUENCE: write GPR to MAS, execute tlbwe        */
/*--------------------------------------------------*/
__asm void WriteMMUTableEntry( void )
{ 
nofralloc
    /* Write MMU Assist Register 0 (MAS0); SPR 624 */
    mtspr   624, r3
    /* Write MMU Assist Register 1 (MAS1); SPR 625 */
    mtspr   625, r4
    /* Write MMU Assist Register 2 (MAS2); SPR 626 */
    mtspr   626, r5
    /* Write MMU Assist Register 3 (MAS3); SPR 627 */
    mtspr   627, r6
    /* Write the table entry */
    tlbwe
    blr
}

/*
   Device MMU table:
   ---------------------------------------------------------
   Name:             TLB entry  Start            Length
   ---------------------------------------------------------
   *Per. AIPS_B+BAM  0          0xFFF0_0000       1MB
   *FLASH            1          0x0000_0000       16MB
   *C_EBI            2          0x2000_0000       16MB
   *SRAM             3          0x4000_0000       256KB
   *Periph. A        4          0xC3F0_0000       1MB
   ---------------------------------------------------------
   (*) Created by BAM. On debug mode the debug tool should initialize these pages.
 */

/* SRAM for MPC5633M and MPC5632M: TLB 3, 0x4000_0000, 64KB, not protected, not guarded, cache off, big-endian, all access, VLE/BOOKE */
MAKE_HLI_COMPATIBLE(TLB_Entry_3_MAS0, MAS0_VALUE(3))
MAKE_HLI_COMPATIBLE(TLB_Entry_3_MAS1, MAS1_VALUE(V_VALID, IPROT_NOTPROTECTED, TID_GLOBAL, TS_OFF, TSIZE_64KB))

#if __option(vle)
MAKE_HLI_COMPATIBLE(TLB_Entry_3_MAS2, MAS2_VALUE(0x40000000, VLE_MODE, WRITE_BACK, CACHE_INHIBIT, MEM_COHERENCE_NREQ, NOT_GUARDED, BIG_ENDIAN))
#else
MAKE_HLI_COMPATIBLE(TLB_Entry_3_MAS2, MAS2_VALUE(0x40000000, BOOK_E_MODE, WRITE_BACK, CACHE_INHIBIT, MEM_COHERENCE_NREQ, NOT_GUARDED, BIG_ENDIAN))
#endif
MAKE_HLI_COMPATIBLE(TLB_Entry_3_MAS3, MAS3_VALUE(0x40000000, READ_WRITE_EXECUTE))

__asm void __initMMU(void)
{
nofralloc

	/* SRAM, TLB3, 0x4000_0000, 648KB */
	lis r3, TLB_Entry_3_MAS0@h
	ori r3, r3, TLB_Entry_3_MAS0@l
	lis r4, TLB_Entry_3_MAS1@h
	ori r4, r4, TLB_Entry_3_MAS1@l
	lis r5, TLB_Entry_3_MAS2@h
	ori r5, r5, TLB_Entry_3_MAS2@l
	lis r6, TLB_Entry_3_MAS3@h
	ori r6, r6, TLB_Entry_3_MAS3@l

	/* Synchronize in case of running from SRAM */
	mflr r28
	bl WriteMMUTableEntryS
	mtlr r28

	blr
}

/*--------------------------------------------------------------------------*/
/* SIU External Bus Interface                                               */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* FUNCTION     : INIT_ExternalBusAndMemory                                 */
/* PURPOSE      : This function configures EBI for the external memory      */
/*--------------------------------------------------------------------------*/
__asm void INIT_ExternalBusAndMemory(void) 
{
nofralloc
    /* TODO */
    blr
}

#ifdef __cplusplus
}
#endif
