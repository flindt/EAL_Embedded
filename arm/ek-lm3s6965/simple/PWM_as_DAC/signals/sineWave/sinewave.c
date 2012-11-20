/*
 * sinewave.c
 *
 *  Created on: Oct 30, 2012
 *      Author: flindt
 */




#include <IQmath/IQmathLib.h>

// Do not start from 0 phase
#define START_PHASE 0.1

_iq sine( _iq phaseStep) {
	static _iq currentPhase = _IQ( START_PHASE );
	_iq output;

	currentPhase += phaseStep;

	output = _IQsin( currentPhase );
	return output;
}

_iq phaseStep( int frequency, int samplerate)
{
	_iq output;

	output = _IQ( frequency*6.28/samplerate);

	return 50000;
}

