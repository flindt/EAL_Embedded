/*

EAL-Erhvervs Akademiet Lillebealt.
Instructor: Peder.
Author: Thomas Egmont Bagge Henriksen.
Date start:21-08-2012.

 */
//*****************************************************************************
//
//! \addtogroup adc_api
//! @{
//
//*****************************************************************************

#include "Led.h"
#include "Control.h"
#include "Functions.h"

void main(void)
{
	led_init(); 																   //The startup initiating the ports and pins and other startup-processes.
	i2c_init();																	   //Sets up the I^2C.
	SET_TIMERS(BANK0,set_default,set_default,set_default,set_default,set_default); //sets up the timers to default values in bank0.
	SET_TIMERS(BANK1,set_default,set_default,set_default,set_default,set_default); //sets up the timers to default values in bank1.
	control();																	   //runs the users commands.
}

/*

  A
 	1. Master sends a START condition.

	2. Master sends the slave address with a write operation (1000 1010).

	3. Master sends a command byte that points 1 of the 11 registers in this device.

	4. Master sends data to the TCA6507 register(s).

	5. Master sends a STOP condition.



 */

















//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

