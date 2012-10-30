/*
 * Led.h
 *
 *  Created on: 23/08/2012
 *      Author: Thomas
 */
//*****************************************************************************
//
//! \addtogroup adc_api
//! @{
//
//*****************************************************************************
#ifndef LED_H_
#define LED_H_



//functions----------------------------------------------------------------

//initializing the display as well as the ports.
extern void led_init(void);

//enables the itoa function
extern void reverse(char s[]);
extern void itoa(int n, char s[]);
extern void i2c_init(void);

//-------------------------------------------------------------------------












#endif /* LED_H_ */


//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

