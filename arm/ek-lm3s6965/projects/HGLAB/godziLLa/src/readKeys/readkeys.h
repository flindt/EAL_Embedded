/*
 * readkeys.h
 *
 *  Created on: Aug 23, 2012
 *      Author: flindt
 */

#ifndef READKEYS_H_
#define READKEYS_H_

// How long must a key be pressed to be an event
const int KEY_PRESS_MINIMUM = 7;

int
ReadKeys(void);
int
GetKeyEvents(void);

#endif /* READKEYS_H_ */
