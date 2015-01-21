/*
 * File: Closed_Loop_NCM_29Ah_change.h
 *
 * Code generated for Simulink model 'Closed_Loop_NCM_29Ah_change'.
 *
 * Model version                  : 1.26
 * Simulink Coder version         : 8.7 (R2014b) 08-Sep-2014
 * C/C++ source code generated on : Thu Dec 11 13:43:24 2014
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Freescale->MPC5xx
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. MISRA-C:2004 guidelines
 *    3. Safety precaution
 * Validation result: Not run
 */

#ifndef RTW_HEADER_Closed_Loop_NCM_29Ah_change_h_
#define RTW_HEADER_Closed_Loop_NCM_29Ah_change_h_
#include <math.h>
#include <stddef.h>
#ifndef Closed_Loop_NCM_29Ah_change_COMMON_INCLUDES_
# define Closed_Loop_NCM_29Ah_change_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* Closed_Loop_NCM_29Ah_change_COMMON_INCLUDES_ */

#include "Closed_Loop_NCM_29Ah_change_types.h"
#include "look1_iflf_binlg.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetStopRequested
# define rtmGetStopRequested(rtm)      ((void*) 0)
#endif

/* Block signals (auto storage) */
typedef struct {
  real32_T soc;                        /* '<S2>/ancount' (Output 1)  */
} BlockIO_Closed_Loop_NCM_29Ah_ch;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  real32_T UnitDelay1_DSTATE[3];       /* '<S1>/Unit Delay1' (DWork 1)  */
  real32_T UnitDelay_DSTATE[9];        /* '<S1>/Unit Delay' (DWork 1)  */
  uint32_T t;                          /* '<S2>/ancount' (Local data 't')  */
  uint8_T F_Init;                      /* '<S2>/ancount' (Local data 'F_Init')  */
  uint8_T F_Init_k;                    /* '<S1>/Event_Call' (Local data 'F_Init')  */
} D_Work_Closed_Loop_NCM_29Ah_cha;

/* Block signals (auto storage) */
extern BlockIO_Closed_Loop_NCM_29Ah_ch Closed_Loop_NCM_29Ah_change_B;

/* Block states (auto storage) */
extern D_Work_Closed_Loop_NCM_29Ah_cha Closed_Loop_NCM_29Ah_chan_DWork;

/* Model entry point functions */
extern void Closed_Loop_NCM_29Ah_initialize(void);

/* Customized model step function */
extern void Closed_Loop_NCM_29Ah_custom(uint32_T voltage, uint16_T dt, int32_T
  current, uint8_T *soc_out);

/* Exported data declaration */
extern real32_T D1[9];
extern real32_T D2;
extern uint16_T K_Dt;
extern uint16_T K_U;
extern uint8_T K_soc;
extern real32_T Qn;
extern real32_T init_p[9];
extern uint8_T init_soc;

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
 * '<Root>' : 'Closed_Loop_NCM_29Ah_change'
 * '<S1>'   : 'Closed_Loop_NCM_29Ah_change/ekf'
 * '<S2>'   : 'Closed_Loop_NCM_29Ah_change/soc_estimate'
 * '<S3>'   : 'Closed_Loop_NCM_29Ah_change/ekf/Event_Call'
 * '<S4>'   : 'Closed_Loop_NCM_29Ah_change/ekf/SOC_Init'
 * '<S5>'   : 'Closed_Loop_NCM_29Ah_change/ekf/Soc_algorithm'
 * '<S6>'   : 'Closed_Loop_NCM_29Ah_change/ekf/Soc_algorithm/InData_Conversion'
 * '<S7>'   : 'Closed_Loop_NCM_29Ah_change/ekf/Soc_algorithm/Soc_algorithm'
 * '<S8>'   : 'Closed_Loop_NCM_29Ah_change/ekf/Soc_algorithm/Soc_algorithm/Measurement'
 * '<S9>'   : 'Closed_Loop_NCM_29Ah_change/ekf/Soc_algorithm/Soc_algorithm/ModelParam'
 * '<S10>'  : 'Closed_Loop_NCM_29Ah_change/ekf/Soc_algorithm/Soc_algorithm/Time'
 * '<S11>'  : 'Closed_Loop_NCM_29Ah_change/ekf/Soc_algorithm/Soc_algorithm/model_out'
 * '<S12>'  : 'Closed_Loop_NCM_29Ah_change/ekf/Soc_algorithm/Soc_algorithm/Measurement/Transpose'
 * '<S13>'  : 'Closed_Loop_NCM_29Ah_change/ekf/Soc_algorithm/Soc_algorithm/Measurement/Transpose1'
 * '<S14>'  : 'Closed_Loop_NCM_29Ah_change/ekf/Soc_algorithm/Soc_algorithm/ModelParam/ocv_docv'
 * '<S15>'  : 'Closed_Loop_NCM_29Ah_change/ekf/Soc_algorithm/Soc_algorithm/ModelParam/parameters'
 * '<S16>'  : 'Closed_Loop_NCM_29Ah_change/ekf/Soc_algorithm/Soc_algorithm/Time/time_updata'
 * '<S17>'  : 'Closed_Loop_NCM_29Ah_change/ekf/Soc_algorithm/Soc_algorithm/Time/time_updata/CALCULATE_A'
 * '<S18>'  : 'Closed_Loop_NCM_29Ah_change/ekf/Soc_algorithm/Soc_algorithm/Time/time_updata/CALCULATE_B'
 * '<S19>'  : 'Closed_Loop_NCM_29Ah_change/ekf/Soc_algorithm/Soc_algorithm/Time/time_updata/TIMEUPDATA'
 * '<S20>'  : 'Closed_Loop_NCM_29Ah_change/ekf/Soc_algorithm/Soc_algorithm/Time/time_updata/TIMEUPDATA/Transpose'
 * '<S21>'  : 'Closed_Loop_NCM_29Ah_change/soc_estimate/OutData_Conversion1'
 * '<S22>'  : 'Closed_Loop_NCM_29Ah_change/soc_estimate/ancount'
 */
#endif                                 /* RTW_HEADER_Closed_Loop_NCM_29Ah_change_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
