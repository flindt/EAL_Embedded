
/*
 * main.c
 *
 *  Created on: Feb 10, 2012
 *      Author: pfl
 */

// Stdlib includes
#include <string.h>

/* Hardware library includes. */
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "inc/hw_ints.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include <inttypes.h>
#include "driverlib/gpio.h"
#include <driverlib/timer.h>

// project includes
#include "itoa.h"

/* Constants used when writing strings to the display. */
#define mainCHARACTER_HEIGHT				( 9 )
#define mainMAX_ROWS_128					( mainCHARACTER_HEIGHT * 14 )
#define mainMAX_ROWS_96						( mainCHARACTER_HEIGHT * 10 )
#define mainMAX_ROWS_64						( mainCHARACTER_HEIGHT * 7 )
#define mainFULL_SCALE						( 15 )
#define ulSSI_FREQUENCY						( 3500000UL )

// Stuff for the Key interface
const int KEY_PRESS_MINIMUM = 7;

// Function prototypes
void
initHW(void);

//
// Theese enums are used as events, and for passing the matching bit for a certain key
// from ReadKeys to GetKeyEvents.
// NO_EVENT must be last.
enum KeyEvents {
	KEY_ENTER, KEY_CANCEL, KEY_UP, KEY_DOWN, NO_EVENT
};
int
ReadKeys(void);
int
GetKeyEvents(void);

//
// Global variables - Keep these to a minimum
//
unsigned long SecondsCount;
unsigned long minutesCount;

// With this setup it would seem like main() must be the first function in this file, otherwise
// the wrong function gets called on reset.
int main(void) {
	unsigned long ulLoop;
	char buffer[32];

	SecondsCount = 0;
	minutesCount = 0;

	initHW();

	// Start the OLED display and write a message on it

	RIT128x96x4Init(ulSSI_FREQUENCY);
	RIT128x96x4StringDraw("Hi :)", 0, 0, mainFULL_SCALE);
	RIT128x96x4StringDraw("Running Timers...", 0, 10, mainFULL_SCALE);

	//
	// Loop forever.
	//
	while (1) {

		//
		// Turn on the LED.
		//
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);
		// GPIO_PORTF_DATA_R |= 0x01;

		RIT128x96x4StringDraw("               ", 50, 50, mainFULL_SCALE);
		itoa(SecondsCount, buffer, 10);
		RIT128x96x4StringDraw(buffer, 50, 50, mainFULL_SCALE);

		RIT128x96x4StringDraw("               ", 50, 60, mainFULL_SCALE);
		itoa(minutesCount, buffer, 10);
		RIT128x96x4StringDraw(buffer, 50, 60, mainFULL_SCALE);

		SysCtlSleep();

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

void initTimers(void) {
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
	TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet());
	TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet() / 2);

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

	initTimers();

	// a short delay to ensure stable IO before running the rest of the program
	for (ulLoop = 0; ulLoop < 200; ulLoop++) {
	}

	//
	// Enable processor interrupts.
	//
	IntMasterEnable();
}

//*****************************************************************************
//
// The interrupt handler for the first timer interrupt.
//
//*****************************************************************************
void Timer0IntHandler(void) {
	//
	// Clear the timer interrupt.
	//
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	SecondsCount++;
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

	minutesCount++;
}
