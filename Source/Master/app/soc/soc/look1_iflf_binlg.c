/*
 * File: look1_iflf_binlg.c
 *
 * Code generated for Simulink model 'Closed_Loop_NCM_29Ah_change'.
 *
 * Model version                  : 1.23
 * Simulink Coder version         : 8.7 (R2014b) 08-Sep-2014
 * C/C++ source code generated on : Tue Dec 09 11:14:15 2014
 */

#include "rtwtypes.h"
#include "look1_iflf_binlg.h"

real32_T look1_iflf_binlg(real32_T u0, const real32_T bp0[], const real32_T
  table[], uint32_T maxIndex)
{
  uint32_T iRght;
  uint32_T iLeft;
  uint32_T bpIdx;

  /* Lookup 1-D
     Search method: 'binary'
     Use previous index: 'off'
     Interpolation method: 'Linear'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'on'
   */
  /* Prelookup - Index and Fraction
     Index Search method: 'binary'
     Use previous index: 'off'
     Use last breakpoint for index at or above upper limit: 'off'
     Remove protection against out-of-range input in generated code: 'on'
   */
  /* Binary Search */
  bpIdx = (maxIndex >> 1U);
  iLeft = 0U;
  iRght = maxIndex;
  while ((iRght - iLeft) > 1U) {
    if (u0 < bp0[bpIdx]) {
      iRght = bpIdx;
    } else {
      iLeft = bpIdx;
    }

    bpIdx = ((iRght + iLeft) >> 1U);
  }

  /* Interpolation 1-D
     Interpolation method: 'Linear'
     Use last breakpoint for index at or above upper limit: 'off'
     Overflow mode: 'wrapping'
   */
  return table[iLeft] + (((u0 - bp0[iLeft]) / (bp0[iLeft + 1U] - bp0[iLeft])) *
                         (table[iLeft + 1U] - table[iLeft]));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
