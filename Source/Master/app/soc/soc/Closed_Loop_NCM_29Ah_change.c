/*
 * File: Closed_Loop_NCM_29Ah_change.c
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

#include "Closed_Loop_NCM_29Ah_change.h"
#include "Closed_Loop_NCM_29Ah_change_private.h"

/* Named constants for Chart: '<S1>/Event_Call' */
#define Closed_Loop_NCM_29Ah_CALL_EVENT (-1)
#define Closed_Loop__IN_NO_ACTIVE_CHILD ((uint8_T)0U)

/* Block signals (auto storage) */
BlockIO_Closed_Loop_NCM_29Ah_ch Closed_Loop_NCM_29Ah_change_B;

/* Block states (auto storage) */
D_Work_Closed_Loop_NCM_29Ah_cha Closed_Loop_NCM_29Ah_chan_DWork;

/* Exported data definition */
real32_T D1[9] = { 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F } ;

real32_T D2 = 0.1F;
uint16_T K_Dt = 1000U;
uint16_T K_U = 1000U;
uint8_T K_soc = 100U;
real32_T Qn = 8.14F;
real32_T init_p[9] = { 0.0001F, 0.0F, 0.0F, 0.0F, 0.0001F, 0.0F, 0.0F, 0.0F,
  0.0001F } ;

uint8_T init_soc = 100U;

/* Model step function */
void Closed_Loop_NCM_29Ah_custom(uint32_T voltage, uint16_T dt, int32_T current,
  uint8_T *soc_out)
{
  real32_T rtb_Dt_float_s;
  real32_T rtb_R0_LOOKUPTABLE;
  real32_T rtb_dt_q1;
  real32_T rtb_C1_LOOKUPTABLE;
  real32_T rtb_dt_q2;
  real32_T rtb_C2_LOOKUPTABLE;
  real32_T rtb_Reshape3[3];
  real32_T rtb_MatrixA[9];
  real32_T rtb_K[3];
  real32_T rtb_P1_time[9];
  real32_T PMerge1[9];
  int32_T i;
  real32_T tmp[9];
  real32_T tmp_0[9];
  int32_T i_0;
  real32_T rtb_Dt_float_s_0[3];
  uint32_T qY;

  /* Chart: '<S1>/Event_Call' incorporates:
   *  Constant: '<Root>/init_p'
   *  Inport: '<S4>/IN_P_Init'
   */
  /* Gateway: ekf/Event_Call */
  /* During: ekf/Event_Call */
  /* Entry Internal: ekf/Event_Call */
  /* Transition: '<S3>:5' */
  if (Closed_Loop_NCM_29Ah_chan_DWork.F_Init_k == 1) {
    /* Transition: '<S3>:7' */
    /* Transition: '<S3>:9' */
    Closed_Loop_NCM_29Ah_chan_DWork.F_Init_k = 0U;

    /* Outputs for Function Call SubSystem: '<S1>/SOC_Init' */
    /* Reshape: '<S4>/Reshape1' incorporates:
     *  Constant: '<Root>/init_soc'
     *  Constant: '<S4>/Constant1'
     *  Constant: '<S4>/Constant2'
     *  DataTypeConversion: '<Root>/Data Type Conversion'
     *  Product: '<S4>/Divide'
     */
    /* Event: '<S3>:2' */
    rtb_Reshape3[0] = ((real32_T)init_soc) / ((real32_T)K_soc);
    rtb_Reshape3[1] = 0.0F;
    rtb_Reshape3[2] = 0.0F;
    for (i = 0; i < 9; i = i + 1) {
      PMerge1[i] = init_p[(i)];
    }

    /* End of Outputs for SubSystem: '<S1>/SOC_Init' */
    /* Transition: '<S3>:11' */
  } else {
    /* Outputs for Function Call SubSystem: '<S1>/Soc_algorithm' */
    /* Product: '<S6>/Dt_float_s' incorporates:
     *  Constant: '<S6>/K_Dt'
     *  DataTypeConversion: '<Root>/Data Type Conversion3'
     *  Inport: '<Root>/dt'
     */
    /* Transition: '<S3>:12' */
    /* Event: '<S3>:3' */
    rtb_Dt_float_s = ((real32_T)dt) / ((real32_T)K_Dt);

    /* Saturate: '<S7>/SOC_Saturation' incorporates:
     *  UnitDelay: '<S1>/Unit Delay1'
     */
    if (Closed_Loop_NCM_29Ah_chan_DWork.UnitDelay1_DSTATE[0] > 1.0F) {
      rtb_R0_LOOKUPTABLE = 1.0F;
    } else if (Closed_Loop_NCM_29Ah_chan_DWork.UnitDelay1_DSTATE[0] < 0.0F) {
      rtb_R0_LOOKUPTABLE = 0.0F;
    } else {
      rtb_R0_LOOKUPTABLE = Closed_Loop_NCM_29Ah_chan_DWork.UnitDelay1_DSTATE[0];
    }

    /* End of Saturate: '<S7>/SOC_Saturation' */

    /* Lookup_n-D: '<S15>/R1_LOOKUPTABLE' */
    rtb_dt_q1 = look1_iflf_binlg(rtb_R0_LOOKUPTABLE, rtCP_R1_LOOKUPTABLE_bp01D,
      rtCP_R1_LOOKUPTABLE_table, 11U);

    /* Lookup_n-D: '<S15>/C1_LOOKUPTABLE' */
    rtb_C1_LOOKUPTABLE = look1_iflf_binlg(rtb_R0_LOOKUPTABLE,
      rtCP_C1_LOOKUPTABLE_bp01D, rtCP_C1_LOOKUPTABLE_table, 11U);

    /* Lookup_n-D: '<S15>/R2_LOOKUPTABLE' */
    rtb_dt_q2 = look1_iflf_binlg(rtb_R0_LOOKUPTABLE, rtCP_R2_LOOKUPTABLE_bp01D,
      rtCP_R2_LOOKUPTABLE_table, 11U);

    /* Lookup_n-D: '<S15>/C2_LOOKUPTABLE' */
    rtb_C2_LOOKUPTABLE = look1_iflf_binlg(rtb_R0_LOOKUPTABLE,
      rtCP_C2_LOOKUPTABLE_bp01D, rtCP_C2_LOOKUPTABLE_table, 11U);

    /* Reshape: '<S17>/Reshape' incorporates:
     *  Constant: '<S17>/Constant'
     *  Constant: '<S17>/Constant1'
     *  Constant: '<S17>/Constant2'
     *  Math: '<S17>/e_RC1'
     *  Math: '<S17>/e_RC2'
     *  Product: '<S17>/Divide'
     *  Product: '<S17>/Divide1'
     *  UnaryMinus: '<S17>/Unary Minus1'
     *  UnaryMinus: '<S17>/Unary Minus2'
     *
     * About '<S17>/e_RC1':
     *  Operator: exp
     *
     * About '<S17>/e_RC2':
     *  Operator: exp
     */
    rtb_MatrixA[0] = 1.0F;
    rtb_MatrixA[1] = 0.0F;
    rtb_MatrixA[2] = 0.0F;
    rtb_MatrixA[3] = 0.0F;
    rtb_MatrixA[4] = (real32_T)exp((real_T)(-((rtb_Dt_float_s / rtb_dt_q1) /
      rtb_C1_LOOKUPTABLE)));
    rtb_MatrixA[5] = 0.0F;
    rtb_MatrixA[6] = 0.0F;
    rtb_MatrixA[7] = 0.0F;
    rtb_MatrixA[8] = (real32_T)exp((real_T)(-((rtb_Dt_float_s / rtb_dt_q2) /
      rtb_C2_LOOKUPTABLE)));

    /* Product: '<S18>/dt_q1' incorporates:
     *  Constant: '<S18>/Constant4'
     *  Math: '<S18>/e_RC3'
     *  Product: '<S18>/Divide3'
     *  Sum: '<S18>/Subtract'
     *  UnaryMinus: '<S18>/Unary Minus4'
     *
     * About '<S18>/e_RC3':
     *  Operator: exp
     */
    rtb_dt_q1 = rtb_dt_q1 * (1.0F - ((real32_T)exp((real_T)(-((rtb_Dt_float_s /
      rtb_dt_q1) / rtb_C1_LOOKUPTABLE)))));

    /* Product: '<S18>/dt_q2' incorporates:
     *  Constant: '<S18>/Constant5'
     *  Math: '<S18>/e_RC4'
     *  Product: '<S18>/Divide2'
     *  Sum: '<S18>/Subtract1'
     *  UnaryMinus: '<S18>/Unary Minus5'
     *
     * About '<S18>/e_RC4':
     *  Operator: exp
     */
    rtb_dt_q2 = rtb_dt_q2 * (1.0F - ((real32_T)exp((real_T)(-((rtb_Dt_float_s /
      rtb_dt_q2) / rtb_C2_LOOKUPTABLE)))));

    /* End of Outputs for SubSystem: '<S1>/Soc_algorithm' */

    /* UnitDelay: '<S1>/Unit Delay' incorporates:
     *  Product: '<S19>/Matrix Multiply2'
     */
    for (i = 0; i < 9; i = i + 1) {
      /* Outputs for Function Call SubSystem: '<S1>/Soc_algorithm' */
      tmp[i] = Closed_Loop_NCM_29Ah_chan_DWork.UnitDelay_DSTATE[i];

      /* End of Outputs for SubSystem: '<S1>/Soc_algorithm' */
    }

    /* End of UnitDelay: '<S1>/Unit Delay' */

    /* Outputs for Function Call SubSystem: '<S1>/Soc_algorithm' */
    /* Product: '<S19>/Matrix Multiply2' incorporates:
     *  Math: '<S20>/Math Function'
     */
    for (i = 0; i < 3; i = i + 1) {
      for (i_0 = 0; i_0 < 3; i_0 = i_0 + 1) {
        tmp_0[i + (3 * i_0)] = 0.0F;
        tmp_0[i + (3 * i_0)] = tmp_0[i + (3 * i_0)] + (tmp[i + 0] *
          rtb_MatrixA[i_0 + 0]);
        tmp_0[i + (3 * i_0)] = tmp_0[i + (3 * i_0)] + (tmp[i + 3] *
          rtb_MatrixA[i_0 + 3]);
        tmp_0[i + (3 * i_0)] = tmp_0[i + (3 * i_0)] + (tmp[i + 6] *
          rtb_MatrixA[i_0 + 6]);
      }
    }

    /* Sum: '<S19>/P1_time' incorporates:
     *  Constant: '<S10>/D1'
     *  Product: '<S19>/Matrix Multiply2'
     */
    for (i = 0; i < 3; i = i + 1) {
      for (i_0 = 0; i_0 < 3; i_0 = i_0 + 1) {
        rtb_P1_time[i + (3 * i_0)] = (((0.0F + (rtb_MatrixA[i + 0] * tmp_0[0 +
          (3 * i_0)])) + (rtb_MatrixA[i + 3] * tmp_0[1 + (3 * i_0)])) +
          (rtb_MatrixA[i + 6] * tmp_0[2 + (3 * i_0)])) + D1[i + (3 * i_0)];
      }
    }

    /* End of Sum: '<S19>/P1_time' */

    /* Lookup_n-D: '<S14>/docv_LOOKUPTABLE' */
    rtb_C1_LOOKUPTABLE = look1_iflf_binlg(rtb_R0_LOOKUPTABLE,
      rtCP_docv_LOOKUPTABLE_bp0, rtCP_docv_LOOKUPTABLE_tab, 11U);

    /* Reshape: '<S11>/Reshape3' */
    rtb_Reshape3[0] = rtb_C1_LOOKUPTABLE;

    /* Product: '<S8>/CPC Multiply' incorporates:
     *  Constant: '<S11>/Constant'
     *  Constant: '<S11>/Constant1'
     *  Reshape: '<S11>/Reshape3'
     */
    for (i = 0; i < 3; i = i + 1) {
      rtb_K[i] = 0.0F;
      rtb_K[i] = rtb_K[i] + (rtb_P1_time[i + 0] * rtb_C1_LOOKUPTABLE);
      rtb_K[i] = rtb_K[i] + (rtb_P1_time[i + 3] * 1.0F);
      rtb_K[i] = rtb_K[i] + (rtb_P1_time[i + 6] * 1.0F);
    }

    /* Product: '<S8>/Divide' incorporates:
     *  Constant: '<S11>/Constant'
     *  Constant: '<S11>/Constant1'
     *  Constant: '<S8>/D2'
     *  Product: '<S8>/CPC Multiply'
     *  Reshape: '<S11>/Reshape3'
     *  Sum: '<S8>/CPC_D'
     */
    rtb_C1_LOOKUPTABLE = 1.0F / ((((0.0F + (rtb_C1_LOOKUPTABLE * rtb_K[0])) +
      (1.0F * rtb_K[1])) + (1.0F * rtb_K[2])) + D2);

    /* Product: '<S8>/K Multiply' incorporates:
     *  Constant: '<S11>/Constant'
     *  Constant: '<S11>/Constant1'
     *  Math: '<S12>/Math Function'
     *  Reshape: '<S11>/Reshape3'
     */
    for (i = 0; i < 3; i = i + 1) {
      rtb_K[i] = 0.0F;
      rtb_K[i] = rtb_K[i] + (rtb_P1_time[i + 0] * (rtb_Reshape3[0] *
        rtb_C1_LOOKUPTABLE));
      rtb_K[i] = rtb_K[i] + (rtb_P1_time[i + 3] * (1.0F * rtb_C1_LOOKUPTABLE));
      rtb_K[i] = rtb_K[i] + (rtb_P1_time[i + 6] * (1.0F * rtb_C1_LOOKUPTABLE));
    }

    /* End of Product: '<S8>/K Multiply' */

    /* Lookup_n-D: '<S14>/ocv_LOOKUPTABLE' */
    rtb_C1_LOOKUPTABLE = look1_iflf_binlg(rtb_R0_LOOKUPTABLE,
      rtCP_ocv_LOOKUPTABLE_bp01, rtCP_ocv_LOOKUPTABLE_tabl, 11U);

    /* Lookup_n-D: '<S15>/R0_LOOKUPTABLE' */
    rtb_R0_LOOKUPTABLE = look1_iflf_binlg(rtb_R0_LOOKUPTABLE,
      rtCP_R0_LOOKUPTABLE_bp01D, rtCP_R0_LOOKUPTABLE_table, 11U);

    /* Sum: '<S8>/U_y' incorporates:
     *  Constant: '<S6>/K_U'
     *  DataTypeConversion: '<Root>/Data Type Conversion2'
     *  DataTypeConversion: '<Root>/Data Type Conversion4'
     *  Inport: '<Root>/current'
     *  Inport: '<Root>/voltage'
     *  Product: '<S11>/Product2'
     *  Product: '<S6>/U_float_V'
     *  Sum: '<S11>/I_Id1'
     *  UnitDelay: '<S1>/Unit Delay1'
     */
    rtb_C1_LOOKUPTABLE = (((real32_T)voltage) / ((real32_T)K_U)) -
      (((rtb_C1_LOOKUPTABLE + Closed_Loop_NCM_29Ah_chan_DWork.UnitDelay1_DSTATE
         [1]) + Closed_Loop_NCM_29Ah_chan_DWork.UnitDelay1_DSTATE[2]) +
       (((real32_T)current) * rtb_R0_LOOKUPTABLE));

    /* Sum: '<S8>/Add2' incorporates:
     *  Constant: '<S11>/Constant'
     *  Constant: '<S11>/Constant1'
     *  Constant: '<S8>/Constant1'
     *  Product: '<S8>/Matrix Multiply3'
     *  Product: '<S8>/Matrix Multiply4'
     *  Reshape: '<S11>/Reshape3'
     */
    for (i = 0; i < 3; i = i + 1) {
      tmp[i + 0] = rtCP_Constant1_Value_d[i + 0] - (0.0F + (rtb_K[i] *
        rtb_Reshape3[0]));
      tmp[i + 3] = rtCP_Constant1_Value_d[i + 3] - (0.0F + (rtb_K[i] * 1.0F));
      tmp[i + 6] = rtCP_Constant1_Value_d[i + 6] - (0.0F + (rtb_K[i] * 1.0F));
    }

    /* End of Sum: '<S8>/Add2' */

    /* Product: '<S8>/Matrix Multiply4' */
    for (i = 0; i < 3; i = i + 1) {
      for (i_0 = 0; i_0 < 3; i_0 = i_0 + 1) {
        PMerge1[i + (3 * i_0)] = 0.0F;
        PMerge1[i + (3 * i_0)] = PMerge1[i + (3 * i_0)] + (tmp[i + 0] *
          rtb_P1_time[0 + (3 * i_0)]);
        PMerge1[i + (3 * i_0)] = PMerge1[i + (3 * i_0)] + (tmp[i + 3] *
          rtb_P1_time[1 + (3 * i_0)]);
        PMerge1[i + (3 * i_0)] = PMerge1[i + (3 * i_0)] + (tmp[i + 6] *
          rtb_P1_time[2 + (3 * i_0)]);
      }
    }

    /* Saturate: '<S10>/SOC_Saturation' incorporates:
     *  UnitDelay: '<S1>/Unit Delay1'
     */
    if (Closed_Loop_NCM_29Ah_chan_DWork.UnitDelay1_DSTATE[0] > 1.0F) {
      rtb_R0_LOOKUPTABLE = 1.0F;
    } else if (Closed_Loop_NCM_29Ah_chan_DWork.UnitDelay1_DSTATE[0] < 0.0F) {
      rtb_R0_LOOKUPTABLE = 0.0F;
    } else {
      rtb_R0_LOOKUPTABLE = Closed_Loop_NCM_29Ah_chan_DWork.UnitDelay1_DSTATE[0];
    }

    /* End of Saturate: '<S10>/SOC_Saturation' */

    /* SignalConversion: '<S19>/TmpSignal ConversionAtMatrix MultiplyInport1' incorporates:
     *  Constant: '<S10>/Qn'
     *  Constant: '<S18>/CONSTANT'
     *  Product: '<S18>/dt_q'
     */
    rtb_Dt_float_s_0[0] = (rtb_Dt_float_s / Qn) / 3600.0F;
    rtb_Dt_float_s_0[1] = rtb_dt_q1;
    rtb_Dt_float_s_0[2] = rtb_dt_q2;

    /* Reshape: '<S8>/Reshape3' incorporates:
     *  DataTypeConversion: '<Root>/Data Type Conversion4'
     *  Inport: '<Root>/current'
     *  Product: '<S19>/Matrix Multiply'
     *  Product: '<S19>/Matrix Multiply1'
     *  Product: '<S8>/K(U-y) Multiply'
     *  SignalConversion: '<S19>/TmpSignal ConversionAtMatrix Multiply1Inport2'
     *  Sum: '<S19>/X1_time'
     *  Sum: '<S8>/ADD_X'
     *  UnitDelay: '<S1>/Unit Delay1'
     */
    for (i = 0; i < 3; i = i + 1) {
      rtb_Reshape3[i] = ((((0.0F + (rtb_MatrixA[i + 0] * rtb_R0_LOOKUPTABLE)) +
                           (rtb_MatrixA[i + 3] *
                            Closed_Loop_NCM_29Ah_chan_DWork.UnitDelay1_DSTATE[1]))
                          + (rtb_MatrixA[i + 6] *
        Closed_Loop_NCM_29Ah_chan_DWork.UnitDelay1_DSTATE[2])) +
                         (rtb_Dt_float_s_0[i] * ((real32_T)current))) + (rtb_K[i]
        * rtb_C1_LOOKUPTABLE);
    }

    /* End of Reshape: '<S8>/Reshape3' */
    /* End of Outputs for SubSystem: '<S1>/Soc_algorithm' */
  }

  /* End of Chart: '<S1>/Event_Call' */

  /* Chart: '<S2>/ancount' incorporates:
   *  Constant: '<Root>/init_soc'
   *  Constant: '<S2>/capacity'
   *  DataTypeConversion: '<Root>/Data Type Conversion'
   *  DataTypeConversion: '<Root>/Data Type Conversion3'
   *  DataTypeConversion: '<Root>/Data Type Conversion4'
   *  Inport: '<Root>/current'
   *  Inport: '<Root>/dt'
   */
  /* Transition: '<S3>:14' */
  /* Gateway: soc_estimate/ancount */
  /* During: soc_estimate/ancount */
  /* Entry Internal: soc_estimate/ancount */
  /* Transition: '<S22>:63' */
  if (Closed_Loop_NCM_29Ah_chan_DWork.F_Init == 1) {
    /* Transition: '<S22>:65' */
    /* Transition: '<S22>:69' */
    Closed_Loop_NCM_29Ah_chan_DWork.F_Init = 0U;
    Closed_Loop_NCM_29Ah_change_B.soc = ((real32_T)init_soc) / 100.0F;

    /* Transition: '<S22>:70' */
  } else {
    /* Transition: '<S22>:76' */
  }

  /* Transition: '<S22>:77' */
  Closed_Loop_NCM_29Ah_change_B.soc = Closed_Loop_NCM_29Ah_change_B.soc +
    ((((((real32_T)dt) * ((real32_T)current)) / Qn) / 3600.0F) / 1000.0F);
  if (Closed_Loop_NCM_29Ah_change_B.soc >= 1.0F) {
    /* Transition: '<S22>:24' */
    /* Transition: '<S22>:25' */
    Closed_Loop_NCM_29Ah_change_B.soc = 1.0F;

    /* Transition: '<S22>:28' */
    /* Transition: '<S22>:29' */
  } else {
    /* Transition: '<S22>:23' */
    if (Closed_Loop_NCM_29Ah_change_B.soc <= 0.0F) {
      /* Transition: '<S22>:26' */
      /* Transition: '<S22>:27' */
      Closed_Loop_NCM_29Ah_change_B.soc = 0.0F;

      /* Transition: '<S22>:29' */
    } else {
      /* Transition: '<S22>:30' */
    }
  }

  /* Transition: '<S22>:31' */
  qY = Closed_Loop_NCM_29Ah_chan_DWork.t + 1U;
  if (qY < Closed_Loop_NCM_29Ah_chan_DWork.t) {
    qY = MAX_uint32_T;
  }

  Closed_Loop_NCM_29Ah_chan_DWork.t = qY;
  if (Closed_Loop_NCM_29Ah_chan_DWork.t > 7200U) {
    /* Transition: '<S22>:44' */
    /* Transition: '<S22>:43' */
    Closed_Loop_NCM_29Ah_chan_DWork.t = 0U;
    Closed_Loop_NCM_29Ah_change_B.soc = rtb_Reshape3[0];

    /* Transition: '<S22>:42' */
  } else {
    /* Transition: '<S22>:41' */
  }

  /* End of Chart: '<S2>/ancount' */

  /* Outport: '<Root>/soc_out' incorporates:
   *  Constant: '<S21>/K_soc'
   *  DataTypeConversion: '<S21>/Data Type Conversion'
   *  Product: '<S21>/Product'
   */
  /* Transition: '<S22>:91' */
  *soc_out = (uint8_T)((uint16_T)(Closed_Loop_NCM_29Ah_change_B.soc * ((real32_T)
                         ((uint16_T)K_soc))));

  /* Update for UnitDelay: '<S1>/Unit Delay1' */
  Closed_Loop_NCM_29Ah_chan_DWork.UnitDelay1_DSTATE[0] = rtb_Reshape3[0];
  Closed_Loop_NCM_29Ah_chan_DWork.UnitDelay1_DSTATE[1] = rtb_Reshape3[1];
  Closed_Loop_NCM_29Ah_chan_DWork.UnitDelay1_DSTATE[2] = rtb_Reshape3[2];

  /* Update for UnitDelay: '<S1>/Unit Delay' */
  for (i = 0; i < 9; i = i + 1) {
    Closed_Loop_NCM_29Ah_chan_DWork.UnitDelay_DSTATE[i] = PMerge1[i];
  }

  /* End of Update for UnitDelay: '<S1>/Unit Delay' */
}

/* Model initialize function */
void Closed_Loop_NCM_29Ah_initialize(void)
{
  /* InitializeConditions for Chart: '<S1>/Event_Call' */
  Closed_Loop_NCM_29Ah_chan_DWork.F_Init_k = 1U;

  /* InitializeConditions for Chart: '<S2>/ancount' */
  Closed_Loop_NCM_29Ah_chan_DWork.t = 1U;
  Closed_Loop_NCM_29Ah_chan_DWork.F_Init = 1U;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
