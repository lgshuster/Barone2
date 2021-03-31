#ifndef BUTTONS_H
#define BUTTONS_H

/**
 * @file
 * This library provides an interface to the 4 pushbuttons on the Digilent Basic IO shield for their
 * Uno32 development board platform..
 *
 * This library relies on continuously polling the pins connected to the pushbuttons. It then
 * provides an event-based interface for use. The resultant output are either UpEvents or DownEvents
 * corresponding to whether the button has been pushed-down or released.
 *
 * Be aware that the ButtonsInit() function exists for configuring the appropriate pins on the PIC
 * processor and must be done before ButtonsCheckEvents() will work.
 */

#include <stdint.h>
#include <genericTypeDefs.h>

// We rely on this file to define various macros for working with the hardware buttons.
#include "BOARD.h"

/**
 * Specify the minimum length of time between button events. This means that once ButtonsCheckEvents()
 * returns an event for a particular button, no events for that button will be called until ButtonsCheckEvents()
 * has been called 4 more times.
 */

/**
 * This enum{} lists all of the possible button events that could occur. Each event constants were
 * chosen so that multiple button events can be recorded in a single call to ButtonsCheckEvents().
 * 
 * All possible event flags will also fit into a char datatype.  This is to handle the rare situation
 * where two buttons change state simultaneously (or at least, within the frequency of the timer
 * interrupt).  So, a single char could indicate that button 1 was released at the same time that
 * while button 2 was pressed with (BUTTON_EVENT_1UP | BUTTON_EVENT_2DOWN)
 *
 */
/**
 * This function initializes the proper pins such that the buttons 1-4 may be used by modifying
 * the necessary bits in TRISD/TRISF. Only the bits necessary to enable the 1-4 buttons are
 * modified, so that this library does not interfere with other libraries.
 */
void Buttons_Init(void);

/**
 * ButtonsCheckEvents function checks the current button states and returns
 *  any events that have occured since its last
 * call.  This function should be called repeatedly in a Timer ISR, though it can
 * be called in main() during testing.
 *
 * In normal use, this function should only be used after ButtonsInit().
 * 
 * This function should assume that the buttons start in an off state with 
 * value 0. Therefore if no buttons are
 * pressed when ButtonsCheckEvents() is first called, 
 * BUTTONS_EVENT_NONE should be returned. 
 * 
 * @return  Each bit of the return value corresponds to one ButtonEvent flag,
 *          as described in Buttons.h.  If no events are detected, BUTTONS_EVENT_NONE is returned.
 * 
 * Note that more than one event can occur simultaneously, though this situation is rare.
 * To handle this, the output should be a bitwise OR of all applicable event flags. 
 * For example, if button 1 was released at the same time that button 2 was pressed,
 * this function should return (BUTTON_EVENT_1UP | BUTTON_EVENT_2DOWN) 
 * 
 */
uint8_t Buttons_Get(void);

#endif // BUTTONS_H
