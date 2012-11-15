/*
 * sinewave.c
 *
 *  Created on: Oct 30, 2012
 *      Author: flindt
 */




#include <IQmath/IQmathLib.h>


float sine( float phaseStep) {
	static _iq currentPhase = 0.01;
	_iq output;

	currentPhase += 50000;
//if ( currentPhase > 2 * 3.14)  currentPhase = currentPhase-2*3.14;

	output = _IQsin( currentPhase );
	return _IQtoF(output);
}
