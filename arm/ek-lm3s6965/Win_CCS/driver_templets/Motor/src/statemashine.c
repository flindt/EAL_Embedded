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

#include "drivers/motor.h"

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

//! State definitions
//! Line 48 is where to set where in the Menu its starts up when you power up the statemashine
int TS_State = UPSTARTMENU;

/* The SM does not know anything about the system. This way it can be tested on a
 * different C compiler very easily.
 */
int statemashine( int event )
{
    int NextState = TS_State;

    volatile unsigned long ulLoop;

	static int Speed = 0;
	static int SetSpeed = 0;

	char buffer[32];
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
			NextState = MOVEFOBMOTOR;
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
			NextState = UPSTARTMENU;
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
			NextState = UPSTARTMENU;
			break;

		case KEY4_EVENT_CANCEL:
			NextState = UPSTARTMENU;
			break;
		default:
			break;
		}
		break;

	case MOTOR:
		switch (event)
		{
		case KEY0_EVENT_SELECT:
			NextState = UPSTARTMENU;
			break;

		case KEY1_EVENT_UP:
			NextState = MOVEFOBMOTOR;
			break;

		case KEY2_EVENT_DOWN:
			NextState = HASTMOTOR;
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

	case HASTMOTOR:
		RIT128x96x4StringDraw("set        ",				0,	77, mainFULL_SCALE);
		itoa(SetSpeed, buffer, 10);
		RIT128x96x4StringDraw(buffer,						25,	77, mainFULL_SCALE);
		RIT128x96x4StringDraw("on               ",			0,	86, mainFULL_SCALE);
		itoa(Speed, buffer, 10);
		RIT128x96x4StringDraw(buffer, 						25,	86, mainFULL_SCALE);
		switch (event)
		{
		case KEY0_EVENT_SELECT:
			NextState = MOTOR;
			break;

		case KEY1_EVENT_UP:
			SetSpeed = 100 + SetSpeed;
			RIT128x96x4StringDraw("set             ",		0,	77, mainFULL_SCALE);
			itoa(SetSpeed, buffer, 10);
			RIT128x96x4StringDraw(buffer,					25,	77, mainFULL_SCALE);
			break;

		case KEY2_EVENT_DOWN:
			if(Speed != 0||SetSpeed !=0)
			{
				SetSpeed = SetSpeed - 100;
				RIT128x96x4StringDraw("set          ",		0,	77, mainFULL_SCALE);
				itoa(SetSpeed, buffer, 10);
				RIT128x96x4StringDraw(buffer,				25, 77, mainFULL_SCALE);
			}
			break;

		case KEY3_EVENT_ENTER:
			Speed = SetSpeed;
			RIT128x96x4StringDraw("on         ",			0,	86, mainFULL_SCALE);
			itoa(Speed, buffer, 10);
			RIT128x96x4StringDraw(buffer, 					25,	86, mainFULL_SCALE);
			break;

		case KEY4_EVENT_CANCEL:
			SetSpeed = 0;
			Speed = SetSpeed;
			RIT128x96x4StringDraw("on         ",			0,	86, mainFULL_SCALE);
			itoa(Speed, buffer, 10);
			RIT128x96x4StringDraw(buffer,					25,	86, mainFULL_SCALE);
			break;
		default:
			break;
		}
		break;

	case MOVEFOBMOTOR:
		RIT128x96x4StringDraw("on               ",			0,	86, mainFULL_SCALE);
		itoa(Speed, buffer, 10);
		RIT128x96x4StringDraw(buffer, 						25,	86, mainFULL_SCALE);
		switch (event)
		{
		case KEY0_EVENT_SELECT:
			NextState = UPSTARTMENU;
			break;

		case KEY1_EVENT_UP:
			if(Speed != SetSpeed)
			{
				Speed = SetSpeed;
			}
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
			  for (ulLoop = 0; ulLoop < 20; ulLoop++)
			    {
			    }
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
			break;

		case KEY2_EVENT_DOWN:
			if(Speed != SetSpeed)
			{
				Speed = SetSpeed;
			}
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
			  for (ulLoop = 0; ulLoop < 20; ulLoop++)
			    {
			    }
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
			break;

		case KEY3_EVENT_ENTER:
			NextState = MOTOR;
			break;

		case KEY4_EVENT_CANCEL:
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
			Speed = 0;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	// The program should never get here !
    }

    if (NextState != TS_State)//!line 249 is set want the state mashine do after it round one time ture the program after a state is changed
    {
    	OnExit(TS_State);//! line 251 befor it changes
		OnEnter(NextState);//! line 252 after it changes
        TS_State = NextState;
    }

    SetMotorParams( TS_State, Speed, event);//!what the state mashine every time it run ture the program

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
	switch (State)
		{
			case UPSTARTMENU:
				RIT128x96x4StringDraw("LED Control",			2,	41, mainFULL_SCALE);
				RIT128x96x4StringDraw(" WelCome To App Home",	2,	49, mainFULL_SCALE);
				RIT128x96x4StringDraw("Curtian Control",		2,	57, mainFULL_SCALE);
				break;
			case LED:
				RIT128x96x4StringDraw("LED on  ",				2,	41, mainFULL_SCALE);
				RIT128x96x4StringDraw(" Motor Control ",		2,	49, mainFULL_SCALE);
				RIT128x96x4StringDraw("LED off ", 				2,	57, mainFULL_SCALE);
				RIT128x96x4StringDraw("Cancel to Menu",			2,	65, mainFULL_SCALE);
				break;
			case MOTOR:
				RIT128x96x4StringDraw("Curtain O/C", 			2,	41, mainFULL_SCALE);
				RIT128x96x4StringDraw(" Motor Control ", 		2,	49, mainFULL_SCALE);
				RIT128x96x4StringDraw("Movement speed", 		2,	57, mainFULL_SCALE);
				RIT128x96x4StringDraw("Cancel to StartMenu", 	2,	65, mainFULL_SCALE);
				break;
			case HASTMOTOR:
				RIT128x96x4StringDraw("Select to MotorMenu", 	2,	24, mainFULL_SCALE);
				RIT128x96x4StringDraw("Enter SAVE value", 		2,	32, mainFULL_SCALE);
				RIT128x96x4StringDraw("Up   +100 to PWM", 		2,	41, mainFULL_SCALE);
				RIT128x96x4StringDraw(" Motor Control ", 		2,	49, mainFULL_SCALE);
				RIT128x96x4StringDraw("Down -100 to PWM", 		2,	57, mainFULL_SCALE);
				RIT128x96x4StringDraw("Cancel makes PWM to 0", 	2,	65, mainFULL_SCALE);
				break;
			case MOVEFOBMOTOR:
				RIT128x96x4StringDraw("Up          ", 			2,	41, mainFULL_SCALE);
				RIT128x96x4StringDraw(" Curtian O/C", 			2,	49, mainFULL_SCALE);
				RIT128x96x4StringDraw("Down        ", 			2,	57, mainFULL_SCALE);
				RIT128x96x4StringDraw("Cancel to Stop",			2,	65, mainFULL_SCALE);
				break;
			default:
				break;
			// The program should never get here !
		}
}

void OnExit( int State)
{

}


void SetMotorParams( int State,int returnspeed, int button)
{
	switch (State)// to show menu display at start i set it here
		{
			case UPSTARTMENU:
				RIT128x96x4StringDraw("LED Control",			2,	41, mainFULL_SCALE);
				RIT128x96x4StringDraw(" WelCome To App Home",	2,	49, mainFULL_SCALE);
				RIT128x96x4StringDraw("Curtian Control",		2,	57, mainFULL_SCALE);
				break;
		}
	switch (button)//!line 311 to 334 to see want button is pressed in all menus they are set here
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
	if(returnspeed == 0)
	{
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
	}
	motor(returnspeed);
}
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
