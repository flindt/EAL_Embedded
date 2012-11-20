/*
 * main.c
 *
 *  Created on: Feb 10, 2012
 *      Author: pfl
 */

// Stdlib includes
#include <string.h>

/* Hardware library includes. */
#include <hw_memmap.h>
#include <hw_types.h>
#include <hw_sysctl.h>
#include <sysctl.h>
#include <gpio.h>
#include <pwm.h>
#include <timer.h>

// needed for interrupts
#include <interrupt.h>
#include <inttypes.h>
#include <hw_ints.h>

// project includes
#include "rit128x96x4.h"
#include "settings.h"
#include "../externalFunctions/itoa.h"
#include "../signals/signals.h"

/* Constants used when writing strings to the display. */
#define mainCHARACTER_HEIGHT				( 9 )
#define mainMAX_ROWS_128					( mainCHARACTER_HEIGHT * 14 )
#define mainMAX_ROWS_96						( mainCHARACTER_HEIGHT * 10 )
#define mainMAX_ROWS_64						( mainCHARACTER_HEIGHT * 7 )
#define mainFULL_SCALE						( 15 )
#define ulSSI_FREQUENCY						( 3500000UL )

// Stuff for the Key interface
const int KEY_PRESS_MINIMUM = 7;

// Stuff for PWM control
const long int PWM_pulsewidth_10Khz = 5000; 	//	PWM count for 10KHz. 50M / 5K = 10K (implies pwm clock divisor = 1)
const long int TimerAFreq = SAMPLE_RATE;				//	Timer frequency -> translates into sampling rate in this program


// Function prototypes
void
initHW(void);

//
// These enums are used as events, and for passing the matching bit for a certain key
// from ReadKeys to GetKeyEvents.
// NO_EVENT must be last.
enum KeyEvents {
	KEY_ENTER, KEY_CANCEL, KEY_UP, KEY_DOWN, NO_EVENT
};
int
ReadKeys(void);
int
GetKeyEvents(void);

// With this setup it would seem like main() must be the first function in this file, otherwise
// the wrong function gets called on reset.
int main(void) {
	unsigned long ulLoop;

	initHW();

	// Start the OLED display and write a message on it

	RIT128x96x4Init(ulSSI_FREQUENCY);
	RIT128x96x4StringDraw("Hi :)", 0, 0, mainFULL_SCALE);
	RIT128x96x4StringDraw("Doing PWM...", 50, 0, mainFULL_SCALE);

	//
	// Loop forever.
	//
	while (1) {

		// GPIO_PORTF_DATA_R |= 0x01;

		//
		// Delay for a bit.
		// This is BAD STYLE (tm) any embedded system should be either free-running or timer based
		for (ulLoop = 0; ulLoop < 900000; ulLoop++) {
		}

		//
		// Delay for a bit.
		//
		for (ulLoop = 0; ulLoop < 900000; ulLoop++) {
		}
	}

	return 0;
}

// This function translates a set of bits read from the key IO ports into key events.
// Adding more keys is a matter of adding a new enum value, and one reading
// one more port in ReadKeys()
int GetKeyEvents(void) {
	static int KeyPressFlag = 0;
	static int Count[NO_EVENT] = { 0 };
	int LoopCount;
	int RawKeys;

	RawKeys = ReadKeys();

	for (LoopCount = 0; LoopCount < NO_EVENT; LoopCount++) {
		if (RawKeys & (1 << LoopCount)) {
			Count[LoopCount]++;
			if ((Count[LoopCount] >= KEY_PRESS_MINIMUM)
					&& !(KeyPressFlag & (1 << LoopCount))) {
				KeyPressFlag |= (1 << LoopCount);
				Count[LoopCount] = 0;
				return LoopCount;
			}
		} else {
			KeyPressFlag &= ~(1 << LoopCount);
			Count[LoopCount] = 0;
		}

	}

	return NO_EVENT;
}

// This function will read the actual io  ports associated with each key
// This is the only function besides initHW, that will ever need to know
// where each button is connected
int ReadKeys(void) {
	int KeyBits = 0;
	if (!GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0))
		KeyBits |= 1 << KEY_UP;
	if (!GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1))
		KeyBits |= 1 << KEY_DOWN;
	if (!GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_2))
		KeyBits |= 1 << KEY_CANCEL;
	if (!GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3))
		KeyBits |= 1 << KEY_ENTER;
	return KeyBits;
}

void initPWM2_with_int() {
	///
	/// Set up PWM2 / PB0
	/// Period: 2.4ms	Ton: 0.4ms
	///

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
	// Set the PWM clock
	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

	GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_0);
	//
	// Configure the PWM generator for count down mode with immediate updates
	// to the parameters.
	//
	PWMGenConfigure(PWM_BASE, PWM_GEN_1,
			PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC | PWM_GEN_MODE_DBG_RUN);
	//
	// Set the period.
	//
	PWMGenPeriodSet(PWM_BASE, PWM_GEN_1, PWM_pulsewidth_10Khz);
	//
	// Set the pulse width of PWM2
	//
	PWMPulseWidthSet(PWM_BASE, PWM_OUT_2, PWM_pulsewidth_10Khz / 2);	// start at 50%

	//
	// Start the timers in generator 1.
	//
	PWMGenEnable(PWM_BASE, PWM_GEN_1);
	//
	// Enable the output.
	//
	PWMOutputState(PWM_BASE, PWM_OUT_2_BIT, true);
	// PWMOutputInvert(PWM_BASE, PWM_OUT_2_BIT, true);

	// Set up interrupt for PWM load
	PWMGenIntTrigEnable(PWM_BASE, PWM_GEN_1,
			PWM_INT_CNT_LOAD | PWM_INT_CNT_ZERO);
	PWMIntEnable(PWM_BASE, PWM_INT_GEN_1);
	IntEnable(INT_PWM1);

}

void initTimer1_SampleClock() {
	///
	/// Set up CCP0 / PD4 as Timer 0 Capture input
	///

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

	//
	// Configure timer 0.
	//
	TimerConfigure(TIMER0_BASE, TIMER_CFG_32_BIT_PER);

	// Set output type of pin PD4
	GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_4);

	//
	// Set the count time (TimerA).
	//
	TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet() / TimerAFreq);

	//
	// Configure the counter (TimerB) to count both edges.
	//
	// TimerControlEvent(TIMER0_BASE, TIMER_A, TIMER_EVENT_NEG_EDGE);

	//
	// Enable the timer.
	//
	TimerEnable(TIMER0_BASE, TIMER_A);

	// Set up interrupt for Timer capture
	TimerIntEnable(TIMER0_BASE, TIMER_CAPA_EVENT | TIMER_TIMA_TIMEOUT);
	IntEnable(INT_TIMER0A);

}

void initHW(void) {
	volatile unsigned long ulLoop;

	SysCtlClockSet(
			SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN
					| SYSCTL_XTAL_8MHZ);

	// Enable the ports
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
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
			GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,
			GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_PORTE_BASE,
			GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,
			GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA,
			GPIO_PIN_TYPE_STD_WPU);

	// Outputs:
	// PF0 : Status LED
	GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_DIR_MODE_OUT);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA,
			GPIO_PIN_TYPE_STD);
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 1);

	initPWM2_with_int();

	initTimer1_SampleClock();

	// a short delay to ensure stable IO before running the rest of the program
	for (ulLoop = 0; ulLoop < 20; ulLoop++) {
	}

	//
	// Enable processor interrupts.
	//
	IntMasterEnable();
}

//*****************************************************************************
//
// The PWM interrupt handler
//
//*****************************************************************************
void PWM_1_IntHandler(void) {
	volatile long int ulLoop;

	long int thisInterrupt = 0;

	//
	// Clear the PWM interrupt.
	//

	thisInterrupt = PWMGenIntStatus(PWM_BASE, PWM_GEN_1, true);

	// Each interrupt can have different causes
	if (thisInterrupt & PWM_INT_CNT_ZERO) {

		PWMGenIntClear(PWM_BASE, PWM_GEN_1, PWM_INT_CNT_ZERO);

	}
	if (thisInterrupt & PWM_INT_CNT_LOAD) {

		PWMGenIntClear(PWM_BASE, PWM_GEN_1, PWM_INT_CNT_LOAD);

	}

	for (ulLoop = 0; ulLoop < 6; ulLoop++)
		; // Wait a little to make sure interrupt has cleared
}

void TIMER_0_IntHandler(void) {
	volatile long int ulLoop;

	static float outputValue = 0.1;
	int PWMDuty;

	long int thisInterrupt = 0;

	//
	// Turn on the LED.
	//
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);

	//
	// Clear the PWM interrupt.
	// For now we only check 1 interrupt source, as it is the only one enabled

	thisInterrupt = TimerIntStatus(TIMER0_BASE, true);
	if (thisInterrupt & TIMER_CAPA_EVENT) {

		TimerIntClear(TIMER0_BASE, TIMER_CAPA_EVENT);

	}

	if (thisInterrupt & TIMER_TIMA_TIMEOUT) {

		TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

		// Actual signal processing is done in the "signals" module
		outputValue = calcNextOutputValue();

		// Limit the output, just in case the calcNextOutputValue has errors
		if (outputValue > 1) {
			outputValue = 1.0;
		}
		if (outputValue < 0) {
			outputValue = 0.0;
		}

		//
		// Set the pulse width of PWM2
		//
		PWMDuty = PWM_pulsewidth_10Khz * outputValue;
		PWMPulseWidthSet(PWM_BASE, PWM_OUT_2, PWMDuty);

	}

	for (ulLoop = 0; ulLoop < 6; ulLoop++)
		; // Wait a little to make sure interrupt has cleared

	//
	// Turn off the LED.
	//
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
}
