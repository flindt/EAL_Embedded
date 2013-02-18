/*
 * control.h
 *
 *  Created on: 24/09/2012
 *      Author: Lasse
 */

#ifndef CONTROL_H_
#define CONTROL_H_

void updateParameters(double nK, double nTi, double nTd, double nBeta, double nTr, double nN, double nh);
double calculateOutput(double yref, double newY);
void updateState(double u);
#endif /* CONTROL_H_ */
