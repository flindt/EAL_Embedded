/*
 * Example code for a very simple statemachine.
 *
 * Used in teaching C programming on embedded systems at EAL
 *
 */
//Includes
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
//Defines
#define mainCHARACTER_HEIGHT				( 9 )
#define mainMAX_ROWS_128					( mainCHARACTER_HEIGHT * 14 )
#define mainMAX_ROWS_96						( mainCHARACTER_HEIGHT * 10 )
#define mainMAX_ROWS_64						( mainCHARACTER_HEIGHT * 7 	)
#define mainFULL_SCALE						( 15 )
#define ulSSI_FREQUENCY						( 3500000UL )

// State definitions




int TS_State = UPSTARTMENU;


/* The SM does not know anything about the system. This way it can be tested on a
 * different C compiler very easily.
 */
int statemashine( int event )
{
    int NextState = TS_State;

	static int Speed = 0;
	static int SetSpeed = 0;

	char buffer[32];

	switch( TS_State )
    {
	case UPSTARTMENU:
		RIT128x96x4StringDraw("Up LED", 5, 42, mainFULL_SCALE);
		RIT128x96x4StringDraw("Welcome to app home", 5, 49, mainFULL_SCALE);
		RIT128x96x4StringDraw("Down Motor", 5,56 , mainFULL_SCALE);
		RIT128x96x4StringDraw("               ", 5, 77, mainFULL_SCALE);
		itoa(SetSpeed, buffer, 10);
		RIT128x96x4StringDraw(buffer, 5, 77, mainFULL_SCALE);
		switch (event )
		{
		case KEY0_EVENT_SELECT:
			RIT128x96x4StringDraw("Select Pressed", 0, 0, mainFULL_SCALE);
			break;

		case KEY1_EVENT_UP:
			RIT128x96x4StringDraw("Up Pressed", 0, 0, mainFULL_SCALE);
			NextState = LED;
			break;

		case KEY2_EVENT_DOWN:

			RIT128x96x4StringDraw("Down Pressed", 0, 0, mainFULL_SCALE);
			NextState = MOTOR;
			break;

		case KEY3_EVENT_ENTER:
			RIT128x96x4StringDraw("Enter Pressed", 0, 0, mainFULL_SCALE);
			break;

		case KEY4_EVENT_CANCEL:
			RIT128x96x4StringDraw("Cancel Pressed", 0, 0, mainFULL_SCALE);
			break;
		default:
			break;
		}
		break;

	case LED:
		RIT128x96x4StringDraw("Up ON", 5, 42, mainFULL_SCALE);
		RIT128x96x4StringDraw("LED Control", 5, 49, mainFULL_SCALE);
		RIT128x96x4StringDraw("Down off", 5, 56, mainFULL_SCALE);
		switch (event )
		{
			case KEY0_EVENT_SELECT:
				RIT128x96x4StringDraw("Select Pressed", 0, 0, mainFULL_SCALE);
				NextState = UPSTARTMENU;
				break;

			case KEY1_EVENT_UP:

				RIT128x96x4StringDraw("Up Pressed    ", 0, 0, mainFULL_SCALE);
				//turn on led on the board
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_PIN_0);
				RIT128x96x4StringDraw("            ", 5, 63, mainFULL_SCALE);
				RIT128x96x4StringDraw("LED ON", 5, 35, mainFULL_SCALE);
				break;

			case KEY2_EVENT_DOWN:
				RIT128x96x4StringDraw("Down Pressed  ", 5, 0, mainFULL_SCALE);
				//turn on led off the board
				GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, 0);
				RIT128x96x4StringDraw("            ", 5, 35, mainFULL_SCALE);
				RIT128x96x4StringDraw("LED OFF", 5, 63, mainFULL_SCALE);
				break;

			case KEY3_EVENT_ENTER:
				RIT128x96x4StringDraw("Enter Pressed ", 0, 0, mainFULL_SCALE);
				break;

			case KEY4_EVENT_CANCEL:

				RIT128x96x4StringDraw("Cancel Pressed", 0, 0, mainFULL_SCALE);
				NextState = UPSTARTMENU;
				break;
		default:
			break;
		}
		break;

	case MOTOR:
		RIT128x96x4StringDraw("Enter SAVE value", 5, 35, mainFULL_SCALE);
		RIT128x96x4StringDraw("Up + 100 to PWM", 5, 42, mainFULL_SCALE);
		RIT128x96x4StringDraw("Motor Control", 5, 49, mainFULL_SCALE);
		RIT128x96x4StringDraw("Down -100 to PWM", 5,56 , mainFULL_SCALE);
		RIT128x96x4StringDraw("Cancel makes PWM to 0", 5, 63, mainFULL_SCALE);
		switch (event)
		{
			case KEY0_EVENT_SELECT:

				RIT128x96x4StringDraw("Select Pressed", 0, 0, mainFULL_SCALE);
				NextState = UPSTARTMENU;
				break;

			case KEY1_EVENT_UP:

				RIT128x96x4StringDraw("Up Pressed", 0, 0, mainFULL_SCALE);
				SetSpeed = 100 + SetSpeed;

				RIT128x96x4StringDraw("               ", 0, 77, mainFULL_SCALE);
				itoa(SetSpeed, buffer, 10);
				RIT128x96x4StringDraw(buffer, 20, 77, mainFULL_SCALE);
				break;

			case KEY2_EVENT_DOWN:

				RIT128x96x4StringDraw("Down Pressed", 0, 0, mainFULL_SCALE);
				if(Speed != 0)
				{
					SetSpeed = SetSpeed - 100;
					RIT128x96x4StringDraw("               ", 0, 77, mainFULL_SCALE);
					itoa(SetSpeed, buffer, 10);
					RIT128x96x4StringDraw(buffer, 20, 77, mainFULL_SCALE);
				}
				break;

			case KEY3_EVENT_ENTER:

				RIT128x96x4StringDraw("Enter Pressed", 0, 0, mainFULL_SCALE);
				Speed = SetSpeed;
				RIT128x96x4StringDraw("               ", 0, 77, mainFULL_SCALE);
				itoa(Speed, buffer, 10);
				RIT128x96x4StringDraw(buffer, 20, 77, mainFULL_SCALE);
				break;

			case KEY4_EVENT_CANCEL:

				RIT128x96x4StringDraw("Cancel Pressed", 0, 0, mainFULL_SCALE);

				SetSpeed = 0;
				Speed = SetSpeed;

				RIT128x96x4StringDraw("               ", 0, 77, mainFULL_SCALE);
				itoa(Speed, buffer, 10);
				RIT128x96x4StringDraw(buffer, 20, 77, mainFULL_SCALE);
				break;

			default:
				break;
		}
		break;

	default:
		break;
	// The program should never get here !
    }

    if (NextState != TS_State)
    {
		OnEnter(NextState);
        OnExit(TS_State);
        TS_State = NextState;
    }

    SetMotorParams( TS_State, Speed);

return TS_State;
}

/* The 3 functions OnEnter, OnExit and Do are were all the fun happens.
 * Theee functions are written specifically for this system, and must be replaced
 * if the SM is used somewhere else - say on an embedded system.
 *
 * For simulating the SM a bunch of printf statements should be put here.
 */

void OnEnter( int State )
{
	RIT128x96x4Clear();
}

void OnExit( int State)
{

}

int SetMotorParams( int State,int value)
{

	motor(value);

	return value;
}

