/*
 * Functions.c
 *
 *  Created on: 27/10/2012
 *      Author: Thomas
 */
//*****************************************************************************
//
//! \addtogroup adc_api
//! @{
//
//*****************************************************************************

//includes---------------------------------------------------------------------
#include "Functions.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "drivers/rit128x96x4.h"
#include "driverlib/i2c.h"
#include "driverlib/interrupt.h"
#include <string.h>
//-----------------------------------------------------------------------------


//Global Variables & Lists-----------------------------------------------------



int set_default=0x04;

// int select_data_handler [LED_NUMBER][LED_COMMAND][SELECT_DATA];
// This array have each possible option available put into parts
//(7blocks)one for each led , (6blocks)one for each led command, and (3blocks) for the SELECT bit-codes.
int select_data_handler[7][6][3]={

		{{0x00,0x00,0x00},{0x00,0x01,0x00},{0x01,0x01,0x00},{0x00,0x00,0x01},{0x00,0x01,0x01},{0x01,0x01,0x01}},
		{{0x00,0x00,0x00},{0x00,0x02,0x00},{0x02,0x02,0x00},{0x00,0x00,0x02},{0x00,0x02,0x02},{0x02,0x02,0x02}},
		{{0x00,0x00,0x00},{0x00,0x04,0x00},{0x04,0x04,0x00},{0x00,0x00,0x04},{0x00,0x04,0x04},{0x04,0x04,0x04}},
		{{0x00,0x00,0x00},{0x00,0x08,0x00},{0x08,0x08,0x00},{0x00,0x00,0x08},{0x00,0x08,0x08},{0x08,0x08,0x08}},
		{{0x00,0x00,0x00},{0x00,0x10,0x00},{0x10,0x10,0x00},{0x00,0x00,0x10},{0x00,0x10,0x10},{0x10,0x10,0x10}},
		{{0x00,0x00,0x00},{0x00,0x20,0x00},{0x20,0x20,0x00},{0x00,0x00,0x20},{0x00,0x20,0x20},{0x32,0x20,0x20}},
		{{0x00,0x00,0x00},{0x00,0x40,0x00},{0x40,0x40,0x00},{0x00,0x00,0x40},{0x00,0x40,0x40},{0x40,0x40,0x40}}};

// Led current setting is used to see what the different select values are right now.
int led_current_setting[3];
// This is used to reset a bit. Will be detracted from the current value of the led_current_setting.
int LED_NUMBER_HEX [7]={0x01,0x02,0x04,0x08,0x10,0x20,0x40};
//this is used to transform the user decimal input to a readable hex.
int NUMBER_HEX[15]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};


//-----------------------------------------------------------------------------



//Defined values---------------------------------------------------------------

//sets the user defined delay to seconds since the clock speed is 8MHz.
#define time							( 8000000 )

//defines the command byte as increment.
#define COMMAND_BYTE_INCREMENT			(0x10)// ( 0001 0000) in binary.
//defines the command byte to Maximum Intensity
#define COMMAND_BYTE_MAXIMUM_INTENSITY  (0x08)// ( 0000 1000) in binary.

/*the command byte:
BIT FUNCTION
B7 Reserved. Must be programmed as 0.
B6 Reserved. Must be programmed as 0.
B5 Reserved. Must be programmed as 0.
B4 Auto increment
B3 Register address 3
B2 Register address 2
B1 Register address 1
B0 Register address 0
*/

//-----------------------------------------------------------------------------




//DATA-HANDLER-----------------------------------------------------------------


// this handle the information to get right values in the three led_current_setting values.
//using this system its possible to change a single led without changing any settings on the other.
void data_handler(int LED_NUMBER,int LED_COMMAND)
{
	int SELECT;
	for (SELECT=0;SELECT!=3;SELECT++)
	{
		//enters if Select date(checks three times 1,2,3) is 0x00 and if the current_select data contains a 1.
		if (select_data_handler[LED_NUMBER][LED_COMMAND][SELECT] == 0x00 && (led_current_setting[SELECT] &= LED_NUMBER_HEX[LED_NUMBER]))
		{
			//changes the 1 in the select(1,2 or 3) to a 0.
			led_current_setting[SELECT] = led_current_setting[SELECT] - LED_NUMBER_HEX[LED_NUMBER];
		}
		//enters if Select date(checks three times 1,2,3) is not 0x00 and if the current_select data contains a 0.
		else if (select_data_handler[LED_NUMBER][LED_COMMAND][SELECT] != 0x00 && (led_current_setting[SELECT] &! LED_NUMBER_HEX[LED_NUMBER]))
		{
			//changes the 0 in the select(1,2 or 3) to a 1.
			led_current_setting[SELECT] = led_current_setting[SELECT] + LED_NUMBER_HEX[LED_NUMBER];
		}
	}
}


//-----------------------------------------------------------------------------



//The functions----------------------------------------------------------------


//this function enables the user to set a time delay in seconds.
void Delay(int delay_time)
{
//Delay for a moment.
SysCtlDelay(delay_time * time);
}


//used to control the leds
void LED_CONTROL(int LED_NUMBER, int LED_COMMAND)
{

	data_handler(LED_NUMBER,LED_COMMAND);

	// Slave address of TCA6507 is 0x45 (binary 1001 101)
	// false=write / true=read
	I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, 0x45, false);

// the for loop is there to make the code more compact and removes some redundant commands.
	int COUNTER;
		for (COUNTER=0;COUNTER!=5;COUNTER++)
		{

			if(COUNTER == 2)
			{
				//puts the command-byte in the dataput getting ready to sending it.
				I2CMasterDataPut(I2C0_MASTER_BASE, COMMAND_BYTE_INCREMENT);
				//starts sending the data.
				I2CMasterControl(I2C0_MASTER_BASE,I2C_MASTER_CMD_BURST_SEND_START);
			}
			else if(COUNTER == 3)
			{
				//gets the first led_current_setting containing the byte for select0 ready to transmitting.
				I2CMasterDataPut(I2C0_MASTER_BASE,led_current_setting[0] );
				//keeps sending data.
				I2CMasterControl(I2C0_MASTER_BASE,I2C_MASTER_CMD_BURST_SEND_CONT);
			}
			else if(COUNTER == 4)
			{
				//gets the second led_current_setting containing the byte for select1 ready to transmitting.
				I2CMasterDataPut(I2C0_MASTER_BASE,led_current_setting[1] );
				//keeps sending data.
				I2CMasterControl(I2C0_MASTER_BASE,I2C_MASTER_CMD_BURST_SEND_CONT);
			}
			else if(COUNTER == 5)
			{
				//gets the third led_current_setting containing the byte for select2 ready to transmitting.
				I2CMasterDataPut(I2C0_MASTER_BASE,led_current_setting[2] );
				//transmitting the final byte and a stop command.
				I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH );
			}


			// Wait for I2C to finish.
			while(I2CMasterBusy(I2C0_MASTER_BASE));

			//a short delay.
			SysCtlDelay(80000);
		}

}

//used to control the brightness value.
void CHANGE_BRIGHTNESS_VALUES(int value0,int value1)
{
	// Slave address of TCA6507 is 0x45 (binary 1001 101)
		// false=write / true=read
		I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, 0x45, false);

		int COUNTER;
		for (COUNTER=0;COUNTER!=4;COUNTER++)
		{

			//ensure that the user input value is a legal value if not it skips out of the function.
			if(value0||value1 > 15 )
			{
				COUNTER=4;
			}
			else if(value0||value1 <  1 )
			{
				COUNTER=4;
			}



			else if(COUNTER == 2)
			{
				//puts the command-byte in the dataput getting ready to sending it.
				I2CMasterDataPut(I2C0_MASTER_BASE, COMMAND_BYTE_MAXIMUM_INTENSITY);

				//starts sending the data.
				I2CMasterControl(I2C0_MASTER_BASE,I2C_MASTER_CMD_BURST_SEND_START);
			}
			else if(COUNTER == 3)
				{
					//value0 and value1 is send through the same byte where value0 is the first 4bit and value1 is the last 4bit.
					I2CMasterDataPut(I2C0_MASTER_BASE,NUMBER_HEX[value0]+(10*NUMBER_HEX[value1]));

					//transmitting the final byte and a stop command.
					I2CMasterControl(I2C0_MASTER_BASE,I2C_MASTER_CMD_BURST_SEND_FINISH );
				}


			// Wait for I2C to finish.
			while(I2CMasterBusy(I2C0_MASTER_BASE));

			//a short delay.
			SysCtlDelay(80000);

		}


}



// this function is used when the user needs to change the time-settings. the data is saved to bank1 or bank0.
void SET_TIMERS(int BANK,int FADE_ON_TIME,int FULLY_ON_TIME,int FADE_OFF_TIME,int FIRST_FULLY_OFF,int SECOND_FULLY_OFF)
{

			// Slave address of TCA6507 is 0x45 (binary 1001 101)
			// false=write / true=read
			I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, 0x45, false);
			// Wait for I2C to finish.
			while(I2CMasterBusy(I2C0_MASTER_BASE));

			//this is used to tell if the data goes into the first or last 4bit of the byte.
			int defined_bank;
			if(BANK == BANK0)
			{
			defined_bank=1;
			}
			else if(BANK == BANK1)
			{
				defined_bank=10;
			}

			//again the forloop is used to reduce redundant code and reduce the sheer size of the code
			int COUNTER;
			for (COUNTER=0;COUNTER!=11;COUNTER++)
			{
				//the functions start out by sending the allready defined select0,1,2 to reach the registers that are used.
				if(COUNTER == 2)
				{
					//puts the command-byte in the dataput getting ready to sending it.
					I2CMasterDataPut(I2C0_MASTER_BASE, COMMAND_BYTE_INCREMENT);

					//starts sending the data.
					I2CMasterControl(I2C0_MASTER_BASE,I2C_MASTER_CMD_BURST_SEND_START);
				}
				else if(COUNTER == 3)
				{
					//gets the first led_current_setting containing the byte for select0 ready to transmitting.
					I2CMasterDataPut(I2C0_MASTER_BASE,led_current_setting[0] );
					//keeps sending data.
					I2CMasterControl(I2C0_MASTER_BASE,I2C_MASTER_CMD_BURST_SEND_CONT);
				}
				else if(COUNTER == 4)
				{
					//gets the second led_current_setting containing the byte for select1 ready to transmitting.
					I2CMasterDataPut(I2C0_MASTER_BASE,led_current_setting[1] );
					//keeps sending data.
					I2CMasterControl(I2C0_MASTER_BASE,I2C_MASTER_CMD_BURST_SEND_CONT);
				}
				else if(COUNTER == 5)
				{
					//gets the third led_current_setting containing the byte for select2 ready to transmitting.
					I2CMasterDataPut(I2C0_MASTER_BASE,led_current_setting[2] );
					//keeps sending data.
					I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_CONT );
				}
				else if(COUNTER == 6)
				{
					//Puts in the value for the FADE_ON_TIME to get ready to transmit to bank0/bank1.
					I2CMasterDataPut(I2C0_MASTER_BASE,NUMBER_HEX[FADE_ON_TIME]*defined_bank);
					//keeps sending data.
					I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_CONT );
				}
				else if(COUNTER == 7)
				{
					//Puts in the value for the FULLY_ON_TIME to get ready to transmit to bank0/bank1.
					I2CMasterDataPut(I2C0_MASTER_BASE,NUMBER_HEX[FULLY_ON_TIME]*defined_bank );
					//keeps sending data.
					I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_CONT );
				}
				else if(COUNTER == 8)
				{
					//Puts in the value for the FADE_OFF_TIME to get ready to transmit to bank0/bank1.
					I2CMasterDataPut(I2C0_MASTER_BASE,NUMBER_HEX[FADE_OFF_TIME]*defined_bank);
					//keeps sending data.
					I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_CONT );
				}
				else if(COUNTER == 9)
				{
					//Puts in the value for the FIRST_FULLY_OFF to get ready to transmit to bank0/bank1.
					I2CMasterDataPut(I2C0_MASTER_BASE,NUMBER_HEX[FIRST_FULLY_OFF]*defined_bank);
					//keeps sending data.
					I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_CONT );
				}
				else if(COUNTER == 10)
				{
					//Puts in the value for the SECOND_FULLY_OFF to get ready to transmit to bank0/bank1.
					I2CMasterDataPut(I2C0_MASTER_BASE,NUMBER_HEX[SECOND_FULLY_OFF]*defined_bank);
					//transmitting the final byte and a stop command.
					I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_CONT );
				}

			}


				// Wait for I2C to finish.
				while(I2CMasterBusy(I2C0_MASTER_BASE));

				//a short delay.
				SysCtlDelay(80000);

}






//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
