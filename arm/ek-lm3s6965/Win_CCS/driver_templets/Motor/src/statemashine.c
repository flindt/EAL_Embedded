/*
 ============================================================================
 Name        : statemashine.c
 Author      : Lars
 Version     :
 Copyright   : Your copyright notice
 Description : statemashine in C, Ansi-style
 ============================================================================
 */
#include "readkeys/readkeys.h"
#include "statemashine.h"

#include "externalFunctions/itoa.h"

#include "drivers/rit128x96x4.h"
#include "drivers/control.h"
#include "drivers/motor.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#define mainCHARACTER_HEIGHT				( 9 )
#define mainMAX_ROWS_128					( mainCHARACTER_HEIGHT * 14 )
#define mainMAX_ROWS_96						( mainCHARACTER_HEIGHT * 10 )
#define mainMAX_ROWS_64						( mainCHARACTER_HEIGHT * 7 	)
#define mainFULL_SCALE						( 15 )
#define ulSSI_FREQUENCY						( 3500000UL )

int statemashine(int event)
{
	static int Speed = 0;
	static int SetSpeed = 0;
	char buffer[32];

	if (event == KEY0_EVENT_SELECT)
	{
		RIT128x96x4Clear();
		RIT128x96x4StringDraw("Select Pressed", 0, 0, mainFULL_SCALE);
	}
	if (event == KEY1_EVENT_UP)
	{
		RIT128x96x4Clear();
		RIT128x96x4StringDraw("Up Pressed", 0, 0, mainFULL_SCALE);
		SetSpeed = 100 + SetSpeed;

		RIT128x96x4StringDraw("               ", 50, 50, mainFULL_SCALE);
		itoa(SetSpeed, buffer, 10);
		RIT128x96x4StringDraw(buffer, 50, 50, mainFULL_SCALE);
	}
	if (event == KEY2_EVENT_DOWN)
	{
		RIT128x96x4Clear();
		RIT128x96x4StringDraw("Down Pressed", 0, 0, mainFULL_SCALE);
		if(Speed != 0)
		{
			SetSpeed = SetSpeed - 100;
			RIT128x96x4StringDraw("               ", 50, 50, mainFULL_SCALE);
			itoa(SetSpeed, buffer, 10);
			RIT128x96x4StringDraw(buffer, 50, 50, mainFULL_SCALE);
		}
	}
	if (event == KEY3_EVENT_ENTER)
	{
		RIT128x96x4Clear();
		RIT128x96x4StringDraw("Enter Pressed", 0, 0, mainFULL_SCALE);
		Speed = SetSpeed;
		RIT128x96x4StringDraw("               ", 50, 50, mainFULL_SCALE);
		itoa(Speed, buffer, 10);
		RIT128x96x4StringDraw(buffer, 50, 50, mainFULL_SCALE);
	}
	if (event == KEY4_EVENT_CANCEL)
	{
		RIT128x96x4Clear();
		RIT128x96x4StringDraw("Cancel Pressed", 0, 0, mainFULL_SCALE);

		SetSpeed = 0;
		Speed = SetSpeed;

		RIT128x96x4StringDraw("               ", 50, 50, mainFULL_SCALE);
		itoa(Speed, buffer, 10);
		RIT128x96x4StringDraw(buffer, 50, 50, mainFULL_SCALE);
	}



	return Speed;
}
