#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/can.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include "utils/uartstdio.h"
#include "inc/lm3s9b92.h"
#include "CAN_Tx.h"
#include "CAN_Rx.h"


//*****************************************************************************
// This function sets up UART0 to be used for a console to display information
// as the example is running.
//*****************************************************************************
void 
InitConsole(void)
{
	// Enable GPIO port A which is used for UART0 pins
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	
    // Configure the pin muxing for UART0 functions on port A0 and A1.
    // This step is not necessary if your part does not support pin muxing.
    GPIOPinConfigure( 0x00000001 );//GPIO_PA0_U0RX
    GPIOPinConfigure( 0x00000401 );//GPIO_PA1_U0TX

    // Select the alternate (UART) function for these pins.
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Initialize the UART for console I/O.
    UARTStdioInit(0);
    
    UARTprintf("\033[2JEthernet (uIP) & CAN 5OOK communication \n");
}

void 
InitPheripherals(void)
{
    // Enable the GPIO port that is used for the on-board LED (PD0).   
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    
    SimpleDelay();
    
    // Enable the GPIO pin for the LED (PD0).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    GPIO_PORTD_DIR_R |= 0x01;
    GPIO_PORTD_DEN_R |= 0x01;
}

void 
InitCAN(void)
{
    // For this example CAN0 is used with RX and TX pins on port A4 and A5.
    // The actual port and pins used may be different on your part, consult
    // the data sheet for more information.
    // GPIO port A needs to be enabled so these pins can be used.
    // TODO: change this to whichever GPIO port you are using
    //
    // Already done in initialisation
        //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Configure the GPIO pin muxing to select CAN0 functions for these pins.
    // This step selects which alternate function is available for these pins.
    // This is necessary if your part supports GPIO pin function muxing.
    // Consult the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using
    GPIOPinConfigure(0x00001005);
    GPIOPinConfigure(0x00001405);

    // Enable the alternate function on the GPIO pins.  The above step selects
    // which alternate function is available.  This step actually enables the
    // alternate function instead of GPIO for these pins.
    // TODO: change this to match the port/pin you are using
    GPIOPinTypeCAN(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    
    // The GPIO port and pins have been set up for CAN.  The CAN peripheral
    // must be enabled.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);

    // Initialize the CAN controller
    CANInit(CAN0_BASE);

    // Set up the bit rate for the CAN bus.  This function sets up the CAN
    // bus timing for a nominal configuration.  You can achieve more control
    // over the CAN bus timing by using the function CANBitTimingSet() instead
    // of this one, if needed.
    // In this example, the CAN bus is set to 500 kHz.  In the function below,
    // the call to SysCtlClockGet() is used to determine the clock rate that
    // is used for clocking the CAN peripheral.  This can be replaced with a
    // fixed value if you know the value of the system clock, saving the extra
    // function call.  For some parts, the CAN peripheral is clocked by a fixed
    // 8 MHz regardless of the system clock in which case the call to
    // SysCtlClockGet() should be replaced with 8000000.  Consult the data
    // sheet for more information about CAN peripheral clocking.
    CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 500000);

    // Enable interrupts on the CAN peripheral.  This example uses static
    // allocation of interrupt handlers which means the name of the handler
    // is in the vector table of startup code.  If you want to use dynamic
    // allocation of the vector table, then you must also call CANIntRegister()
    // here.
    //
    // CANIntRegister(CAN0_BASE, CANIntHandler); // if using dynamic vectors
    //
    CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);

    // Enable the GPIOB interrupt and the CAN interrupt on the processor (NVIC).
    IntEnable(INT_GPIOB);
    IntEnable(INT_CAN0);

    // Enable the CAN for operation.
    CANEnable(CAN0_BASE);
    	ucMsgDatatx[0]= 0x00;
        ucMsgDatatx[1]= 0x00;
        ucMsgDatatx[2]= 0x00;
        ucMsgDatatx[3]= 0x00;
        ucMsgDatatx[4]= 0x00;
        ucMsgDatatx[5]= 0x00;
        ucMsgDatatx[6]= 0x00;
        ucMsgDatatx[7]= 0x00;
}
