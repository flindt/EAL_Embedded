/*
 * inition.c
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
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "drivers/rit128x96x4.h"
#include "driverlib/i2c.h"
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"
#include <string.h>
#include <driverlib/timer.h>
#include <inttypes.h>
#include "initiation.h"


void initiation(void)
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
char* itoa(int value, char* result, int base) {
         // check that the base if valid
         if (base < 2 || base > 36) { *result = '\0'; return result; }

         char* ptr = result, *ptr1 = result, tmp_char;
         int tmp_value;

         do {
                 tmp_value = value;
                 value /= base;
                 *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
         } while ( value );

         // Apply negative sign
         if (tmp_value < 0) *ptr++ = '-';
         *ptr-- = '\0';
         while(ptr1 < ptr) {
                 tmp_char = *ptr;
                 *ptr--= *ptr1;
                 *ptr1++ = tmp_char;
         }
         return result;
 }

//*****************************************************************************
//COPY PASTA
//// This function enables Itoa
//COPY PASTA
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

		//Delay for a moment.
		SysCtlDelay(80000);

}



void adc_init(void)
{
	//
	// The ADC0 peripheral must be enabled for use.
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);


	//
	// Enable the first sample sequencer to capture the value of channel 0 when
	// the processor trigger occurs.
	//
	ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);



	//
	//(which base it conected to, sequence number, sequence step, config)
	// ADC_CTL_IE sens a interrupt when its completed.
	// ADC_CTL_END the step is defined as the last of a sequence.
	// ADC_CTL_CH0 selects the sampling channel.
	//
	ADCSequenceStepConfigure(ADC0_BASE, 0, 0,
	ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);


    //
    // Since sample sequence 0 is now configured, it must be enabled.
    //
	ADCSequenceEnable(ADC0_BASE, 0);

	 //Delay for a moment.
	SysCtlDelay(80000);


}


 void timers_init(void)
 {

		//
		// Enable the peripherals used by this example.
		//
		SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

		//
		// Configure the two 32-bit periodic timers.
		//
		TimerConfigure(TIMER0_BASE, TIMER_CFG_32_BIT_PER);
		TimerConfigure(TIMER1_BASE, TIMER_CFG_32_BIT_PER);


		// sets the time for each interrupt.
		TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/2);
		TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet());

		//
		// Setup the interrupts for the timer timeouts.
		//
		IntEnable(INT_TIMER0A);
		IntEnable(INT_TIMER1A);
		TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
		TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

		//
		// Enable the timers.
		//
		TimerEnable(TIMER0_BASE, TIMER_A);
		TimerEnable(TIMER1_BASE, TIMER_A);



 }




//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************


