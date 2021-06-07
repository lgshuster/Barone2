/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: PRH_PI.h
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

#ifndef RTW_HEADER_PRH_PI_h_
#define RTW_HEADER_PRH_PI_h_
#include <math.h>
#ifndef PRH_PI_COMMON_INCLUDES_
#define PRH_PI_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* PRH_PI_COMMON_INCLUDES_ */

/* Model Code Variants */

/* Macros for accessing real-time model data structure */

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  real_T DiscreteTimeIntegrator_DSTATE[3];/* '<S2>/Discrete-Time Integrator' */
} DW;

/* Invariant block signals (default storage) */
typedef struct {
  const real_T FeedforwardPath[8];     /* '<S2>/Feedforward Gain' */
} ConstB;

/* Constant parameters (default storage) */
typedef struct {
  /* Expression: [-1 1 -1 1 0 0 0 0]
   * Referenced by: '<S1>/Turn Vector'
   */
  real_T TurnVector_Gain[8];

  /* Expression: [1 1 1 1 0 0 0 0]
   * Referenced by: '<S1>/Foward Vector'
   */
  real_T FowardVector_Gain[8];

  /* Expression: kp
   * Referenced by: '<S2>/State Feedback Gain'
   */
  real_T StateFeedbackGain_Gain[48];

  /* Expression: transpose(-1*kr)
   * Referenced by: '<S2>/Integral Gain'
   */
  real_T IntegralGain_Gain[24];
} ConstP;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real_T Foward;                       /* '<Root>/Foward' */
  real_T Turn;                         /* '<Root>/Turn' */
  real_T MaxForce;                     /* '<Root>/Max Force' */
  real_T PRH_States[6];                /* '<Root>/PRH_States' */
} ExtU;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real_T ClosedLoopRCCmd[8];           /* '<Root>/Closed Loop RC Cmd' */
} ExtY;

/* Block signals and states (default storage) */
extern DW rtDW;

/* External inputs (root inport signals with default storage) */
extern ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY rtY;
extern const ConstB rtConstB;          /* constant block i/o */

/* Constant parameters (default storage) */
extern const ConstP rtConstP;

/* Model entry point functions */
extern void PRH_PI_initialize(void);
extern void PRH_PI_step(void);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S2>/Motor Commands' : Unused code path elimination
 * Block '<Root>/RC Cmd' : Unused code path elimination
 * Block '<Root>/Saturated Cmd' : Unused code path elimination
 * Block '<Root>/Unsaturated Cmd' : Unused code path elimination
 * Block '<S2>/ZOH' : Eliminated since input and output rates are identical
 */

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
 * '<Root>' : 'PRH_PI'
 * '<S1>'   : 'PRH_PI/Open Loop RC CMD'
 * '<S2>'   : 'PRH_PI/PRH_PI'
 * '<S3>'   : 'PRH_PI/Saturator'
 * '<S4>'   : 'PRH_PI/Open Loop RC CMD/No scaling needed'
 * '<S5>'   : 'PRH_PI/Open Loop RC CMD/Scale to <1'
 * '<S6>'   : 'PRH_PI/Saturator/If Action Subsystem'
 * '<S7>'   : 'PRH_PI/Saturator/Saturation Adjustment'
 */
#endif                                 /* RTW_HEADER_PRH_PI_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
