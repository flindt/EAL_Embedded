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
#include "../externalFunctions/itoa.h"

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
#define NO_OFF_PPM_CHANNELS 8
const long int PWM_pulsewidth_ticks = 400000 / 640;
const long int PWM_ns_per_tick = 640;
const long int PPM_Frame_Length_ns = 20000000;
const long int PPM_Frame_Length_ticks = 20000000 / 640;
const long int PPM_No_Channels = NO_OFF_PPM_CHANNELS;
float PPM_Channel_values[NO_OFF_PPM_CHANNELS] = { 0.4, 0.5, 0.55, 0.5, 0.5, 0.5,
		0.5, 0.5 }; // Fill array with 0.5 - meaning 50 %
const int long PPM_minimum_period_ns = 1000 * 1000; // Define minimum and maximum time between pulses
const int long PPM_maximum_period_ns = 2000 * 1000;	// Each channel will be used to modulate between these
const int long PPM_minimum_period_ticks = (1000 * 1000) / 640;// Define minimum and maximum time between pulses
const int long PPM_maximum_period_ticks = (2000 * 1000) / 640;// Each channel will be used to modulate between these

// Stuff for PPM input capture
long int PPM_input_count[NO_OFF_PPM_CHANNELS] = { 0, 0, 0, 0, 0, 0, 0, 0 };

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
	char buffer[32];

	initHW();

	// Start the OLED display and write a message on it

	RIT128x96x4Init(ulSSI_FREQUENCY);
	RIT128x96x4StringDraw("Hi :)", 0, 0, mainFULL_SCALE);
	RIT128x96x4StringDraw("Doing PPM...", 50, 0, mainFULL_SCALE);

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

		for (ulLoop = 0; ulLoop < 8; ulLoop++) {
			RIT128x96x4StringDraw("               ", 50, 10*ulLoop, mainFULL_SCALE);
			itoa(PPM_input_count[ulLoop], buffer, 10);
			RIT128x96x4StringDraw(buffer, 50, 10*ulLoop, mainFULL_SCALE);
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

void initPWM_PPM() {
	///
	/// Set up PWM2 / PB0
	/// Period: 2.4ms	Ton: 0.4ms
	///

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);
	// Set the PWM clock for 6.25 MHz
	SysCtlPWMClockSet(SYSCTL_PWMDIV_32);

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
	PWMGenPeriodSet(PWM_BASE, PWM_GEN_1, 200);
	//
	// Set the pulse width of PWM2
	//
	PWMPulseWidthSet(PWM_BASE, PWM_OUT_2, 15);

	//
	// Start the timers in generator 1.
	//
	PWMGenEnable(PWM_BASE, PWM_GEN_1);
	//
	// Enable the output.
	// Invert it to match Trainer input
	PWMOutputState(PWM_BASE, PWM_OUT_2_BIT, true);
	PWMOutputInvert(PWM_BASE, PWM_OUT_2_BIT, true);

	// Set up interrupt for PWM load
	PWMGenIntTrigEnable(PWM_BASE, PWM_GEN_1,
			PWM_INT_CNT_LOAD | PWM_INT_CNT_ZERO);
	PWMIntEnable(PWM_BASE, PWM_INT_GEN_1);
	IntEnable(INT_PWM1);

}

void initTimer_Capture() {
	///
	/// Set up CCP0 / PD4 as Timer 0 Capture input
	///

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

	//
	// Configure timer 0 as capture.
	//
	TimerConfigure(TIMER0_BASE, TIMER_CFG_16_BIT_PAIR | TIMER_CFG_A_CAP_TIME);

	// Set output type of pin PD4
	GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_4);

	//
	// Set the count time (TimerA).
	//
	//TimerLoadSet(TIMER0_BASE, TIMER_A, 60000);

	//
	// Configure the counter (TimerB) to count both edges.
	//
	TimerControlEvent(TIMER0_BASE, TIMER_A, TIMER_EVENT_NEG_EDGE);
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

	initPWM_PPM();

	initTimer_Capture();

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
	static int i = 0;
	static long int TotalCount = 0;

	long int countThisChannel;
	long int thisInterrupt = 0;

	//
	// Clear the PWM interrupt.
	// For now we only check 1 interrupt source, as it is the only one enabled

	thisInterrupt = PWMGenIntStatus(PWM_BASE, PWM_GEN_1, true);
	if (thisInterrupt & PWM_INT_CNT_ZERO) {

		// This interrupt is just here to show how to use more than
		// one interrupt source for the same interrupt vector
		PWMGenIntClear(PWM_BASE, PWM_GEN_1, PWM_INT_CNT_ZERO);

	}
	if (thisInterrupt & PWM_INT_CNT_LOAD) {

		PWMGenIntClear(PWM_BASE, PWM_GEN_1, PWM_INT_CNT_LOAD);

		if (i == PPM_No_Channels) {
			PWMGenPeriodSet(PWM_BASE, PWM_GEN_1,
					PPM_Frame_Length_ticks - TotalCount);
			//
			// Set the pulse width of PWM2
			//
			PWMPulseWidthSet(PWM_BASE, PWM_OUT_2, PWM_pulsewidth_ticks);
			i = 0;
			TotalCount = 0;
		} else {
			countThisChannel = PPM_minimum_period_ticks
					+ PPM_Channel_values[i]
							* (PPM_maximum_period_ticks
									- PPM_minimum_period_ticks);
			//countThisChannel = 1200;
			PWMGenPeriodSet(PWM_BASE, PWM_GEN_1, countThisChannel);
			//
			// Set the pulse width of PWM2
			//
			PWMPulseWidthSet(PWM_BASE, PWM_OUT_2, PWM_pulsewidth_ticks);
			TotalCount += countThisChannel;
			i++;
		}

	}

	for (ulLoop = 0; ulLoop < 6; ulLoop++)
		; // Wait a little to make sure interrupt has cleared
}

void TIMER_0_IntHandler(void) {
	volatile long int ulLoop;

	long int thisInterrupt = 0;

	enum PPM_POSITIONS {
		PPMSTART, PPMCHANNEL1, PPMCHANNEL2, PPMCHANNEL3, PPMCHANNEL4, PPMCHANNEL5, PPMCHANNEL6, PPMCHANNEL7, PPMCHANNEL8, PPMMAXPOS
	};
	static long int ppmPosition = PPMSTART;
	static long int lastCount = 0;
	static long int timeOutCount = 0;
	long int thisCount = 0;
	long int deltaCount = 0;

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

		thisCount = TimerValueGet(TIMER0_BASE, TIMER_A);
		deltaCount = lastCount-thisCount;
		lastCount = thisCount;

		switch (ppmPosition) {
		case PPMSTART:
			if (timeOutCount >= 7) {
				ppmPosition = PPMCHANNEL1;
			}
			timeOutCount = 0;
			break;
		case PPMCHANNEL1:
		case PPMCHANNEL2:
		case PPMCHANNEL3:
		case PPMCHANNEL4:
		case PPMCHANNEL5:
		case PPMCHANNEL6:
		case PPMCHANNEL7:
		//case PPMCHANNEL8:

			// PPM_input_count = deltaCount;
			PPM_input_count[ppmPosition-1] = timeOutCount * 0xffff - (deltaCount );
			ppmPosition++;
			timeOutCount = 0;
			break;
		case PPMCHANNEL8:
			ppmPosition = PPMSTART;
			break;
		}
	}

	if (thisInterrupt & TIMER_TIMA_TIMEOUT) {
		TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

		timeOutCount++;
	}

	for (ulLoop = 0; ulLoop < 6; ulLoop++)
		; // Wait a little to make sure interrupt has cleared

	//
	// Turn off the LED.
	//
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
}
