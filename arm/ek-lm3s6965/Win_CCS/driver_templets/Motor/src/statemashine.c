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
		static int speed = 0;
		if (event == KEY0_EVENT)
		{
			RIT128x96x4Clear();
			RIT128x96x4StringDraw("Select Pressed", 0, 0, mainFULL_SCALE);
		}
		if (event == KEY1_EVENT)
		{
			RIT128x96x4Clear();
			RIT128x96x4StringDraw("Up Pressed", 0, 0, mainFULL_SCALE);
			speed + 1000;
			RIT128x96x4StringDraw("Speed + 1000", 0, 7, mainFULL_SCALE);
		}
		if (event == KEY2_EVENT)
		{
			RIT128x96x4Clear();
			RIT128x96x4StringDraw("Down Pressed", 0, 0, mainFULL_SCALE);
			speed - 1000;
			RIT128x96x4StringDraw("Speed - 1000", 0, 7, mainFULL_SCALE);

		}
		if (event == KEY3_EVENT)
		{
			RIT128x96x4Clear();
			RIT128x96x4StringDraw("Enter Pressed", 0, 0, mainFULL_SCALE);
			speed = 10000;
			RIT128x96x4StringDraw("Speed = 10000", 0, 7, mainFULL_SCALE);
		}
		if (event == KEY4_EVENT)
		{
			RIT128x96x4Clear();
			RIT128x96x4StringDraw("Cancel Pressed", 0, 0, mainFULL_SCALE);
			speed = 0;
			RIT128x96x4StringDraw("Speed = 0", 0, 7, mainFULL_SCALE);
		}
		return speed;
}
