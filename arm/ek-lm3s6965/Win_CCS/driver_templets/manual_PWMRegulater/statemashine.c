/*
 * Example code for a very simple statemachine.
 *
 * Used in teaching C programming on embedded systems at EAL with modyfaksion from Lars
 *
 */
/*
 ============================================================================
 Name        : Statemashine.c
 Author      : Lars
 Version     :
 Copyright   : Your copyright notice
 Description : Statemashine to motor and LED outputs in C, Ansi-style
 ============================================================================
 */
//*****************************************************************************
//
//! \addtogroup StateMashine
//! @{
//
//*****************************************************************************
//Includes
#include "readkeys/readkeys.h"
#include "statemashine.h"

#include "externalFunctions/itoa.h"

#include "drivers/rit128x96x4.h"

#include "drivers/F_PWM.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
//Defines
#define mainCHARACTER_HEIGHT				( 9 )
#define mainMAX_ROWS_128					( mainCHARACTER_HEIGHT * 14 )
#define mainMAX_ROWS_96						( mainCHARACTER_HEIGHT * 10 )
#define mainMAX_ROWS_64						( mainCHARACTER_HEIGHT * 7 	)
#define mainFULL_SCALE						( 15 )
#define ulSSI_FREQUENCY						( 3500000UL )

#define SETSTEP 0.01
//! State definitions
//! Line 48 is where to set where in the Menu its starts up when you power up the statemashine
int TS_State = UPSTARTMENU;
char Rpct = 0;
char buffer[32];
/* The SM does not know anything about the system. This way it can be tested on a
 * different C compiler very easily.
 */
int statemashine( int event )
{


	static float T1set = 1;
	static float T2set = 0;
	static float setvalue = 0;

    int NextState = TS_State;

	//!there is 5 buttons to use in this statemashine its @param KEY?_EVENT_?
	//! and under them is what happen if you push it... every case end on break;
	switch( TS_State )
    {
	case UPSTARTMENU:
		switch (event)
		{
		case KEY0_EVENT_SELECT:
			break;

		case KEY1_EVENT_UP:
			NextState = LED;
			break;

		case KEY2_EVENT_DOWN:
			NextState = TEMP;
			break;

		case KEY3_EVENT_ENTER:
			break;

		case KEY4_EVENT_CANCEL:
			break;
		default:
			break;
		}
		break;

	case LED:
		switch (event)
		{
		case KEY0_EVENT_SELECT:
			break;

		case KEY1_EVENT_UP:
			//turn on led on the board
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);
			RIT128x96x4StringDraw("LED ON  ",				2,	77, mainFULL_SCALE);
			break;

		case KEY2_EVENT_DOWN:
			//turn on led off the board
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
			RIT128x96x4StringDraw("LED OFF ",				2,	77, mainFULL_SCALE);
			break;

		case KEY3_EVENT_ENTER:
			break;

		case KEY4_EVENT_CANCEL:
			NextState = UPSTARTMENU;
			break;
		default:
			break;
		}
		break;

	case TEMP:
		switch (event)
		{
		case KEY0_EVENT_SELECT:
			if(valuereturn == T1set)
				{
				RIT128x96x4StringDraw("T2on     ",			25,	86, mainFULL_SCALE);
					valuereturn = T2set;
					Rpct = T2set *100;
					itoa(Rpct, buffer, 10);
				}
				else
				{
				RIT128x96x4StringDraw("T1on     ",			25,	86, mainFULL_SCALE);
					valuereturn = T1set;
					Rpct = T1set *100;
					itoa(Rpct, buffer, 10);
				}
			break;

		case KEY1_EVENT_UP:
			NextState = FNIVEAU;
			setvalue = 0;
			Rpct = (T1set  * 100);
			break;

		case KEY2_EVENT_DOWN:
			NextState = SNIVEAU;
			setvalue = 0;
			Rpct = (T2set  * 100);
			break;

		case KEY3_EVENT_ENTER:
			break;

		case KEY4_EVENT_CANCEL:
			NextState = UPSTARTMENU;
			break;
		default:
			break;
		}
		break;

	case FNIVEAU:
		Rpct = setvalue *100;
		itoa(Rpct, buffer, 10);
		RIT128x96x4StringDraw(buffer,						25,	77, mainFULL_SCALE);

		Rpct = T1set *100;
		itoa(Rpct, buffer, 10);
		RIT128x96x4StringDraw(buffer, 						25,	86, mainFULL_SCALE);
	switch (event)
		{
		case KEY0_EVENT_SELECT:
			break;

		case KEY1_EVENT_UP:
			setvalue = SETSTEP + setvalue;
			itoa(setvalue, buffer, 10);
			Rpct = setvalue *100;

			break;

		case KEY2_EVENT_DOWN:
				setvalue = setvalue - SETSTEP;
				itoa(setvalue, buffer, 10);
				Rpct = (setvalue * 100);
			break;

		case KEY3_EVENT_ENTER:
			T1set = setvalue;
			Rpct = (T1set  * 100);
			itoa(Rpct, buffer, 10);
			break;

		case KEY4_EVENT_CANCEL:
			NextState = TEMP;
			setvalue = 0;
			break;
		default:
			break;
		}
		break;

	case SNIVEAU:
		Rpct = setvalue *100;
		itoa(Rpct, buffer, 10);
		RIT128x96x4StringDraw(buffer,						25,	77, mainFULL_SCALE);

		Rpct = T2set *100;
		itoa(Rpct, buffer, 10);
		RIT128x96x4StringDraw(buffer, 						25,	86, mainFULL_SCALE);
	switch (event)
		{
		case KEY0_EVENT_SELECT:
			break;

		case KEY1_EVENT_UP:
			setvalue = SETSTEP + setvalue;
			Rpct = setvalue *100;
			itoa(setvalue, buffer, 10);
			break;

		case KEY2_EVENT_DOWN:
			setvalue = setvalue - SETSTEP;
			Rpct = (setvalue * 100);
			itoa(setvalue, buffer, 10);
			break;

		case KEY3_EVENT_ENTER:
			T2set = setvalue;
			Rpct = (T2set  * 100);
			itoa(Rpct, buffer, 10);
			break;

		case KEY4_EVENT_CANCEL:
			NextState = TEMP;
			setvalue = 0;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	// The program should never get here !
    }
    if(setvalue >= 1)
		{
		setvalue = 0.99;
		}
    if(setvalue < 0.00)
		{
			setvalue = 0.000;
		}
    if (NextState != TS_State)//!line 249 is set want the state mashine do after it round one time ture the program after a state is changed
    {
    	OnExit(TS_State);//! line 251 befor it changes
		OnEnter(NextState);//! line 252 after it changes
        TS_State = NextState;
    }

    DoDisplay( TS_State, event);//!what the state mashine every time it run ture the program

return TS_State;
}

/* The 3 functions OnEnter, OnExit and Do are were all the fun happens.
 * Theee functions are written specifically for this system, and must be replaced
 * if the SM is used somewhere else - say on an embedded system.
 *
 * For simulating the SM a bunch of statements should be put here.
 */

void OnEnter( int State)
{
	RIT128x96x4Clear();
}

void OnExit( int State)
{

}


void DoDisplay( int State, int button)
{
	switch (State)// to show menu display at start i set it here
		{
		case UPSTARTMENU:
			RIT128x96x4StringDraw("LED Test",				2,	41, mainFULL_SCALE);
			RIT128x96x4StringDraw(" PWM Duty Control",		2,	49, mainFULL_SCALE);
			RIT128x96x4StringDraw("PWM Settings",			2,	57, mainFULL_SCALE);
			break;
		case LED:
			RIT128x96x4StringDraw("LED on  ",				2,	41, mainFULL_SCALE);
			RIT128x96x4StringDraw(" LED Test",				2,	49, mainFULL_SCALE);
			RIT128x96x4StringDraw("LED off ", 				2,	57, mainFULL_SCALE);
			RIT128x96x4StringDraw(" Cancel to Menu",			2,	65, mainFULL_SCALE);
			break;
		case TEMP:
			RIT128x96x4StringDraw("1th level", 				2,	41, mainFULL_SCALE);
			RIT128x96x4StringDraw(" Duty Settings ", 		2,	49, mainFULL_SCALE);
			RIT128x96x4StringDraw("2nd level", 				2,	57, mainFULL_SCALE);
			RIT128x96x4StringDraw(" Cancel to StartMenu", 	2,	65, mainFULL_SCALE);
			switch (button)
			{
			case KEY0_EVENT_SELECT:
				RIT128x96x4StringDraw(buffer, 				55,	86, mainFULL_SCALE);
				break;
			}
			break;
		case FNIVEAU:
			RIT128x96x4StringDraw("up   1pct", 			2,	41, mainFULL_SCALE);
			RIT128x96x4StringDraw(" 1th Niveau ", 			2,	49, mainFULL_SCALE);
			RIT128x96x4StringDraw("down 1pct", 			2,	57, mainFULL_SCALE);
			RIT128x96x4StringDraw("	Back = Cancel",					2,	65, mainFULL_SCALE);
			RIT128x96x4StringDraw("set     ",				0,	77, mainFULL_SCALE);
			RIT128x96x4StringDraw("on     ",				0,	86, mainFULL_SCALE);
			switch (button)
				{
				case KEY1_EVENT_UP:
					RIT128x96x4StringDraw("set     ",		0,	77, mainFULL_SCALE);
					RIT128x96x4StringDraw(buffer,			25,	77, mainFULL_SCALE);
					break;

				case KEY2_EVENT_DOWN:
					RIT128x96x4StringDraw("set     ",		0,	77, mainFULL_SCALE);
					RIT128x96x4StringDraw(buffer,			25, 77, mainFULL_SCALE);
					break;

				case KEY3_EVENT_ENTER:
					RIT128x96x4StringDraw("on     ",		0,	86, mainFULL_SCALE);
					RIT128x96x4StringDraw(buffer, 			25,	86, mainFULL_SCALE);
					break;
				}
			break;
		case SNIVEAU:
			RIT128x96x4StringDraw("up   1pct", 			2,	41, mainFULL_SCALE);
			RIT128x96x4StringDraw(" 2nd Niveau ", 			2,	49, mainFULL_SCALE);
			RIT128x96x4StringDraw("down 1pct", 			2,	57, mainFULL_SCALE);
			RIT128x96x4StringDraw("	Back = Cancel", 				2,	65, mainFULL_SCALE);
			RIT128x96x4StringDraw("set     ",				0,	77, mainFULL_SCALE);
			RIT128x96x4StringDraw("on     ",				0,	86, mainFULL_SCALE);
			switch (button)
				{
				case KEY1_EVENT_UP:
					RIT128x96x4StringDraw("set     ",		0,	77, mainFULL_SCALE);
					RIT128x96x4StringDraw(buffer,			25,	77, mainFULL_SCALE);
					break;

				case KEY2_EVENT_DOWN:
					RIT128x96x4StringDraw("set     ",		0,	77, mainFULL_SCALE);
					RIT128x96x4StringDraw(buffer,			25, 77, mainFULL_SCALE);
					break;

				case KEY3_EVENT_ENTER:
					RIT128x96x4StringDraw("on     ",		0,	86, mainFULL_SCALE);
					RIT128x96x4StringDraw(buffer, 			25,	86, mainFULL_SCALE);
					break;
				}
			break;
		default:
			break;
			// The program should never get here
		}
	switch (button)//!to see want button is pressed in all menus they are set here(debug)
		{
		case KEY0_EVENT_SELECT:
			RIT128x96x4StringDraw("Select Pressed",			0,	0, mainFULL_SCALE);
			break;

		case KEY1_EVENT_UP:
			RIT128x96x4StringDraw("Up Pressed    ",			0,	0, mainFULL_SCALE);
			break;

		case KEY2_EVENT_DOWN:
			RIT128x96x4StringDraw("Down Pressed  ",			0,	0, mainFULL_SCALE);
			break;

		case KEY3_EVENT_ENTER:
			RIT128x96x4StringDraw("Enter Pressed ",			0,	0, mainFULL_SCALE);
			break;

		case KEY4_EVENT_CANCEL:
			RIT128x96x4StringDraw("Cancel Pressed",			0,	0, mainFULL_SCALE);
			break;

		default:
			break;
		}
}
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
