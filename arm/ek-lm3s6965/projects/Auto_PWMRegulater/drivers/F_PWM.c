/*
 ============================================================================
 Name        : motor.c
 Author      : Lars
 Version     :
 Copyright   : Your copyright notice
 Description : output to motor controller in C, Ansi-style
 ============================================================================
 */
//*****************************************************************************
//
//! \addtogroup Motor Settings
//! @{
//
//************************************************************************** ***
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_sysctl.h"

#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

#include "F_PWM.h"
#include "rit128x96x4.h"

#define PWMperiedset 1000
//globel
volatile unsigned long ulPeriod;
float valuereturn = 0;
//! This example application utilizes the PWM peripheral to output a 25% duty
//! cycle PWM signal and a 75% duty cycle PWM signal, both at 440 Hz.  Once
//! configured, the application enters an infinite loop, doing nothing while
//! the PWM peripheral continues to output its signals.
//

//*****************************************************************************

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

//*****************************************************************************
//
// This example demonstrates how to setup the PWM block to generate signals.
//
//*****************************************************************************
void F_PWM_init(void)
{
//!setting up PWN

    volatile unsigned long ulLoop;


    //
    // Set the clocking to run directly from the crystal.
    //

    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

    //
    // Enable the peripherals used by this example.
    //
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    //
    //! Set GPIO B0 and B1 as PWM pins.  They are used to output the PWM0 and
    //! PWM1 signals.
    //
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	//
	//! Enable the PWM2 and PWM3 output signals if true.
	//
	PWMOutputState(PWM0_BASE,PWM_OUT_2_BIT|PWM_OUT_3_BIT, false);
	//
	//! Enable the PWM generator.
	//
	PWMGenEnable(PWM0_BASE, PWM_GEN_1);

    //
    //! Compute the PWM period based on the system clock.
    //
    ulPeriod = SysCtlClockGet() / PWMperiedset;

    //
    // Set the PWM2 period
    //
    PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC | PWM_GEN_MODE_DBG_RUN);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, ulPeriod);
}

void F_PWM(float hastighed)
{

	int setpulsewidth = (float)ulPeriod * hastighed;

	//
    //! Set PWM2 duty cycle
	//@parma PWMPulseWidthSet
    //

    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, setpulsewidth);

	if(hastighed > 0)//! line 115 to turn on PWM if they a set to a speed
	{
		PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, true);
	}
	if(hastighed == 0)//!line 119 to turn of PWM and set the port to 0 so they a not run some thing on the motors
	{
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0);
		PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT | PWM_OUT_3_BIT, false);
	}

}
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
