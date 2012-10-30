/*
 * Functions.h
 *
 *  Created on: 27/10/2012
 *      Author: Thomas
 */
//*****************************************************************************
//
//! \addtogroup adc_api
//! @{
//
//*****************************************************************************





#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_


//lists--------------------------------------------------------------------

//the LED_NUMBER is used to point to the specific led that needs a command.
enum LED_NUMBER {LED_1 = 1 , LED_2 = 2 , LED_3 = 3 , LED_4 = 4, LED_5 = 5 , LED_6 = 6 , LED_7 = 7};
//LED_COMMAND contains all the commands available to use in the LED_CONTROL function.
enum LED_COMMAND {OFF, LED_ON_VALUE_0 , LED_ON_VALUE_1 , LED_ON, LED_BLINK_VALUE_0, LED_BLINK_VALUE_1};
//the BANKS hold two sets of information making to sets of setups available at once.
enum BANKS {BANK0,BANK1};

//-------------------------------------------------------------------------




//functions----------------------------------------------------------------

//the function used to make a delay in seconds.
extern void Delay(int delay_time);

//the function used to control the led's.
extern void LED_CONTROL(int LED_NUMBER,int LED_COMMAND);

//the function used to change the timer values, the values are saved in BANK0 or BANK1.
extern void SET_TIMERS(int BANK,int FADE_ON_TIME,int FULLY_ON_TIME,int FADE_OFF_TIME,int FIRST_FULLY_OFF,int SECOND_FULLY_OFF);

//the function used to change the intensity of the led's, the values are saved in BANK0 or BANK1.
extern void CHANGE_BRIGHTNESS_VALUES(int value0,int value1);

//-----------------------------------------------------------------------





//Variables--------------------------------------------------------------

// this int contains the hex-value of the default value for the timers.
extern int set_default;

//-----------------------------------------------------------------------




#endif /* FUNCTIONS_H_ */


//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
