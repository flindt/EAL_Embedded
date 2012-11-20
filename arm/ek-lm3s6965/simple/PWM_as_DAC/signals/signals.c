/*
 * signals.c
 *
 *  Created on: Nov 13, 2012
 *      Author: flindt
 */

#include <IQmath/IQmathLib.h>
#include "sineWave/sinewave.h"
#include "../src/settings.h"

float calcNextOutputValue(){
	_iq output;

	output = sine( phaseStep(10,100));

	//return output;
	return _IQtoF( _IQdiv2( output ) + _IQ( 0.5 ) );
}
