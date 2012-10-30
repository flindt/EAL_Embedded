/*
 * Light_led.c
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
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "drivers/rit128x96x4.h"
#include "driverlib/i2c.h"
#include "driverlib/interrupt.h"
#include <string.h>
#include "Led.h"


void led_init(void)
{
	 // Set the clocking to run directly from the crystal.
	  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);


	  // *****Initialize the OLED display.*****

	  //
	  RIT128x96x4Init(3500000);


	  // *****Initialize the Ports*****


	  //IC2											(port A)
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	  //											(port B)
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	  //											(port C)
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	  //up, down, left, right. 						(port E)
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	  //Select & LED 								(port F)
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	  //											(port D)
	  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);


	  //*****Initialize the buttons*****

	  // PE0 : Up button
	  // PE1 : Down button
	  // PE2 : Left button
	  // PE3 : Right button
	  // PF1 : Select button


	  //setting the buttons: Up button, Down button, Left button, Right button.
	  //which is on port E pins 0,1,2,3, as a input.
	  GPIODirModeSet(GPIO_PORTE_BASE,
	      GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_DIR_MODE_IN);


	  //setting the buttons: Up button, Down button, Left button, Right button.
	  //which is on port E pins 0,1,2,3, as a Push-pull with weak pull-up at 2MA Strength.
	  GPIOPadConfigSet(GPIO_PORTE_BASE,
	      GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_STRENGTH_2MA,
	      GPIO_PIN_TYPE_STD_WPU);


	  //setting the button: Select button  which is on port F pin 1 as a input.
	  GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_DIR_MODE_IN);


	  //setting the button: Select button  which is on port F pin 1 as-
	  // a Push-pull with weak pull-up at 2MA Strength.
	  GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA,
	      GPIO_PIN_TYPE_STD_WPU);


	 //*****Initialize the LED*****

	 //Setting the Led which is on Port F pin 0 as a output.
	  GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_DIR_MODE_OUT);


	 //Setting the Led which is on Port F pin 0 as a output.
	  GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3 , GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD);

	 //The Comand for writing to a pin (this example turns on the led)
	 //  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 1);


	  //*****Initialize the I2C*****

	  //enabling the I2C
	   SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
	  //enabling the I2C on port A pin 6 and 7.
	   GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7);



	 //Delay for a moment.
	  SysCtlDelay(80000);

	// Enable processor interrupts.
	IntMasterEnable();
}








//*****************************************************************************
//COPY PASTA
//// This function enables Itoa
//COPY PASTA
//*****************************************************************************
/* reverse:  reverse string s in place */
void
reverse(char s[])
{
  int i, j;
  char c;

  for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
    {
      c = s[i];
      s[i] = s[j];
      s[j] = c;
    }
}

/* itoa:  convert n to characters in s */
void
itoa(int n, char s[])
{
  int i, sign;

  if ((sign = n) < 0) /* record sign */
    n = -n; /* make n positive */
  i = 0;
  do
    { /* generate digits in reverse order */
      s[i++] = n % 10 + '0'; /* get next digit */
    }
  while ((n /= 10) > 0); /* delete it */
  if (sign < 0)
    s[i++] = '-';
  s[i] = '\0';
  reverse(s);
}
//*****************************************************************************




void i2c_init(void)
{
		//This function enables peripherals. At power-up, all peripherals are disabled; they must be
		//enabled in order to operate or respond to register reads/writes.s
		SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);


		// Enable i^2C on PortA and I2C on pin 6 and 7
		GPIOPinTypeI2C(GPIO_PORTA_BASE,GPIO_PIN_6 | GPIO_PIN_7);



		//false = 100kHz, true = 400kHz
		I2CMasterInitExpClk(I2C0_MASTER_BASE, SysCtlClockGet(), false);


}




//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************


