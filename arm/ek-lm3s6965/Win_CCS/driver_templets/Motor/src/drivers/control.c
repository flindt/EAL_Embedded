/*
 * motor.c
 *
 *  Created on: 23/08/2012
 *      Author: Lasse
 */


//
//*****************************************************************************

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "rit128x96x4.h"

//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>PWM (pwmgen)</h1>
//!
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
int Control()
{

    unsigned long ulPeriod;
    volatile unsigned long ulLoop;
    //
    // Set the clocking to run directly from the crystal.
    //
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

    //
    // Initialize the OLED display.
    //
    RIT128x96x4Init(1000000);

    //
    // Clear the screen and tell the user what is happening.
    //
    RIT128x96x4StringDraw("Generating PWM", 18, 24, 15);
    RIT128x96x4StringDraw("on PF0 and PD1", 18, 32, 15);

    //
    // Enable the peripherals used by this example.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Set GPIO F0 and D1 as PWM pins.  They are used to output the PWM0 and
    // PWM1 signals.
    //
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_1);

    //
    // Compute the PWM period based on the system clock.
    //
    ulPeriod = SysCtlClockGet() / 1;

    //
    // Set the PWM period to 440 (A) Hz.
    //
    PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, ulPeriod);

    //
    // Set PWM0 to a duty cycle of 25% and PWM1 to a duty cycle of 75%.
    //
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, ulPeriod / 4);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, ulPeriod * 3 / 4);

    //
    // Enable the PWM0 and PWM1 output signals.
    //
    PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT, true);

    //
    // Enable the PWM generator.
    //
    PWMGenEnable(PWM0_BASE, PWM_GEN_0);

    //
    // Loop forever while the PWM signals are generated.
    //
    while(1)
    {
    	//motor(A,%,F);
    }
}
