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

// Project includes
#include "events.h"


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


// With this setup it would seem like main() must be the first function in this file, otherwise
// the wrong function gets called on reset.
int
main(void)
{
  volatile unsigned long ulLoop;
  volatile int event;

  initHW();

  // Testing
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
  GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, GPIO_PIN_1);

  // Start the OLED display and write a message on it

  RIT128x96x4Init(ulSSI_FREQUENCY);
  RIT128x96x4StringDraw("Im godZillA", 0, 0, mainFULL_SCALE);

  //
  // Loop forever.
  //
  while (1)
    {

      // This is where a statemachine could be added
      event = GetKeyEvents();

      switch( event )
      {
      case KEY_ENTER:
    	  	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
    	    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
    	    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
    	    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_1, 0);
    	    break;
      default:
    	  break;
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

  // Enable the ports
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
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
  // PF0 : Motor 1 enable
  // PF2 : Motor 1 direction
  // PF3 : Motor 2 direction
  // PD1 : Motor 2 enable
  GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_DIR_MODE_OUT);
  GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_2MA,
      GPIO_PIN_TYPE_STD);

  GPIODirModeSet(GPIO_PORTD_BASE, GPIO_PIN_1 , GPIO_DIR_MODE_OUT);
  GPIOPadConfigSet(GPIO_PORTD_BASE, GPIO_PIN_1 , GPIO_STRENGTH_2MA,
        GPIO_PIN_TYPE_STD);

  // a short delay to ensure stable IO before running the rest of the program
  for (ulLoop = 0; ulLoop < 200; ulLoop++)
    {
    }
}
