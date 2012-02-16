/*
 * main.c
 *
 *  Created on: Feb 10, 2012
 *      Author: pfl
 */


#include "rit128x96x4.h"

/* Hardware library includes. */
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_sysctl.h"
#include "sysctl.h"
#include "gpio.h"

/* Constants used when writing strings to the display. */
#define mainCHARACTER_HEIGHT				( 9 )
#define mainMAX_ROWS_128					( mainCHARACTER_HEIGHT * 14 )
#define mainMAX_ROWS_96						( mainCHARACTER_HEIGHT * 10 )
#define mainMAX_ROWS_64						( mainCHARACTER_HEIGHT * 7 )
#define mainFULL_SCALE						( 15 )
#define ulSSI_FREQUENCY						( 3500000UL )

int main(void) {
	volatile unsigned long ulLoop;

	SysCtlClockSet(
			SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN
					| SYSCTL_XTAL_8MHZ);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIODirModeSet( GPIO_PORTF_BASE, GPIO_PIN_1,GPIO_DIR_MODE_IN );
	GPIOPadConfigSet( GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU );

	GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_DIR_MODE_OUT);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA,
				GPIO_PIN_TYPE_STD);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 1);


	for (ulLoop = 0; ulLoop < 200; ulLoop++) {
	}

	//
	// Enable the GPIO pin for the LED (PF0).  Set the direction as output, and
	// enable the GPIO pin for digital function.
	//

	// Start the OLED display and wirte a message on it

	RIT128x96x4Init(ulSSI_FREQUENCY);
	RIT128x96x4StringDraw("Hi :)", 0, 0, mainFULL_SCALE);
	RIT128x96x4StringDraw("Press the ""select"" key..", 0, 5, mainFULL_SCALE);

	// Wait for the select key to be pressed
	//GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_1);


	while( GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1) );

	//
	// Loop forever.
	//
	while (1) {
		//
		// Turn on the LED.
		//
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);
		// GPIO_PORTF_DATA_R |= 0x01;

		//
		// Delay for a bit.
		//
		for (ulLoop = 0; ulLoop < 200000; ulLoop++) {
		}

		//
		// Turn off the LED.
		//
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);

		//
		// Delay for a bit.
		//
		for (ulLoop = 0; ulLoop < 200000; ulLoop++) {
		}
	}

	return 0;
}
