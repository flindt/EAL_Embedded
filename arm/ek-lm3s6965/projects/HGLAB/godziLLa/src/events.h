/*
 * events.h
 *
 *  Created on: Aug 23, 2012
 *      Author: flindt
 *
 * This file defines all events in the system
 *
 */

#ifndef EVENTS_H_
#define EVENTS_H_

//
// Theese enums are used as events, and for passing the matching bit for a certain key
// from ReadKeys to GetKeyEvents.
// NO_EVENT must be last.
enum Events
{
  KEY_ENTER, KEY_CANCEL, KEY_UP, KEY_DOWN, NO_EVENT
};

#endif /* EVENTS_H_ */
