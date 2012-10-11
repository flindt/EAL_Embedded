/*
 * readkeys.c
 *
 *  Created on: 08/02/2012
 *      Author: Lars
 */
/*
 ============================================================================
 Name        : ReadKey_program.c
 Author      : Lars
 Version     :
 Copyright   : Your copyright notice
 Description : Input reading in C, Ansi-style
 ============================================================================
 */

#include "readkeys.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

// Stuff for the Key interface

int GetKeyEvents(void)
{
	const int KEY_PRESS_MINIMUM = 4;
	static int KeyPressFlag = 0;
	static int Count[NO_EVENT] = { 0 };

	int LoopCount;
	int RawKeys;

	RawKeys = ReadKeys();

	for(LoopCount = 0; LoopCount < NO_EVENT; LoopCount++)
		{
		if (RawKeys & (1 << LoopCount))
			{
				Count[LoopCount]++;
				if (( Count[LoopCount] >= KEY_PRESS_MINIMUM) && !(KeyPressFlag & (1<<LoopCount)))
					{
						KeyPressFlag |=  (1<<LoopCount);
						Count[LoopCount] = 0;
					return LoopCount;
					}
			}
		else
			{
				KeyPressFlag &=  ~(1<<LoopCount);
				Count[LoopCount] = 0;
			}


		}

	return NO_EVENT;
}

int ReadKeys(void)
{
	int KeyBits = 0;

	if (!GPIOPinRead( GPIO_PORTF_BASE, GPIO_PIN_1))
	{
		KeyBits |= 1 << KEY0_EVENT_SELECT;
	}
	if (!GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0))
	{
		KeyBits |= 1 << KEY1_EVENT_UP;
	}
	if (!GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1))
	{
		KeyBits |= 1 << KEY2_EVENT_DOWN;
	}
	if (!GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_2))
	{
		KeyBits |= 1 << KEY3_EVENT_ENTER;
	}
	if (!GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3))
	{
		KeyBits |= 1 << KEY4_EVENT_CANCEL;
	}

	return KeyBits;
}
