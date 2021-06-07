//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: PRH_PI.h
//
// Code generated for Simulink model 'PRH_PI'.
//
// Model version                  : 1.9
// Simulink Coder version         : 9.5 (R2021a) 14-Nov-2020
// C/C++ source code generated on : Fri May 21 17:11:29 2021
//
// Target selection: ert.tlc
// Embedded hardware selection: Microchip->PIC18
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
// Validation result: Not run
//
#ifndef RTW_HEADER_PRH_PI_h_
#define RTW_HEADER_PRH_PI_h_
#include <stddef.h>
#include <cmath>
#include "rtwtypes.h"

// Model Code Variants

// Macros for accessing real-time model data structure
extern "C" {
  static real_T rtGetNaN(void);
  static real32_T rtGetNaNF(void);
}                                      // extern "C"
  extern "C"
{
  extern real_T rtInf;
  extern real_T rtMinusInf;
  extern real_T rtNaN;
  extern real32_T rtInfF;
  extern real32_T rtMinusInfF;
  extern real32_T rtNaNF;
  static void rt_InitInfAndNaN(size_t realSize);
  static boolean_T rtIsInf(real_T value);
  static boolean_T rtIsInfF(real32_T value);
  static boolean_T rtIsNaN(real_T value);
  static boolean_T rtIsNaNF(real32_T value);
  struct BigEndianIEEEDouble {
    struct {
      uint32_T wordH;
      uint32_T wordL;
    } words;
  };

  struct LittleEndianIEEEDouble {
    struct {
      uint32_T wordL;
      uint32_T wordH;
    } words;
  };

  struct IEEESingle {
    union {
      real32_T wordLreal;
      uint32_T wordLuint;
    } wordL;
  };
}                                      // extern "C"

extern "C" {
  static real_T rtGetInf(void);
  static real32_T rtGetInfF(void);
  static real_T rtGetMinusInf(void);
  static real32_T rtGetMinusInfF(void);
}                                      // extern "C"
  // Class declaration for model PRH_PI
  class PRH_PIModelClass
{
  // public data and function members
 public:
  // Block signals and states (default storage) for system '<Root>'
  struct DW {
    real_T DiscreteTimeIntegrator_DSTATE[3];// '<S3>/Discrete-Time Integrator'
  };

  // Invariant block signals (default storage)
  struct ConstB {
    real_T FeedforwardPath[8];         // '<S3>/Feedforward Gain'
  };

  // Constant parameters (default storage)
  struct ConstP {
    // Expression: [-1 1 -1 1 0 0 0 0]
    //  Referenced by: '<S2>/Turn Vector'

    real_T TurnVector_Gain[8];

    // Expression: [1 1 1 1 0 0 0 0]
    //  Referenced by: '<S2>/Foward Vector'

    real_T FowardVector_Gain[8];

    // Expression: kp
    //  Referenced by: '<S3>/State Feedback Gain'

    real_T StateFeedbackGain_Gain[48];

    // Expression: transpose(-1*kr)
    //  Referenced by: '<S3>/Integral Gain'

    real_T IntegralGain_Gain[24];
  };

  // External inputs (root inport signals with default storage)
  struct ExtU {
    real_T Foward;                     // '<Root>/Foward'
    real_T Turn;                       // '<Root>/Turn'
    real_T MaxForce;                   // '<Root>/Max Force'
    real_T PRH_States[6];              // '<Root>/PRH_States'
  };

  // External outputs (root outports fed by signals with default storage)
  struct ExtY {
    real_T Angles[4];                  // '<Root>/Angles'
    real_T Throttles[4];               // '<Root>/Throttles'
  };

  // External inputs
  ExtU rtU;

  // External outputs
  ExtY rtY;

  // model initialize function
  void initialize();

  // model step function
  void step();

  // Constructor
  PRH_PIModelClass();

  // Destructor
  ~PRH_PIModelClass();

  // private data and function members
 private:
  // Block signals and states
  DW rtDW;

  // private member function(s) for subsystem '<S1>/If Action Subsystem'
  static void IfActionSubsystem(real_T *rty_Out1);

  // private member function(s) for subsystem '<S1>/If Action Subsystem1'
  static void IfActionSubsystem1(real_T *rty_Out1);
}

;
extern const PRH_PIModelClass::ConstB rtConstB;// constant block i/o

// Constant parameters (default storage)
extern const PRH_PIModelClass::ConstP rtConstP;

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S3>/Motor Commands' : Unused code path elimination
//  Block '<Root>/RC Cmd' : Unused code path elimination
//  Block '<Root>/Saturated Cmd' : Unused code path elimination
//  Block '<Root>/Unsaturated Cmd' : Unused code path elimination
//  Block '<S3>/ZOH' : Eliminated since input and output rates are identical
//  Block '<Root>/Zero-Order Hold' : Eliminated since input and output rates are identical


//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'PRH_PI'
//  '<S1>'   : 'PRH_PI/Force to throttle//angle'
//  '<S2>'   : 'PRH_PI/Open Loop RC CMD'
//  '<S3>'   : 'PRH_PI/PRH_PI'
//  '<S4>'   : 'PRH_PI/Saturator'
//  '<S5>'   : 'PRH_PI/Force to throttle//angle/If Action Subsystem'
//  '<S6>'   : 'PRH_PI/Force to throttle//angle/If Action Subsystem1'
//  '<S7>'   : 'PRH_PI/Force to throttle//angle/If Action Subsystem2'
//  '<S8>'   : 'PRH_PI/Force to throttle//angle/If Action Subsystem3'
//  '<S9>'   : 'PRH_PI/Force to throttle//angle/If Action Subsystem4'
//  '<S10>'  : 'PRH_PI/Force to throttle//angle/If Action Subsystem5'
//  '<S11>'  : 'PRH_PI/Force to throttle//angle/If Action Subsystem6'
//  '<S12>'  : 'PRH_PI/Force to throttle//angle/If Action Subsystem7'
//  '<S13>'  : 'PRH_PI/Open Loop RC CMD/No scaling needed'
//  '<S14>'  : 'PRH_PI/Open Loop RC CMD/Scale to <1'
//  '<S15>'  : 'PRH_PI/Saturator/If Action Subsystem'
//  '<S16>'  : 'PRH_PI/Saturator/Saturation Adjustment'

#endif                                 // RTW_HEADER_PRH_PI_h_

//
// File trailer for generated code.
//
// [EOF]
//
