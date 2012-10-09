/*
 * statemashine.h
 *
 *  Created on: 04/10/2012
 *      Author: Lars
 */

#ifndef STATEMASHINE_H_
#define STATEMASHINE_H_

enum states { UPSTARTMENU, LED, MOTOR, STOPMOTOR, FORWARDMOTOR, BACKMOTOR, MAXSTATES };

// forward function declarations
int SetMotorParams( int State,int value );
void OnEnter( int State);
void OnExit( int State);
int statemashine(int event);
//------------------------------

#endif /* STATEMASHINE_H_ */
