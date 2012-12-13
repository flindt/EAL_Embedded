/**
 * statemashine.h
 *
 *  Created on: 04/10/2012
 *      Author: Lars
 */

#ifndef STATEMASHINE_H_
#define STATEMASHINE_H_
//! This is the @param enum of the states are so need a state more here is to make it
enum states { UPSTARTMENU, LED, TEMP, SNIVEAU, FNIVEAU, MAXSTATES };

// forward function declarations
void SetParams( int State,float tempvalue1, float tempvalue2, int button );
void OnEnter( int State);
void OnExit( int State);
int statemashine(int event);

//------------------------------

#endif /* STATEMASHINE_H_ */
