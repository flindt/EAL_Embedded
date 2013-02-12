//*****************************************************************************
//
// simple_tx.c - Example demonstrating simple CAN message transmission.
//
// Copyright (c) 2010-2012 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 9107 of the Stellaris Firmware Development Package.
//
//*****************************************************************************
#include "initCANprog.h"
#include "CAN_Tx.h"
#include "CAN_Rx.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "driverlib/can.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "utils/uartstdio.h"
#include "inc/lm3s9b92.h"
//*****************************************************************************
//
//! \addtogroup can_examples_list
//! <h1>Simple CAN TX (simple_tx)</h1>
//!
//! This example shows the basic setup of CAN in order to transmit messages
//! on the CAN bus.  The CAN peripheral is configured to transmit messages
//! with a specific CAN ID.  A message is then transmitted once per second,
//! using a simple delay loop for timing.  The message that is sent is a 4
//! byte message that contains an incrementing pattern.  A CAN interrupt
//! handler is used to confirm message transmission and count the number of
//! messages that have been sent.
//!
//! This example uses the following peripherals and I/O signals.  You must
//! review these and change as needed for your own board:
//! - CAN0 peripheral
//! - GPIO Port D peripheral (for CAN0 pins)
//! - CAN0RX - PD0
//! - CAN0TX - PD1
//!
//! The following UART signals are configured only for displaying console
//! messages for this example.  These are not required for operation of CAN.
//! - GPIO port A peripheral (for UART0 pins)
//! - UART0RX - PA0
//! - UART0TX - PA1
//!
//! This example uses the following interrupt handlers.  To use this example
//! in your own application you must add these interrupt handlers to your
//! vector table.
//! - INT_CAN0 - CANIntHandler
//!
//
//*****************************************************************************

//*****************************************************************************
//
// A counter that keeps track of the number of times the TX interrupt has
// occurred, which should match the number of TX messages that were sent.
//
//*****************************************************************************
volatile unsigned long g_ulMsgCount = 0;

//*****************************************************************************
//
// A flag to indicate that some transmission error occurred.
//
//*****************************************************************************
volatile unsigned long g_bErrFlag = 0;

//*****************************************************************************
//
// This function provides a 1 second delay using a simple polling method.
//
//*****************************************************************************
void
SimpleDelay(void)
{
    //
    // Delay cycles for 1 second
    //
    SysCtlDelay(16000000 / 3);
}

//*****************************************************************************
//
// This function is the interrupt handler for the CAN peripheral.  It checks
// for the cause of the interrupt, and maintains a count of all messages that
// have been transmitted.
//
//*****************************************************************************
void CANIntHandler(void)
{
    unsigned long ulStatus;

    //
    // Read the CAN interrupt status to find the cause of the interrupt
    //
    ulStatus = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE);

    //
    // If the cause is a controller status interrupt, then get the status
    //
    if(ulStatus == CAN_INT_INTID_STATUS)
    {
        //
        // Read the controller status.  This will return a field of status
        // error bits that can indicate various errors.  Error processing
        // is not done in this example for simplicity.  Refer to the
        // API documentation for details about the error status bits.
        // The act of reading this status will clear the interrupt.  If the
        // CAN peripheral is not connected to a CAN bus with other CAN devices
        // present, then errors will occur and will be indicated in the
        // controller status.
        //
        ulStatus = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL);

        //
        // Set a flag to indicate some errors may have occurred.
        //
        g_bErrFlag = 1;
    }

    //
    // Check if the cause is message object 1, which what we are using for
    // sending messages.
    //
    else if(ulStatus == 1)
    {
        //
        // Getting to this point means that the TX interrupt occurred on
        // message object 1, and the message TX is complete.  Clear the
        // message object interrupt.
        //
        CANIntClear(CAN0_BASE, 1);

        //
        // Increment a counter to keep track of how many messages have been
        // sent.  In a real application this could be used to set flags to
        // indicate when a message is sent.
        //
        g_ulMsgCount++;

        //
        // Since the message was sent, clear any error flags.
        //
        g_bErrFlag = 0;
    }

    //
    // Otherwise, something unexpected caused the interrupt.  This should
    // never happen.
    //
    else
    {
        //
        // Spurious interrupt handling can go here.
        //
    }
}

//*****************************************************************************
//
// Configure the CAN and enter a loop to transmit periodic CAN messages.
//
//*****************************************************************************
int main(void){


    //
    // Set the clocking to run directly from the external crystal/oscillator.
    // TODO: The SYSCTL_XTAL_ value must be changed to match the value of the
    // crystal on your board.
    //
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    //
    // Set up the serial console to use for displaying messages.  This is
    // just for this example program and is not needed for CAN operation.
    //
   InitConsole();

   InitCAN();

    //
    // Enter loop to send messages.  A new message will be sent once per
    // second.  The 4 bytes of message content will be treated as an unsigned
    // long and incremented by one each time.
    //
    for(;;)
    {
        //
        // Print a message to the console showing the message count and the
        // contents of the message being sent.
        //
/*
        UARTprintf("Sending msg: 0x%02X %02X %02X %02X",
                   ucMsgData[0], ucMsgData[1], ucMsgData[2], ucMsgData[3]);
*/
    	canmsgrx();

    	canmsgtx();


        //
        // Now wait 1 second before continuing
        //
        SimpleDelay();

        //
        // Check the error flag to see if errors occurred
        //
/*
        if(g_bErrFlag)
        {
            UARTprintf(" error - cable connected?\n");
        }
        else
        {
            //
            // If no errors then print the count of message sent
            //
            UARTprintf(" total count = %u\n", g_ulMsgCount);
        }

        //
        // Increment the value in the message data.
        //
        (*(unsigned long *)ucMsgData)++;
*/
    }

    //
    // Return no errors
    //
    return(0);
}

