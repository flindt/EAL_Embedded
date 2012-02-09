/*
 * main.c
 *
 *  Created on: Feb 8, 2012
 *      Author: pfl
 */
#include <stdio.h>
#include <hw_memmap.h>
#include <hw_types.h>
#include <hw_sysctl.h>
#include <gpio.h>
#include <grlib.h>
#include <sysctl.h>

int main( void )
{
	volatile unsigned long ulLoop;
	 //
				    // Enable the GPIO port that is used for the on-board LED.
				    //

	SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOF );

				    //
				    // Do a dummy read to insert a few cycles after enabling the peripheral.
				    //
	//
					        // Delay for a bit.
					        //
					        for(ulLoop = 0; ulLoop < 2000; ulLoop++)
					        {
					        }

				    //
				    // Enable the GPIO pin for the LED (PF0).  Set the direction as output, and
				    // enable the GPIO pin for digital function.
				    //

					GPIODirModeSet( GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_DIR_MODE_OUT );
					GPIOPadConfigSet( GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD );
					GPIOPinWrite( GPIO_PORTF_BASE, GPIO_PIN_0, 1 );
				    //
				    // Loop forever.
				    //
				    while(1)
				    {
				        //
				        // Turn on the LED.
				        //
				    	GPIOPinWrite( GPIO_PORTF_BASE, GPIO_PIN_0,  GPIO_PIN_0);
				        // GPIO_PORTF_DATA_R |= 0x01;

				        //
				        // Delay for a bit.
				        //
				        for(ulLoop = 0; ulLoop < 200000; ulLoop++)
				        {
				        }

				        //
				        // Turn off the LED.
				        //
				        GPIOPinWrite( GPIO_PORTF_BASE, GPIO_PIN_0,  0);

				        //
				        // Delay for a bit.
				        //
				        for(ulLoop = 0; ulLoop < 200000; ulLoop++)
				        {
				        }
				    }
}
