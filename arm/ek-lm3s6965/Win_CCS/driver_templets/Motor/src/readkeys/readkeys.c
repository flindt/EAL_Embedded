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



/*
int Readkeys(int RawKeys)
{

	static int key_count[MAXBIT] = { 0,0,0,0,0,0,0,0 };

	int loop_counter;

		for( loop_counter = 0; loop_counter < MAXBIT; loop_counter++)
			{
				if( RawKeys & (1 << loop_counter) )
					{
						key_count[loop_counter] = key_count[loop_counter] + 1;
					}
				else
					{
						key_count[loop_counter] = 0;
					}
			}

		for( loop_counter = 0; loop_counter < MAXBIT; loop_counter++)
			{
				if( key_count[loop_counter] > 5 )
					{
						key_count[loop_counter] = 0;
						return loop_counter;
					}
			}


//	if (RawKeys == 1 <<KEY0BIT)
//		{
//		if(RawKeys == 5 << key0_count)
//			{
//				key0_count = 0;
//			return KEY0_EVENT;
//			}
//
//			key0_count++;
//		}
//
//	if (RawKeys == 1 <<KEY1BIT)
//			{
//			if(RawKeys == 5 << key1_count)
//				{
//					key1_count = 0;
//				return KEY1_EVENT;
//				}
//
//				key1_count++;
//			}

	return NO_EVENT;
}
*/

// Stuff for the Key interface

int GetKeyEvents(void)
{
	const int KEY_PRESS_MINIMUM = 5;
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
		KeyBits |= 1 << KEY0_EVENT;
	}
	if (!GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0))
	{
		KeyBits |= 1 << KEY1_EVENT;
	}
	if (!GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1))
	{
		KeyBits |= 1 << KEY2_EVENT;
	}
	if (!GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_2))
	{
		KeyBits |= 1 << KEY3_EVENT;
	}
	if (!GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3))
	{
		KeyBits |= 1 << KEY4_EVENT;
	}

	return KeyBits;
}
