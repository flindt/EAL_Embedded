/*
 * readkeys.h
 *
 *  Created on: Feb 10, 2012
 *      Author: Lars
 */

#ifndef READKEYS_H_
#define READKEYS_H_

// Function prototypes
int GetKeyEvents(void);
int ReadKeys(void);

enum bitNumbers { KEY0BIT, KEY1BIT, KEY2BIT, KEY3BIT, KEY4BIT, KEY5BIT, KEY6BIT, KEY7BIT, MAXBIT};
enum events 	{ KEY0_EVENT_SELECT, KEY1_EVENT_UP, KEY2_EVENT_DOWN, KEY3_EVENT_ENTER, KEY4_EVENT_CANCEL, KEY5_EVENT, KEY6_EVENT, KEY7_EVENT,NO_EVENT };


#endif /* READKEYS_H_ */
