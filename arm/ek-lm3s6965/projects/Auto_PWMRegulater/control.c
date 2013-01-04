/*
 ============================================================================
 Name        : PID.c
 Author      : Knud Kjer Simonsen
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "statemashine.h"
#include "drivers/setADC.h"

double u,e,v,y;
/*
	K 	Loop gain parameter
	Ti 	Integrator time constant
	Td  Differentiator time constant
 */
double K,Ti,Td,Beta,Tr,N,h;
double ad, bd;
double D,I,yOld;

// user sets D
// First run updateParameters once
//
// yref = user set goal
// newY = measured value
// h = sample time. time between to samples..Update time interval
//

void updateParameters(double nK, double nTi, double nTd, double nBeta, double nTr, double nN, double nh)
{
 K = nK;
 Ti = nTi;
 Td = nTd;
 Beta = nBeta;
 Tr = nTr;
 N = nN;
 h = nh;
 ad = Td / (Td + N*h);
 bd = K*ad*N;
}

double calculateOutput(double yref, double newY)
{
 y = newY;
 e = yref - y;
 D = ad*D - bd*(y-yOld);
 v = K*(Beta*yref - y) + I + D;
 return v;
}

void updateState(double u)
{
 I = I + (K*h/Ti)*e + (h/Tr)*(u - v);
 yOld = y;
}
