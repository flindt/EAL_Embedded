/*
 * Control.c
 *
 *  Created on: 21/08/2012
 *      Author: Thomas
 */
//*****************************************************************************
//
//! \addtogroup adc_api
//! @{
//
//*****************************************************************************

//includes------
#include "Led.h"
//#include "Functions.h"
#include "Control.h"
//--------------




void control(void)
{
//WRITE YOUR CODE IN HERE.





}




/**Command-list/guide..
 *
 * Delay(insert a number);
 * --delays in x seconds where the x is the number inserted.
 *
 * LED_CONTROL(int LED_NUMBER,int LED_COMMAND);
 * --LED_NUMBER can be one of the 7 leds and have to be written as follows:
 * --LED_X where x is the number of the led you wish to control (1-7).
 * --LED_COMMAND is the command you give which can be the following:
 * --OFF, LED_ON_VALUE_0 , LED_ON_VALUE_1 , LED_ON, LED_BLINK_VALUE_0, LED_BLINK_VALUE_1.
 * --The VALUE_0 connects to the values in BANK0.
 * --The VALUE_1 connects to the values in BANK1.
 *
 * CHANGE_BRIGHTNESS_VALUES(int value0,int value1);
 * --Changes the two stored brightness values.
 * --The two values MUST! be one of the following numbers: 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15.
 * --The value0 connects to the values in BANK0.
 * --The value1 connects to the values in BANK1.
 *
 * SET_TIMERS(int BANK,int FADE_ON_TIME,int FULLY_ON_TIME,int FADE_OFF_TIME,int FIRST_FULLY_OFF,int SECOND_FULLY_OFF);
 * --BANK decides if you want your new set timers saved in (BANK0 or BANK1)
 * --if you don't want a time to change just put in set_default as value fx:SET_TIMERS(set_default,set_default,set_default,etc...)
 * --each of the values MUST! be one of the following numbers: 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15.
 * -- each number is represent a time (see table below)
 * CODE TIME (ms)
 * 0	= 	0ms
 * 1	= 	64ms
 * 2	= 	128ms
 * 3	= 	192ms
 * 4	= 	256ms
 * 5	= 	384ms
 * 6	= 	512ms
 * 7	= 	768ms
 * 8	= 	1024ms
 * 9	= 	1536ms
 * 10	= 	2048ms
 * 11	= 	3072ms
 * 12	= 	4096ms
 * 13	= 	5760ms
 * 14	= 	8128ms
 * 15	= 	16320ms
 * --TIMER DEFINITIONS:
 *
 * --FADE_ON_TIME
 * defines the time from the fully-off state to the fully-on state (Brightness Values).
 * --FULLY_ON_TIME
 * defines the time spent at maximum intensity between the fade-on state and fade-off state.
 * --FADE_OFF_TIME
 * defines the time from the fully-on state to the fully-off state.
 * --FIRST_FULLY_OFF
 * define the time spent at zero.
 * --SECOND_FULLY_OFF
 * define the time spent at zero on second dive.
 *
 *FX
 *
 *FADE_ON_TIME			3
 *FULLY_ON_TIME			5
 *FADE_OFF_TIME			3
 *FIRST_FULLY_OFF		6
 *SECOND_FULLY_OFF		8
 *
 * would look like this if blinking:
 *
 *       ___   6  ___     8      ___   6  ___
 *      / 5 \____/ 5 \__________/ 5	\____/ 5 \__________  etc.....
 *      3   3    3   3          3   3    3   3
 *
 *
 */



//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
