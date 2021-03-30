
#ifndef FREERUNNINGTIMER_H
#define	FREERUNNINGTIMER_H

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/

/*******************************************************************************
 * PUBLIC FUNCTION PROTOTYPES                                                  *
 ******************************************************************************/

/**
 * @Function TIMERS_Init(void)
 * @param none
 * @return None.
 * @brief  Initializes the timer module */
void FreeRunningTimer_Init(void);

/**
 * Function: TIMERS_GetMilliSeconds
 * @param None
 * @return the current MilliSecond Count
   */
unsigned int FreeRunningTimer_GetMilliSeconds(void);

/**
 * Function: TIMERS_GetMicroSeconds
 * @param None
 * @return the current MicroSecond Count
   */
unsigned int FreeRunningTimer_GetMicroSeconds(void);
#endif	/* FREERUNNINGTIMER_H */

