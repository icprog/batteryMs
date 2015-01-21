/*
 * File: SOH.c
 *
 * Code generated for Simulink model 'SOH'.
 *
 * Model version                  : 1.15
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Sun Jan 04 18:10:31 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Freescale->MPC5xx
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 *    3. MISRA-C:2004 guidelines
 * Validation result: Not run
 */

#include "soh/SOH.h"
#include "soh/SOH_private.h"

/* Named constants for Chart: '<S1>/SHO_CHART' */
#define SOH_IN_NO_ACTIVE_CHILD         ((uint8_T)0U)
#define SOH_IN_RQ                      ((uint8_T)1U)
#define SOH_IN_SOH                     ((uint8_T)2U)
#define SOH_IN_SOH_BEGAIN              ((uint8_T)3U)
#define SOH_IN_calculate_SOH           ((uint8_T)4U)
#define SOH_IN_cc                      ((uint8_T)5U)

/* Block signals (auto storage) */
BlockIO_SOH SOH_B;

/* Block states (auto storage) */
D_Work_SOH SOH_DWork;

/* Real-time model */
RT_MODEL_SOH SOH_M_;
RT_MODEL_SOH *const SOH_M = &SOH_M_;

/* Model step function */
void SOH_custom(boolean_T RQ_ChaAbrt, boolean_T RQ_ChaStart, uint8_T
                soc_chg_init, real32_T capacity, int32_T current, uint16_T dt,
                uint8_T *soh)
{
  real32_T tmp;
  int32_T tmp_0;

  /* Chart: '<S1>/SHO_CHART' incorporates:
   *  Inport: '<Root>/RQ_ChaAbrt'
   *  Inport: '<Root>/RQ_ChaStart'
   *  Inport: '<Root>/capacity'
   *  Inport: '<Root>/current'
   *  Inport: '<Root>/dt'
   *  Inport: '<Root>/soc_chg_init'
   */
  /* Gateway: Subsystem/SHO_CHART */
  /* During: Subsystem/SHO_CHART */
  if (SOH_DWork.bitsForTID0.is_active_c1_SOH == 0U) {
    /* Entry: Subsystem/SHO_CHART */
    SOH_DWork.bitsForTID0.is_active_c1_SOH = 1U;

    /* Entry Internal: Subsystem/SHO_CHART */
    /* Transition: '<S2>:56' */
    SOH_DWork.bitsForTID0.is_c1_SOH = SOH_IN_SOH_BEGAIN;
  } else {
    switch (SOH_DWork.bitsForTID0.is_c1_SOH) {
     case SOH_IN_RQ:
      /* During 'RQ': '<S2>:27' */
      if (RQ_ChaAbrt && (SOH_DWork.Exp_Chg_Capa != 0.0F)) {
        /* Transition: '<S2>:30' */
        SOH_DWork.bitsForTID0.is_c1_SOH = SOH_IN_calculate_SOH;

        /* Entry 'calculate_SOH': '<S2>:29' */
        tmp = SOH_DWork.Cal_Chg_Capa / SOH_DWork.Exp_Chg_Capa;
        if (tmp < 256.0F) {
          if (tmp >= 0.0F) {
            SOH_DWork.soh_calculate = (uint8_T)tmp;
          } else {
            SOH_DWork.soh_calculate = 0U;
          }
        } else {
          SOH_DWork.soh_calculate = MAX_uint8_T;
        }
      } else {
        SOH_DWork.Exp_Chg_Capa = (1.0F - (real32_T)soc_chg_init) * capacity;
        SOH_DWork.Cal_Chg_Capa += (real32_T)div_s32_sat(mul_s32_s32_u32_sat
          (current, dt), 3600);
      }
      break;

     case SOH_IN_SOH:
      /* During 'SOH': '<S2>:46' */
      if ((!RQ_ChaAbrt) && RQ_ChaStart) {
        /* Transition: '<S2>:50' */
        SOH_DWork.bitsForTID0.is_c1_SOH = SOH_IN_RQ;
      } else if ((!RQ_ChaAbrt) && (!RQ_ChaStart)) {
        /* Transition: '<S2>:58' */
        SOH_DWork.bitsForTID0.is_c1_SOH = SOH_IN_SOH_BEGAIN;
      } else {
        tmp_0 = SOH_DWork.soh_calculate * 100;
        if (tmp_0 > 255) {
          tmp_0 = 255;
        }

        SOH_B.soh_o = (uint8_T)tmp_0;
      }
      break;

     case SOH_IN_SOH_BEGAIN:
      /* During 'SOH_BEGAIN': '<S2>:55' */
      if (RQ_ChaStart) {
        /* Transition: '<S2>:28' */
        SOH_DWork.bitsForTID0.is_c1_SOH = SOH_IN_RQ;
      }
      break;

     case SOH_IN_calculate_SOH:
      /* During 'calculate_SOH': '<S2>:29' */
      if ((!RQ_ChaAbrt) && RQ_ChaStart) {
        /* Transition: '<S2>:31' */
        SOH_DWork.bitsForTID0.is_c1_SOH = SOH_IN_RQ;
      } else if (SOH_DWork.Cal_Chg_Capa > SOH_DWork.Exp_Chg_Capa) {
        /* Transition: '<S2>:49' */
        SOH_DWork.bitsForTID0.is_c1_SOH = SOH_IN_cc;
      } else {
        if (SOH_DWork.Cal_Chg_Capa <= SOH_DWork.Exp_Chg_Capa) {
          /* Transition: '<S2>:47' */
          SOH_DWork.bitsForTID0.is_c1_SOH = SOH_IN_SOH;
        }
      }
      break;

     default:
      /* During 'cc': '<S2>:48' */
      if ((!RQ_ChaAbrt) && RQ_ChaStart) {
        /* Transition: '<S2>:51' */
        SOH_DWork.bitsForTID0.is_c1_SOH = SOH_IN_RQ;
      } else if ((!RQ_ChaAbrt) && (!RQ_ChaStart)) {
        /* Transition: '<S2>:57' */
        SOH_DWork.bitsForTID0.is_c1_SOH = SOH_IN_SOH_BEGAIN;
      } else {
        SOH_B.soh_o = 100U;
      }
      break;
    }
  }

  /* End of Chart: '<S1>/SHO_CHART' */

  /* Outport: '<Root>/soh' */
  *soh = SOH_B.soh_o;
}

/* Model initialize function */
void SOH_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  rtmSetErrorStatus(SOH_M, (NULL));

  /* block I/O */
  (void) memset(((void *) &SOH_B), 0,
                sizeof(BlockIO_SOH));

  /* states (dwork) */
  (void) memset((void *)&SOH_DWork, 0,
                sizeof(D_Work_SOH));

  /* InitializeConditions for Chart: '<S1>/SHO_CHART' */
  SOH_DWork.bitsForTID0.is_active_c1_SOH = 0U;
  SOH_DWork.bitsForTID0.is_c1_SOH = SOH_IN_NO_ACTIVE_CHILD;
  SOH_DWork.Cal_Chg_Capa = 0.0F;
  SOH_DWork.Exp_Chg_Capa = 0.0F;
  SOH_DWork.soh_calculate = 0U;
  SOH_B.soh_o = 0U;
}

/* Model terminate function */
void SOH_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
