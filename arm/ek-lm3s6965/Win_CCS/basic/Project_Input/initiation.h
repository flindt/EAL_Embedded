/*
 * initiation.h
 *
 *  Created on: 08/11/2012
 *      Author: Thomas
 */







//*****************************************************************************
//
//! \addtogroup adc_api
//! @{
//
//*****************************************************************************
#ifndef INITIATION_H_
#define INITIATION_H_



//functions----------------------------------------------------------------

//initializing the display as well as the ports.
extern void initiation(void);

//enables the itoa function
char* itoa(int value, char* result, int base) ;

// enables the i2c
extern void i2c_init(void);
// enables the adc
extern void adc_init(void);
//enables the timers: timer0 and the timer1
extern void timers_init(void);


//-------------------------------------------------------------------------





#endif /* INITIATION_H_ */


//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

