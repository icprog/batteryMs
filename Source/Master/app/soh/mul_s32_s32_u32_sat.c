/*
 * File: mul_s32_s32_u32_sat.c
 *
 * Code generated for Simulink model 'SOH'.
 *
 * Model version                  : 1.13
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Sun Jan 04 15:20:30 2015
 */

#include "rtwtypes.h"
#include "soh/mul_wide_su32.h"
#include "soh/mul_s32_s32_u32_sat.h"

int32_T mul_s32_s32_u32_sat(int32_T a, uint32_T b)
{
  int32_T result;
  uint32_T u32_chi;
  uint32_T u32_clo;
  mul_wide_su32(a, b, &u32_chi, &u32_clo);
  if (((int32_T)u32_chi > 0) || ((u32_chi == 0U) && (u32_clo >= 2147483648U))) {
    result = MAX_int32_T;
  } else if (((int32_T)u32_chi < -1) || (((int32_T)u32_chi == -1) && (u32_clo <
               2147483648U))) {
    result = MIN_int32_T;
  } else {
    result = (int32_T)u32_clo;
  }

  return result;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
