//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: PRH_PI.cpp
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
#include "PRH_PI.h"
#define NumBitsPerChar                 8U

extern real_T rt_atan2d_snf(real_T u0, real_T u1);

//===========*
//  Constants *
// ===========
#define RT_PI                          3.14159265358979323846
#define RT_PIF                         3.1415927F
#define RT_LN_10                       2.30258509299404568402
#define RT_LN_10F                      2.3025851F
#define RT_LOG10E                      0.43429448190325182765
#define RT_LOG10EF                     0.43429449F
#define RT_E                           2.7182818284590452354
#define RT_EF                          2.7182817F

//
//  UNUSED_PARAMETER(x)
//    Used to specify that a function parameter (argument) is required but not
//    accessed by the function body.

#ifndef UNUSED_PARAMETER
#if defined(__LCC__)
#define UNUSED_PARAMETER(x)                                      // do nothing
#else

//
//  This is the semi-ANSI standard way of indicating that an
//  unused function parameter is required.

#define UNUSED_PARAMETER(x)            (void) (x)
#endif
#endif

extern "C" {
  real_T rtInf;
  real_T rtMinusInf;
  real_T rtNaN;
  real32_T rtInfF;
  real32_T rtMinusInfF;
  real32_T rtNaNF;
}
  extern "C"
{
  //
  // Initialize rtNaN needed by the generated code.
  // NaN is initialized as non-signaling. Assumes IEEE.
  //
  static real_T rtGetNaN(void)
  {
    size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
    real_T nan = 0.0;
    if (bitsPerReal == 32U) {
      nan = rtGetNaNF();
    } else {
      union {
        LittleEndianIEEEDouble bitVal;
        real_T fltVal;
      } tmpVal;

      tmpVal.bitVal.words.wordH = 0xFFF80000U;
      tmpVal.bitVal.words.wordL = 0x00000000U;
      nan = tmpVal.fltVal;
    }

    return nan;
  }

  //
  // Initialize rtNaNF needed by the generated code.
  // NaN is initialized as non-signaling. Assumes IEEE.
  //
  static real32_T rtGetNaNF(void)
  {
    IEEESingle nanF = { { 0.0F } };

    nanF.wordL.wordLuint = 0xFFC00000U;
    return nanF.wordL.wordLreal;
  }
}

extern "C" {
  //
  // Initialize the rtInf, rtMinusInf, and rtNaN needed by the
  // generated code. NaN is initialized as non-signaling. Assumes IEEE.
  //
  static void rt_InitInfAndNaN(size_t realSize)
  {
    (void) (realSize);
    rtNaN = rtGetNaN();
    rtNaNF = rtGetNaNF();
    rtInf = rtGetInf();
    rtInfF = rtGetInfF();
    rtMinusInf = rtGetMinusInf();
    rtMinusInfF = rtGetMinusInfF();
  }

  // Test if value is infinite
  static boolean_T rtIsInf(real_T value)
  {
    return (boolean_T)((value==rtInf || value==rtMinusInf) ? 1U : 0U);
  }

  // Test if single-precision value is infinite
  static boolean_T rtIsInfF(real32_T value)
  {
    return (boolean_T)(((value)==rtInfF || (value)==rtMinusInfF) ? 1U : 0U);
  }

  // Test if value is not a number
  static boolean_T rtIsNaN(real_T value)
  {
    boolean_T result = (boolean_T) 0;
    size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
    if (bitsPerReal == 32U) {
      result = rtIsNaNF((real32_T)value);
    } else {
      union {
        LittleEndianIEEEDouble bitVal;
        real_T fltVal;
      } tmpVal;

      tmpVal.fltVal = value;
      result = (boolean_T)((tmpVal.bitVal.words.wordH & 0x7FF00000) ==
                           0x7FF00000 &&
                           ( (tmpVal.bitVal.words.wordH & 0x000FFFFF) != 0 ||
                            (tmpVal.bitVal.words.wordL != 0) ));
    }

    return result;
  }

  // Test if single-precision value is not a number
  static boolean_T rtIsNaNF(real32_T value)
  {
    IEEESingle tmp;
    tmp.wordL.wordLreal = value;
    return (boolean_T)( (tmp.wordL.wordLuint & 0x7F800000) == 0x7F800000 &&
                       (tmp.wordL.wordLuint & 0x007FFFFF) != 0 );
  }
}
  extern "C"
{
  //
  // Initialize rtInf needed by the generated code.
  // Inf is initialized as non-signaling. Assumes IEEE.
  //
  static real_T rtGetInf(void)
  {
    size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
    real_T inf = 0.0;
    if (bitsPerReal == 32U) {
      inf = rtGetInfF();
    } else {
      union {
        LittleEndianIEEEDouble bitVal;
        real_T fltVal;
      } tmpVal;

      tmpVal.bitVal.words.wordH = 0x7FF00000U;
      tmpVal.bitVal.words.wordL = 0x00000000U;
      inf = tmpVal.fltVal;
    }

    return inf;
  }

  //
  // Initialize rtInfF needed by the generated code.
  // Inf is initialized as non-signaling. Assumes IEEE.
  //
  static real32_T rtGetInfF(void)
  {
    IEEESingle infF;
    infF.wordL.wordLuint = 0x7F800000U;
    return infF.wordL.wordLreal;
  }

  //
  // Initialize rtMinusInf needed by the generated code.
  // Inf is initialized as non-signaling. Assumes IEEE.
  //
  static real_T rtGetMinusInf(void)
  {
    size_t bitsPerReal = sizeof(real_T) * (NumBitsPerChar);
    real_T minf = 0.0;
    if (bitsPerReal == 32U) {
      minf = rtGetMinusInfF();
    } else {
      union {
        LittleEndianIEEEDouble bitVal;
        real_T fltVal;
      } tmpVal;

      tmpVal.bitVal.words.wordH = 0xFFF00000U;
      tmpVal.bitVal.words.wordL = 0x00000000U;
      minf = tmpVal.fltVal;
    }

    return minf;
  }

  //
  // Initialize rtMinusInfF needed by the generated code.
  // Inf is initialized as non-signaling. Assumes IEEE.
  //
  static real32_T rtGetMinusInfF(void)
  {
    IEEESingle minfF;
    minfF.wordL.wordLuint = 0xFF800000U;
    return minfF.wordL.wordLreal;
  }
}

//
// Output and update for action system:
//    '<S1>/If Action Subsystem'
//    '<S1>/If Action Subsystem2'
//    '<S1>/If Action Subsystem4'
//    '<S1>/If Action Subsystem6'
//
void PRH_PIModelClass::IfActionSubsystem(real_T *rty_Out1)
{
  // SignalConversion generated from: '<S5>/Out1' incorporates:
  //   Constant: '<S5>/Constant'

  *rty_Out1 = 3.1415926535897931;
}

//
// Output and update for action system:
//    '<S1>/If Action Subsystem1'
//    '<S1>/If Action Subsystem3'
//    '<S1>/If Action Subsystem5'
//    '<S1>/If Action Subsystem7'
//
void PRH_PIModelClass::IfActionSubsystem1(real_T *rty_Out1)
{
  // SignalConversion generated from: '<S6>/Out1' incorporates:
  //   Constant: '<S6>/Constant'

  *rty_Out1 = 0.0;
}

real_T rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T y;
  int16_T u0_0;
  int16_T u1_0;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    if (u0 > 0.0) {
      u0_0 = 1;
    } else {
      u0_0 = -1;
    }

    if (u1 > 0.0) {
      u1_0 = 1;
    } else {
      u1_0 = -1;
    }

    y = std::atan2(static_cast<real_T>(u0_0), static_cast<real_T>(u1_0));
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = std::atan2(u0, u1);
  }

  return y;
}

// Model step function
void PRH_PIModelClass::step()
{
  real_T rtb_StateFeedbackPath[8];
  real_T rtb_Sum1[8];
  real_T tmp[8];
  real_T rtb_Merge2;
  real_T rtb_Merge3;
  real_T rtb_Square6;
  real_T rtb_Sum3;
  int16_T i;
  int16_T i_0;
  for (i = 0; i < 8; i++) {
    // Sum: '<S2>/Sum1' incorporates:
    //   Gain: '<S2>/Foward Vector'
    //   Gain: '<S2>/Turn Vector'
    //   Inport: '<Root>/Foward'
    //   Inport: '<Root>/Turn'

    rtb_Merge2 = rtConstP.TurnVector_Gain[i] * rtU.Turn +
      rtConstP.FowardVector_Gain[i] * rtU.Foward;

    // Abs: '<S2>/Abs'
    rtb_StateFeedbackPath[i] = std::abs(rtb_Merge2);

    // Sum: '<S2>/Sum1'
    rtb_Sum1[i] = rtb_Merge2;
  }

  // MinMax: '<S2>/Max'
  rtb_Merge3 = rtb_StateFeedbackPath[0];
  for (i = 0; i < 7; i++) {
    rtb_Merge2 = rtb_StateFeedbackPath[static_cast<int32_T>(i + 1)];
    if ((!(rtb_Merge3 > rtb_Merge2)) && (!rtIsNaN(rtb_Merge2))) {
      rtb_Merge3 = rtb_Merge2;
    }
  }

  // Outputs for IfAction SubSystem: '<S2>/Scale to <1' incorporates:
  //   ActionPort: '<S14>/Action Port'

  for (i = 0; i < 8; i++) {
    // If: '<S2>/If' incorporates:
    //   MinMax: '<S2>/Max'
    //   Product: '<S14>/Divide'

    rtb_Merge2 = rtb_Sum1[i];
    if (rtb_Merge3 > 1.0) {
      rtb_Merge2 /= rtb_Merge3;
    }

    // Gain: '<S3>/State Feedback Gain' incorporates:
    //   Inport: '<Root>/PRH_States'

    tmp[i] = 0.0;
    for (i_0 = 0; i_0 < 6; i_0++) {
      tmp[i] += rtConstP.StateFeedbackGain_Gain[(i_0 << 3) + i] *
        rtU.PRH_States[i_0];
    }

    // End of Gain: '<S3>/State Feedback Gain'

    // Sum: '<S3>/Sum' incorporates:
    //   DiscreteIntegrator: '<S3>/Discrete-Time Integrator'
    //   Gain: '<S3>/Feedforward Gain'
    //   Gain: '<S3>/Integral Gain'

    rtb_StateFeedbackPath[i] = (rtConstB.FeedforwardPath[i] -
      ((rtConstP.IntegralGain_Gain[i + 8] * rtDW.DiscreteTimeIntegrator_DSTATE[1]
        + rtConstP.IntegralGain_Gain[i] * rtDW.DiscreteTimeIntegrator_DSTATE[0])
       + rtConstP.IntegralGain_Gain[i + 16] *
       rtDW.DiscreteTimeIntegrator_DSTATE[2])) - tmp[i];

    // If: '<S2>/If' incorporates:
    //   Product: '<S14>/Divide'

    rtb_Sum1[i] = rtb_Merge2;
  }

  // End of Outputs for SubSystem: '<S2>/Scale to <1'

  // Sum: '<S4>/Sum1' incorporates:
  //   Math: '<S4>/Square4'
  //   Math: '<S4>/Square9'

  rtb_Merge3 = rtb_StateFeedbackPath[1] * rtb_StateFeedbackPath[1] +
    rtb_StateFeedbackPath[5] * rtb_StateFeedbackPath[5];

  // Sum: '<S4>/Sum2' incorporates:
  //   Math: '<S4>/Square2'
  //   Math: '<S4>/Square5'

  rtb_Merge2 = rtb_StateFeedbackPath[2] * rtb_StateFeedbackPath[2] +
    rtb_StateFeedbackPath[6] * rtb_StateFeedbackPath[6];

  // Sum: '<S4>/Sum3' incorporates:
  //   Math: '<S4>/Square3'
  //   Math: '<S4>/Square6'

  rtb_Square6 = rtb_StateFeedbackPath[3] * rtb_StateFeedbackPath[3] +
    rtb_StateFeedbackPath[7] * rtb_StateFeedbackPath[7];

  // Sum: '<S4>/Sum' incorporates:
  //   Math: '<S4>/Square'
  //   Math: '<S4>/Square1'

  rtb_Sum3 = rtb_StateFeedbackPath[0] * rtb_StateFeedbackPath[0] +
    rtb_StateFeedbackPath[4] * rtb_StateFeedbackPath[4];

  // MinMax: '<S4>/Max'
  if ((rtb_Sum3 > rtb_Merge3) || rtIsNaN(rtb_Merge3)) {
    rtb_Merge3 = rtb_Sum3;
  }

  if ((rtb_Merge3 > rtb_Merge2) || rtIsNaN(rtb_Merge2)) {
    rtb_Merge2 = rtb_Merge3;
  }

  if ((rtb_Merge2 > rtb_Square6) || rtIsNaN(rtb_Square6)) {
    rtb_Square6 = rtb_Merge2;
  }

  // End of MinMax: '<S4>/Max'

  // Sqrt: '<S4>/Square Root'
  rtb_Merge3 = std::sqrt(rtb_Square6);

  // Sum: '<S4>/Sum4' incorporates:
  //   Inport: '<Root>/Max Force'

  rtb_Sum3 = rtU.MaxForce - rtb_Merge3;

  // Saturate: '<S4>/Saturation'
  if (rtb_Sum3 > 5.0) {
    rtb_Sum3 = 5.0;
  } else if (rtb_Sum3 < 0.0) {
    rtb_Sum3 = 0.0;
  }

  // End of Saturate: '<S4>/Saturation'

  // Outputs for IfAction SubSystem: '<S4>/Saturation Adjustment' incorporates:
  //   ActionPort: '<S16>/Action Port'

  for (i = 0; i < 8; i++) {
    // If: '<S4>/If' incorporates:
    //   Inport: '<Root>/Max Force'
    //   Product: '<S16>/Divide'
    //   Product: '<S16>/Product'
    //   Product: '<S2>/Product'
    //   Sum: '<Root>/Sum'
    //   Sum: '<S3>/Sum'

    rtb_Merge2 = rtb_StateFeedbackPath[i];
    if (!(rtb_Merge3 < rtU.MaxForce)) {
      rtb_Merge2 = rtb_Merge2 / rtb_Merge3 * rtU.MaxForce;
    }

    rtb_StateFeedbackPath[i] = rtb_Sum1[i] * rtb_Sum3 + rtb_Merge2;

    // End of If: '<S4>/If'
  }

  // End of Outputs for SubSystem: '<S4>/Saturation Adjustment'

  // If: '<S1>/If'
  if (rtb_StateFeedbackPath[0] < 0.0) {
    // Outputs for IfAction SubSystem: '<S1>/If Action Subsystem' incorporates:
    //   ActionPort: '<S5>/Action Port'

    IfActionSubsystem(&rtb_Sum3);

    // End of Outputs for SubSystem: '<S1>/If Action Subsystem'
  } else {
    // Outputs for IfAction SubSystem: '<S1>/If Action Subsystem1' incorporates:
    //   ActionPort: '<S6>/Action Port'

    IfActionSubsystem1(&rtb_Sum3);

    // End of Outputs for SubSystem: '<S1>/If Action Subsystem1'
  }

  // End of If: '<S1>/If'

  // If: '<S1>/If1'
  if (rtb_StateFeedbackPath[1] < 0.0) {
    // Outputs for IfAction SubSystem: '<S1>/If Action Subsystem2' incorporates:
    //   ActionPort: '<S7>/Action Port'

    IfActionSubsystem(&rtb_Square6);

    // End of Outputs for SubSystem: '<S1>/If Action Subsystem2'
  } else {
    // Outputs for IfAction SubSystem: '<S1>/If Action Subsystem3' incorporates:
    //   ActionPort: '<S8>/Action Port'

    IfActionSubsystem1(&rtb_Square6);

    // End of Outputs for SubSystem: '<S1>/If Action Subsystem3'
  }

  // End of If: '<S1>/If1'

  // Outputs for IfAction SubSystem: '<S1>/If Action Subsystem5' incorporates:
  //   ActionPort: '<S10>/Action Port'

  // If: '<S1>/If2'
  IfActionSubsystem1(&rtb_Merge2);

  // End of Outputs for SubSystem: '<S1>/If Action Subsystem5'

  // Outputs for IfAction SubSystem: '<S1>/If Action Subsystem7' incorporates:
  //   ActionPort: '<S12>/Action Port'

  // If: '<S1>/If3'
  IfActionSubsystem1(&rtb_Merge3);

  // End of Outputs for SubSystem: '<S1>/If Action Subsystem7'

  // Outport: '<Root>/Angles' incorporates:
  //   Sum: '<S1>/Sum4'
  //   Trigonometry: '<S1>/Atan1'
  //   Trigonometry: '<S1>/Atan2'
  //   Trigonometry: '<S1>/Atan3'
  //   Trigonometry: '<S1>/Atan4'

  rtY.Angles[0] = rtb_Sum3 + rt_atan2d_snf(rtb_StateFeedbackPath[4],
    rtb_StateFeedbackPath[0]);
  rtY.Angles[1] = rtb_Square6 + rt_atan2d_snf(rtb_StateFeedbackPath[5],
    rtb_StateFeedbackPath[1]);
  rtY.Angles[2] = rtb_Merge2 + rt_atan2d_snf(rtb_StateFeedbackPath[6],
    rtb_StateFeedbackPath[2]);
  rtY.Angles[3] = rtb_Merge3 + rt_atan2d_snf(rtb_StateFeedbackPath[7],
    rtb_StateFeedbackPath[3]);

  // Outport: '<Root>/Throttles' incorporates:
  //   Inport: '<Root>/Max Force'
  //   Math: '<S1>/Square'
  //   Math: '<S1>/Square1'
  //   Math: '<S1>/Square2'
  //   Math: '<S1>/Square3'
  //   Math: '<S1>/Square4'
  //   Math: '<S1>/Square5'
  //   Math: '<S1>/Square6'
  //   Math: '<S1>/Square9'
  //   Product: '<S1>/Divide'
  //   Sum: '<S1>/Sum'
  //   Sum: '<S1>/Sum1'
  //   Sum: '<S1>/Sum2'
  //   Sum: '<S1>/Sum3'

  rtY.Throttles[0] = (rtb_StateFeedbackPath[0] * rtb_StateFeedbackPath[0] +
                      rtb_StateFeedbackPath[4] * rtb_StateFeedbackPath[4]) /
    rtU.MaxForce;
  rtY.Throttles[1] = (rtb_StateFeedbackPath[1] * rtb_StateFeedbackPath[1] +
                      rtb_StateFeedbackPath[5] * rtb_StateFeedbackPath[5]) /
    rtU.MaxForce;
  rtY.Throttles[2] = (rtb_StateFeedbackPath[2] * rtb_StateFeedbackPath[2] +
                      rtb_StateFeedbackPath[6] * rtb_StateFeedbackPath[6]) /
    rtU.MaxForce;
  rtY.Throttles[3] = (rtb_StateFeedbackPath[3] * rtb_StateFeedbackPath[3] +
                      rtb_StateFeedbackPath[7] * rtb_StateFeedbackPath[7]) /
    rtU.MaxForce;

  // Update for DiscreteIntegrator: '<S3>/Discrete-Time Integrator' incorporates:
  //   Inport: '<Root>/PRH_States'
  //   Sum: '<S3>/Sum1'

  rtb_Merge3 = rtDW.DiscreteTimeIntegrator_DSTATE[0];
  rtb_Merge2 = rtDW.DiscreteTimeIntegrator_DSTATE[1];
  rtb_Square6 = rtDW.DiscreteTimeIntegrator_DSTATE[2];
  rtDW.DiscreteTimeIntegrator_DSTATE[0] = (0.0 - rtU.PRH_States[0]) * 0.08 +
    rtb_Merge3;
  rtDW.DiscreteTimeIntegrator_DSTATE[1] = (0.0 - rtU.PRH_States[2]) * 0.08 +
    rtb_Merge2;
  rtDW.DiscreteTimeIntegrator_DSTATE[2] = (1.0 - rtU.PRH_States[4]) * 0.08 +
    rtb_Square6;
}

// Model initialize function
void PRH_PIModelClass::initialize()
{
  // Registration code

  // initialize non-finites
  rt_InitInfAndNaN(sizeof(real_T));
}

// Constructor
PRH_PIModelClass::PRH_PIModelClass() :
  rtU(),
  rtY(),
  rtDW()
{
  // Currently there is no constructor body generated.
}

// Destructor
PRH_PIModelClass::~PRH_PIModelClass()
{
  // Currently there is no destructor body generated.
}

//
// File trailer for generated code.
//
// [EOF]
//
