/*
 * sinewave.c
 *
 *  Created on: Oct 30, 2012
 *      Author: flindt
 */




#include <IQmath/IQmathLib.h>

// Do not start from 0 phase
#define START_PHASE 0.1

_iq sine( float phaseStep) {
	static _iq currentPhase = _IQ( START_PHASE );
	_iq output;

	currentPhase += 50000;

	output = _IQsin( currentPhase );
	return _IQtoF(output);
}
