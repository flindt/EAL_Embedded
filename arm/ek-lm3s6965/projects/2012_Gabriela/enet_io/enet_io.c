
/*
 * Author: Gabriela Hondru
 *
 *
 *  This program is made as a final project implementation.The project it is environmental controller with a temperature sensor.
 *  The user it is able to change the set point on the board and see the value of the temperature on the OLED or web page.
 *  This program will use I2C, PWM and Ethernet modules.
 *  I2C: Will read the value of temperature.
 *  PWM: will be implemented an P controller
 *  Ethernet: The board will be set as server and will connect will devices which sends requests. The server will send
 *            TCP/IP packages.
 * Buttons: Used for changing the set point of the temperature
 *



*/

//
//Library source
//
#include <string.h>
#include <stdio.h>
#include "C:/StellarisWare2/inc/hw_ints.h"
#include "C:/StellarisWare2/inc/hw_i2c.h"
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
#include "initialize.h"
#include "audio.h"
#include "sounds.h"





//
// Defines for setting up the system clock.
//
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
struct tcp_pcb *my_pcb;
// vector which will save the value of temperature
int val[];
//Each digit will be save in a buffer so it could be send as TCP/IP packages
char buff[3],buff2[3],buff3[3];




//
// Display an lwIP type IP Address.When it is called it should be send the IP address and the
//column and row where to display it.
//
void DisplayIPAddress(unsigned long ipaddr, unsigned long ulCol,
                 unsigned long ulRow)
{
    char pucBuf[16];
    unsigned char *pucTemp = (unsigned char *)&ipaddr;

    //
    // Convert the IP Address into a string.
    //
    usprintf(pucBuf, "%d.%d.%d.%d", pucTemp[0], pucTemp[1], pucTemp[2],
             pucTemp[3]);

    //
    // Display the string.
    //
    RIT128x96x4StringDraw(pucBuf, ulCol, ulRow, 15);
}
//
//Function used for display on OLED the IP address, subnet and gateway addresses at a certain row and collomn.
//If the user want to change the OLED page it suppose to change this function as well.Has calls of DisplayIPAddress
//
void DisplayIP()
{
	unsigned long ulIPAddress;

	  ulIPAddress = lwIPLocalIPAddrGet();
	   DisplayIPAddress(ulIPAddress, 36, 16);
	   ulIPAddress = lwIPLocalNetMaskGet();
	   DisplayIPAddress(ulIPAddress, 36, 24);
	   ulIPAddress = lwIPLocalGWAddrGet();
	   DisplayIPAddress(ulIPAddress, 36, 32);
}


//
// The interrupt handler for the SysTick interrupt and the lwip timer.
// The Function is called automatic at each system tick.The system tick is configured upper
// when it is configured the system clock .
//
void
SysTickIntHandler(void)
{
    //
    // Indicate that a SysTick interrupt has occurred.
    //
    HWREGBITW(&g_ulFlags, FLAG_SYSTICK) = 1;

    //
    // Call the lwIP timer handler.
    //
    lwIPTimer(SYSTICKMS);
}

//
// The interrupt handler for the lwip interrupt.
// The function it is called automatic at each system clock.
//

void
lwIPHostTimerHandler(void)
{

}

//
//Initialize  display and display the OLED page.
//
void InitDisplay()
{
	        //
		    // Initialize the OLED display.
		    //
		    RIT128x96x4Init(1000000);
		    //Display Strings
		    RIT128x96x4StringDraw("Environment controller-temperature", 0, 0, 15);
		    RIT128x96x4StringDraw("4th edition", 0, 88, 15);
		    RIT128x96x4StringDraw("                       ", 0, 16, 15);
		   	RIT128x96x4StringDraw("                       ", 0, 24, 15);
		   	RIT128x96x4StringDraw("IP:   ", 0, 16, 15);
		    RIT128x96x4StringDraw("MASK: ", 0, 24, 15);
		   	RIT128x96x4StringDraw("GW:   ", 0, 32, 15);
		   	RIT128x96x4StringDraw("Temp1: ", 0, 48, 15);
		    RIT128x96x4StringDraw("Chosen temp:   ", 0, 64, 15);
		    RIT128x96x4StringDraw("25", 76, 64, 15);


}

//
//Initialize hardware.This function calls others. The called functions are contained by Init.c and it is using as header file Init.h.
//Will initiate all the modules that will be used during the program.
//
void InitHardware()
{
	InitSysClock();
	InitDisplay();
	InitEthernet();
	// Enable processor interrupts.
    IntMasterEnable();
    ConfigMACaddress();
    InitlwIP();
    InitLocator();
    InitButtons();
    I2C_Init();
    PWM_Init();



}

//
// This function prepared and close the connection saved under the pcb vector.
//
static void close_my_conn( struct tcp_pcb *pcb )
{
    tcp_arg( pcb, NULL );
    //The embedded system will send an NULL character as a TCP package.
    tcp_sent( pcb, NULL );
    //The embedded system will not save the last TCP package received and will not call other function
    //for saving the TCP package.
    tcp_recv( pcb, NULL );
    //The connection will be closed
    tcp_close( pcb );
}
//
//This function will be called after the packages are sent and will display a message
//and will call the function to close the connection
//
static err_t client_sent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
LWIP_UNUSED_ARG(arg);
RIT128x96x4StringDraw("Packages sent!", 0, 72, 15);
close_my_conn(pcb);
return ERR_OK;
}
//
//In case the client is connect the following stings will be send as TCP/IP packages.
//For this project will be displayed a HTML page which will contain information about the value of temperature read.
//
static err_t client_connected(void *arg, struct tcp_pcb *pcb, err_t err)
{
	//Strings that will be send to the client
    char *my_buff=buff;
    char *my_buff2=buff2;
    char stringf[]="Set point= 20";
    char *my_stringf[]="Set point= 20";
	    //It can be send as will strings or a character.
	   char string[]="<html><head><title>-EnviC-The environment controller</title></head><body> The temperature in the room is :";
	    char *myString ="<html><head><title>-EnviC-The environment controller</title></head><body> The temperature in the room is :";
	    err_t write_error;


	     LWIP_UNUSED_ARG(arg);

	     //
	     //If there are no any errors in connection will be send the string/buffer
	     //If there are errors a notice will appear on display.
	     //After the data is sent the connection will be turn it off.
	     //
	     if (err == ERR_OK)

	     {


	    	 write_error=tcp_write(pcb, myString,sizeof(string),0);
	    	 RIT128x96x4StringDraw("<BR CLEAR=ALL> ", 0, 72, 15);
	    	 write_error=tcp_write(pcb,my_buff,strlen(buff),0);
	    	 write_error=tcp_write(pcb,my_buff2,strlen(buff2),0);
	    	// RIT128x96x4StringDraw("<BR CLEAR=ALL> ", 0, 72, 15);
	    	 //write_error=tcp_write(pcb,my_stringf,strlen(stringf),0);


	    	 tcp_output(pcb);
	         tcp_sent(pcb, client_sent);




	     }

return err;
}

//
// This function will connect to the PC and will bind the IP of the Master.
//

void connection( void )
{
	//
	//Declare the variable
	//
	// save the Ip of the PC
     unsigned long PCipaddr;
     //Save the errors that will occur
     err_t err;
     //Save the IP of the controller
     struct ip_addr ipaddr;

     //
     // The IP address 169.254.10.10 to which i need to send the data.
     // The IP address it is saved as a hex number
     //
     PCipaddr = 0xa9fe0a0a;

     //
     //The htonl function converts a u_long from host to TCP/IP network byte order (which is big-endian).
     //
     ipaddr.addr = htonl( PCipaddr );

     //
     // Initialize the TCP/IP stack
     //
     tcp_init();

     //
     //Set the connection as an new one and save it
     //
     my_pcb = tcp_new();

     //
     // Bind to any local address that the board had been assigned
     //
     tcp_bind( my_pcb, IP_ADDR_ANY, 80 );
     //
     //The connection will listen for any clients that want to connect
     //
     my_pcb = tcp_listen(my_pcb);
     //
     //The server will accept the connection and will send the packages
     //
     tcp_accept(my_pcb,client_connected);


}


//
//If this function is called by the main function will increase (if the up button is pressed) and decrease (if the down button is pressed)
//the set point. The settings will be saved if the LEFT button is pressed.Otherwise in few seconds this function will be closed and the set
// point will remain the same.
//
int ChangingManualSetPoint(int m)
{
	int f=0;
	float i,j=0;
	char buff[10];

	while(f==0 && j<=160000)
	{
	         for(i=1;i<40000;i++);
	         if((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0))==0 && m<50)
	    		{
	        	   m++;
	        	   sprintf(buff, "%d", m);
	    	       RIT128x96x4StringDraw(buff, 76,64, 15);
	    		}
	         if((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_1))==0 && m>-25)
	        	 {m--;
	        	  sprintf(buff, "%d", m);
	        	  RIT128x96x4StringDraw(buff, 76, 64, 15);}

	         if((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_2))==0)
	        	 f++;
	         j++;
	}
	return m;
}

//
//This function will use I2C functions and will read the temperature from the sensor.
//The first bit will be prepared for sending as TCP/IP package. It will be devided in digits and each digit in a char.
//This function will display the temperature.
//

void I2C_reading()
{

// variables which will save the digits.
int digit1, digit2;

	    // Slave address is 0x4f (binary 1001 000)
		// Send address with R/W bit set to Write
		I2CMasterSlaveAddrSet(I2C1_MASTER_BASE, 0x48, false);

		// 0xAA = read temperature
		I2CMasterDataPut(I2C1_MASTER_BASE, 0xAA);
		I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND);
		while(I2CMasterBusy(I2C1_MASTER_BASE));

		// Send address again with R/W bit to Read
	    I2CMasterSlaveAddrSet(I2C1_MASTER_BASE, 0x48, true);

		// ------- Read temperature ------
		// Receive high byte
		I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
		while(I2CMasterBusy(I2C1_MASTER_BASE));
		val[0]=I2CMasterDataGet(I2C1_MASTER_BASE);

	    digit2=val[0]%10;
	    digit1=val[0]/10;
		RIT128x96x4StringDraw("       ",70, 48, 15);
		sprintf(buff, "%d", digit1);
		RIT128x96x4StringDraw(buff, 70, 48, 15);
		sprintf(buff2, "%d", digit2);
		RIT128x96x4StringDraw(buff2, 76, 48, 15);


		// Receive low byte
		I2CMasterControl(I2C1_MASTER_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
		while(I2CMasterBusy(I2C1_MASTER_BASE));
		val[1]=I2CMasterDataGet(I2C1_MASTER_BASE);

	}
//
//The next function will be called from the main for controlling the environment temperature. It is a PWM low frequency controlled implemented as
//P controller.The Pcontroller will manage to bring the ambient temperature as close as possible from the set point.
//The temperature will be manage if it is in the interval (-20, 50).
//The duty cycle of the PWM signal will be changed in function of the error which it is the difference between the value and the set point.
//
void Pcontroller(int setp)
{
 int err;
 unsigned long ulPeriod;
 ulPeriod = SysCtlClockGet()/4;
	if(val[0]>=setp && val[0]<50 && val[0]>-20)
	   {SysCtlDelay(160000);
		err=val[0]-setp;

		PWMPulseWidthSet(PWM_BASE, PWM_OUT_0, ulPeriod*err/15);
		PWMOutputState(PWM_BASE, PWM_OUT_0_BIT, true);
		PWMGenEnable(PWM_BASE, PWM_GEN_0);
	   }


}
//
// Main function
//
int main(void)
{
	//The set point will be initialize as a value of 25.
	int setp=25;

	//
	//Initialize the controller components/ modules it will be used
	//
    InitHardware();
    //
    //Complete the first page with IP, gateway and subnet actual addresses
    //
    DisplayIP();
    //
    //Calling the function for connecting, sending and receiving packages.
    //
   connection();

    while(1)
    {
    	//will read the temperature
    	I2C_reading();
    	//will check if the user wants to change the set point from  the buttons
    	if((GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3))==0 )
    		 setp=ChangingManualSetPoint(setp);
    	//If the temperature is outside the interval (-20,50), an alarm will start.
    	if(val[0]>=50 && val[0]<-20)
    		{RIT128x96x4StringDraw("ALARM!!!!!      ",70, 48, 15);
    	    //Can also set audio but the audio will use the PWM_base0 so better if the fan will move at PWM_base1
    		//
    	   // Play the start of game song.
    	   //
    		//   AudioPlaySong(g_pusEndOfGame, strlen(g_pusEndOfGame));
    		}
    	//A PWM speed controller for the fan will start
    	Pcontroller(setp);





    };




}
