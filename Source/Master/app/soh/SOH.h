/*
 * File: SOH.h
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

#ifndef RTW_HEADER_SOH_h_
#define RTW_HEADER_SOH_h_
#include <stddef.h>
#include <string.h>
#ifndef SOH_COMMON_INCLUDES_
# define SOH_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* SOH_COMMON_INCLUDES_ */

#include "soh/SOH_types.h"
#include "soh/div_s32_sat.h"
#include "soh/mul_s32_s32_u32_sat.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* Block signals (auto storage) */
typedef struct {
  uint8_T soh_o;                       /* '<S1>/SHO_CHART' */
} BlockIO_SOH;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real32_T Cal_Chg_Capa;               /* '<S1>/SHO_CHART' */
  real32_T Exp_Chg_Capa;               /* '<S1>/SHO_CHART' */
  struct {
    uint_T is_c1_SOH:3;                /* '<S1>/SHO_CHART' */
    uint_T is_active_c1_SOH:1;         /* '<S1>/SHO_CHART' */
  } bitsForTID0;

  uint8_T soh_calculate;               /* '<S1>/SHO_CHART' */
} D_Work_SOH;

/* Real-time Model Data Structure */
struct tag_RTM_SOH {
  const char_T * volatile errorStatus;
};

/* Block signals (auto storage) */
extern BlockIO_SOH SOH_B;

/* Block states (auto storage) */
extern D_Work_SOH SOH_DWork;

/* Model entry point functions */
extern void SOH_initialize(void);
extern void SOH_terminate(void);

/* Customized model step function */
extern void SOH_custom(boolean_T RQ_ChaAbrt, boolean_T RQ_ChaStart, uint8_T
  soc_chg_init, real32_T capacity, int32_T current, uint16_T dt, uint8_T *soh);

/* Real-time Model object */
extern RT_MODEL_SOH *const SOH_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'SOH'
 * '<S1>'   : 'SOH/Subsystem'
 * '<S2>'   : 'SOH/Subsystem/SHO_CHART'
 */
#endif                                 /* RTW_HEADER_SOH_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
