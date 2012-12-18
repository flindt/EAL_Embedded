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

//prototyping and global variables.

void sampling(void);
unsigned long ulValue;
char buffer0[32] = "";
char buffer1[32] = "";




void main(void)
{

initiation();
adc_init();
timers_init();

sampling();

}


void sampling (void)
{



	 while (1)
	    {




	      RIT128x96x4StringDraw("     ", 50, 50, 15);
	      itoa(ulValue, buffer0, 10 );
	      RIT128x96x4StringDraw(buffer0, 50, 50, 15);
		 SysCtlDelay(80000);


	    }


}


void take_sample(void)
{
	 // Trigger the sample sequence.
	 //
	 ADCProcessorTrigger(ADC0_BASE, 0);
	 //
	 // Wait until the sample sequence has completed.
	 //
	 while(!ADCIntStatus(ADC0_BASE, 0, false))
	 {
	 }
	 //
	 // Read the value from the ADC.
	 //
	 ADCSequenceDataGet(ADC0_BASE, 0, &ulValue);


}


void Timer0IntHandler(void) {
	//
	// Clear the timer interrupt.
	//
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	take_sample();

}

//*****************************************************************************
//
// The interrupt handler for the first timer interrupt.
//
//*****************************************************************************
void Timer1IntHandler(void) {
	//
	// Clear the timer interrupt.
	//
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);


}





//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

