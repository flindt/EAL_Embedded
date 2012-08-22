/*
 * initialize.h
 *
 *  Created on: May 8, 2012
 *      Author: G
 */

#ifndef INITIALIZE_H_
#define INITIALIZE_H_

void InitSysClock(void);
void InitEthernet(void);
void ConfigMACaddress(void);
void InitlwIP(void);
void InitLocator(void);
void I2C_Init(void);
void InitButtons(void);
void PWM_Init(void);

#endif /* INITIALIZE_H_ */
