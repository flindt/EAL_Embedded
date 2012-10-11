/*
 * statemashine.h
 *
 *  Created on: 04/10/2012
 *      Author: Lars
 */

#ifndef STATEMASHINE_H_
#define STATEMASHINE_H_

enum states { UPSTARTMENU, LED, MOTOR, HASTMOTOR, MOVEFOBMOTOR, MAXSTATES };

// forward function declarations
void SetMotorParams( int State,int returnspeed );
void OnEnter( int State);
void OnExit( int State, int value, int setvalue);
int statemashine(int event);
//------------------------------

#endif /* STATEMASHINE_H_ */
