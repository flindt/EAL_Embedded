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
#include <systick.h>

#include "../externalFunctions/itoa.h"

/* Constants used when writing strings to the display. */
#define mainCHARACTER_HEIGHT				( 9 )
#define mainMAX_ROWS_128					( mainCHARACTER_HEIGHT * 14 )
#define mainMAX_ROWS_96						( mainCHARACTER_HEIGHT * 10 )
#define mainMAX_ROWS_64						( mainCHARACTER_HEIGHT * 7 )
#define mainFULL_SCALE						( 15 )
#define ulSSI_FREQUENCY						( 3500000UL )

// Function prototypes
void
initHW(void);



int main(void) {
	volatile unsigned long ulLoop;
	char buffer[32] = "123";

	// Setup the ports and other hardware
	initHW();

	// Start the OLED display and write a message on it

	RIT128x96x4Init(ulSSI_FREQUENCY);
	RIT128x96x4StringDraw("Hello Out there :)", 0, 0, mainFULL_SCALE);

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

		itoa((int)SysTickValueGet(), buffer, 10 );
		//itoa(277, buffer, 10 );
		RIT128x96x4StringDraw(buffer, 0, 10, mainFULL_SCALE);

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


void
initHW(void)
{
  volatile unsigned long ulLoop;

  //initHW();

  SysCtlClockSet(
      SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

  // Enable the system tick
          SysTickPeriodSet(10000);
          SysTickEnable();


  // Enable the ports
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

  // Inputs :
  // PE0 : Up button
  // PE1 : Down button
  // PE2 : Left button
  // PE3 : Right button
  // PF1 : Select button
  //
  GPIODirModeSet(GPIO_PORTE_BASE,
      GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_DIR_MODE_IN);
  GPIOPadConfigSet(GPIO_PORTE_BASE,
      GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_2MA,
      GPIO_PIN_TYPE_STD_WPU);

  GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_DIR_MODE_IN);
  GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA,
      GPIO_PIN_TYPE_STD_WPU);

  // Outputs:
  // PF0 : Status LED
  GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_DIR_MODE_OUT);
  GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA,
      GPIO_PIN_TYPE_STD);
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 1);




  // a short delay to ensure stable IO before running the rest of the program
  for (ulLoop = 0; ulLoop < 200; ulLoop++)
    {
    }
}
