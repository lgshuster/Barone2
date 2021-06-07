/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: PRH_PI.c
 *
 * Code generated for Simulink model 'PRH_PI'.
 *
 * Model version                  : 1.7
 * Simulink Coder version         : 9.5 (R2021a) 14-Nov-2020
 * C/C++ source code generated on : Tue May 11 09:10:05 2021
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Microchip->PIC18
 * Emulation hardware selection:
 *    Differs from embedded hardware (MATLAB Host)
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#include "PRH_PI.h"

/* Block signals and states (default storage) */
DW rtDW;

/* External inputs (root inport signals with default storage) */
ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
ExtY rtY;

/* Model step function */
void PRH_PI_step(void)
{
  real_T rtb_StateFeedbackPath[8];
  real_T rtb_Sum1[8];
  real_T tmp[8];
  real_T rtb_SquareRoot;
  real_T rtb_Sum1_n;
  real_T tmp_0;
  int32_T i;
  int32_T i_0;
  for (i = 0; i < 8; i++) {
    /* Sum: '<S1>/Sum1' incorporates:
     *  Gain: '<S1>/Foward Vector'
     *  Gain: '<S1>/Turn Vector'
     *  Inport: '<Root>/Foward'
     *  Inport: '<Root>/Turn'
     */
    rtb_Sum1_n = rtConstP.TurnVector_Gain[i] * rtU.Turn +
      rtConstP.FowardVector_Gain[i] * rtU.Foward;

    /* Abs: '<S1>/Abs' */
    rtb_StateFeedbackPath[i] = fabs(rtb_Sum1_n);

    /* Sum: '<S1>/Sum1' */
    rtb_Sum1[i] = rtb_Sum1_n;
  }

  /* MinMax: '<S1>/Max' */
  rtb_SquareRoot = rtb_StateFeedbackPath[0];
  for (i = 0; i < 7; i++) {
    rtb_SquareRoot = fmax(rtb_SquareRoot, rtb_StateFeedbackPath[i + 1]);
  }

  /* Outputs for IfAction SubSystem: '<S1>/Scale to <1' incorporates:
   *  ActionPort: '<S5>/Action Port'
   */
  for (i = 0; i < 8; i++) {
    /* If: '<S1>/If' incorporates:
     *  MinMax: '<S1>/Max'
     *  Product: '<S5>/Divide'
     */
    rtb_Sum1_n = rtb_Sum1[i];
    if (rtb_SquareRoot > 1.0) {
      rtb_Sum1_n /= rtb_SquareRoot;
    }

    /* Gain: '<S2>/State Feedback Gain' incorporates:
     *  Inport: '<Root>/PRH_States'
     */
    tmp[i] = 0.0;
    for (i_0 = 0; i_0 < 6; i_0++) {
      tmp[i] += rtConstP.StateFeedbackGain_Gain[(i_0 << 3) + i] *
        rtU.PRH_States[i_0];
    }

    /* End of Gain: '<S2>/State Feedback Gain' */

    /* Sum: '<S2>/Sum' incorporates:
     *  DiscreteIntegrator: '<S2>/Discrete-Time Integrator'
     *  Gain: '<S2>/Feedforward Gain'
     *  Gain: '<S2>/Integral Gain'
     */
    rtb_StateFeedbackPath[i] = (rtConstB.FeedforwardPath[i] -
      ((rtConstP.IntegralGain_Gain[i + 8] * rtDW.DiscreteTimeIntegrator_DSTATE[1]
        + rtConstP.IntegralGain_Gain[i] * rtDW.DiscreteTimeIntegrator_DSTATE[0])
       + rtConstP.IntegralGain_Gain[i + 16] *
       rtDW.DiscreteTimeIntegrator_DSTATE[2])) - tmp[i];

    /* If: '<S1>/If' incorporates:
     *  Product: '<S5>/Divide'
     */
    rtb_Sum1[i] = rtb_Sum1_n;
  }

  /* End of Outputs for SubSystem: '<S1>/Scale to <1' */

  /* Sqrt: '<S3>/Square Root' incorporates:
   *  Math: '<S3>/Square'
   *  Math: '<S3>/Square1'
   *  Math: '<S3>/Square2'
   *  Math: '<S3>/Square3'
   *  Math: '<S3>/Square4'
   *  Math: '<S3>/Square5'
   *  Math: '<S3>/Square6'
   *  Math: '<S3>/Square9'
   *  MinMax: '<S3>/Max'
   *  Sum: '<S3>/Sum'
   *  Sum: '<S3>/Sum1'
   *  Sum: '<S3>/Sum2'
   *  Sum: '<S3>/Sum3'
   */
  rtb_SquareRoot = sqrt(fmax(fmax(fmax(rtb_StateFeedbackPath[0] *
    rtb_StateFeedbackPath[0] + rtb_StateFeedbackPath[4] * rtb_StateFeedbackPath
    [4], rtb_StateFeedbackPath[1] * rtb_StateFeedbackPath[1] +
    rtb_StateFeedbackPath[5] * rtb_StateFeedbackPath[5]), rtb_StateFeedbackPath
    [2] * rtb_StateFeedbackPath[2] + rtb_StateFeedbackPath[6] *
    rtb_StateFeedbackPath[6]), rtb_StateFeedbackPath[3] * rtb_StateFeedbackPath
    [3] + rtb_StateFeedbackPath[7] * rtb_StateFeedbackPath[7]));

  /* If: '<S3>/If' incorporates:
   *  Inport: '<Root>/Max Force'
   */
  if (!(rtb_SquareRoot < rtU.MaxForce)) {
    /* Outputs for IfAction SubSystem: '<S3>/Saturation Adjustment' incorporates:
     *  ActionPort: '<S7>/Action Port'
     */
    /* Product: '<S7>/Product' incorporates:
     *  Merge: '<S3>/Merge'
     *  Product: '<S7>/Divide'
     *  Sum: '<S2>/Sum'
     */
    for (i = 0; i < 8; i++) {
      rtb_StateFeedbackPath[i] = rtb_StateFeedbackPath[i] / rtb_SquareRoot *
        rtU.MaxForce;
    }

    /* End of Product: '<S7>/Product' */
    /* End of Outputs for SubSystem: '<S3>/Saturation Adjustment' */
  }

  /* End of If: '<S3>/If' */

  /* Sum: '<S3>/Sum4' incorporates:
   *  Inport: '<Root>/Max Force'
   */
  rtb_SquareRoot = rtU.MaxForce - rtb_SquareRoot;

  /* Saturate: '<S3>/Saturation' */
  if (rtb_SquareRoot > 5.0) {
    rtb_SquareRoot = 5.0;
  } else if (rtb_SquareRoot < 0.0) {
    rtb_SquareRoot = 0.0;
  }

  /* End of Saturate: '<S3>/Saturation' */

  /* Outport: '<Root>/Closed Loop RC Cmd' incorporates:
   *  Merge: '<S3>/Merge'
   *  Product: '<S1>/Product'
   *  Sum: '<Root>/Sum'
   */
  for (i = 0; i < 8; i++) {
    rtY.ClosedLoopRCCmd[i] = rtb_Sum1[i] * rtb_SquareRoot +
      rtb_StateFeedbackPath[i];
  }

  /* End of Outport: '<Root>/Closed Loop RC Cmd' */

  /* Update for DiscreteIntegrator: '<S2>/Discrete-Time Integrator' incorporates:
   *  Inport: '<Root>/PRH_States'
   *  Sum: '<S2>/Sum1'
   */
  rtb_Sum1_n = rtDW.DiscreteTimeIntegrator_DSTATE[0];
  rtb_SquareRoot = rtDW.DiscreteTimeIntegrator_DSTATE[1];
  tmp_0 = rtDW.DiscreteTimeIntegrator_DSTATE[2];
  rtDW.DiscreteTimeIntegrator_DSTATE[0] = (0.0 - rtU.PRH_States[0]) * 0.1 +
    rtb_Sum1_n;
  rtDW.DiscreteTimeIntegrator_DSTATE[1] = (0.0 - rtU.PRH_States[2]) * 0.1 +
    rtb_SquareRoot;
  rtDW.DiscreteTimeIntegrator_DSTATE[2] = (1.0 - rtU.PRH_States[4]) * 0.1 +
    tmp_0;
}

/* Model initialize function */
void PRH_PI_initialize(void)
{
  /* (no initialization code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
