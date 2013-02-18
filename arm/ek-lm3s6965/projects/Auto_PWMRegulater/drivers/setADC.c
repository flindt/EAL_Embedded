
// Stdlib includes
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
/* Hardware library includes. */
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"

#include "driverlib/debug.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/timer.h"
#include "driverlib/gpio.h"

#include "../externalFunctions/itoa.h"
#include "setADC.h"
#include "rit128x96x4.h"

#define mainCHARACTER_HEIGHT				( 9 )
#define mainMAX_ROWS_128					( mainCHARACTER_HEIGHT * 14 )
#define mainMAX_ROWS_96						( mainCHARACTER_HEIGHT * 10 )
#define mainMAX_ROWS_64						( mainCHARACTER_HEIGHT * 7 )
#define mainFULL_SCALE						( 15 )
#define ulSSI_FREQUENCY						( 3500000UL )

void initADC()
{

	//
	// Enable the clock to the ADC module
	//
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC);

	//
	// Enable the first sample sequencer to capture the value of channel 0 when
	// the processor trigger occurs.
	//
	ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 0, 0,ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH1);
	ADCSequenceEnable(ADC0_BASE, 0);
}

int setADC (void)
	{
	unsigned long ulValue;
	//char buffer[32] = "";
	  //
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

	  /*debug show value
	  RIT128x96x4StringDraw("     ", 90, 88, mainFULL_SCALE);
	  itoa(ulValue, buffer, 10 );
	  RIT128x96x4StringDraw(buffer, 90, 88, mainFULL_SCALE);
	  */
    return ulValue;
	}
