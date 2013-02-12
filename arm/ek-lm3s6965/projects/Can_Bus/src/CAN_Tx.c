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
#include "CAN_Rx.h"
unsigned char ucMsgDatatx[8];

void canmsgtx(void)
{
    tCANMsgObject sCANMessage;
    //! Initialize data buffer for sending a response to the HUD
    //! setting # of additional data bytes(3) and the mode(41) = show current data.
    // *(unsigned long *)ucMsgDatatx = 0;//first 8 numf
    if(*ucMsgDatarx == 0x0210C00000000000 || 0x0201000000000000 )
    {
    ucMsgDatatx[0]= 0x03;
    ucMsgDatatx[1]= 0x01;
    ucMsgDatatx[2]= 0x0D;
    ucMsgDatatx[3]= 0xD0;
    ucMsgDatatx[4]= 0x00;
    ucMsgDatatx[5]= 0x00;
    ucMsgDatatx[6]= 0x00;
    ucMsgDatatx[7]= 0x00;
    }
    //
    //! Initialize the message object that will be used for sending CAN
    //! messages.  The message will be 4 bytes that will contain an incrementing
    //! value.  Initially it will be set to 0.
    //


    sCANMessage.ulMsgID = 0x7E8;                        // CAN message ID - use 1
    sCANMessage.ulMsgIDMask = 0;                    // no mask needed for TX
    sCANMessage.ulFlags = MSG_OBJ_TX_INT_ENABLE;    // enable interrupt on TX
    sCANMessage.ulMsgLen = sizeof(ucMsgDatatx);       // size of message is 8
    sCANMessage.pucMsgData = ucMsgDatatx;             // ptr to message content

    //
    //! Send the CAN message using object number 1 (not the same thing as
    //! CAN ID, which is also 1 in this example).  This function will cause
    //! the message to be transmitted right away.
    //
    if(* ucMsgDatatx != 0x0000000000000000)
    CANMessageSet(CAN0_BASE, 1, &sCANMessage, MSG_OBJ_TYPE_TX);
}
