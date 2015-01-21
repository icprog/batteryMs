/*
 * File: div_s32_sat.c
 *
 * Code generated for Simulink model 'SOH'.
 *
 * Model version                  : 1.13
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Sun Jan 04 15:20:30 2015
 */

#include "rtwtypes.h"
#include "soh/div_s32_sat.h"

int32_T div_s32_sat(int32_T numerator, int32_T denominator)
{
  int32_T quotient;
  uint32_T tempAbsQuotient;
  boolean_T quotientNeedsNegation;
  if (denominator == 0) {
    quotient = numerator >= 0 ? MAX_int32_T : MIN_int32_T;

    /* Divide by zero handler */
  } else {
    quotientNeedsNegation = ((numerator < 0) != (denominator < 0));
    tempAbsQuotient = (uint32_T)(numerator >= 0 ? numerator : -numerator) /
      (denominator >= 0 ? denominator : -denominator);
    if ((!quotientNeedsNegation) && (tempAbsQuotient >= 2147483647U)) {
      quotient = MAX_int32_T;
    } else if (quotientNeedsNegation && (tempAbsQuotient > 2147483647U)) {
      quotient = MIN_int32_T;
    } else {
      quotient = quotientNeedsNegation ? -(int32_T)tempAbsQuotient : (int32_T)
        tempAbsQuotient;
    }
  }

  return quotient;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
