#include "initCANprog.h"
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
#include "CAN_Tx.h"
//*****************************************************************************
//
// A flag for the interrupt handler to indicate that a message was received.
//
//*****************************************************************************
volatile unsigned long g_bRXFlag = 0;


unsigned char ucMsgDatarx[8];

void canmsgrx(void)
  {
	  tCANMsgObject sCANReceive;

	    // Initialize data buffer for sending a response to the HUD
	    // setting # of additional data bytes(3) and the mode(41) = show current data.
	    //*(unsigned long *)ucMsgDatarx = 0;//first 8 numf


  // INITIALIZE A MESSAGE OBJECT to be used for RECEIVING CAN MESSAGES with
    // any CAN ID.  In order to receive any CAN ID, the ID and mask must both
    // be set to 0, and the ID filter enabled.
    sCANReceive.ulMsgID = 0x7E0;                        // CAN msg ID - 0 for any
    sCANReceive.ulMsgIDMask = 0;                    // mask is 0 for any ID
    sCANReceive.ulFlags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER;
    sCANReceive.ulMsgLen = sizeof(ucMsgDatarx);;                       // allow up to 8 bytes

    CANMessageSet(CAN0_BASE, 1, &sCANReceive, MSG_OBJ_TYPE_RX);

            //
            // If the flag is set, that means that the RX interrupt occurred and
            // there is a message ready to be read from the CAN
            //
            if(g_bRXFlag)
            {
                //
                // Reuse the same message object that was used earlier to configure
                // the CAN for receiving messages.  A buffer for storing the
                // received data must also be provided, so set the buffer pointer
                // within the message object.
                //
                sCANReceive.pucMsgData = ucMsgDatarx;

                //
                // Read the message from the CAN.  Message object number 1 is used
                // (which is not the same thing as CAN ID).  The interrupt clearing
                // flag is not set because this interrupt was already cleared in
                // the interrupt handler.
                //
                CANMessageGet(CAN0_BASE, 1, &sCANReceive, 0);

                //
                // Clear the pending message flag so that the interrupt handler can
                // set it again when the next message arrives.
                //
                g_bRXFlag = 0;

                //
                // Check to see if there is an indication that some messages were
                // lost.
                //
                if(sCANReceive.ulFlags & MSG_OBJ_DATA_LOST)
                {
                    UARTprintf("CAN message loss detected\n");
                }


            }
  }
