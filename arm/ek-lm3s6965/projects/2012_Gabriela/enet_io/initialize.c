/*
 * initialize.c
 *
 *  Created on: Apr 5, 2012
 *      Author: G
 */
#include <string.h>
#include "C:/StellarisWare2/inc/hw_ints.h"
#include "C:/StellarisWare2/inc/hw_memmap.h"
#include "C:/StellarisWare2/inc/hw_nvic.h"
#include "C:/StellarisWare2/inc/hw_types.h"
#include "C:/StellarisWare2/driverlib/ethernet.h"
#include "C:/StellarisWare2/driverlib/flash.h"
#include "C:/StellarisWare2/driverlib/gpio.h"
#include "C:/StellarisWare2/driverlib/interrupt.h"
#include "C:/StellarisWare2/driverlib/sysctl.h"
#include "C:/StellarisWare2/driverlib/systick.h"
#include "C:/StellarisWare2/utils/locator.h"
#include "C:/StellarisWare2/utils/lwiplib.h"
#include "C:/StellarisWare2/utils/ustdlib.h"
#include "C:/StellarisWare2/boards/ek-lm3s6965/drivers/rit128x96x4.h"
#include "C:/StellarisWare2/driverlib/i2c.h"
#include "C:/StellarisWare2/driverlib/pwm.h"

// Defines for setting up the system clock.
#define SYSTICKHZ               100
#define SYSTICKMS               (1000 / SYSTICKHZ)
#define SYSTICKUS               (1000000 / SYSTICKHZ)
#define SYSTICKNS               (1000000000 / SYSTICKHZ)

//
//It is defined  parameter in case the embedded system will not receive any information or
//any connection
//
#define PARAM_ERROR_RESPONSE    "/perror.htm"
//
//  In case it will be used DHCP IP please uncomment the next functions.
//  Timeout for DHCP address request (in seconds).
//
/*#ifndef DHCP_EXPIRE_TIMER_SECS
#define DHCP_EXPIRE_TIMER_SECS  20
#endif*/




//
// A set of flags.  The flag bits are defined as follows:
//
//     0 -> An indicator that a SysTick interrupt has occurred.
//

#define FLAG_SYSTICK            0
static volatile unsigned long g_ulFlags;

//
// The error routine that is called if the driver library encounters an error.
//
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

//
//Global variables used in program
//

// Pointer which will save my actual connection, named as well TCP protocol control block

unsigned char pucMACArray[8];

unsigned long ulPeriod;
//Initialize system clock and system
void InitSysClock(void)
{
	 //
	 // Set the clocking to run directly from the crystal.
    //
	    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
	                   SYSCTL_XTAL_8MHZ);
    //
   // Configure SysTick for a periodic interrupt.
   //
	 SysTickPeriodSet(SysCtlClockGet() / SYSTICKHZ);
	 SysTickEnable();
	 SysTickIntEnable();
}
void InitEthernet(void)
{
	 //
	    // Enable and Reset the Ethernet Controller.
	    //
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_ETH);
	    SysCtlPeripheralReset(SYSCTL_PERIPH_ETH);

	    //
	    // Enable Port F for Ethernet LEDs.
	    //  LED0        Bit 3   Output
	    //  LED1        Bit 2   Output
	    //
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	    GPIOPinTypeEthernetLED(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3);

}
void ConfigMACaddress(void)
{
	unsigned long ulUser0, ulUser1;
	    // Configure the hardware MAC address for Ethernet Controller filtering of
	    // incoming packets.
	    //
	    // For the LM3S6965 Evaluation Kit, the MAC address will be stored in the
	    // non-volatile USER0 and USER1 registers.  These registers can be read
	    // using the FlashUserGet function, as illustrated below.
	    //
	    FlashUserGet(&ulUser0, &ulUser1);
	    if((ulUser0 == 0xffffffff) || (ulUser1 == 0xffffffff))
	    {   //
	        // We should never get here.  This is an error if the MAC address
	        // has not been programmed into the device.  Exit the program.
	        //
	        RIT128x96x4StringDraw("MAC Address", 0, 16, 15);
	        RIT128x96x4StringDraw("Not Programmed!", 0, 24, 15);
	        while(1);
	    }
	    //
	    // Convert the 24/24 split MAC address from NV ram into a 32/16 split
	    // MAC address needed to program the hardware registers, then program
	    // the MAC address into the Ethernet Controller registers.
	    //
	    pucMACArray[0] = ((ulUser0 >>  0) & 0xff);
	    pucMACArray[1] = ((ulUser0 >>  8) & 0xff);
	    pucMACArray[2] = ((ulUser0 >> 16) & 0xff);
	    pucMACArray[3] = ((ulUser1 >>  0) & 0xff);
	    pucMACArray[4] = ((ulUser1 >>  8) & 0xff);
	    pucMACArray[5] = ((ulUser1 >> 16) & 0xff);

}

//Initialize the lwIP
void InitlwIP(void)
{
	    //
	    // Initialze the lwIP library, using static IP.
	    //It can use as well DHCP and auto IP(look into the documentation or into the declaration of the function
	    //
	   lwIPInit(pucMACArray,0x0a9fe1414, 0xffff0000, 0, IPADDR_USE_STATIC);

}
//
// Setup the device locator service.
//
void InitLocator(void)
{

    LocatorInit();
    LocatorMACAddrSet(pucMACArray);
    LocatorAppTitleSet("EK-LM3S6965 enet_io");
}
//
//Initialize the I2C
//
void I2C_Init(void)
{
	// Enable PortA and I2C on pin 6 and 7
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
	GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7);

	//false = 100kHz, true = 400kHz
	I2CMasterInitExpClk(I2C1_MASTER_BASE, SysCtlClockGet(), false);

	// Slave address of DS1624 is 0x4f (binary 1001 000)
	// false=write / true=read
	I2CMasterSlaveAddrSet(I2C1_MASTER_BASE, 0x48, false);

	// Send 0xEE = start convert temperature
	I2CMasterDataPut(I2C1_MASTER_BASE, 0xEE);
	I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);
	// Wait for I2C to finish
	while(I2CMasterBusy(I2C1_MASTER_BASE));

}


void I2C_Init2(void)
{
	    // Enable PortA and I2C on pin 6 and 7
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
		GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_6 | GPIO_PIN_7);

		//false = 100kHz, true = 400kHz
		I2CMasterInitExpClk(I2C0_MASTER_BASE, SysCtlClockGet(), false);

		// Slave address of Ds1624 is 0x4f (binary 0101000)
		// false=write / true=read
		I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, 50, false);

		// Send 0xEE = start convert temperature
		I2CMasterDataPut(I2C0_MASTER_BASE, 0xEE);
		I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);
		// Wait for I2C to finish
		while(I2CMasterBusy(I2C0_MASTER_BASE));
}
void PWM_Init(void)
{
	//Enable Peripherals
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM);

	ulPeriod = SysCtlClockGet()/4;

	//Set GPIO F0 as PWM pins for PWM 0;
	GPIOPinTypePWM(GPIO_PORTF_BASE,GPIO_PIN_0);
	PWMGenConfigure(PWM_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN|PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM_BASE, PWM_GEN_0, ulPeriod);
	PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, ulPeriod*2);
	PWMOutputState(PWM_BASE, PWM_OUT_0_BIT, true);
	PWMGenEnable(PWM_BASE, PWM_GEN_0);
}
void InitButtons(void)
{
	//Init buttons switches up, down, left, right
		    SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOE );
		 	GPIODirModeSet( GPIO_PORTE_BASE,GPIO_PIN_0,GPIO_DIR_MODE_IN );
		 	GPIOPadConfigSet( GPIO_PORTE_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU );

		 	SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOE );
		 	GPIODirModeSet( GPIO_PORTE_BASE,GPIO_PIN_1,GPIO_DIR_MODE_IN );
		 	GPIOPadConfigSet( GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU );

		 	SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOE );
		 	GPIODirModeSet( GPIO_PORTE_BASE,GPIO_PIN_2,GPIO_DIR_MODE_IN );
		 	GPIOPadConfigSet( GPIO_PORTE_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU );

		 	SysCtlPeripheralEnable( SYSCTL_PERIPH_GPIOE );
		 	GPIODirModeSet( GPIO_PORTE_BASE,GPIO_PIN_3,GPIO_DIR_MODE_IN );
		 	GPIOPadConfigSet( GPIO_PORTE_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU );


}


